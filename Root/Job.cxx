/** @file Job.cxx
 *  @brief TL::Job class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/Core/Job.h>
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/Utils.h>
#include <TopLoop/Core/FileManager.h>
#include <TopLoop/tqdm/tqdm.h>

#include <TTreeIndex.h>

TL::Job::Job() : TL::Loggable("TL::Job") {}

TL::Job::~Job() {}

TL::StatusCode TL::Job::setAlgorithm(std::unique_ptr<TL::Algorithm> alg) {
  if ( alg == nullptr ) {
    return TL::StatusCode::FAILURE;
  }
  m_algorithm = std::move(alg);
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Job::setFileManager(std::unique_ptr<TL::FileManager> fm) {
  if ( fm == nullptr ) {
    return TL::StatusCode::FAILURE;
  }
  m_fm = std::move(fm);
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Job::run() {
  if ( m_algorithm->isData() && m_enableParticleLevel ) {
    logger()->error("You cannot have the algorithm set to data mode"
                    " and also enable particle level!");
    return TL::StatusCode::FAILURE;
  }

  if ( m_enableParticleLevel ) {
    TL_CHECK(constructIndices());
  }

  TL_CHECK(m_algorithm->setFileManager(std::move(m_fm)));
  TL_CHECK(m_algorithm->init());
  if ( not m_algorithm->initCalled() ) {
    logger()->error("You didn't call TL::Algorithm::init()");
    logger()->error("in your algorithm's init() function");
    logger()->error("This is a required line!");
    return TL::StatusCode::FAILURE;
  }
  TL_CHECK(m_algorithm->setupOutput());
  m_algorithm->reader()->Restart();
  if ( !m_useProgressBar ) {
    logger()->info("Progress bar disabled.. you're in the dark");
  }

  tqdm bar;
  bar.set_theme_braille_spin();

  // if particle level is not enabled, do the standard loop over the
  // normal tree.
  if ( !m_enableParticleLevel ) {
    while ( m_algorithm->reader()->Next() ) {
      if ( m_useProgressBar ) {
        bar.progress(m_algorithm->m_eventCounter,m_algorithm->m_totalEntries);
      }
      TL_CHECK(m_algorithm->execute());
    }
  } // end if standard (not using particle level)

  // when particle level is enabled we have a few more logical cases
  // to work through.
  else {
    m_algorithm->particleLevelReader()->Restart();
    // first, if we want particle level only (i.e. events that didn't
    // end up in reco tree
    if ( m_loopOverParticleLevelOnly ) {
      logger()->info("Entering particle level only loop");
      for ( const auto idx : m_particleLevelOnly ) {
        m_algorithm->particleLevelReader()->SetEntry(idx);
        if ( m_useProgressBar ) {
          bar.progress(m_algorithm->m_eventCounter,m_algorithm->m_totalParticleLevelEntries);
        }
        TL_CHECK(m_algorithm->execute());
      }
    } // end if particle level only

    // next, we do all particle level, agnostic to reco information
    else if ( m_loopOverAllParticleLevel ) {
      logger()->info("Entering all particle level loop");
      while ( m_algorithm->particleLevelReader()->Next() ) {
        if ( m_useProgressBar ) {
          bar.progress(m_algorithm->m_eventCounter,m_algorithm->m_totalParticleLevelEntries);
        }
        TL_CHECK(m_algorithm->execute());
      }
    } // end if all particle level

    // finally, we do reco and particle info together
    else {
      logger()->info("Entering loop over reco _and_  particle level information");
      for ( const auto idx : m_particleAndReco ) {
        m_algorithm->particleLevelReader()->SetEntry(std::get<0>(idx));
        m_algorithm->reader()->SetEntry(std::get<1>(idx));
        if ( m_useProgressBar ) {
          bar.progress(m_algorithm->m_eventCounter,m_algorithm->m_totalEntries);
        }
        TL_CHECK(m_algorithm->execute());
      }
    } // end if reco and particle

  } // end if particle level enabled

  TL_CHECK(m_algorithm->finish());
  return TL::StatusCode::SUCCESS;
}

void TL::Job::disableProgressBar() {
  m_useProgressBar = false;
}

void TL::Job::enableParticleLevel() {
  logger()->info("Particle level information enabled");
  m_enableParticleLevel = true;
}

void TL::Job::loopOverAllParticleLevel() {
  logger()->info("Going to loop over all particle level");
  if ( m_loopOverParticleLevelOnly || m_loopOverRecoOnly ) {
    logger()->warn("You've asked for loopOverParticleLevelOnly() or "
                   "loopOverRecoOnly() already. "
                   "Overriding that in favor of loopOverAllParticleLevel()");
    m_loopOverParticleLevelOnly = false;
    m_loopOverRecoOnly = false;
  }
  m_loopOverAllParticleLevel = true;
}

void TL::Job::loopOverParticleLevelOnly() {
  logger()->info("Going to loop over particle level _only_ (skip reco'd)");
  if ( m_loopOverAllParticleLevel || m_loopOverRecoOnly ) {
    logger()->warn("You've asked for loopOverAllParticleLevel() or "
                   "loopOverRecoOnly() already. "
                   "Overriding that in favor of loopOverParticleLevelOnly()");
    m_loopOverAllParticleLevel = false;
    m_loopOverRecoOnly = false;
  }
  m_loopOverParticleLevelOnly = true;
}

void TL::Job::loopOverRecoOnly() {
  logger()->info("Going to loop over reco only events "
                 "(skip if in particle level tree)");
  if ( m_loopOverAllParticleLevel || m_loopOverParticleLevelOnly ) {
    logger()->warn("You've alreaady asked for loopOverAllParticleLevel() or "
                   "loopOverParticleLevelOnly(), overriding that in favor "
                   "of loopOverRecoOnly");
    m_loopOverParticleLevelOnly = false;
    m_loopOverAllParticleLevel = false;
  }
  m_loopOverRecoOnly = true;
}

TL::StatusCode TL::Job::constructIndices() {
  logger()->info("Constructing particle level and reco level indices");
  if ( not m_particleLevelOnly.empty() ||
       not m_particleAndReco.empty() ||
       not m_recoLevelOnly.empty() ) {
    logger()->error("Index containers already contain data... "
                    " somehow the constructIndices() function has been "
                    " called twice, but it shouldn't have.");
    return TL::StatusCode::FAILURE;
  }
  auto c_PL = m_fm->particleLevelChain();
  if ( !c_PL ) {
    logger()->error("Particle level chain is null... "
                    "Enable particle level in your TL::FileManager!");
  }
  auto c_RL = m_fm->rootChain();
  auto idx_PL = std::make_unique<TTreeIndex>(c_PL,"runNumber","eventNumber");
  auto idx_RL = std::make_unique<TTreeIndex>(c_RL,"runNumber","eventNumber");
  c_PL->SetTreeIndex(idx_PL.get());
  c_RL->SetTreeIndex(idx_RL.get());

  UInt_t runNumber_PL;
  UInt_t runNumber_RL;
  ULong64_t eventNumber_PL;
  ULong64_t eventNumber_RL;

  c_PL->SetBranchAddress("runNumber",  &runNumber_PL);
  c_RL->SetBranchAddress("runNumber",  &runNumber_RL);
  c_PL->SetBranchAddress("eventNumber",&eventNumber_PL);
  c_RL->SetBranchAddress("eventNumber",&eventNumber_RL);

  c_PL->SetBranchStatus("*",0);
  c_RL->SetBranchStatus("*",0);
  c_PL->SetBranchStatus("runNumber",1);
  c_RL->SetBranchStatus("runNumber",1);
  c_PL->SetBranchStatus("eventNumber",1);
  c_RL->SetBranchStatus("eventNumber",1);

  // do some conservative reserving to save time spent allocating
  // memory... it shouldn't waste too much memory...
  m_particleAndReco.reserve(c_RL->GetEntries());
  m_particleLevelOnly.reserve(c_PL->GetEntries());
  m_recoLevelOnly.reserve(c_RL->GetEntries());

  // get indices for particle+reco and particle only
  for ( ULong64_t i = 0; i < c_PL->GetEntries(); ++i ) {
    c_PL->GetEntry(i);
    auto index_RL = c_RL->GetEntryNumberWithIndex(runNumber_PL, eventNumber_PL);
    if ( index_RL > 0 ) {
      m_particleAndReco.push_back(std::make_pair(i,index_RL));
    }
    else {
      m_particleLevelOnly.push_back(i);
    }
  }

  // get indices for reco only
  for ( ULong64_t i = 0; i < c_RL->GetEntries(); ++i ) {
    c_RL->GetEntry(i);
    auto index_CL = c_RL->GetEntryNumberWithIndex(runNumber_RL, runNumber_RL);
    if ( index_CL < 0 ) {
      m_recoLevelOnly.push_back(i);
    }
  }

  c_PL->SetBranchStatus("*",1);
  c_RL->SetBranchStatus("*",1);

  return TL::StatusCode::SUCCESS;
}

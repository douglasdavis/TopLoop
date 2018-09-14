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
    logger()->info("Progress bar disabled.. you'll have to be patient");
  }
  tqdm bar;
  bar.set_theme_braille_spin();
  while ( m_algorithm->reader()->Next() ) {
    if ( m_useProgressBar ) {
      bar.progress(m_algorithm->m_eventCounter,m_algorithm->m_totalEntries);
    }
    TL_CHECK(m_algorithm->execute());
  }
  TL_CHECK(m_algorithm->finish());
  return TL::StatusCode::SUCCESS;
}

void TL::Job::disableProgressBar() {
  m_useProgressBar = false;
}

void TL::Job::enableParticleLevel() {
  m_enableParticleLevel = true;
}

void TL::Job::loopOverAllParticleLevel() {
  if ( m_loopOverParticleLevelOnly ) {
    logger()->warn("You've asked for loopOverParticleLevelOnly() already. "
                   "Overriding that in favor of loopOverAllParticleLevel()");
    m_loopOverParticleLevelOnly = false;
  }
  m_loopOverAllParticleLevel = true;
}

void TL::Job::loopOverParticleLevelOnly() {
  if ( m_loopOverAllParticleLevel ) {
    logger()->warn("You've asked for loopOverAllParticleLevel() already. "
                   "Overriding that in favor of loopOverParticleLevelOnly()");
  }
  m_loopOverParticleLevelOnly = true;
}

TL::StatusCode TL::Job::constructIndices() {
  if ( not m_particleLevelOnly.empty() ||
       not m_particleAndReco.empty() ||
       not m_recoLevelOnly.empty() ) {
    logger()->error("Index containers already contain data... "
                    " somehow the constructIndices() function has been "
                    " called twice, but it shouldn't have.");
    return TL::StatusCode::FAILURE;
  }
  auto c_PL = m_fm->particleLevelChain();
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
  return TL::StatusCode::SUCCESS;
}

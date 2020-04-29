/** @file Job.cxx
 *  @brief TL::Job class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/FileManager.h>
#include <TopLoop/Core/Job.h>
#include <TopLoop/Core/Utils.h>
#include <TopLoop/tqdm/tqdm.h>

#include <TTreeIndex.h>

TL::Job::Job() : TL::Loggable("TL::Job") {}

TL::StatusCode TL::Job::setAlgorithm(std::unique_ptr<TL::Algorithm> alg) {
  if (alg == nullptr) {
    return TL::StatusCode::FAILURE;
  }
  m_algorithm = std::move(alg);
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Job::setFileManager(std::unique_ptr<TL::FileManager> fm) {
  if (fm == nullptr) {
    return TL::StatusCode::FAILURE;
  }
  m_fm = std::move(fm);
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Job::run() {
  if (m_algorithm->isData() && m_loopType != LoopType::RecoStandard) {
    logger()->error(
        "Algorithm is processing data, which can only work with a RecoStandard LoopType");
    return TL::StatusCode::FAILURE;
  }

  // indices required for loops that are not standard reco or all particle
  if (not(m_loopType == LoopType::RecoStandard || m_loopType == LoopType::ParticleAll)) {
    TL_CHECK(constructIndices());
  }

  TL_CHECK(m_algorithm->setFileManager(std::move(m_fm)));
  TL_CHECK(m_algorithm->init());
  if (not m_algorithm->initCalled()) {
    logger()->error("You didn't call TL::Algorithm::init()");
    logger()->error("in your algorithm's init() function");
    logger()->error("This is a required line!");
    return TL::StatusCode::FAILURE;
  }
  TL_CHECK(m_algorithm->setupOutput());
  m_algorithm->reader()->Restart();

  tqdm bar;
  bar.set_theme_braille_spin();

  // if particle level is not enabled, do the standard loop over the
  // normal tree.
  if (m_loopType == LoopType::RecoStandard) {
    while (m_algorithm->reader()->Next()) {
      if (m_useProgressBar) {
        bar.progress(m_algorithm->m_eventCounter, m_algorithm->m_totalEntries);
      }
      else {
        printProgress(5, m_algorithm->m_totalEntries, m_algorithm->m_eventCounter);
      }
      TL_CHECK(m_algorithm->execute());
    }
    std::cout << std::endl;
  }  // end if standard (not using particle level)

  // when particle level is enabled we have a few more logical cases
  // to work through.
  else {
    m_algorithm->particleLevelReader()->Restart();
    // first, if we want particle level only (i.e. events that didn't
    // end up in reco tree
    if (m_loopType == LoopType::ParticleOnly) {
      m_algorithm->m_truthAvailable = true;
      logger()->info("Entering particle level only loop");
      for (const auto idx : m_particleLevelOnly) {
        m_algorithm->particleLevelReader()->SetEntry(idx);
        m_algorithm->truthReader()->SetEntry(idx);
        if (m_useProgressBar) {
          bar.progress(m_algorithm->m_eventCounter,
                       m_algorithm->m_totalParticleLevelEntries);
        }
        else {
          printProgress(5, m_algorithm->m_totalParticleLevelEntries,
                        m_algorithm->m_eventCounter);
        }
        TL_CHECK(m_algorithm->execute());
      }
    }  // end if particle level only

    // next, we do all particle level, agnostic to reco information
    else if (m_loopType == LoopType::ParticleAll) {
      m_algorithm->m_truthAvailable = true;
      logger()->info("Entering all particle level loop");
      while (m_algorithm->particleLevelReader()->Next() &&
             m_algorithm->truthReader()->Next()) {
        if (m_useProgressBar) {
          bar.progress(m_algorithm->m_eventCounter,
                       m_algorithm->m_totalParticleLevelEntries);
        }
        else {
          printProgress(5, m_algorithm->m_totalParticleLevelEntries,
                        m_algorithm->m_eventCounter);
        }
        TL_CHECK(m_algorithm->execute());
      }
    }  // end if all particle level

    // finally, we do reco and particle info together
    else if (m_loopType == LoopType::RecoWithParticle) {
      m_algorithm->m_truthAvailable = true;
      logger()->info("Entering loop over reco _and_  particle level information");
      for (const auto idx : m_particleAndReco) {
        m_algorithm->particleLevelReader()->SetEntry(std::get<0>(idx));
        m_algorithm->truthReader()->SetEntry(std::get<0>(idx));
        m_algorithm->reader()->SetEntry(std::get<1>(idx));
        if (m_useProgressBar) {
          bar.progress(m_algorithm->m_eventCounter, m_algorithm->m_totalEntries);
        }
        else {
          printProgress(5, m_algorithm->m_totalEntries, m_algorithm->m_eventCounter);
        }
        TL_CHECK(m_algorithm->execute());
      }
    }  // end if reco and particle

    else if (m_loopType == LoopType::RecoOnly) {
      logger()->warn("RecoOnly not supported yet. Exiting.");
      std::exit(EXIT_SUCCESS);
    }

    else {
      logger()->error("Someone went wrong with the LoopType");
      return TL::StatusCode::FAILURE;
    }

  }  // end if particle level enabled

  TL_CHECK(m_algorithm->finish());
  return TL::StatusCode::SUCCESS;
}

void TL::Job::disableProgressBar() { m_useProgressBar = false; }

void TL::Job::setLoopType(const TL::LoopType loopType) { m_loopType = loopType; }

TL::StatusCode TL::Job::constructIndices() {
  logger()->info("Constructing particle level and reco level indices");
  if (not m_particleLevelOnly.empty() || not m_particleAndReco.empty() ||
      not m_recoLevelOnly.empty()) {
    logger()->error(
        "Index containers already contain data... "
        " somehow the constructIndices() function has been "
        " called twice, but it shouldn't have.");
    return TL::StatusCode::FAILURE;
  }

  if (m_fm->particleLevelChain() == nullptr) {
    logger()->error(
        "Particle level chain is null... "
        "Enable particle level in your TL::FileManager!");
  }

  // make copies for this isolated task
  auto chain_partLevel =
      std::unique_ptr<TChain>(dynamic_cast<TChain*>(m_fm->particleLevelChain()->Clone()));
  auto chain_recoLevel =
      std::unique_ptr<TChain>(dynamic_cast<TChain*>(m_fm->mainChain()->Clone()));

  chain_partLevel->SetBranchStatus("*", 0);
  chain_recoLevel->SetBranchStatus("*", 0);
  chain_partLevel->SetBranchStatus("runNumber", 1);
  chain_recoLevel->SetBranchStatus("runNumber", 1);
  chain_partLevel->SetBranchStatus("eventNumber", 1);
  chain_recoLevel->SetBranchStatus("eventNumber", 1);
  chain_partLevel->SetCacheSize(25000000);  // 25 MB cache
  chain_recoLevel->SetCacheSize(25000000);  // 25 MB cache
  chain_partLevel->AddBranchToCache("*", false);
  chain_recoLevel->AddBranchToCache("*", false);
  chain_partLevel->AddBranchToCache("runNumber", true);
  chain_recoLevel->AddBranchToCache("runNumber", true);
  chain_partLevel->AddBranchToCache("eventNumber", true);
  chain_recoLevel->AddBranchToCache("eventNumber", true);

  auto idx_PL =
      std::make_unique<TTreeIndex>(chain_partLevel.get(), "runNumber", "eventNumber");
  auto idx_RL =
      std::make_unique<TTreeIndex>(chain_recoLevel.get(), "runNumber", "eventNumber");
  chain_partLevel->SetTreeIndex(idx_PL.get());
  chain_recoLevel->SetTreeIndex(idx_RL.get());

  UInt_t runNumber_partLevel;
  UInt_t runNumber_recoLevel;
  ULong64_t eventNumber_partLevel;
  ULong64_t eventNumber_recoLevel;

  chain_partLevel->SetBranchAddress("runNumber", &runNumber_partLevel);
  chain_recoLevel->SetBranchAddress("runNumber", &runNumber_recoLevel);
  chain_partLevel->SetBranchAddress("eventNumber", &eventNumber_partLevel);
  chain_recoLevel->SetBranchAddress("eventNumber", &eventNumber_recoLevel);

  // do some conservative reserving to save time spent allocating
  // memory... it shouldn't waste too much memory...
  if (m_loopType == TL::LoopType::RecoWithParticle) {
    m_particleAndReco.reserve(chain_recoLevel->GetEntries());
  }
  if (m_loopType == TL::LoopType::ParticleOnly) {
    m_particleLevelOnly.reserve(chain_partLevel->GetEntries());
  }
  if (m_loopType == TL::LoopType::RecoOnly) {
    m_recoLevelOnly.reserve(chain_recoLevel->GetEntries());
  }

  ULong64_t totalPartLevel = static_cast<ULong64_t>(chain_partLevel->GetEntries());
  ULong64_t totalRecoLevel = static_cast<ULong64_t>(chain_recoLevel->GetEntries());

  if (m_loopType == TL::LoopType::RecoWithParticle ||
      m_loopType == TL::LoopType::ParticleOnly) {
    tqdm index_bar;
    index_bar.set_theme_braille_spin();
    // get indices for particle+reco and particle only
    for (ULong64_t i = 0; i < totalPartLevel; ++i) {
      chain_partLevel->GetEntry(i);
      auto index_RL = chain_recoLevel->GetEntryNumberWithIndex(runNumber_partLevel,
                                                               eventNumber_partLevel);
      if (index_RL > 0) {
        if (m_loopType == TL::LoopType::RecoWithParticle) {
          m_particleAndReco.push_back(std::make_pair(i, index_RL));
        }
      }
      else {
        if (m_loopType == TL::LoopType::ParticleOnly) {
          m_particleLevelOnly.push_back(i);
        }
      }
      index_bar.progress(i, totalPartLevel);
    }
  }

  if (m_loopType == TL::LoopType::RecoOnly) {
    // get indices for reco only
    for (ULong64_t i = 0; i < totalRecoLevel; ++i) {
      chain_recoLevel->GetEntry(i);
      auto index_CL = chain_recoLevel->GetEntryNumberWithIndex(runNumber_recoLevel,
                                                               eventNumber_recoLevel);
      if (index_CL < 0) {
        m_recoLevelOnly.push_back(i);
      }
    }
  }

  chain_partLevel->SetBranchStatus("*", 1);
  chain_recoLevel->SetBranchStatus("*", 1);

  return TL::StatusCode::SUCCESS;
}

void TL::Job::printProgress(int n_prints, long total_entries, long event_count) const {
  if (total_entries > n_prints) {
    int gap = total_entries / n_prints;
    if (event_count % gap == 0) {
      auto progress = std::round(100.0 * event_count / total_entries);
      logger()->info(" -- [{:3.0f}%] Event: {}", progress, event_count);
    }
  }
}

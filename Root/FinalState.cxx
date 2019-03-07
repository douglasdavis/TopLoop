/** @file FinalState.cxx
 *  @brief TL::EDM::FinalState class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/EDM/FinalState.h>

TL::EDM::FinalState::FinalState() : TL::Loggable("TL::EDM::FinalState") {}

void TL::EDM::FinalState::makeLeptonPairs() {
  for (std::size_t i = 0; i < m_leptons.size(); ++i) {
    for (std::size_t j = (i + 1); j < m_leptons.size(); ++j) {
      TL::EDM::LeptonPair lp(m_leptons.at(i), m_leptons.at(j), i, j);
      addLeptonPair(lp);
    }
  }
}

void TL::EDM::FinalState::evaluateSelf(bool sort_leptons, bool manual_promptness,
                                       bool sort_jets) {
  m_hasFakeElectronMC = false;
  m_hasFakeMuonMC = false;
  m_hasManTrigMatched = false;
  for (const TL::EDM::Electron& el : m_electrons) {
    addLepton(el);
    if (manual_promptness) {
      if (not el.isManualTruthPrompt()) m_hasFakeElectronMC = true;
    }
    else {
      if (not el.true_isPrompt()) m_hasFakeElectronMC = true;
    }
  }
  for (const TL::EDM::Muon& mu : m_muons) {
    addLepton(mu);
    if (manual_promptness) {
      if (not mu.isManualTruthPrompt()) m_hasFakeMuonMC = true;
    }
    else {
      if (not mu.true_isPrompt()) m_hasFakeMuonMC = true;
    }
  }
  if (sort_leptons) {
    std::sort(m_leptons.begin(), m_leptons.end(),
              [](const TL::EDM::Lepton& lep1, const TL::EDM::Lepton& lep2) {
                return lep1.pt() > lep2.pt();
              });
  }
  m_hasManTrigMatched =
      std::any_of(std::begin(m_leptons), std::end(m_leptons),
                  [](const TL::EDM::Lepton& lep) { return lep.isManTrigMatched(); });
  // jets are nominally sorted by pt at the sgtop ntuple level
  // this should be used only if that changes in the future
  // as of december 2018 v25 ntuples they are still sorted
  if (sort_jets) {
    std::sort(m_jets.begin(), m_jets.end(),
              [](const TL::EDM::Jet& jet1, const TL::EDM::Jet& jet2) {
                return jet1.pt() > jet2.pt();
              });
  }

  makeLeptonPairs();
  if (m_leptons.size() > 10) {
    logger()->warn(
        "Lepton container size has grown to over 10! "
        "You probably forgot to call TL::EDM::FinalState::reset()");
  }
}

std::size_t TL::EDM::FinalState::nbjets(const TL::EDM::BTagWP wp) const {
  return TL::EDM::FinalState::nbjets(m_jets, wp);
}

std::size_t TL::EDM::FinalState::nbjets(const std::vector<TL::EDM::Jet>& container,
                                        const TL::EDM::BTagWP wp) {
  std::function<char(const Jet&)> isbtagged;
  if (wp == BTagWP::mv2c10_70) {
    isbtagged = &Jet::isbtagged_MV2c10_70;
  }
  else if (wp == BTagWP::mv2c10_77) {
    isbtagged = &Jet::isbtagged_MV2c10_77;
  }
  else if (wp == BTagWP::mv2c10_85) {
    isbtagged = &Jet::isbtagged_MV2c10_85;
  }
  else if (wp == BTagWP::mv2c10_PC) {
    spdlog::get("TL::EDM::FinalState")
        ->warn(
            "BTagWP::mv2c10_PC not supported with this nbjets() overload. "
            "You should use overload for BTagBin. Returning 0");
    return 0;
  }
  else {
    spdlog::get("TL::EDM::FinalState")
        ->warn("BTagWP supplied to nbjets() doesn't exist. Returning 0");
    return 0;
  }
  return std::count_if(container.begin(), container.end(),
                       [&](const TL::EDM::Jet& a) { return isbtagged(a); });
}

std::size_t TL::EDM::FinalState::nbjets(const std::vector<TL::EDM::Jet>& container,
                                        const TL::EDM::BTagBin bin_req) {
  return std::count_if(container.begin(), container.end(), [&](const TL::EDM::Jet& a) {
    return a.isbtaggedContinuous(bin_req);
  });
}

std::size_t TL::EDM::FinalState::mostForwardJetIdx() const {
  if (jets().empty()) {
    logger()->warn("Asking for forward jet index but no jets! return 0");
    return 0;
  }
  if (jets().size() == 1) {
    return 0;
  }
  auto eta_compare = [](const TL::EDM::Jet& j1, const TL::EDM::Jet& j2) {
    return (j1.aeta() < j2.aeta());
  };
  auto res = std::max_element(jets().begin(), jets().end(), eta_compare);
  auto idx = std::distance(jets().begin(), res);
  if (idx < 0) {
    logger()->warn("mostForwardJetIdx yielding negative index: {}", idx);
  }
  return idx;
}

void TL::EDM::FinalState::reset() {
  m_hasFakeElectronMC = false;
  m_hasFakeMuonMC = false;
  m_hasManTrigMatched = false;
  m_leptons.clear();
  m_jets.clear();
  m_looseJets.clear();
  m_leptonPairs.clear();
  m_electrons.clear();
  m_muons.clear();
  m_missingET.reset();
}

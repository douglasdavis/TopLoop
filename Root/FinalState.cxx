/** @file FinalState.cxx
 *  @brief TL::EDM::FinalState class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/EDM/FinalState.h>

TL::EDM::FinalState::FinalState() : TL::Loggable("TL::EDM::FinalState") {}

void TL::EDM::FinalState::makeLeptonPairs() {
  for ( std::size_t i = 0; i < m_leptons.size(); ++i ) {
    for ( std::size_t j = (i+1); j < m_leptons.size(); ++j ) {
      TL::EDM::LeptonPair lp(m_leptons.at(i),m_leptons.at(j),i,j);
      addLeptonPair(lp);
    }
  }
}

void TL::EDM::FinalState::evaluateSelf(bool sort_leptons) {
  m_hasFakeElectronMC = false;
  m_hasFakeMuonMC     = false;
  for ( const TL::EDM::Electron& el : m_electrons ) {
    addLepton(el);
    if ( el.isMCfake() ) {
      m_hasFakeElectronMC = true;
    }
  }
  for ( const TL::EDM::Muon& mu : m_muons ) {
    addLepton(mu);
    if ( mu.isMCfake() ) {
      m_hasFakeMuonMC = true;
    }
  }
  if ( sort_leptons ) {
    std::sort(m_leptons.begin(),m_leptons.end(),
              [](const TL::EDM::Lepton& lep1, const TL::EDM::Lepton& lep2) {
                return (lep1.pT() > lep2.pT());
              });
  }
  makeLeptonPairs();
  if ( m_leptons.size() > 10 ) {
    logger()->warn("Lepton container size has grown to over 10! "
                   "You probably forgot to call TL::EDM::FinalState::reset()");
  }
}

void TL::EDM::FinalState::reset() {
  m_leptons.clear();
  m_jets.clear();
  m_leptonPairs.clear();
  m_electrons.clear();
  m_muons.clear();
  m_missingET.reset();
}

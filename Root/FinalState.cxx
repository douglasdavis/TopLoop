/** @file FinalState.cxx
 *  @brief TL::EDM::FinalState class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

// TL
#include <TopLoop/EDM/FinalState.h>

void TL::EDM::FinalState::evaluateLepPairs() {
  for ( std::size_t i = 0; i < m_leptons.size(); ++i ) {
    for ( std::size_t j = (i+1); j < m_leptons.size(); ++j ) {
      TL::EDM::LeptonPair lp(m_leptons.at(i),m_leptons.at(j),i,j);
      addLeptonPair(lp);
    }
  }
}

void TL::EDM::FinalState::evaluateSelf(bool sort_leptons) {
  if ( sort_leptons ) {
    std::sort(m_leptons.begin(),m_leptons.end(),
              [](const auto& lep1, const auto& lep2) { return (lep1.pT() > lep2.pT()); });
  }
  evaluateLepPairs();
  m_HT = 0.0;
  TLorentzVector eventFourVector;
  eventFourVector.SetPxPyPzE(0,0,0,0);
  for ( auto const& lep : m_leptons ) {
    eventFourVector += lep.p();
    m_HT += lep.pT();
  }
  for ( auto const& jet : m_jets ) {
    eventFourVector += jet.p();
    m_HT += jet.pT();
  }
  eventFourVector += m_MET.p();
  m_M = eventFourVector.M();

}

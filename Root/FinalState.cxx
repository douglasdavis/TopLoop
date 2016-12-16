/** @file FinalState.cxx
 *  @brief TL::EDM::FinalState class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/EDM/FinalState.h>

void TL::EDM::FinalState::evaluateLepPairs() {
  if  ( m_leptons.size() > 1 ) {
    TL::EDM::LeptonPair lp(m_leptons.at(0),
                           m_leptons.at(1),0,1);
    addLeptonPair(lp);
    if ( m_leptons.size() > 2 ) {
      TL::EDM::LeptonPair lp2(m_leptons.at(0),
                              m_leptons.at(2),0,2);
      TL::EDM::LeptonPair lp3(m_leptons.at(1),
                              m_leptons.at(2),1,2);
      addLeptonPairs({lp2,lp3});
      if ( m_leptons.size() > 3 ) {
        TL::EDM::LeptonPair lp4(m_leptons.at(0),
                                m_leptons.at(3),0,3);
        TL::EDM::LeptonPair lp5(m_leptons.at(1),
                                m_leptons.at(3),1,3);
        TL::EDM::LeptonPair lp6(m_leptons.at(2),
                                m_leptons.at(3),2,3);
        addLeptonPairs({lp4,lp5,lp6});
      }
    }
  }
}

void TL::EDM::FinalState::evaluateSelf() {
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
  m_M   = eventFourVector.M();

  m_llidx = 0;
  if ( !m_leptons.empty() ) {
    auto llpt = m_leptons.at(0).pT();
    for ( std::size_t i = 0; i < m_leptons.size(); ++i ) {
      auto cur_pt = m_leptons.at(i).pT();
      if ( cur_pt > llpt ) {
        llpt = cur_pt;
        m_llidx = i;
      }
      else {
        continue;
      }
    }
  }
  //  According to Kerim S. jets will always be in order of leading pT
  //  down.
  m_ljidx = 0;

  /*
    if ( !m_jets.empty() ) {
    m_ljidx = 0;
    auto ljpt = m_jets.at(0).pT();
    for ( std::size_t i = 0; i < m_jets.size(); ++i ) {
    auto cur_pt = m_jets.at(i).pT();
    if ( cur_pt > ljpt ) {
    ljpt = cur_pt;
    m_ljidx = i;
    }
    else { continue; }
    }
    }  
  */
}

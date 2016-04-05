/** @file LeptonPair.cxx
 *  @brief TL::EDM::LeptonPair class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoopEDM/LeptonPair.h>
#include <TopLoop/Utils.h>

TL::EDM::LeptonPair::LeptonPair() : TL::EDM::PhysicsObject() {}

TL::EDM::LeptonPair::LeptonPair(const TL::EDM::Lepton& lep1,
				const TL::EDM::Lepton& lep2,
				const size_t idxf, const size_t idxs) :
  TL::EDM::PhysicsObject()
{
  m_idxp = std::make_pair(idxf,idxs);
  m_p = lep1.p() + lep2.p();
  m_deltaR   = lep1.p().DeltaR(lep2.p());
  m_deltaEta = lep1.p().Eta() - lep2.p().Eta();
  m_deltaPhi = lep1.p().DeltaPhi(lep2.p());

  auto pdgsum    = lep1.pdgId() + lep2.pdgId();
  auto chargesum = std::abs(lep1.charge() + lep2.charge());

  if ( pdgsum == 26 ) {
    m_mumu = true;
    m_elel = false;
    m_elmu = false;
  }
  else if ( pdgsum == 24 ) {
    m_mumu = false;
    m_elmu = true;
    m_elel = false;
  }
  else if ( pdgsum == 22 ) {
    m_mumu = false;
    m_elmu = false;
    m_elel = true;
  }
  else {
    TL::Warning("LeptonPair()","Bad PDG sum!",pdgsum);
  }

  if ( chargesum == 0 ) {
    m_OS = true;
    m_SS = false;
  }
  else if ( chargesum == 2 ) {
    m_SS = true;
    m_OS = false;
  }
  else {
    TL::Warning("LeptonPair()","Unexpected charge sum!",chargesum);
  }
}

TL::EDM::LeptonPair::~LeptonPair() {}

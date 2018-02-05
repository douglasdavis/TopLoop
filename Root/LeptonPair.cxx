/** @file LeptonPair.cxx
 *  @brief TL::EDM::LeptonPair class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

// TL
#include <TopLoop/EDM/LeptonPair.h>
#include <TopLoop/spdlog/spdlog.h>

TL::EDM::LeptonPair::LeptonPair(const TL::EDM::Lepton& lep1,
                                const TL::EDM::Lepton& lep2,
                                const size_t idxf, const size_t idxs) :
  TL::EDM::PhysicsObject() {
  m_fIdx = idxf;
  m_sIdx = idxs;
  m_p = lep1.p4() + lep2.p4();
  m_deltaR   = lep1.p4().DeltaR(lep2.p4());
  m_deltaEta = lep1.p4().Eta() - lep2.p4().Eta();
  m_deltaPhi = lep1.p4().DeltaPhi(lep2.p4());

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
    if ( spdlog::get("TL::EDM::LeptonPair") == nullptr ) {
      spdlog::stdout_color_mt("TL::EDM::LeptonPair");
    }
    else {
      spdlog::get("TL::EDM::LeptonPair")->warn("Bad pdgsum: {}",pdgsum);
    }
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
    if ( spdlog::get("TL::EDM::LeptonPair") == nullptr ) {
      spdlog::stdout_color_mt("TL::EDM::LeptonPair");
    }
    else {
      spdlog::get("TL::EDM::LeptonPair")->warn("Bad charge sum: {}",chargesum);
    }
  }
}

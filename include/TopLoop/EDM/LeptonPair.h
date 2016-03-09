/** @file  LeptonPair.h
 *  @brief TL::EDM::LeptonPair class header
 *  @class TL::EDM::LeptonPair
 *  @brief A container for lepton-lepton information.
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a pair of leptons.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_EDM_LeptonPair_h
#define TL_EDM_LeptonPair_h

// TL
#include <TopLoop/EDM/Lepton.h>

// C++
#include <cmath>
#include <iostream>

namespace TL {
  namespace EDM {

    class LeptonPair : public TL::EDM::PhysicsObject {
    private:

      double m_deltaR;
      double m_deltaPhi;
      double m_deltaEta;
      bool   m_SS;
      bool   m_OS;
      bool   m_elel;
      bool   m_mumu;
      bool   m_elmu;
      
      ClassDef(LeptonPair,1);
      
    public:
      
      LeptonPair() {}
      LeptonPair(const TL::EDM::Lepton& lep1,
		 const TL::EDM::Lepton& lep2) {
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
	  std::cout << "Unexpected pdg sum" << std::endl;
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
	  std::cout << "Unexpected charge sum" << std::endl;
	}
      }

      virtual ~LeptonPair() {}

      double deltaR()   const;
      double deltaPhi() const;
      double deltaEta() const;

      bool SS()   const;
      bool OS()   const;
      bool mumu() const;
      bool elel() const;
      bool elmu() const;
      
    };

  }
}

inline double TL::EDM::LeptonPair::deltaR()   const { return m_deltaR;   }
inline double TL::EDM::LeptonPair::deltaPhi() const { return m_deltaPhi; }
inline double TL::EDM::LeptonPair::deltaEta() const { return m_deltaEta; }

inline bool TL::EDM::LeptonPair::SS()   const { return m_SS;   }
inline bool TL::EDM::LeptonPair::OS()   const { return m_OS;   }
inline bool TL::EDM::LeptonPair::mumu() const { return m_mumu; }
inline bool TL::EDM::LeptonPair::elel() const { return m_elel; }
inline bool TL::EDM::LeptonPair::elmu() const { return m_elmu; }

#endif

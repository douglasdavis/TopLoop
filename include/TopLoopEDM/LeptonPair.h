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
#include <TopLoopEDM/Lepton.h>

// C++
#include <cmath>
#include <iostream>
#include <utility>

namespace TL {
  namespace EDM {

    class LeptonPair : public TL::EDM::PhysicsObject {
    private:

      float  m_deltaR;
      float  m_deltaPhi;
      float  m_deltaEta;
      bool   m_SS;
      bool   m_OS;
      bool   m_elel;
      bool   m_mumu;
      bool   m_elmu;

      std::pair<size_t,size_t> m_idxp;
      
      ClassDef(LeptonPair,1);
      
    public:
      
      LeptonPair();
      LeptonPair(const TL::EDM::Lepton& lep1,
		 const TL::EDM::Lepton& lep2,
		 const size_t idxf, const size_t idxs);
      virtual ~LeptonPair();

      float deltaR()   const;
      float deltaPhi() const;
      float deltaEta() const;

      bool SS()   const;
      bool OS()   const;
      bool mumu() const;
      bool elel() const;
      bool elmu() const;

      const std::pair<size_t,size_t>& indexPair() const;

      size_t firstIdx()  const;
      size_t secondIdx() const;
      
    };

  }
}

inline float TL::EDM::LeptonPair::deltaR()   const { return m_deltaR;   }
inline float TL::EDM::LeptonPair::deltaPhi() const { return m_deltaPhi; }
inline float TL::EDM::LeptonPair::deltaEta() const { return m_deltaEta; }

inline bool TL::EDM::LeptonPair::SS()   const { return m_SS;   }
inline bool TL::EDM::LeptonPair::OS()   const { return m_OS;   }
inline bool TL::EDM::LeptonPair::mumu() const { return m_mumu; }
inline bool TL::EDM::LeptonPair::elel() const { return m_elel; }
inline bool TL::EDM::LeptonPair::elmu() const { return m_elmu; }

inline const std::pair<size_t,size_t>& TL::EDM::LeptonPair::indexPair() const { return m_idxp; }
inline size_t TL::EDM::LeptonPair::firstIdx()  const { return m_idxp.first;  }
inline size_t TL::EDM::LeptonPair::secondIdx() const { return m_idxp.second; }

#endif

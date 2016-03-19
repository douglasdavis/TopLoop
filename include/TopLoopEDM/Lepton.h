/** @file  Lepton.h
 *  @brief TL::EDM::Lepton class header
 *  @class TL::EDM::Lepton
 *  @brief A container for electron/muon information.
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a lepton.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_EDM_Lepton_h
#define TL_EDM_Lepton_h

// TL
#include <TopLoopEDM/PhysicsObject.h>

namespace TL {
  namespace EDM {

    class Lepton : public TL::EDM::PhysicsObject {
    private:
      unsigned int   m_pdgId;
      int            m_charge;
      
      ClassDef(Lepton,1);
      
    public:
      Lepton() : TL::EDM::PhysicsObject(), m_pdgId(0), m_charge(-99) {}
      virtual ~Lepton() {}

      void set_pdgId(const unsigned int i);
      void set_charge(const int i);

      unsigned int pdgId()  const;
      int          charge() const;
      
    };

  }
}

inline void TL::EDM::Lepton::set_pdgId(const unsigned int i) { m_pdgId  = i; }
inline void TL::EDM::Lepton::set_charge(const int i)         { m_charge = i; }

inline unsigned int TL::EDM::Lepton::pdgId()  const { return m_pdgId;  }
inline int          TL::EDM::Lepton::charge() const { return m_charge; }

#endif

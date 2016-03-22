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
      float          m_cl_eta;          // for electrons;
      float          m_topoetcone20;
      float          m_ptvarcone20;     // for electrons;
      float          m_ptvarcone30;     // for muons;
      float          m_d0sig;
      float          m_delta_z0_sintheta;
      
      ClassDef(Lepton,1);
      
    public:
      Lepton() : TL::EDM::PhysicsObject(), m_pdgId(0), m_charge(-99999),
		 m_cl_eta(-99999), m_topoetcone20(-99999), m_ptvarcone20(-99999),
		 m_ptvarcone30(-99999), m_d0sig(-99999), m_delta_z0_sintheta(-99999) {}
      virtual ~Lepton() {}

      void set_pdgId(const unsigned int val);
      void set_charge(const int val);
      void set_cl_eta(const float val);
      void set_topoetcone20(const float val);
      void set_ptvarcone20(const float val);
      void set_ptvarcone30(const float val);
      void set_d0sig(const float val);
      void set_delta_z0_sintheta(const float val);

      unsigned int pdgId()             const;
      int          charge()            const;
      float        cl_eta()            const;
      float        topoetcone20()      const;
      float        ptvarcone20()       const;
      float        ptvarcone30()       const;
      float        d0sig()             const;
      float        delta_z0_sintheta() const;
      
    };

  }
}

inline void TL::EDM::Lepton::set_pdgId(const unsigned int val)      { m_pdgId             = val; }
inline void TL::EDM::Lepton::set_charge(const int val)              { m_charge            = val; }
inline void TL::EDM::Lepton::set_cl_eta(const float val)            { m_cl_eta            = val; }
inline void TL::EDM::Lepton::set_topoetcone20(const float val)      { m_topoetcone20      = val; }
inline void TL::EDM::Lepton::set_ptvarcone20(const float val)       { m_ptvarcone20       = val; }
inline void TL::EDM::Lepton::set_ptvarcone30(const float val)       { m_ptvarcone30       = val; }
inline void TL::EDM::Lepton::set_d0sig(const float val)             { m_d0sig             = val; }
inline void TL::EDM::Lepton::set_delta_z0_sintheta(const float val) { m_delta_z0_sintheta = val; }

inline unsigned int TL::EDM::Lepton::pdgId()             const { return m_pdgId;             }
inline int          TL::EDM::Lepton::charge()            const { return m_charge;            }
inline float        TL::EDM::Lepton::cl_eta()            const { return m_cl_eta;            }
inline float        TL::EDM::Lepton::topoetcone20()      const { return m_topoetcone20;      }
inline float        TL::EDM::Lepton::ptvarcone20()       const { return m_ptvarcone20;       }
inline float        TL::EDM::Lepton::ptvarcone30()       const { return m_ptvarcone30;       }
inline float        TL::EDM::Lepton::d0sig()             const { return m_d0sig;             }
inline float        TL::EDM::Lepton::delta_z0_sintheta() const { return m_delta_z0_sintheta; }

#endif

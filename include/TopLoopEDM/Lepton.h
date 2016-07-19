/** @file  Lepton.h
 *  @brief TL::EDM::Lepton class header
 *  @class TL::EDM::Lepton
 *  @brief A container for electron/muon information.
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a lepton.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
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
      float          m_cl_eta;          //!< for electrons;
      float          m_topoetcone20;
      float          m_ptvarcone20;     //!< for electrons;
      float          m_ptvarcone30;     //!< for muons;
      float          m_d0sig;
      float          m_delta_z0_sintheta;
      char           m_el_trigMatch_HLT_e60_lhmedium;
      char           m_el_trigMatch_HLT_e24_lhmedium_L1EM18VH;
      char           m_el_trigMatch_HLT_e120_lhloose;
      char           m_mu_trigMatch_HLT_mu50;
      char           m_mu_trigMatch_HLT_mu20_iloose_L1MU15;
      
      ClassDef(Lepton,1);
      
    public:
      Lepton() : TL::EDM::PhysicsObject(), m_pdgId(0), m_charge(-99999),
                 m_cl_eta(-99999), m_topoetcone20(-99999), m_ptvarcone20(-99999),
                 m_ptvarcone30(-99999), m_d0sig(-99999), m_delta_z0_sintheta(-99999),
                 m_el_trigMatch_HLT_e60_lhmedium(0), m_el_trigMatch_HLT_e24_lhmedium_L1EM18VH(0),
                 m_el_trigMatch_HLT_e120_lhloose(0),m_mu_trigMatch_HLT_mu50(0),
                 m_mu_trigMatch_HLT_mu20_iloose_L1MU15(0) {}
      virtual ~Lepton() {}

      void set_pdgId(const unsigned int val);
      void set_charge(const int val);
      void set_cl_eta(const float val);
      void set_topoetcone20(const float val);
      void set_ptvarcone20(const float val);
      void set_ptvarcone30(const float val);
      void set_d0sig(const float val);
      void set_delta_z0_sintheta(const float val);
      void set_el_trigMatch_HLT_e60_lhmedium(const char val);
      void set_el_trigMatch_HLT_e24_lhmedium_L1EM18VH(const char val);
      void set_el_trigMatch_HLT_e120_lhloose(const char val);
      void set_mu_trigMatch_HLT_mu50(const char val);
      void set_mu_trigMatch_HLT_mu20_iloose_L1MU15(const char val);
      
      unsigned int pdgId()             const;
      int          charge()            const;
      float        cl_eta()            const;
      float        topoetcone20()      const;
      float        ptvarcone20()       const;
      float        ptvarcone30()       const;
      float        d0sig()             const;
      float        delta_z0_sintheta() const;

      char el_trigMatch_HLT_e60_lhmedium()          const;
      char el_trigMatch_HLT_e24_lhmedium_L1EM18VH() const;
      char el_trigMatch_HLT_e120_lhloose()          const;
      char mu_trigMatch_HLT_mu50()                  const;
      char mu_trigMatch_HLT_mu20_iloose_L1MU15()    const;
      
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

inline void TL::EDM::Lepton::set_el_trigMatch_HLT_e60_lhmedium(const char val) {
  m_el_trigMatch_HLT_e60_lhmedium = val;
}
inline void TL::EDM::Lepton::set_el_trigMatch_HLT_e24_lhmedium_L1EM18VH(const char val) {
  m_el_trigMatch_HLT_e24_lhmedium_L1EM18VH = val;
}
inline void TL::EDM::Lepton::set_el_trigMatch_HLT_e120_lhloose(const char val) {
  m_el_trigMatch_HLT_e120_lhloose = val;
}
inline void TL::EDM::Lepton::set_mu_trigMatch_HLT_mu50(const char val) {
  m_mu_trigMatch_HLT_mu50 = val;
}
inline void TL::EDM::Lepton::set_mu_trigMatch_HLT_mu20_iloose_L1MU15(const char val) {
  m_mu_trigMatch_HLT_mu20_iloose_L1MU15 = val;
}

inline unsigned int TL::EDM::Lepton::pdgId()             const { return m_pdgId;             }
inline int          TL::EDM::Lepton::charge()            const { return m_charge;            }
inline float        TL::EDM::Lepton::cl_eta()            const { return m_cl_eta;            }
inline float        TL::EDM::Lepton::topoetcone20()      const { return m_topoetcone20;      }
inline float        TL::EDM::Lepton::ptvarcone20()       const { return m_ptvarcone20;       }
inline float        TL::EDM::Lepton::ptvarcone30()       const { return m_ptvarcone30;       }
inline float        TL::EDM::Lepton::d0sig()             const { return m_d0sig;             }
inline float        TL::EDM::Lepton::delta_z0_sintheta() const { return m_delta_z0_sintheta; }

inline char TL::EDM::Lepton::el_trigMatch_HLT_e60_lhmedium()          const {
  return m_el_trigMatch_HLT_e60_lhmedium;
}
inline char TL::EDM::Lepton::el_trigMatch_HLT_e24_lhmedium_L1EM18VH() const {
  return m_el_trigMatch_HLT_e24_lhmedium_L1EM18VH;
}
inline char TL::EDM::Lepton::el_trigMatch_HLT_e120_lhloose()          const {
  return m_el_trigMatch_HLT_e120_lhloose;
}
inline char TL::EDM::Lepton::mu_trigMatch_HLT_mu50()                  const {
  return m_mu_trigMatch_HLT_mu50;
}
inline char TL::EDM::Lepton::mu_trigMatch_HLT_mu20_iloose_L1MU15()    const {
  return m_mu_trigMatch_HLT_mu20_iloose_L1MU15;
}

#endif

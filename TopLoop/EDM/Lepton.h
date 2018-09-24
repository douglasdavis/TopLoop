/** @file  Lepton.h
 *  @brief TL::EDM::Lepton class header
 *  @class TL::EDM::Lepton
 *  @brief A container for electron/muon information.
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a lepton. Variables that are shared by
 *  both electrons and muons are used here. Variables specific to each
 *  respective flavor of lepton are in the dedicated class for each.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_EDM_Lepton_h
#define TL_EDM_Lepton_h

// TL
#include <TopLoop/EDM/PhysicsObject.h>

// ATLAS
#include "MCTruthClassifier/MCTruthClassifierDefs.h"

namespace TL {
  namespace EDM {

    class Lepton : public TL::EDM::PhysicsObject {
    private:
      unsigned int m_pdgId;
      float        m_charge;
      float        m_topoetcone20;
      float        m_d0sig;
      float        m_delta_z0_sintheta;
      int          m_true_type;
      int          m_true_origin;
      char         m_true_isPrompt;
      float        m_e_branch;
      char         m_isTight;

      bool         m_isManTrigMatched;

    public:
      /// default constructor
      Lepton() = default;
      /// default destructor
      virtual ~Lepton() = default;
      /// default copy constructor
      Lepton(const Lepton&) = default;
      /// default assignment operator
      Lepton& operator=(const Lepton&) = default;
      /// default move copy constructor
      Lepton(Lepton&&) = default;
      /// default move assignment operator
      Lepton& operator=(Lepton&&) = default;

      /// constructor defining pdgId
      /**
       *  This is mainly for the Electron and Muon classes which
       *  inherit from Lepton
       *
       *  @param pdgId the PDG code
       */
      Lepton(const unsigned int pdgId) : m_pdgId(pdgId) {}

      /// @name setters
      /// @{

      void set_pdgId(const unsigned int val);
      void set_charge(const float val);
      void set_topoetcone20(const float val);
      void set_d0sig(const float val);
      void set_delta_z0_sintheta(const float val);
      void set_true_type(const int val);
      void set_true_origin(const int val);
      void set_true_isPrompt(const char val);
      void set_isTight(const char val);

      /// allows user to manually flag a lepton as trigger matched
      void set_isManTrigMatched(const bool val);

      /// Value from the energy branch that AnalysisTop ntuples include
      /**
       *  a branch for the reconstructed energy. The energy in the
       *  _four vector_ of this object will be calculated using the
       *  (pT, eta, phi, m) information. the "e_branch" variable is
       *  separate.
       */
      void set_e_branch(const float val);

      /// @}

      /// @name getters
      /// @{

      unsigned int pdgId()             const;
      float        e_branch()          const;
      float        charge()            const;
      float        topoetcone20()      const;
      float        d0sig()             const;
      float        delta_z0_sintheta() const;
      int          true_type()         const;
      int          true_origin()       const;
      char         true_isPrompt()     const;
      char         isTight()           const;

      bool         isManTrigMatched()  const;

      /// @}

    };
  }
}

inline void TL::EDM::Lepton::set_pdgId(const unsigned int val)      { m_pdgId             = val; }
inline void TL::EDM::Lepton::set_e_branch(const float val)          { m_e_branch          = val; }
inline void TL::EDM::Lepton::set_charge(const float val)            { m_charge            = val; }
inline void TL::EDM::Lepton::set_topoetcone20(const float val)      { m_topoetcone20      = val; }
inline void TL::EDM::Lepton::set_d0sig(const float val)             { m_d0sig             = val; }
inline void TL::EDM::Lepton::set_delta_z0_sintheta(const float val) { m_delta_z0_sintheta = val; }
inline void TL::EDM::Lepton::set_true_type(const int val)           { m_true_type         = val; }
inline void TL::EDM::Lepton::set_true_origin(const int val)         { m_true_origin       = val; }
inline void TL::EDM::Lepton::set_true_isPrompt(const char val)      { m_true_isPrompt     = val; }
inline void TL::EDM::Lepton::set_isTight(const char val)            { m_isTight           = val; }

inline void TL::EDM::Lepton::set_isManTrigMatched(const bool val)   { m_isManTrigMatched  = val; }

inline unsigned int TL::EDM::Lepton::pdgId()             const { return m_pdgId;             }
inline float        TL::EDM::Lepton::e_branch()          const { return m_e_branch;          }
inline float        TL::EDM::Lepton::charge()            const { return m_charge;            }
inline float        TL::EDM::Lepton::topoetcone20()      const { return m_topoetcone20;      }
inline float        TL::EDM::Lepton::d0sig()             const { return m_d0sig;             }
inline float        TL::EDM::Lepton::delta_z0_sintheta() const { return m_delta_z0_sintheta; }
inline int          TL::EDM::Lepton::true_type()         const { return m_true_type;         }
inline int          TL::EDM::Lepton::true_origin()       const { return m_true_origin;       }
inline char         TL::EDM::Lepton::true_isPrompt()     const { return m_true_isPrompt;     }
inline char         TL::EDM::Lepton::isTight()           const { return m_isTight;           }

inline bool         TL::EDM::Lepton::isManTrigMatched()  const { return m_isManTrigMatched;  }

#endif

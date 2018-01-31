/** @file  Electron.h
 *  @brief TL::EDM::Electron class header
 *  @class TL::EDM::Electron
 *  @brief A container for electron information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a electron
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef TL_EDM_Electron_h
#define TL_EDM_Electron_h

#include <TopLoop/EDM/Lepton.h>

namespace TL {
  namespace EDM {

    class Electron : public TL::EDM::Lepton {
    private:
      float m_ptvarcone20;
      char  m_CF;
      float m_cl_eta;
      int   m_true_originbkg;
      int   m_true_typebkg;

    public:
      Electron() : TL::EDM::Lepton() {
        set_pdgId(11);
      }
      virtual ~Electron() = default;

      Electron& operator=(const Electron&) = default;
      Electron(const Electron&) = default;

      /// @name setters
      /// @{

      void set_ptvarcone20(const float val);
      void set_CF(const char val);
      void set_cl_eta(const float val);
      void set_true_originbkg(const int val);
      void set_true_typebkg(const int val);

      /// @}

      /// @name getters
      /// @{

      float ptvarcone20()    const;
      char  CF()             const;
      float cl_eta()         const;
      int   true_originbkg() const;
      int   true_typebkg()   const;

      bool isMCfake() const;

      /// @}

    };

  }
}

inline void TL::EDM::Electron::set_ptvarcone20(const float val)  { m_ptvarcone20    = val; }
inline void TL::EDM::Electron::set_CF(const char val)            { m_CF             = val; }
inline void TL::EDM::Electron::set_cl_eta(const float val)       { m_cl_eta         = val; }
inline void TL::EDM::Electron::set_true_originbkg(const int val) { m_true_originbkg = val; }
inline void TL::EDM::Electron::set_true_typebkg(const int val)   { m_true_typebkg   = val; }

inline float TL::EDM::Electron::ptvarcone20()    const { return m_ptvarcone20;    }
inline char  TL::EDM::Electron::CF()             const { return m_CF;             }
inline float TL::EDM::Electron::cl_eta()         const { return m_cl_eta;         }
inline int   TL::EDM::Electron::true_originbkg() const { return m_true_originbkg; }
inline int   TL::EDM::Electron::true_typebkg()   const { return m_true_typebkg;   }

inline bool TL::EDM::Electron::isMCfake() const {
  if ( true_type() == MCTruthPartClassifier::ParticleType::IsoElectron ) return false;
  if ( true_type() == MCTruthPartClassifier::ParticleType::BkgElectron &&
       true_origin() == MCTruthPartClassifier::ParticleOrigin::PhotonConv &&
       ( true_originbkg() == MCTruthPartClassifier::ParticleOrigin::TauLep ||
         true_originbkg() == MCTruthPartClassifier::ParticleOrigin::ZBoson ||
         true_originbkg() == MCTruthPartClassifier::ParticleOrigin::WBoson ) ) return false;
  return true;
}

#endif

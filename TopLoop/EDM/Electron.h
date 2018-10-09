/** @file  Electron.h
 *  @brief TL::EDM::Electron class header
 *  @class TL::EDM::Electron
 *  @brief A class for electron information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a electron
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
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

      bool m_isManualTruthPrompt{false};

    public:
      /// default constructor
      Electron() : TL::EDM::Lepton(11) {}
      /// default destructor
      virtual ~Electron() = default;
      /// default copy constructor
      Electron(const Electron&) = default;
      /// default assignment operator
      Electron& operator=(const Electron&) = default;
      /// default move constructor
      Electron(Electron&&) = default;
      /// default move assignment operator
      Electron& operator=(Electron&&) = default;

      /// @name setters
      /// @{

      void set_ptvarcone20(const float val) {
        m_ptvarcone20 = val;
      }
      void set_CF(const char val) {
        m_CF = val;
      }
      void set_cl_eta(const float val) {
        m_cl_eta = val;
      }

      /// manually define if prompt
      /**
       *  This function allows a user to manually tag an electron as
       *  prompt based on some selection they define.
       */
      void set_isManualTruthPrompt(const bool val) {
        m_isManualTruthPrompt = val;
      }

      /// @}

      /// @name getters
      /// @{

      float ptvarcone20()    const { return m_ptvarcone20; }
      char  CF()             const { return m_CF; }
      float cl_eta()         const { return m_cl_eta; }

      /// if the electron is matched to an IsoElectron in MC
      bool isTruthPrompt() const {
        return true_type() == MCTruthPartClassifier::ParticleType::IsoElectron;
      }

      /// using a mix of variables, determine promptness (for 20.7)
      bool isManualTruthPrompt() const { return m_isManualTruthPrompt; }

      /// @}

    };

  }
}

#endif

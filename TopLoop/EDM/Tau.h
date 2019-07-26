/** @file  Tau.h
 *  @brief TL::EDM::Tau class header
 *  @class TL::EDM::Tau
 *  @brief A class for tau information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a tau
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_EDM_Tau_h
#define TL_EDM_Tau_h

#include <TopLoop/EDM/Lepton.h>

namespace TL {
namespace EDM {

class Tau : public TL::EDM::Lepton {
 private:
  float m_ptvarcone30;

  bool m_isManualTruthPrompt{false};

 public:
  /// default constructor
  Tau() : TL::EDM::Lepton(15) {}
  /// default destructor
  virtual ~Tau() = default;
  /// default copy constructor
  Tau(const Tau&) = default;
  /// default assignement operator
  Tau& operator=(const Tau&) = default;
  /// default move copy constructor
  Tau(Tau&&) = default;
  /// default move assignment operator
  Tau& operator=(Tau&&) = default;

  /// @name setters
  /// @{

  void set_ptvarcone30(const float val) { m_ptvarcone30 = val; }

  void set_isManualTruthPrompt(const bool val) { m_isManualTruthPrompt = val; }
  /// @}

  /// @name getters
  /// @{

  float ptvarcone30() const { return m_ptvarcone30; }

  /// if the tau is matched to an IsoTau in MC
  bool isTruthPrompt() const {
    return true_type() == MCTruthPartClassifier::ParticleType::IsoTau;
  }

  /// if the manual promptness is flag
  bool isManualTruthPrompt() const { return m_isManualTruthPrompt; }

  /// @}
};

}  // namespace EDM
}  // namespace TL

#endif

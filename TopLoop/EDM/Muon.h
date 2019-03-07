/** @file  Muon.h
 *  @brief TL::EDM::Muon class header
 *  @class TL::EDM::Muon
 *  @brief A class for muon information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a muon
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_EDM_Muon_h
#define TL_EDM_Muon_h

#include <TopLoop/EDM/Lepton.h>

namespace TL {
namespace EDM {

class Muon : public TL::EDM::Lepton {
 private:
  float m_ptvarcone30;

  bool m_isManualTruthPrompt{false};

 public:
  /// default constructor
  Muon() : TL::EDM::Lepton(13) {}
  /// default destructor
  virtual ~Muon() = default;
  /// default copy constructor
  Muon(const Muon&) = default;
  /// default assignement operator
  Muon& operator=(const Muon&) = default;
  /// default move copy constructor
  Muon(Muon&&) = default;
  /// default move assignment operator
  Muon& operator=(Muon&&) = default;

  /// @name setters
  /// @{

  void set_ptvarcone30(const float val) { m_ptvarcone30 = val; }

  void set_isManualTruthPrompt(const bool val) { m_isManualTruthPrompt = val; }
  /// @}

  /// @name getters
  /// @{

  float ptvarcone30() const { return m_ptvarcone30; }

  /// if the muon is matched to an IsoMuon in MC
  bool isTruthPrompt() const {
    return true_type() == MCTruthPartClassifier::ParticleType::IsoMuon;
  }

  /// if the manual promptness is flag
  bool isManualTruthPrompt() const { return m_isManualTruthPrompt; }

  /// @}
};

}  // namespace EDM
}  // namespace TL

#endif

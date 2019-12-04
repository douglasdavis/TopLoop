/** @file  Lepton.h
 *  @brief TL::EDM::Lepton class header
 *  @class TL::EDM::Lepton
 *  @brief A container for electron/muon/tau information.
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a lepton. Variables that are shared by
 *  both electrons, muons and taus are used here. Variables specific to each
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
  float m_charge;
  float m_topoetcone20;
  float m_d0sig;
  float m_delta_z0_sintheta;
  int m_true_type;
  int m_true_origin;
  char m_true_isPrompt;
  float m_e_branch;
  char m_isTight;

  bool m_isManTrigMatched;

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
   *  This is mainly for the Electron, Muon and Tau classes which
   *  inherit from Lepton
   *
   *  @param pdgId the PDG code
   */
  Lepton(const unsigned int pdgId) : m_pdgId(pdgId) {}

  /// @name setters
  /// @{

  void set_pdgId(const unsigned int val) { m_pdgId = val; }
  void set_charge(const float val) { m_charge = val; }
  void set_topoetcone20(const float val) { m_topoetcone20 = val; }
  void set_d0sig(const float val) { m_d0sig = val; }
  void set_delta_z0_sintheta(const float val) { m_delta_z0_sintheta = val; }
  void set_true_type(const int val) { m_true_type = val; }
  void set_true_origin(const int val) { m_true_origin = val; }
  void set_true_isPrompt(const char val) { m_true_isPrompt = val; }
  void set_isTight(const char val) { m_isTight = val; }

  /// allows user to manually flag a lepton as trigger matched
  void set_isManTrigMatched(const bool val) { m_isManTrigMatched = val; }

  /// Value from the energy branch that Top ntuples include
  /**
   *  a branch for the reconstructed energy. The energy in the
   *  _four vector_ of this object will be calculated using the
   *  (pT, eta, phi, m) information. the "e_branch" variable is
   *  separate.
   */
  void set_e_branch(const float val) { m_e_branch = val; }

  /// @}

  /// @name getters
  /// @{

  unsigned int pdgId() const { return m_pdgId; }
  float e_branch() const { return m_e_branch; }
  float charge() const { return m_charge; }
  float topoetcone20() const { return m_topoetcone20; }
  float d0sig() const { return m_d0sig; }
  float delta_z0_sintheta() const { return m_delta_z0_sintheta; }
  int true_type() const { return m_true_type; }
  int true_origin() const { return m_true_origin; }
  char true_isPrompt() const { return m_true_isPrompt; }
  char isTight() const { return m_isTight; }
  bool isManTrigMatched() const { return m_isManTrigMatched; }

  /// @}
};
}  // namespace EDM
}  // namespace TL

#endif

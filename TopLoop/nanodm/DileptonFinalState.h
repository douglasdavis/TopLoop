/** @file  nanodm/DileptonFinalState.h
 *  @brief DileptonFinalState class header
 *
 *  @author Doug Davis <ddavis@cern.ch>
 */

#ifndef nanodm_DileptonFinalState_h
#define nanodm_DileptonFinalState_h

#include <TopLoop/nanodm/Jet.h>
#include <TopLoop/nanodm/Lepton.h>
#include <TopLoop/nanodm/MissingET.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

namespace nanodm {

/** @class DileptonFinalState
 *  @brief A class providing structure to describe a dilepton final state
 *
 *  This class is built from other classes in the nanodm namespace
 */
class DileptonFinalState {
 public:
  /// convenient enum for dilepton flavor combination identification
  enum class FlavComb {
    ELMU = 0,
    ELEL = 1,
    MUMU = 2,
    UNKNOWN = 3,
  };

 private:
  std::unique_ptr<nanodm::Lepton> m_lepton1;
  std::unique_ptr<nanodm::Lepton> m_lepton2;
  std::vector<std::unique_ptr<nanodm::Jet>> m_jets;
  std::vector<std::unique_ptr<nanodm::Jet>> m_softjets;
  std::unique_ptr<nanodm::MissingET> m_met;

 public:
  /// default constructor
  DileptonFinalState() {
    // reserve space to reduce allocations
    m_jets.reserve(15);
    m_softjets.reserve(5);
  }
  /// default destructor
  virtual ~DileptonFinalState() = default;

  /// delete copy
  DileptonFinalState(const DileptonFinalState&) = delete;
  /// delete assignment
  DileptonFinalState& operator=(const DileptonFinalState&) = delete;
  /// delete move
  DileptonFinalState(DileptonFinalState&&) = delete;
  /// delete move assignment
  DileptonFinalState& operator=(DileptonFinalState&&) = delete;

  /// @name smart pointer/container modifiers for building the final state
  /// @{

  /// assignable smart pointer for setting lepton1
  std::unique_ptr<nanodm::Lepton>& lepton1() { return m_lepton1; }
  /// assignable smart pointer for setting lepton2
  std::unique_ptr<nanodm::Lepton>& lepton2() { return m_lepton2; }
  /// modifiable vector for adding smart pointer jets
  std::vector<std::unique_ptr<nanodm::Jet>>& jets() { return m_jets; }
  /// modifiable vector for adding smart pointer jets
  std::vector<std::unique_ptr<nanodm::Jet>>& softjets() { return m_softjets; }
  /// assignable smart pointer for setting the @f$E_\mathrm{T}^\mathrm{miss}@f$ object
  std::unique_ptr<nanodm::MissingET>& missingET() { return m_met; }

  /// @}

  /// @name const pointer getters
  /// @{

  /// retrieve a const pointer to the leading lepton
  const nanodm::Lepton* lepton1() const { return m_lepton1.get(); }
  /// retrieve a const pointer to the second lepton
  const nanodm::Lepton* lepton2() const { return m_lepton2.get(); }
  /// retrieve a const ref to the jet container
  const std::vector<std::unique_ptr<nanodm::Jet>>& jets() const { return m_jets; }
  /// retrieve a const ref to the soft jet container
  const std::vector<std::unique_ptr<nanodm::Jet>>& softjets() const { return m_softjets; }
  /// retrieve a const pointer to the @f$i^\mathrm{th}@f$ jet.
  const nanodm::Jet* jet(const std::size_t i) const { return m_jets.at(i).get(); }
  /// retrieve a const pointer to the @f$i^\mathrm{th}@f$ soft jet.
  const nanodm::Jet* softjet(const std::size_t i) const { return m_softjets.at(i).get(); }
  /// retrieve a const pointer to the MissingET
  const nanodm::MissingET* missingET() const { return m_met.get(); }

  /// @}

  /// @name read properties of final state
  /// @{

  /// retrieve the number of jets in the final state
  std::size_t njets() const { return m_jets.size(); }
  /// retrieve the number of soft jets in the final state
  std::size_t nsoftjets() const { return m_softjets.size(); }

  /// grab the index of most forward jet (largest @f$|\eta|@f$).
  std::size_t mostForwardJetIdx() const {
    if (jets().empty()) return 0;
    if (jets().size() == 1) return 0;

    auto eta_compare = [](const auto& j1, const auto& j2) {
      return (j1->abseta() < j2->abseta());
    };
    auto res = std::max_element(jets().begin(), jets().end(), eta_compare);
    auto idx = std::distance(jets().begin(), res);
    return idx;
  }

  /// sum of the lepton pdg codes
  int pdgSum() const { return m_lepton1->pdgId() + m_lepton2->pdgId(); }
  /// true if either lepton is fake/non-prompt based on MC information
  bool hasFake() const { return m_lepton1->isMCNonPrompt() || m_lepton2->isMCNonPrompt(); }
  /// true if two electrons in final state (@f$ee@f$)
  bool elel() const { return pdgSum() == 22; }
  /// true if electron + muon final state (@f$e\mu@f$)
  bool elmu() const { return pdgSum() == 24; }
  /// true if two muons in final state (@f$\mu\mu@f$)
  bool mumu() const { return pdgSum() == 26; }
  /// true if leptons are oppositely sign (@f$\ell^\pm\ell^\mp@f$).
  bool OS() const { return (m_lepton1->charge() * m_lepton2->charge()) < 0; }
  /// true if leptons are same sign (@f$\ell^\pm\ell^\pm@f$).
  bool SS() const { return !OS(); }
  /// true if leptons are opposite flavor (@f$e\mu@f$)
  bool OF() const { return elmu(); }
  /// true if leptons are same flavor (@f$ee/\mu\mu@f$)
  bool SF() const { return !elmu(); }
  /// get a specific flavor combination label
  FlavComb flavComb() const {
    if (elmu())
      return FlavComb::ELMU;
    else if (elel())
      return FlavComb::ELEL;
    else if (mumu())
      return FlavComb::MUMU;
    else
      return FlavComb::UNKNOWN;
  }

  /// @}

  /// @name utilities
  /// @{

  /// erase all information from the final state
  void destroy() {
    m_lepton1.reset(nullptr);
    m_lepton2.reset(nullptr);
    m_jets.clear();
    m_softjets.clear();
    m_met.reset(nullptr);
  }

  /// crashes if something nonphysicsal is happening, return true if OK
  bool checkSelf() const {
    if (m_lepton1->pt() < m_lepton2->pt()) {
      std::cerr << "lepton pt ordering wrong" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    if (OS() == SS()) {
      std::cerr << "lepton charges registering OS and SS" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    if (OF() == SF()) {
      std::cerr << "lepton flavors registering OF and SF" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    if (m_jets.size() > 1) {
      for (std::size_t i = 1; i < m_jets.size() - 1; ++i) {
        if (jet(i - 1)->pt() < jet(i)->pt()) {
          std::cerr << "jet pt ordering wrong" << std::endl;
          std::exit(EXIT_FAILURE);
        }
      }
    }
    return true;
  }

  /// @}

  /// @name static utilities
  /// @{

  /// determine the number of b-tagged jets in a container based on the bin requirement
  /**
   *  @param container the jet container
   *  @param bin_req the bin requirement
   */
  static std::size_t nbtagged(const std::vector<std::unique_ptr<nanodm::Jet>>& container,
                              const nanodm::BTagBin bin_req) {
    return std::count_if(std::begin(container), std::end(container),
                         [&](const auto& j) { return j->isbtaggedContinuous(bin_req); });
  }

  /// @}
};
}  // namespace nanodm

#endif

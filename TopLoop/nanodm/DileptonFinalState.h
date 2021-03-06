/*! @file  nanodm/DileptonFinalState.h
 *  @brief DileptonFinalState class header
 *
 *  @author Doug Davis <ddavis@cern.ch>
 */

#ifndef nanodm_DileptonFinalState_h
#define nanodm_DileptonFinalState_h

#include <TopLoop/nanodm/Jet.h>
#include <TopLoop/nanodm/Lepton.h>
#include <TopLoop/nanodm/MissingET.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

namespace nanodm {

/*! @class DileptonFinalState
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
    TAUTAU = 4,
    ELTAU = 5,
    MUTAU = 6,
    UNKNOWN = 99,
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
  bool mumu() const { return (m_lepton1->pdgId() == 13 && m_lepton2->pdgId() == 13); }
  /// true if two taus in final state (@f$\tau\tau@f$)
  bool tautau() const { return pdgSum() == 30; }
  /// true if electron + tau final state (@f$e\tau@f$)
  bool eltau() const {
    return ((m_lepton1->pdgId() == 11 && m_lepton2->pdgId() == 15) ||
            (m_lepton1->pdgId() == 15 && m_lepton2->pdgId() == 11));
  }
  /// true if muon + tau final state (@f$\mu\tau@f$)
  bool mutau() const { return pdgSum() == 28; }
  /// true if leptons are oppositely sign (@f$\ell^\pm\ell^\mp@f$).
  bool OS() const { return (m_lepton1->charge() * m_lepton2->charge()) < 0; }
  /// true if leptons are same sign (@f$\ell^\pm\ell^\pm@f$).
  bool SS() const { return !OS(); }
  /// true if leptons are opposite flavor (@f$e\mu@f$)
  bool OF() const { return (elmu() || eltau() || mutau()); }
  /// true if leptons are same flavor (@f$ee/\mu\mu@f$)
  bool SF() const { return !OF(); }
  /// get a specific flavor combination label
  FlavComb flavComb() const {
    if (elmu())
      return FlavComb::ELMU;
    else if (elel())
      return FlavComb::ELEL;
    else if (mumu())
      return FlavComb::MUMU;
    else if (tautau())
      return FlavComb::TAUTAU;
    else if (eltau())
      return FlavComb::ELTAU;
    else if (mutau())
      return FlavComb::MUTAU;
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

  /// crashes if something nonphysical is happening, return true if OK
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

  /// deprecated function: use the specific tagger based functions
  [[deprecated("use the specific tagger functions")]] static std::size_t nbtagged(
      const std::vector<std::unique_ptr<nanodm::Jet>>& container,
      const nanodm::BTagBin bin_req) {
    return nbtagged_MV2c10_Continuous(container, bin_req);
  }

  /// determine the number of MV2c10 b-tagged jets in a container based on the bin
  /// requirement
  /*!
   *  @param container the jet container
   *  @param bin_req the bin requirement
   */
  static std::size_t nbtagged_MV2c10_Continuous(
      const std::vector<std::unique_ptr<nanodm::Jet>>& container,
      const nanodm::BTagBin bin_req) {
    return std::count_if(std::begin(container), std::end(container), [&](const auto& j) {
      return j->isbtaggedContinuous_MV2c10(bin_req);
    });
  }

  /// determine the number of DL1r b-tagged jets in a container based on the bin requirement
  /*!
   *  @param container the jet container
   *  @param bin_req the bin requirement
   */
  static std::size_t nbtagged_DL1r_Continuous(
      const std::vector<std::unique_ptr<nanodm::Jet>>& container,
      const nanodm::BTagBin bin_req) {
    return std::count_if(std::begin(container), std::end(container), [&](const auto& j) {
      return j->isbtaggedContinuous_DL1r(bin_req);
    });
  }

  /// determine the number of b-tagged jets via the DLr1 60% WP
  static std::size_t nbtagged_DL1r_60(
      const std::vector<std::unique_ptr<nanodm::Jet>>& container) {
    return std::count_if(std::begin(container), std::end(container),
                         [&](const auto& j) { return j->isbtagged_DL1r_60(); });
  }

  /// determine the number of b-tagged jets via the DLr1 70% WP
  static std::size_t nbtagged_DL1r_70(
      const std::vector<std::unique_ptr<nanodm::Jet>>& container) {
    return std::count_if(std::begin(container), std::end(container),
                         [&](const auto& j) { return j->isbtagged_DL1r_70(); });
  }

  /// determine the number of b-tagged jets via the DLr1 77% WP
  static std::size_t nbtagged_DL1r_77(
      const std::vector<std::unique_ptr<nanodm::Jet>>& container) {
    return std::count_if(std::begin(container), std::end(container),
                         [&](const auto& j) { return j->isbtagged_DL1r_77(); });
  }

  /// determine the number of b-tagged jets via the DLr1 85% WP
  static std::size_t nbtagged_DL1r_85(
      const std::vector<std::unique_ptr<nanodm::Jet>>& container) {
    return std::count_if(std::begin(container), std::end(container),
                         [&](const auto& j) { return j->isbtagged_DL1r_85(); });
  }

  /// @}
};
}  // namespace nanodm

#endif

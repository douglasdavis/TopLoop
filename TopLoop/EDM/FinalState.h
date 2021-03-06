/** @file  FinalState.h
 *  @brief TL::EDM::FinalState class header
 *  @class TL::EDM::FinalState
 *  @brief A class for summarizing final state information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the information about a final state.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_EDM_FinalState_h
#define TL_EDM_FinalState_h

// TL
#include <TopLoop/Core/Loggable.h>
#include <TopLoop/EDM/Electron.h>
#include <TopLoop/EDM/Jet.h>
#include <TopLoop/EDM/LeptonPair.h>
#include <TopLoop/EDM/MissingET.h>
#include <TopLoop/EDM/Muon.h>
#include <TopLoop/EDM/Tau.h>

// C++
#include <algorithm>

namespace TL {
namespace EDM {

class FinalState : TL::Loggable {
 private:
  std::vector<TL::EDM::Electron> m_electrons{};
  std::vector<TL::EDM::Muon> m_muons{};
  std::vector<TL::EDM::Tau> m_taus{};
  std::vector<TL::EDM::Jet> m_jets{};
  std::vector<TL::EDM::Jet> m_looseJets{};
  std::vector<TL::EDM::Lepton> m_leptons{};
  std::vector<TL::EDM::LeptonPair> m_leptonPairs{};
  TL::EDM::MissingET m_missingET{};

  bool m_hasFakeElectronMC{false};
  bool m_hasFakeMuonMC{false};
  bool m_hasFakeTauMC{false};
  bool m_hasManTrigMatched{false};

  void addLepton(const TL::EDM::Lepton& lep) { m_leptons.emplace_back(lep); }
  void addLeptonPair(const TL::EDM::LeptonPair& lp) { m_leptonPairs.emplace_back(lp); }
  void makeLeptonPairs();

 public:
  /// default constructor
  FinalState();

  /// destructor
  virtual ~FinalState() = default;

  FinalState(const FinalState&) = delete;
  FinalState& operator=(const FinalState&) = delete;
  FinalState(FinalState&&) = delete;
  FinalState& operator=(FinalState&&) = delete;

  /// @name Functions to define the final state
  /// @{

  /// add a jet to the final state
  void addJet(const TL::EDM::Jet& jet) { m_jets.emplace_back(jet); }
  /// add a loose jet to the final state
  void addLooseJet(const TL::EDM::Jet& jet) { m_looseJets.emplace_back(jet); }
  /// add an electron to electron container
  void addElectron(const TL::EDM::Electron& el) { m_electrons.emplace_back(el); }
  /// add a muon to the muon container
  void addMuon(const TL::EDM::Muon& mu) { m_muons.emplace_back(mu); }
  /// add a tau to the tau container
  void addTau(const TL::EDM::Tau& tau) { m_taus.emplace_back(tau); }

  /// process the physics objects to set some final state properties
  /**
   *  @param sort_leptons flag to apply std::sort to the lepton
   *  container.
   *  @param manual_promptness flag to use the isManualPrompt for
   *  electron promptness determination.
   *  @param sort_jets flag to force sorting jets, they should
   *  already be sorted thought! (as of December 2018)
   */
  void evaluateSelf(bool sort_leptons = true, bool manual_promptness = false,
                    bool sort_jets = false);

  /// @}

  /// @name Functions to grab information about the final state
  /// @{

  /// get the leptons
  const std::vector<TL::EDM::Lepton>& leptons() const { return m_leptons; }
  /// get the jets
  const std::vector<TL::EDM::Jet>& jets() const { return m_jets; }
  /// get loose jets
  const std::vector<TL::EDM::Jet>& looseJets() const { return m_looseJets; }
  /// get the lepton pairs
  const std::vector<TL::EDM::LeptonPair>& leptonPairs() const { return m_leptonPairs; }
  /// get the MissingET object (const)
  const TL::EDM::MissingET& MissingET() const { return m_missingET; }
  /// get the MissingET object (non const)
  TL::EDM::MissingET& MissingET() { return m_missingET; }

  /// true if the event has a fake electron (at least one electron that failed truth
  /// matching)
  bool hasFakeElectronMC() const { return m_hasFakeElectronMC; }
  /// true if the event has a fake muon (at least one muon that failed truth matching)
  bool hasFakeMuonMC() const { return m_hasFakeMuonMC; }
  /// true if the event has a fake tau (at least one tau that failed truth matching)
  bool hasFakeTauMC() const { return m_hasFakeTauMC; }
  /// true if fake electron, fake muon or fake tau
  bool hasFakeLeptonMC() const { return hasFakeMuonMC() || hasFakeElectronMC() || hasFakeTauMC(); }
  /// true if an electron was flagged with isManTrigMatched
  bool hasManTrigMatched() const { return m_hasManTrigMatched; }

  [[deprecated("Use the static function overload nbjets(container, wp)")]] std::size_t
  nbjets(const TL::EDM::BTagWP wp = TL::EDM::BTagWP::mv2c10_77) const;

  /// count the number of jets in the container (default is MV2c10 at 77 percent eff)
  /**
   *  Uses the TL::EDM::BTagWP enum to ask for a specific working point.
   *
   *  @param container the jet container to test
   *  @param wp the Working point enum value (default BTagWP::mv2c10_77).
   */
  static std::size_t nbjets(const std::vector<TL::EDM::Jet>& container,
                            const TL::EDM::BTagWP wp);

  /// count the number of jets in the container passing the requirement
  /**
   *  Uses the TL::EDM::BTagBin enum to ask for a specific WP
   *
   *  @param container hte jet container to test
   *  @param bin_req the minimum b-tagging bin (default minimum is 77->70 bin)
   */
  static std::size_t nbjets(const std::vector<TL::EDM::Jet>& container,
                            const TL::EDM::BTagBin bin_req);

  /// grab the index of most forward jet (largest \f$|\eta|\f$).
  std::size_t mostForwardJetIdx() const;

  /// @}

  /// clear all final state containers
  void reset();
};

}  // namespace EDM
}  // namespace TL

#endif

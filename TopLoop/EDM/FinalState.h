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
#include <TopLoop/EDM/LeptonPair.h>
#include <TopLoop/EDM/Jet.h>
#include <TopLoop/EDM/MissingET.h>
#include <TopLoop/EDM/Electron.h>
#include <TopLoop/EDM/Muon.h>
#include <TopLoop/Core/Loggable.h>

// C++
#include <algorithm>

namespace TL {
  namespace EDM {

    class FinalState : TL::Loggable {
    private:
      std::vector<TL::EDM::Electron>   m_electrons{};
      std::vector<TL::EDM::Muon>       m_muons{};
      std::vector<TL::EDM::Jet>        m_jets{};
      std::vector<TL::EDM::Jet>        m_looseJets{};
      std::vector<TL::EDM::Lepton>     m_leptons{};
      std::vector<TL::EDM::LeptonPair> m_leptonPairs{};
      TL::EDM::MissingET               m_missingET{};

      bool m_hasFakeElectronMC{false};
      bool m_hasFakeMuonMC{false};
      bool m_hasManTrigMatched{false};

      void addLepton(const TL::EDM::Lepton &lep) { m_leptons.emplace_back(lep); }
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

      /// process the physics objects to set some final state properties
      /**
       *  @param sort_leptons flag to apply std::sort to the lepton
       *  container.
       *  @param manual_promptness flag to use the isManualPrompt for
       *  electron promptness determination.
       */
      void evaluateSelf(bool sort_leptons = true, bool manual_promptness = false);

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

      /// true if the event has a fake electron (at least one electron that failed truth matching)
      bool hasFakeElectronMC() const { return m_hasFakeElectronMC; }
      /// true if the event has a fake muon (at least one muon that failed truth matching)
      bool hasFakeMuonMC() const { return m_hasFakeMuonMC; }
      /// true if fake electron or fake muon
      bool hasFakeLeptonMC() const { return hasFakeMuonMC() || hasFakeElectronMC(); }
      /// true if an electron was flagged with isManTrigMatched
      bool hasManTrigMatched() const { return m_hasManTrigMatched; }

      /// count the number of jets in the container (default is MV2c10 at 77 percent eff)
      /**
       *  Uses the TL::EDM::BTagWP enum to ask for a specific working point.
       *
       *  @param wp the Working point enum value (default BTagWP::mv2c10_77).
       */
      std::size_t nbjets(const TL::EDM::BTagWP wp = TL::EDM::BTagWP::mv2c10_77) const;

      /// grab the index of most forward jet (largest \f$|\eta|\f$).
      std::size_t mostForwardJetIdx() const;

      /// @}

      /// clear all final state containers
      void reset();

    };

  }
}

#endif

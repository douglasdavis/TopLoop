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
      std::vector<TL::EDM::Lepton>     m_leptons{};
      std::vector<TL::EDM::LeptonPair> m_leptonPairs{};
      TL::EDM::MissingET               m_missingET{};

      bool m_hasFakeElectronMC{false};
      bool m_hasFakeMuonMC{false};

      void addLepton(const TL::EDM::Lepton& lep);
      void addLeptonPair(const TL::EDM::LeptonPair& lp);
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
      void addJet(const TL::EDM::Jet& jet);
      /// add an electron to electron container
      void addElectron(const TL::EDM::Electron& el);
      /// add a muon to the muon container
      void addMuon(const TL::EDM::Muon& mu);
      /// process the physics objects to set some final state properties.
      /**
       *  @param sort_leptons flag to apply std::sort to the lepton
       *  container
       */
      void evaluateSelf(bool sort_leptons = true);

      /// @}

      /// @name Functions to grab information about the final state
      /// @{

      /// get the leptons
      const std::vector<TL::EDM::Lepton>&     leptons()     const;
      /// get the jets
      const std::vector<TL::EDM::Jet>&        jets()        const;
      /// get the lepton pairs
      const std::vector<TL::EDM::LeptonPair>& leptonPairs() const;
      /// get the MissingET object (const)
      const TL::EDM::MissingET&               MissingET()   const;
      /// get the MissingET object (non const)
      TL::EDM::MissingET&                     MissingET();

      /// true if the event has a fake electron (at least one electron that failed truth matching)
      bool        hasFakeElectronMC() const;
      /// true if the event has a fake muon (at least one muon that failed truth matching)
      bool        hasFakeMuonMC()     const;
      /// true if fake electron or fake muon
      bool        hasFakeLeptonMC()   const;
      /// count the number of jets in the container with MV2c10 at 77 percent eff
      std::size_t nbjets_MV2c10_77()  const;

      /// grab the index of most forward jet (largest \f$|\eta|\f$).
      std::size_t mostForwardJetIdx() const;

      /// @}

      /// clear all final state containers
      void reset();

    };

  }
}

inline void TL::EDM::FinalState::addElectron(const TL::EDM::Electron& el)     { m_electrons.push_back(el);   }
inline void TL::EDM::FinalState::addMuon(const TL::EDM::Muon& mu)             { m_muons.push_back(mu);       }
inline void TL::EDM::FinalState::addJet(const TL::EDM::Jet& jet)              { m_jets.push_back(jet);       }
inline void TL::EDM::FinalState::addLepton(const TL::EDM::Lepton& lep)        { m_leptons.push_back(lep);    }
inline void TL::EDM::FinalState::addLeptonPair(const TL::EDM::LeptonPair& lp) { m_leptonPairs.push_back(lp); }

inline const std::vector<TL::EDM::Lepton>& TL::EDM::FinalState::leptons()   const { return m_leptons;   }
inline const std::vector<TL::EDM::Jet>&    TL::EDM::FinalState::jets()      const { return m_jets;      }
inline const TL::EDM::MissingET&           TL::EDM::FinalState::MissingET() const { return m_missingET; }
inline       TL::EDM::MissingET&           TL::EDM::FinalState::MissingET()       { return m_missingET; }

inline const std::vector<TL::EDM::LeptonPair>& TL::EDM::FinalState::leptonPairs() const {
  return m_leptonPairs;
}

inline bool TL::EDM::FinalState::hasFakeElectronMC()  const { return m_hasFakeElectronMC; }
inline bool TL::EDM::FinalState::hasFakeMuonMC()      const { return m_hasFakeMuonMC;     }
inline bool TL::EDM::FinalState::hasFakeLeptonMC()    const {
  return (m_hasFakeElectronMC || m_hasFakeMuonMC);
}

inline std::size_t TL::EDM::FinalState::nbjets_MV2c10_77() const {
  return std::count_if(m_jets.begin(), m_jets.end(),
                       [](const TL::EDM::Jet& a) { return a.isbtagged_MV2c10_77(); });
}

#endif

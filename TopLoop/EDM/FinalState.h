/** @file  FinalState.h
 *  @brief TL::EDM::FinalState class header
 *  @class TL::EDM::FinalState
 *  @brief A container of final state information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the information about a final state.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef TL_EDM_FinalState_h
#define TL_EDM_FinalState_h

// TL
#include <TopLoop/EDM/LeptonPair.h>
#include <TopLoop/EDM/Jet.h>
#include <TopLoop/EDM/MET.h>

// C++
#include <algorithm>

namespace TL {
  namespace EDM {

    class FinalState : public TObject {
    private:
      std::vector<TL::EDM::Lepton>     m_leptons;
      std::vector<TL::EDM::Jet>        m_jets;
      TL::EDM::MET                     m_MET;
      std::vector<TL::EDM::LeptonPair> m_leptonPairs;

      float       m_M;
      float       m_HT;

      bool m_hasFakeElectron;
      bool m_hasFakeMuon;

      void addLeptonPair(const TL::EDM::LeptonPair& lp);
      void setHasFakeElectron(const bool flag);
      void setHasFakeMuon(const bool flag);
      void evaluateLepPairs();

      ClassDef(FinalState,1);

    public:

      /// default constructor
      FinalState() :
        m_leptons(),
        m_jets(),
        m_MET(),
        m_leptonPairs(),
        m_M(0),
        m_HT(0),
        m_hasFakeElectron(false),
        m_hasFakeMuon(false)
      {}

      /// destructor
      virtual ~FinalState() {}

      /// add a lepton to the lepton container
      void addLepton(const TL::EDM::Lepton& lep);
      /// add a jet to the final state
      void addJet(const TL::EDM::Jet& jet);
      /// parse the leptons, jets, and MET to set some final state properties
      void evaluateSelf(bool sort_leptons = true);

      /// get the leptons
      const std::vector<TL::EDM::Lepton>&     leptons()     const;
      /// get the jets
      const std::vector<TL::EDM::Jet>&        jets()        const;
      /// get the lepton pairs
      const std::vector<TL::EDM::LeptonPair>& leptonPairs() const;
      /// get the MET (const)
      const TL::EDM::MET&                     MET()         const;
      /// get the MET (non const)
      TL::EDM::MET&                           MET();

      /// retrieve the invariant mass of all objects in the event
      float        M()                const;
      /// retrieve the sum of transverse energy of objects in the final state
      float        HT()               const;
      std::size_t  nbjets_c20()       const;
      std::size_t  nbjets_c10()       const;
      /// retrieve the number of bjets using the 70% efficiency working point
      std::size_t  nbjets_AT70()      const;
      /// retrieve the number of bjets using the 77% efficiency working point
      std::size_t  nbjets_AT77()      const;
      /// retrieve the number of bjets using the 85% efficiency working point
      std::size_t  nbjets_AT85()      const;
      /// true if the event has a fake electron (at least one electron that failed truth matching)
      bool         hasFakeElectron()  const;
      /// true if the event has a fake muon (at least one muon that failed truth matching)
      bool         hasFakeMuon()      const;

      /// clear the final state
      void clear();
    };

  }
}

inline void TL::EDM::FinalState::addLepton(const TL::EDM::Lepton& lep)        { m_leptons.emplace_back(lep);    }
inline void TL::EDM::FinalState::addJet(const TL::EDM::Jet& jet)              { m_jets.emplace_back(jet);       }
inline void TL::EDM::FinalState::addLeptonPair(const TL::EDM::LeptonPair& lp) { m_leptonPairs.emplace_back(lp); }

inline void TL::EDM::FinalState::clear() {
  m_leptons.clear();
  m_jets.clear();
  m_leptonPairs.clear();
  m_M = 0;
  m_HT = 0;
  m_hasFakeElectron = false;
  m_hasFakeMuon = false;
}

inline void TL::EDM::FinalState::setHasFakeElectron(const bool flag) { m_hasFakeElectron = flag; }
inline void TL::EDM::FinalState::setHasFakeMuon(const bool flag)     { m_hasFakeMuon     = flag; }

inline const std::vector<TL::EDM::Lepton>& TL::EDM::FinalState::leptons() const { return m_leptons; }
inline const std::vector<TL::EDM::Jet>&    TL::EDM::FinalState::jets()    const { return m_jets;    }
inline const TL::EDM::MET&                 TL::EDM::FinalState::MET()     const { return m_MET;     }
inline       TL::EDM::MET&                 TL::EDM::FinalState::MET()           { return m_MET;     }

inline const std::vector<TL::EDM::LeptonPair>& TL::EDM::FinalState::leptonPairs() const {
  return m_leptonPairs;
}

inline float TL::EDM::FinalState::M()  const { return m_M;  }
inline float TL::EDM::FinalState::HT() const { return m_HT; }

inline std::size_t TL::EDM::FinalState::nbjets_c20() const {
  return std::count_if(m_jets.begin(), m_jets.end(),
                       [](const TL::EDM::Jet& a) { return a.isTagged_c20(); });
}

inline std::size_t TL::EDM::FinalState::nbjets_c10() const {
  return std::count_if(m_jets.begin(), m_jets.end(),
                       [](const TL::EDM::Jet& a) { return a.isTagged_c10(); });
}

inline std::size_t TL::EDM::FinalState::nbjets_AT70() const {
  return std::count_if(m_jets.begin(), m_jets.end(),
                       [](const TL::EDM::Jet& a) { return a.isbtagged_70(); });
}

inline std::size_t TL::EDM::FinalState::nbjets_AT77() const {
  return std::count_if(m_jets.begin(), m_jets.end(),
                       [](const TL::EDM::Jet& a) { return a.isbtagged_77(); });
}

inline std::size_t TL::EDM::FinalState::nbjets_AT85() const {
  return std::count_if(m_jets.begin(), m_jets.end(),
                       [](const TL::EDM::Jet& a) { return a.isbtagged_85(); });
}

inline bool        TL::EDM::FinalState::hasFakeElectron()  const { return m_hasFakeElectron; }
inline bool        TL::EDM::FinalState::hasFakeMuon()      const { return m_hasFakeMuon;     }

#endif

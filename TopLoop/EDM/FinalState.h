/** @file  FinalState.h
 *  @brief TL::EDM::FinalState class header
 *  @class TL::EDM::FinalState
 *  @brief A container of final state information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the information about a final state.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
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
      std::size_t m_llidx;
      std::size_t m_ljidx;
      
      ClassDef(FinalState,1);
      
    public:
      FinalState() :
        m_leptons(), m_jets(), m_MET(), m_leptonPairs(),
        m_M(0), m_HT(0), m_llidx(0), m_ljidx(0)
      {}
      virtual ~FinalState() {}

      void addLepton(const TL::EDM::Lepton& lep);
      void addJet(const TL::EDM::Jet& jet);
      void addLeptonPair(const TL::EDM::LeptonPair& lp);
      void addLeptonPairs(const std::initializer_list<TL::EDM::LeptonPair>& lps);

      void evaluateLepPairs();
      void evaluateSelf();
      void clear();
      
      const std::vector<TL::EDM::Lepton>&     leptons()     const;
      const std::vector<TL::EDM::Jet>&        jets()        const;
      const std::vector<TL::EDM::LeptonPair>& leptonPairs() const;
      const TL::EDM::MET&                     MET()         const;
      TL::EDM::MET&                           MET();
      
      float        M()                const;
      float        HT()               const;
      std::size_t  nbjets_c20()       const;
      std::size_t  nbjets_c10()       const;
      std::size_t  nbjets_AT()        const;
      std::size_t  leadingLeptonIdx() const;
      std::size_t  leadingJetIdx()    const;
    };

  }
}

inline void TL::EDM::FinalState::addLepton(const TL::EDM::Lepton& lep) { m_leptons.emplace_back(lep); }
inline void TL::EDM::FinalState::addJet(const TL::EDM::Jet& jet)       { m_jets.emplace_back(jet);    }
inline void TL::EDM::FinalState::addLeptonPair(const TL::EDM::LeptonPair& lp) {
  m_leptonPairs.emplace_back(lp);
}

inline void TL::EDM::FinalState::addLeptonPairs(const std::initializer_list<TL::EDM::LeptonPair>& lps) {
  for ( auto const& lp : lps ) {
    m_leptonPairs.emplace_back(lp);
  }
}

inline void TL::EDM::FinalState::clear() {
  m_leptons.clear();
  m_jets.clear();
  m_leptonPairs.clear();
}


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

inline std::size_t TL::EDM::FinalState::nbjets_AT() const {
  return std::count_if(m_jets.begin(), m_jets.end(),
                       [](const TL::EDM::Jet& a) { return a.isbtagged_77(); });
}

inline std::size_t TL::EDM::FinalState::leadingLeptonIdx() const { return m_llidx; }
inline std::size_t TL::EDM::FinalState::leadingJetIdx()    const { return m_ljidx; }

#endif

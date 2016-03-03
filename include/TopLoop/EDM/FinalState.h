/** @file  FinalState.h
 *  @brief TL::EDM::FinalState class header
 *  @class TL::EDM::FinalState
 *  @brief An electron object
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the information about a final state.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_EDM_FinalState_h
#define TL_EDM_FinalState_h

#include <TopLoop/EDM/Lepton.h>
#include <TopLoop/EDM/Jet.h>

namespace TL {
  namespace EDM {

    class FinalState : public TObject {
    private:
      std::vector<TL::EDM::Lepton> m_leptons;
      std::vector<TL::EDM::Jet>    m_jets;

      double m_M;

      ClassDef(FinalState,1);
      
    public:
      FinalState() : m_leptons(), m_jets(), m_M(0) {}
      virtual ~FinalState() {}

      void addLepton(const TL::EDM::Lepton& lep);
      void addJet(const TL::EDM::Jet& jet);

      void clear();
      
      void set_M(const double m);
      
      const std::vector<TL::EDM::Lepton>& leptons() const;
      const std::vector<TL::EDM::Jet>&    jets()    const;
      
      double M() const;

    };

  }
}

inline void TL::EDM::FinalState::addLepton(const TL::EDM::Lepton& lep) { m_leptons.emplace_back(lep); }
inline void TL::EDM::FinalState::addJet(const TL::EDM::Jet& jet)       { m_jets.emplace_back(jet);    }

inline void TL::EDM::FinalState::clear() {
  m_leptons.clear();
  m_jets.clear();
}

inline const std::vector<TL::EDM::Lepton>& TL::EDM::FinalState::leptons() const { return m_leptons; }
inline const std::vector<TL::EDM::Jet>&    TL::EDM::FinalState::jets()    const { return m_jets;    }

inline double TL::EDM::FinalState::M() const { return m_M; }

#endif

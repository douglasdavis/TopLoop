/** @file  LeptonPair.h
 *  @brief TL::EDM::LeptonPair class header
 *  @class TL::EDM::LeptonPair
 *  @brief A container for lepton-lepton information.
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a pair of leptons.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_EDM_LeptonPair_h
#define TL_EDM_LeptonPair_h

// TL
#include <TopLoop/EDM/Lepton.h>

// C++
#include <cmath>
#include <iostream>
#include <utility>

namespace TL {
  namespace EDM {

    class LeptonPair : public TL::EDM::PhysicsObject {
    private:

      float  m_deltaR;
      float  m_deltaPhi;
      float  m_deltaEta;
      bool   m_SS;
      bool   m_OS;
      bool   m_elel;
      bool   m_mumu;
      bool   m_elmu;

      std::size_t m_fIdx;
      std::size_t m_sIdx;

    public:

      /// unused default constructor
      LeptonPair() = default;

      /// main constructor
      /**
       *  We feed the leptons *and* their indices to the
       *  constructor. Because we use an std::vector as a
       *  TL::EDM::Lepton container in the TL::EDM::FinalState class,
       *  it can be useful to have the indices of the leptons in that
       *  container. That is why we also feed them in here. If you
       *  don't need them.. just put bogus values. The reason we don't
       *  supply a default value is to make the user have to think
       *  about it.
       *
       *  @param lep1 The first lepton ingrediant
       *  @param lep2 The second lepton ingrediant
       *  @param idxf The index of the first lepton in the TL::EDM::FinalState lepton container
       *  @param idxs The index of the first lepton in the TL::EDM::FinalState lepton container
       */
      LeptonPair(const TL::EDM::Lepton& lep1,
                 const TL::EDM::Lepton& lep2,
                 const size_t idxf, const size_t idxs);

      /// default destructor
      virtual ~LeptonPair() = default;

      LeptonPair& operator=(const LeptonPair&) = default;
      LeptonPair(const LeptonPair&) = default;

      /// get the ATLAS coordinate system \f$\Delta R\f$ between the two leptons.
      /**
       *  ATLAS coordinate system definition (separation of objects in
       *  \f$\eta\text{-}\phi\f$ space):
       *
       *  \f[
       *     \Delta R \equiv \sqrt{(\Delta\eta)^2 + (\Delta\phi)^2}.
       *  \f]
       *
       *  This function uses the ROOT function
       *  `TLorentzVector::DeltaR`.
       */
      float deltaR()   const;
      /// get the \f$\Delta\phi\f$ between the two leptons.
      float deltaPhi() const;
      /// get the \f$\Delta\eta\f$ between the two leptons.
      float deltaEta() const;

      /// return true for same sign lepton pair.
      bool SS()   const;
      /// return true for opposite sign lepton pair.
      bool OS()   const;
      /// return true for same flavor lepton pair.
      bool SF()   const;
      /// return true for opposite flavor lepton pair (same as elmu()).
      bool OF()   const;
      /// return true if \f$\mu\mu\f$ lepton pair.
      bool mumu() const;
      /// return true if \f$ee\f$ lepton pair.
      bool elel() const;
      /// return true if \f$e\mu\f$ lepton pair (same as OF()).
      bool elmu() const;

      /// get the index of the first lepton in the pair
      /**
       *  because we store leptons in a vector in the
       *  TL::EDM::FinalState class, it may be useful to have the
       *  index in that vector, that's what this is for.
       */
      std::size_t firstIdx()  const;
      /// get the index of the second lepton in the pair
      /**
       *  because we store leptons in a vector in the
       *  TL::EDM::FinalState class, it may be useful to have the
       *  index in that vector, that's what this is for.
       */
      std::size_t secondIdx() const;

    };

  }
}

inline float TL::EDM::LeptonPair::deltaR()   const { return m_deltaR;   }
inline float TL::EDM::LeptonPair::deltaPhi() const { return m_deltaPhi; }
inline float TL::EDM::LeptonPair::deltaEta() const { return m_deltaEta; }

inline bool TL::EDM::LeptonPair::SS()   const { return  m_SS;   }
inline bool TL::EDM::LeptonPair::OS()   const { return  m_OS;   }
inline bool TL::EDM::LeptonPair::mumu() const { return  m_mumu; }
inline bool TL::EDM::LeptonPair::elel() const { return  m_elel; }
inline bool TL::EDM::LeptonPair::elmu() const { return  m_elmu; }
inline bool TL::EDM::LeptonPair::OF()   const { return  m_elmu; }
inline bool TL::EDM::LeptonPair::SF()   const { return !m_elmu; }

inline std::size_t TL::EDM::LeptonPair::firstIdx()  const { return m_fIdx; }
inline std::size_t TL::EDM::LeptonPair::secondIdx() const { return m_sIdx; }

#endif

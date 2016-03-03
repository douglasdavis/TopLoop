/** @file  Lepton.h
 *  @brief TL::EDM::Lepton class header
 *  @class TL::EDM::Lepton
 *  @brief An electron object
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the characteristics of a lepton.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_EDM_Lepton_h
#define TL_EDM_Lepton_h

// ROOT
#include <TLorentzVector.h>

namespace TL {
  namespace EDM {

    class Lepton : public TObject {
    private:
      TLorentzVector m_p;
      unsigned int   m_pdgId;

      ClassDef(Lepton,1);
      
    public:
      Lepton() : m_p(), m_pdgId(0) {}
      virtual ~Lepton() {}

      void set_pdgId(const unsigned int i);

      TLorentzVector& p();

      const TLorentzVector& p() const;

    };

  }
}

inline void TL::EDM::Lepton::set_pdgId(const unsigned int i) { m_pdgId = i; }

inline const TLorentzVector& TL::EDM::Lepton::p() const { return m_p; }
inline       TLorentzVector& TL::EDM::Lepton::p()       { return m_p; }

#endif

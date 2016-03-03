/** @file  Jet.h
 *  @brief TL::EDM::Jet class header
 *  @class TL::EDM::Jet
 *  @brief An electron object
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the characteristics of a jet.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_EDM_Jet_h
#define TL_EDM_Jet_h

// ROOT
#include <TLorentzVector.h>

namespace TL {
  namespace EDM {

    class Jet : public TObject {
    private:
      TLorentzVector m_p;

      ClassDef(Jet,1);
      
    public:
      Jet() : m_p() {}
      virtual ~Jet() {}

      TLorentzVector& p();

      const TLorentzVector& p() const;

    };

  }
}

inline const TLorentzVector& TL::EDM::Jet::p() const { return m_p; }
inline       TLorentzVector& TL::EDM::Jet::p()       { return m_p; }

#endif

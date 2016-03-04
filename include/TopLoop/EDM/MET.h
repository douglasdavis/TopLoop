/** @file  MET.h
 *  @brief TL::EDM::MET class header
 *  @class TL::EDM::MET
 *  @brief A container for MET information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of the missing tranvserse energy.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_EDM_MET_h
#define TL_EDM_MET_h

// ROOT
#include <TLorentzVector.h>

namespace TL {
  namespace EDM {

    class MET : public TObject {
    private:
      TLorentzVector m_p;

      ClassDef(MET,1);

    public:
      MET() : m_p() {}
      virtual ~MET() {}
      
      TLorentzVector& p();

      const TLorentzVector& p() const;

    };

  }
}

inline const TLorentzVector& TL::EDM::MET::p() const { return m_p; }
inline       TLorentzVector& TL::EDM::MET::p()       { return m_p; }

#endif

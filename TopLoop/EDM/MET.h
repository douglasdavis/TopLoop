/** @file  MET.h
 *  @brief TL::EDM::MET class header
 *  @class TL::EDM::MET
 *  @brief A container for MET information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of the missing tranvserse energy.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef TL_EDM_MET_h
#define TL_EDM_MET_h

// TL
#include <TopLoop/EDM/PhysicsObject.h>

namespace TL {
  namespace EDM {

    class MET : public TL::EDM::PhysicsObject {
    private:

      ClassDef(MET,1);

    public:
      MET() : TL::EDM::PhysicsObject() {}
      virtual ~MET() {}

    };

  }
}

#endif

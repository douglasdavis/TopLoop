/** @file  Jet.h
 *  @brief TL::EDM::Jet class header
 *  @class TL::EDM::Jet
 *  @brief A container for jet information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a jet.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_EDM_Jet_h
#define TL_EDM_Jet_h

// TL
#include <TopLoopEDM/PhysicsObject.h>

namespace TL {
  namespace EDM {

    class Jet : public TL::EDM::PhysicsObject {
    private:
      float m_MV1;

      ClassDef(Jet,1);
      
    public:
      Jet() : TL::EDM::PhysicsObject(), m_MV1(0) {}
      virtual ~Jet() {}

      void set_MV1(const float mv1);

      float MV1() const;
      
    };

  }
}

inline void TL::EDM::Jet::set_MV1(const float mv1) { m_MV1 = mv1; }

inline float TL::EDM::Jet::MV1() const { return m_MV1; }

#endif

/** @file  PhysicsObject.h
 *  @brief TL::EDM::PhysicsObject class header
 *  @class TL::EDM::PhysicsObject
 *  @brief A container for physics object information.
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a physics object. This includes
 *  leptons, jets, MET..
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_EDM_PhysicsObject_h
#define TL_EDM_PhysicsObject_h

// ROOT
#include <TLorentzVector.h>

namespace TL {
  namespace EDM {

    class PhysicsObject : public TObject {
    protected:
      TLorentzVector m_p;

      ClassDef(PhysicsObject,1);

    public:
      PhysicsObject() : m_p() {}
      virtual ~PhysicsObject() {}

      TLorentzVector& p();

      const TLorentzVector& p() const;

      float pT()  const;
      float eta() const;
      float phi() const;
      float E()   const;
      float m()   const;
      float px()  const;
      float py()  const;
      float pz()  const;
      
    };

  }
}

inline const TLorentzVector& TL::EDM::PhysicsObject::p() const { return m_p; }
inline       TLorentzVector& TL::EDM::PhysicsObject::p()       { return m_p; }

inline float TL::EDM::PhysicsObject::pT()  const { return m_p.Pt();  }
inline float TL::EDM::PhysicsObject::eta() const { return m_p.Eta(); }
inline float TL::EDM::PhysicsObject::phi() const { return m_p.Phi(); }
inline float TL::EDM::PhysicsObject::E()   const { return m_p.E();   }
inline float TL::EDM::PhysicsObject::m()   const { return m_p.M();   }
inline float TL::EDM::PhysicsObject::px()  const { return m_p.Px();  }
inline float TL::EDM::PhysicsObject::py()  const { return m_p.Py();  }
inline float TL::EDM::PhysicsObject::pz()  const { return m_p.Pz();  }

#endif

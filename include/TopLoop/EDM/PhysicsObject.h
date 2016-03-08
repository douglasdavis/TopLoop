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

      double pT()  const;
      double eta() const;
      double phi() const;
      double E()   const;
      double m()   const;
      double px()  const;
      double py()  const;
      double pz()  const;
      
    };

  }
}

inline const TLorentzVector& TL::EDM::PhysicsObject::p() const { return m_p; }
inline       TLorentzVector& TL::EDM::PhysicsObject::p()       { return m_p; }

inline double TL::EDM::PhysicsObject::pT()  const { return m_p.Pt();  }
inline double TL::EDM::PhysicsObject::eta() const { return m_p.Eta(); }
inline double TL::EDM::PhysicsObject::phi() const { return m_p.Phi(); }
inline double TL::EDM::PhysicsObject::E()   const { return m_p.E();   }
inline double TL::EDM::PhysicsObject::m()   const { return m_p.M();   }
inline double TL::EDM::PhysicsObject::px()  const { return m_p.Px();  }
inline double TL::EDM::PhysicsObject::py()  const { return m_p.Py();  }
inline double TL::EDM::PhysicsObject::pz()  const { return m_p.Pz();  }

#endif

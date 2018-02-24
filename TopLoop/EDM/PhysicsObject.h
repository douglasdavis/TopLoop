/** @file  PhysicsObject.h
 *  @brief TL::EDM::PhysicsObject class header
 *  @class TL::EDM::PhysicsObject
 *  @brief A class for physics object information.
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a physics object. This includes
 *  leptons, jets, MissingET.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_EDM_PhysicsObject_h
#define TL_EDM_PhysicsObject_h

// ROOT
#include <TLorentzVector.h>

namespace TL {
  namespace EDM {

    class PhysicsObject {
    protected:
      TLorentzVector m_p{0,0,0,0};

    public:
      /// default constructor
      PhysicsObject() = default;
      /// default destructor
      virtual ~PhysicsObject() = default;

      PhysicsObject(const PhysicsObject&) = default;
      PhysicsObject& operator=(const PhysicsObject&) = default;

      /// retrieve the four vector
      TLorentzVector& p4();
      /// retrieve the const four vector
      const TLorentzVector& p4() const;

      /// get the \f$p_\mathrm{T}\f$.
      float pT()   const;
      /// get the pseudorapidity, \f$\eta\f$.
      float eta()  const;
      /// get the \f$|\eta|\f$.
      float aeta() const;
      /// get the \f$\phi\f$ (angle in the transerve plane).
      float phi()  const;
      /// get the energy *from the ROOT four vector*.
      float E()    const;
      /// get the mass *from the ROOT four vector*.
      float mass() const;
      /// get the x-component of the momentum.
      float px()   const;
      /// get the y-compnent of the momentum.
      float py()   const;
      /// get the z-compnent of the momentum.
      float pz()   const;

    };

  }
}

inline const TLorentzVector& TL::EDM::PhysicsObject::p4() const { return m_p; }
inline       TLorentzVector& TL::EDM::PhysicsObject::p4()       { return m_p; }

inline float TL::EDM::PhysicsObject::pT()   const { return m_p.Pt();            }
inline float TL::EDM::PhysicsObject::eta()  const { return m_p.Eta();           }
inline float TL::EDM::PhysicsObject::aeta() const { return std::abs(m_p.Eta()); }
inline float TL::EDM::PhysicsObject::phi()  const { return m_p.Phi();           }
inline float TL::EDM::PhysicsObject::E()    const { return m_p.E();             }
inline float TL::EDM::PhysicsObject::mass() const { return m_p.M();             }
inline float TL::EDM::PhysicsObject::px()   const { return m_p.Px();            }
inline float TL::EDM::PhysicsObject::py()   const { return m_p.Py();            }
inline float TL::EDM::PhysicsObject::pz()   const { return m_p.Pz();            }

#endif

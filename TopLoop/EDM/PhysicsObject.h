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

      /// copy constructor
      PhysicsObject(const PhysicsObject&) = default;
      /// assignment operator
      PhysicsObject& operator=(const PhysicsObject&) = default;
      /// move constructor
      PhysicsObject(PhysicsObject&&) = default;
      /// move assignment
      PhysicsObject& operator=(PhysicsObject&&) = default;

      /// retrieve the four vector
      TLorentzVector& p4() { return m_p; }
      /// retrieve the const four vector
      const TLorentzVector& p4() const { return m_p; }

      /// get the \f$p_\mathrm{T}\f$.
      float pt()   const { return m_p.Pt(); }
      /// get the \f$p_\mathrm{T}\f$.
      float pT()   const { return m_p.Pt(); }
      /// get the pseudorapidity, \f$\eta\f$.
      float eta()  const { return m_p.Eta(); }
      /// get the \f$|\eta|\f$.
      float aeta() const { return std::abs(m_p.Eta()); }
      /// get the \f$\phi\f$ (angle in the transerve plane).
      float phi()  const { return m_p.Phi(); }
      /// get the energy *from the ROOT four vector*.
      float E()    const { return m_p.E(); }
      /// get the mass *from the ROOT four vector*.
      float mass() const { return m_p.M(); }
      /// get the x-component of the momentum.
      float px()   const { return m_p.Px(); }
      /// get the y-compnent of the momentum.
      float py()   const { return m_p.Py(); }
      /// get the z-compnent of the momentum.
      float pz()   const { return m_p.Pz(); }

    };

  }
}

#endif

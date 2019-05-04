/** @file  PhysicsObject.h
 *  @brief nanodm::PhysicsObject class header
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef nanodm_PhysicsObject_h
#define nanodm_PhysicsObject_h

// ROOT
#include <TLorentzVector.h>

/**
 *  @namespace nanodm
 *  @brief nano data model namespace
 */
namespace nanodm {

/**
 *  @class nanodm::PhysicsObject
 *  @brief A base class for physics object information.
 */
class PhysicsObject {
 protected:
  TLorentzVector m_p{0, 0, 0, 0};

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
  virtual float pt() const { return m_p.Pt(); }
  /// get the pseudorapidity, \f$\eta\f$.
  virtual float eta() const { return m_p.Eta(); }
  /// get the \f$|\eta|\f$.
  virtual float abseta() const { return std::abs(m_p.Eta()); }
  /// get the \f$\phi\f$ (angle in the transerve plane).
  virtual float phi() const { return m_p.Phi(); }
  /// get the energy *from the ROOT four vector*.
  virtual float energy() const { return m_p.E(); }
  /// get the mass *from the ROOT four vector*.
  virtual float mass() const { return m_p.M(); }
  /// get the x-component of the momentum.
  virtual float px() const { return m_p.Px(); }
  /// get the y-compnent of the momentum.
  virtual float py() const { return m_p.Py(); }
  /// get the z-compnent of the momentum.
  virtual float pz() const { return m_p.Pz(); }
};

}  // namespace nanodm

#endif

/** @file IPhysicsObject.h
 *  @brief nanodm::IPhysicsObject class header
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef nanodm_IPhysicsObject_h
#define nanodm_IPhysicsObject_h

#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PtEtaPhiE4D.h>
#include <Math/GenVector/PtEtaPhiM4D.h>
#include <Math/Vector4Dfwd.h>

namespace nanodm {

template <class CoordT>
using FourVec = ROOT::Math::LorentzVector<CoordT>;

using CoordJet = ROOT::Math::PtEtaPhiE4D<float>;
using CoordLep = ROOT::Math::PtEtaPhiM4D<float>;
using CoordMET = ROOT::Math::PtEtaPhiM4D<float>;
using CoordSys = ROOT::Math::PtEtaPhiM4D<float>;

/// An interface class to describe a physics object
/**
 *  This abstraction is used as a base to individual objects and to
 *  build PhysicsSystems from collections of individual objects
 *  inheriting from the interface.
 */
template <class CoordT>
class IPhysicsObject {
 public:
  /// default constructor
  virtual ~IPhysicsObject() = default;

  /// retrieve four vector
  virtual FourVec<CoordT>& p4() = 0;
  /// retrieve const four vector
  virtual const FourVec<CoordT>& p4() const = 0;

  /// get the \f$p_\mathrm{T}\f$.
  virtual float pt() const = 0;
  /// get the pseudorapidity, \f$\eta\f$.
  virtual float eta() const = 0;
  /// get the \f$|\eta|\f$.
  virtual float abseta() const = 0;
  /// get the \f$\phi\f$ (angle in the transerve plane).
  virtual float phi() const = 0;
  /// get the energy *from the ROOT four vector*.
  virtual float energy() const = 0;
  /// get the mass *from the ROOT four vector*.
  virtual float mass() const = 0;
  /// get the x-component of the momentum.
  virtual float px() const = 0;
  /// get the y-compnent of the momentum.
  virtual float py() const = 0;
  /// get the z-compnent of the momentum.
  virtual float pz() const = 0;
};
}  // namespace nanodm

#endif

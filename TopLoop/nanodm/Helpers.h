/** @file nanodm/Helpers.h
 *  @brief nanodm Helper functions
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Carl Suster, <carl.suster@cern.ch>
 */

#ifndef nanodm_Helpers_h
#define nanodm_Helpers_h

#include "IPhysicsObject.h"

#include "Math/GenVector/VectorUtil.h"

#include <cmath>
#include <tuple>

namespace nanodm {

/// A description of a collection of physics objects (four vectors)
/**
 *  this class is to be constructed from from variadic arguments
 */
class PhysicsSystem {
 private:
  FourVec<CoordSys> m_p{0, 0, 0, 0};
  float m_ht{0};
  float m_h{0};
  float m_sumenergy{0};

  template <typename PO>
  void addObjects(const PO obj) {
    m_p += obj->p4();
    m_ht += obj->pt();
    m_h += obj->p4().P();
    m_sumenergy += obj->energy();
  }

  template <typename PO, typename... POs>
  void addObjects(const PO obj, const POs... objs) {
    addObjects(obj);
    addObjects(objs...);
  }

 public:
  /// delete empty default constructor
  PhysicsSystem() = delete;
  /// default the destructor
  ~PhysicsSystem() = default;

  /// delete copy
  PhysicsSystem(const PhysicsSystem&) = delete;
  /// delete copy assign
  PhysicsSystem& operator=(const PhysicsSystem&) = delete;

  /// default move
  PhysicsSystem(PhysicsSystem&&) = default;
  /// delete move assign
  PhysicsSystem& operator=(PhysicsSystem&&) = delete;

  /// the only usable constructor
  template <typename... POs>
  PhysicsSystem(const POs... objs) {
    addObjects(objs...);
  }

  /// get the four vector object
  const FourVec<CoordSys>& p4() const { return m_p; }
  /// get the sum of transverse momenta
  float ht() const { return m_ht; }
  /// get the sum of momenta
  float h() const { return m_h; }
  /// get the sum of energy
  float sumenergy() const { return m_sumenergy; }
};

/// @addtogroup nanodm-helpers nanodm Helpers
/// @brief Helper functions for making calculations using `nanodm`
/// classes.
///
/// @{

/// calculate the \f$p_{\mathrm{T}}\f$ of the system of objects.
/**
 *  @param system the list of objects inheriting from
 *  nanodm::IPhysicsObject to use in the calculation.
 */
inline double ptsys(const PhysicsSystem& system) { return system.p4().pt(); }

/// Calculate the \f$H_{\mathrm{T}}\f$ of the system of objects
/**
 *  The \f$H_\mathrm{T}\f$ is the scalar sum of the
 *  \f$p_\mathrm{T}\f$ of all objects.
 *
 *  @param system the list of objects inhjeriting from
 *  nanodm::IPhysicsObject to use in the calculation.
 */
inline double htsys(const PhysicsSystem& system) { return system.ht(); }

/// Calculate the \f$H\f$ of the system of objects
/**
 *  The \f$H\f$ is the scalar sum of the \f$p\f$ of all objects.
 *
 *  @param system the list of objects inhjeriting from
 *  nanodm::IPhysicsObject to use in the calculation.
 */
inline double hsys(const PhysicsSystem& system) { return system.h(); }

/// calculate the \f$\sigma(p^\mathrm{sys}_{\mathrm{T}})\f$ of a system in the final state
/**
 *  The calculation is defined as
 *  \f[
 *     \sigma(p_\mathrm{T}^\mathrm{sys}(o_1,\ldots,o_n)) =
 *        \frac{p_\mathrm{T}^\mathrm{sys}(o_1,\ldots,o_n)}
             {\sqrt{H_\mathrm{T}(o_1,\ldots,o_n)+\sum E_\mathrm{T}}}.
 *  \f]
 *
 *  \f$H_\mathrm{T}\f$ is calculated from the same system of
 *  objects that is used to calculate
 *  \f$p_\mathrm{T}^\mathrm{sys}\f$, where \f$\sum E_\mathrm{T}\f$
 *  is an event property.
 *
 *  Has units of \f$\sqrt{\mathrm{[Energy]}}\f$.
 *
 *  @param system the list of objects inheriting from nanodm::IPhysicsObject to use in the calculation.
 *  @param sumet the \f$\sum E_{\mathrm{T}}\f$ of the event. (sum of \f$p_\mathrm{T}\f$ of all hard
 *         objects contributing to the \f$E_\mathrm{T}^\mathrm{miss}\f$ calculation).
 */
inline double sigma_ptsys(const PhysicsSystem& system, const float sumet) {
  double ptsys = nanodm::ptsys(system);
  double htsys = nanodm::htsys(system);
  return ptsys / std::sqrt(htsys + sumet);
}

/// Calculate the centrality of the system of objects
/**
 *  Centrality is defined as
 *  \f[
 *     C(o_1,\ldots,o_n) =
         \frac{\sum_\limits{i=1}^{n}p_\mathrm{T}^{(i)}}{\sum_\limits{i=1}^{n}E^{(i)}}
 *  \f]
 *
 *  @param system the list of objects inhjeriting from
 *  nanodm::IPhysicsObject to use in the calculation.
 */
inline double centrality(const PhysicsSystem& system) {
  return system.ht() / system.sumenergy();
}

/// Calculate \f$\Delta R\f$ between systems of objects
/**
 *  This function uses ROOT's `ROOT::Math::VectorUtil::DeltaR` function to calculate:
 *
 *  \f[
 *     \Delta R \equiv \sqrt{(\Delta\eta)^2 + (\Delta\phi)^2}.
 *  \f]
 *
 *  @param system1 list of objects (inheriting from nanodm::IPhysicsObject) in system 1.
 *  @param system2 list of objects (inheriting from nanodm::IPhysicsObject) in system 2.
 */
inline double deltaR(const PhysicsSystem& system1, const PhysicsSystem& system2) {
  return ROOT::Math::VectorUtil::DeltaR(system1.p4(), system2.p4());
}

/// Calculate \f$\Delta R\f$ between two single objects
/**
 *  @param obj1 first object
 *  @param obj2 second object
 */
template <typename T1, typename T2>
inline double deltaR(const IPhysicsObject<T1>* obj1, const IPhysicsObject<T2>* obj2) {
  return ROOT::Math::VectorUtil::DeltaR(obj1->p4(), obj2->p4());
}

/// Calculate \f$\Delta p_{\mathrm{T}}\f$ between systems of objects.
/**
 *  @param system1 list of objects (inheriting from nanodm::IPhysicsObject) in system 1.
 *  @param system2 list of objects (inheriting from nanodm::IPhysicsObject) in system 2.
 */
inline double deltapt(const PhysicsSystem& system1, const PhysicsSystem& system2) {
  return system1.p4().pt() - system2.p4().pt();
}

/// Calculate \f$\Delta \phi\f$ between systems of objects.
/**
 *  The result is wrapped to the interval \f$[-\pi, \pi)\f$.
 *
 *  @param system1 list of objects (inheriting from nanodm::IPhysicsObject) in system 1.
 *  @param system2 list of objects (inheriting from nanodm::IPhysicsObject) in system 2.
 */
inline double deltaphi(const PhysicsSystem& system1, const PhysicsSystem& system2) {
  return ROOT::Math::VectorUtil::DeltaPhi(system1.p4(), system2.p4());
}

/// Calculate the transverse mass (\f$m_{\mathrm{T}}\f$) of two object system.
/**
 *  The transverse mass is defined as
 *  \f[
 *     m_{\mathrm{T}} =
 *        \sqrt{2p_{\mathrm{T}}(o_1)p_{\mathrm{T}}(o_2)(1-\cos\Delta\phi)}.
 *  \f]
 *
 *  @param obj1 first object.
 *  @param obj2 second object.
 */
template <typename T1, typename T2>
inline double transverseMass(const IPhysicsObject<T1>* obj1,
                             const IPhysicsObject<T2>* obj2) {
  return std::sqrt(
      2 * obj1->pt() * obj2->pt() *
      (1.0 - std::cos(ROOT::Math::VectorUtil::DeltaPhi(obj1->p4(), obj2->p4()))));
}

/// Calculate the energy to mass ratio of a system.
/**
 *  @param system the list of objects inhjeriting from
 *  nanodm::IPhysicsObject to use in the calculation.
 */
inline double energyMassRatio(const PhysicsSystem& system) {
  return system.p4().energy() / system.p4().mass();
}

/// @}

}  // namespace nanodm

#endif

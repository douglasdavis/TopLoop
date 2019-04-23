/** @file nanodm/Helpers.h
 *  @brief nanodm Helper functions
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Carl Suster, <carl.suster@cern.ch>
 */

#ifndef nanodm_Helpers_h
#define nanodm_Helpers_h

#include <TopLoop/nanodm/PhysicsObject.h>
#include <cmath>
#include <tuple>

namespace nanodm {

/// wrapper around a collection of four vectors
class PhysicsSystem {
 private:
  TLorentzVector m_p{0, 0, 0, 0};  /// four vector
  float m_ht{0};                   /// sum of transverse momenta
  float m_h{0};                    /// sum of momenta
  float m_sumenergy{0};            /// sum of energies

 public:
  PhysicsSystem() = delete;
  ~PhysicsSystem() = default;

  PhysicsSystem(const PhysicsSystem&) = delete;
  PhysicsSystem& operator=(const PhysicsSystem&) = delete;
  PhysicsSystem(PhysicsSystem&&) = delete;
  PhysicsSystem& operator=(PhysicsSystem&&) = delete;

  PhysicsSystem(const std::initializer_list<const nanodm::PhysicsObject*> list) {
    for (const auto& obj : list) {
      m_p += obj->p4();
      m_ht += obj->pt();
      m_h += obj->p4().P();
      m_sumenergy += obj->energy();
    }
  }

  PhysicsSystem(const nanodm::PhysicsObject* object)
      : m_p(object->p4()),
        m_ht(object->pt()),
        m_h(object->p4().P()),
        m_sumenergy(object->energy()) {}

  /// get the four vector
  const TLorentzVector& p4() const { return m_p; }
  /// get the sum of transverse momenta
  float ht() const { return m_ht; }
  /// get the sum of momenta
  float h() const { return m_h; }
  /// get the sum of energy
  float sumenergy() const { return m_sumenergy; }
};

/// calculate the \f$p_{\mathrm{T}}\f$ of the system of objects.
/**
 *  @param system the list of objects inheriting from
 *  TL::EDM::PhysicsObject to use in the calculation.
 */
inline double ptsys(const PhysicsSystem& system) { return system.p4().Pt(); }

/// Calculate the \f$H_{\mathrm{T}}\f$ of the system of objects
/**
 *  The \f$H_\mathrm{T}\f$ is the scalar sum of the
 *  \f$p_\mathrm{T}\f$ of all objects.
 *
 *  @param system the list of objects inhjeriting from
 *  nanodm::PhysicsObject to use in the calculation.
 */
inline double htsys(const PhysicsSystem& system) { return system.ht(); }

/// Calculate the \f$H\f$ of the system of objects
/**
 *  The \f$H\f$ is the scalar sum of the \f$p\f$ of all objects.
 *
 *  @param system the list of objects inhjeriting from
 *  nanodm::PhysicsObject to use in the calculation.
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
 *  @param system the list of objects inheriting from
 *  nanodm::PhysicsObject to use in the calculation.
 *  @param sumet the \f$\sum E_{\mathrm{T}}\f$ of the event. (sum
 *  of \f$p_\mathrm{T}\f$ of all hard objects contributing to the
 *  \f$E_\mathrm{T}^\mathrm{miss}\f$ calculation).
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
 *  nanodm::PhysicsObject to use in the calculation.
 */
inline double centrality(const PhysicsSystem& system) {
  return system.ht() / system.sumenergy();
}

/// Calculate \f$\Delta R\f$ between systems of objects
/**
 *  This function uses the ROOT function `TLorentzVector::DeltaR` to calculate:
 *
 *  \f[
 *     \Delta R \equiv \sqrt{(\Delta\eta)^2 + (\Delta\phi)^2}.
 *  \f]
 *
 *  @param system1 list of objects (inheriting from nanodm::PhysicsObject) in system 1.
 *  @param system2 list of objects (inheriting from nanodm::PhysicsObject) in system 2.
 */
inline double deltaR(const PhysicsSystem& system1, const PhysicsSystem& system2) {
  return system1.p4().DeltaR(system2.p4());
}

/// Calculate \f$\Delta p_{\mathrm{T}}\f$ between systems of objects.
/**
 *  @param system1 list of objects (inheriting from nanodm::PhysicsObject) in system 1.
 *  @param system2 list of objects (inheriting from nanodm::PhysicsObject) in system 2.
 */
inline double deltapt(const PhysicsSystem& system1, const PhysicsSystem& system2) {
  return system1.p4().Pt() - system2.p4().Pt();
}

/// Calculate \f$\Delta \phi\f$ between systems of objects.
/**
 *  The result is wrapped to the interval \f$[-\pi, \pi)\f$.
 *
 *  @param system1 list of objects (inheriting from nanodm::PhysicsObject) in system 1.
 *  @param system2 list of objects (inheriting from nanodm::PhysicsObject) in system 2.
 */
inline double deltaphi(const PhysicsSystem& system1, const PhysicsSystem& system2) {
  return system1.p4().DeltaPhi(system2.p4());
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
inline double transverseMass(const PhysicsObject* obj1, const PhysicsObject* obj2) {
  return std::sqrt(2 * obj1->pt() * obj2->pt() *
                   (1.0 - std::cos(obj1->p4().DeltaPhi(obj2->p4()))));
}

/// Calculate the energy to mass ratio of a system.
/**
 *  @param system the list of objects inhjeriting from
 *  nanodm::PhysicsObject to use in the calculation.
 */
inline double energyMassRatio(const PhysicsSystem& system) {
  return system.p4().E() / system.p4().M();
}

/// Calculate the thrust of the event.
/**
 *  The thrust is defined as
 *  \f[
 *    T =
 *      max_{\hat{n}}
 *      \frac{\sum_i \left| \hat{p}_i . \hat{n} \right|}
 *           {\sum_i \left| \hat{p}_i \right|}
 *  \f]
 *  where \f$\hat{n}\f$ is the unit vector pointing along the thrust axis.
 *  All objects in the event should be included in the calculation.
 *  Returns the thrust magnitude, phi, and eta in that order.
 *
 *  @param objects the list of objects inhjeriting from
 *  nanodm::PhysicsObject to use in the calculation.
 */
inline std::tuple<double, double, double> thrust(
    const std::initializer_list<const nanodm::PhysicsObject*> objects) {
  double numerator = 0;
  double scalarProduct = 0;
  double thrust_mag = 0;
  double thrust_tmp = 0;
  double thrust_phi = 0;
  double thrust_eta = 0;
  TVector3 n{0, 0, 0};
  double hsys = nanodm::hsys(objects);

  for (float phi = -M_PI; phi < M_PI; phi = phi + 0.05) {
    for (float eta = -5; eta < 5; eta = eta + 0.05) {
      numerator = 0;
      n.SetMagThetaPhi(1, 2 * atan(exp(-eta)), phi);

      for (const auto& obj : objects) {
        scalarProduct = n.Dot(obj->p4().Vect());
        if (scalarProduct > 0) numerator += scalarProduct;
      }

      thrust_tmp = numerator / hsys;
      if (thrust_tmp > thrust_mag) {
        thrust_phi = phi;
        thrust_eta = eta;
        thrust_mag = thrust_tmp;
      }
    }
  }
  return std::make_tuple(1 - thrust_mag, thrust_phi, thrust_eta);
}

/// @}

}  // namespace nanodm

#endif

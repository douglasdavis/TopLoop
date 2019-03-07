/** @file EDMHelpers.cxx
 *  @brief TL::EDM helper implementations
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Carl Suster, <carl.suster@cern.ch>
 */

#include <TopLoop/EDM/Helpers.h>
#include <cmath>

TL::EDM::PhysicsSystem::PhysicsSystem(
    const std::initializer_list<TL::EDM::PhysicsObject> list)
    : m_p{0, 0, 0, 0}, m_ht{0}, m_h{0} {
  for (const auto& obj : list) {
    m_p += obj.p4();
    m_ht += obj.pT();
    m_h += obj.p4().P();
  }
}

double TL::EDM::pTsys(const TL::EDM::PhysicsSystem& system) { return system.p4().Pt(); }

double TL::EDM::sigma_pTsys(const TL::EDM::PhysicsSystem& system, const float sumet) {
  double ptsys = TL::EDM::pTsys(system);
  double htsys = TL::EDM::HTsys(system);
  return (ptsys / (htsys + sumet));
}

double TL::EDM::centrality(const TL::EDM::PhysicsSystem& system) {
  return (system.ht() / system.h());
}

double TL::EDM::deltapT(const TL::EDM::PhysicsSystem& system1,
                        const TL::EDM::PhysicsSystem& system2) {
  return (system1.p4().Pt() - system2.p4().Pt());
}

double TL::EDM::transverseMass(const TL::EDM::PhysicsObject& obj1,
                               const TL::EDM::PhysicsObject& obj2) {
  return std::sqrt(2 * obj1.pT() * obj2.pT() *
                   (1 - std::cos(obj1.p4().DeltaPhi(obj2.p4()))));
}

double TL::EDM::energyMassRatio(const TL::EDM::PhysicsSystem& system) {
  return (system.p4().E() / system.p4().M());
}

std::tuple<double, double, double> TL::EDM::thrust(
    const std::initializer_list<TL::EDM::PhysicsObject> objects) {
  double numerator = 0;
  double scalarProduct = 0;
  double thrust_mag = 0;
  double thrust_tmp = 0;
  double thrust_phi = 0;
  double thrust_eta = 0;
  TVector3 n{0, 0, 0};
  double hsys = TL::EDM::Hsys(objects);

  for (float phi = -M_PI; phi < M_PI; phi = phi + 0.05) {
    for (float eta = -5; eta < 5; eta = eta + 0.05) {
      numerator = 0;
      n.SetMagThetaPhi(1, 2 * atan(exp(-eta)), phi);

      for (const auto& obj : objects) {
        scalarProduct = n.Dot(obj.p4().Vect());
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

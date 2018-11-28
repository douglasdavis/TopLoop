/** @file EDMHelpers.cxx
 *  @brief TL::EDM helper implementations
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#include <TopLoop/EDM/Helpers.h>
#include <cmath>

TLorentzVector TL::EDM::system(const TL::EDM::PhysObjList objects) {
  TLorentzVector retVec{0,0,0,0};
  for ( const auto& obj : objects ) {
    retVec += obj.p4();
  }
  return retVec;
}

double TL::EDM::pTsys(const TL::EDM::PhysObjList objects) {
  return system(objects).Pt();
}

double TL::EDM::sigma_pTsys(const TL::EDM::PhysObjList objects,
                            const float sumet) {
  double ptsys = TL::EDM::pTsys(objects);
  double htsys = TL::EDM::HTsys(objects);
  return (ptsys / (htsys + sumet));
}

double TL::EDM::HTsys(const TL::EDM::PhysObjList objects) {
  double ht = 0.0;
  for ( const auto& obj : objects ) {
    ht += obj.p4().Pt();
  }
  return ht;
}

double TL::EDM::Hsys(const TL::EDM::PhysObjList objects) {
  double h = 0.0;
  for ( const auto& obj : objects ) {
    h += obj.p4().P();
  }
  return h;
}

double TL::EDM::centrality(const TL::EDM::PhysObjList objects) {
  double pT_sum = 0.0;
  double p_sum  = 0.0;
  for ( const auto& obj : objects ) {
    pT_sum += obj.pT();
    p_sum  += obj.p4().P();
  }
  return (pT_sum/p_sum);
}

double TL::EDM::deltaR(const TL::EDM::PhysObjList system1,
                       const TL::EDM::PhysObjList system2) {
  TLorentzVector v1 = system(system1);
  TLorentzVector v2 = system(system2);
  return v1.DeltaR(v2);
}

double TL::EDM::deltapT(const TL::EDM::PhysObjList system1,
                        const TL::EDM::PhysObjList system2) {
  TLorentzVector v1 = system(system1);
  TLorentzVector v2 = system(system2);
  return (v1.Pt() - v2.Pt());
}

double TL::EDM::deltaphi(const TL::EDM::PhysObjList system1,
                         const TL::EDM::PhysObjList system2) {
  TLorentzVector v1 = system(system1);
  TLorentzVector v2 = system(system2);
  return v1.DeltaPhi(v2);
}

double TL::EDM::transverseMass(const TL::EDM::PhysicsObject& obj1,
                               const TL::EDM::PhysicsObject& obj2) {
  return std::sqrt(2*obj1.pT()*obj2.pT()*
                   (1-std::cos(obj1.p4().DeltaPhi(obj2.p4()))));
}

double TL::EDM::energyMassRatio(const TL::EDM::PhysObjList objects) {
  TLorentzVector combinedVec = system(objects);
  return (combinedVec.E() / combinedVec.M());
}

std::tuple<double,double,double> TL::EDM::thrust(const PhysObjList& objects) {
  double numerator = 0;
  double scalarProduct = 0;
  double thrust_mag = 0;
  double thrust_tmp = 0;
  double thrust_phi = 0;
  double thrust_eta = 0;
  TVector3 n{0,0,0};
  double hsys = TL::EDM::Hsys(objects);

  for (float phi = -M_PI; phi < M_PI; phi = phi + 0.05) {
    for (float eta = -5; eta < 5; eta = eta + 0.05) {
      numerator = 0;
      n.SetMagThetaPhi(1, 2*atan(exp(-eta)), phi);

      for ( const auto& obj : objects ) {
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

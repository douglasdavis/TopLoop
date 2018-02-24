/** @file EDMHelpers.cxx
 *  @brief TL::EDM helper implementations
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#include <TopLoop/EDM/Helpers.h>
#include <cmath>

double TL::EDM::pTsys(const TL::EDM::PhysObjList& objects) {
  TLorentzVector retVec{0,0,0,0};
  for ( const auto& obj : objects ) {
    retVec += obj.p4();
  }
  return retVec.Pt();
}

double TL::EDM::sigma_pTsys(const TL::EDM::PhysObjList& objects,
                            const float sumet) {
  double ptsys = TL::EDM::pTsys(objects);
  double htsys = TL::EDM::HTsys(objects);
  return (ptsys / (htsys + sumet));
}

double TL::EDM::HTsys(const TL::EDM::PhysObjList& objects) {
  double ht = 0.0;
  for ( const auto& obj : objects ) {
    ht += obj.p4().Pt();
  }
  return ht;
}

double TL::EDM::centrality(const TL::EDM::PhysObjList& objects) {
  double pT_sum = 0.0;
  double p_sum  = 0.0;
  for ( const auto& obj : objects ) {
    pT_sum += obj.pT();
    p_sum  += obj.p4().P();
  }
  return (pT_sum/p_sum);
}

double TL::EDM::deltaR(const TL::EDM::PhysObjList& system1,
                       const TL::EDM::PhysObjList& system2) {
  TLorentzVector v1{0,0,0,0};
  TLorentzVector v2{0,0,0,0};
  for ( const auto& obj : system1 ) {
    v1 += obj.p4();
  }
  for ( const auto& obj : system2 ) {
    v2 += obj.p4();
  }
  return v1.DeltaR(v2);
}

double TL::EDM::deltapT(const TL::EDM::PhysObjList& system1,
                        const TL::EDM::PhysObjList& system2) {
  TLorentzVector v1{0,0,0,0};
  TLorentzVector v2{0,0,0,0};
  for ( const auto& obj : system1 ) {
    v1 += obj.p4();
  }
  for ( const auto& obj : system2 ) {
    v2 += obj.p4();
  }
  return (v1.Pt() - v2.Pt());
}

double TL::EDM::transverseMass(const TL::EDM::PhysicsObject& obj1,
                               const TL::EDM::PhysicsObject& obj2) {
  return std::sqrt(2*obj1.pT()*obj2.pT()*
                   (1-std::cos(obj1.p4().DeltaPhi(obj2.p4()))));
}

double TL::EDM::energyMassRatio(const TL::EDM::PhysObjList& objects) {
  TLorentzVector combinedVec{0,0,0,0};
  for ( const auto& obj : objects ) {
    combinedVec += obj.p4();
  }
  return (combinedVec.E() / combinedVec.M());
  return 0;
}

#include <TopLoop/EDM/Helpers.h>
#include <cmath>

double TL::EDM::pTsys(const ObjList_t& objects) {
  TLorentzVector retVec{0,0,0,0};
  for ( const auto& obj : objects ) {
    retVec += obj.p4();
  }
  return retVec.Pt();
}

double TL::EDM::sigma_pTsys(const ObjList_t& objects, const float sumet) {
  double ptsys = TL::EDM::pTsys(objects);
  double htsys = TL::EDM::HTsys(objects);
  return (ptsys / (htsys + sumet));
}

double TL::EDM::HTsys(const ObjList_t& objects) {
  double ht = 0.0;
  for ( const auto& obj : objects ) {
    ht += obj.p4().Pt();
  }
  return ht;
}

double TL::EDM::centrality(const ObjList_t& objects) {
  double pT_sum = 0.0;
  double p_sum  = 0.0;
  for ( const auto& obj : objects ) {
    pT_sum += obj.pT();
    p_sum  += obj.p4().P();
  }
  return (pT_sum/p_sum);
}

double TL::EDM::deltaR(const ObjList_t& s1, const ObjList_t& s2) {
  TLorentzVector v1{0,0,0,0};
  TLorentzVector v2{0,0,0,0};
  for ( const auto& obj : s1 ) {
    v1 += obj.p4();
  }
  for ( const auto& obj : s2 ) {
    v2 += obj.p4();
  }
  return v1.DeltaR(v2);
}

double TL::EDM::deltapT(const ObjList_t& s1, const ObjList_t& s2) {
  TLorentzVector v1{0,0,0,0};
  TLorentzVector v2{0,0,0,0};
  for ( const auto& obj : s1 ) {
    v1 += obj.p4();
  }
  for ( const auto& obj : s2 ) {
    v2 += obj.p4();
  }
  return (v1.Pt() - v2.Pt());
}

double TL::EDM::transverseMass(const TL::EDM::PhysicsObject& o1, const TL::EDM::PhysicsObject& o2) {
  return std::sqrt(2*o1.pT()*o2.pT()*(1-std::cos(o1.p4().DeltaPhi(o2.p4()))));
}

double TL::EDM::energyMassRatio(const ObjList_t& objects) {
  TLorentzVector combinedVec{0,0,0,0};
  for ( const auto& obj : objects ) {
    combinedVec += obj.p4();
  }
  return (combinedVec.E() / combinedVec.M());
  return 0;
}

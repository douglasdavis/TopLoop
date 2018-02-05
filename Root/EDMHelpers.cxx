#include <TopLoop/EDM/Helpers.h>

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
  TLorentzVector v;
  for ( const auto& obj : objects ) {
    v += obj.p4();
  }
  return (v.Pt() / v.P());
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

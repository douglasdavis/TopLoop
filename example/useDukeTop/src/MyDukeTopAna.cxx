// DT
#include <MyDukeTopAna/MyDukeTopAna.h>
#include <DukeTop/Utils.h>

// C++
#include <iostream>

// ROOT
#include <TLorentzVector.h>

MyDukeTopAna::MyDukeTopAna() :
  DT::AnaBase()
{}

MyDukeTopAna::~MyDukeTopAna() {}

DT::STATUS MyDukeTopAna::init() {
  DT::Info("init()","running init()");
  // ALWAYS MUST CALL THIS FUNCTION in init();
  init_core_vars();

  m_eventCounter = 0;

  return DT::STATUS::Good;
}

DT::STATUS MyDukeTopAna::execute() {
  m_eventCounter++;
  TLorentzVector total;
  for ( auto const& el : DT::zip(*(*el_pt),*(*el_eta),*(*el_phi)) ) {
    auto pt  = boost::get<0>(el);
    auto eta = boost::get<1>(el);
    auto phi = boost::get<2>(el);
    auto m   = 0.511; // MeV
    TLorentzVector tempv;
    tempv.SetPtEtaPhiM(pt,eta,phi,m);
    total += tempv;
  }
  for ( auto const& mu : DT::zip(*(*mu_pt),*(*mu_eta),*(*mu_phi)) ) {
    auto pt  = boost::get<0>(mu);
    auto eta = boost::get<1>(mu);
    auto phi = boost::get<2>(mu);
    auto m   = 105; // MeV
    TLorentzVector tempv;
    tempv.SetPtEtaPhiM(pt,eta,phi,m);
    total += tempv;
  }
  if ( total.M() > 105.5 && m_eventCounter%400 == 0 ) {
    DT::Info("execute()",
	     "All leptons ( > 1 ) invariant mass = "+std::to_string(total.M())+" GeV");
  }
  return DT::STATUS::Good;
}

DT::STATUS MyDukeTopAna::finish() {
  DT::Info("finish()","Total number of events = "+std::to_string(m_eventCounter));
  DT::Info("finish()","running finished()");
  return DT::STATUS::Good;
}

// DT
#include <MyDukeTopAna/MyDukeTopAna.h>
#include <DukeTop/Utils.h>

// C++
#include <iostream>

// ROOT
#include <TLorentzVector.h>
#include <TFile.h>
#include <TH1D.h>

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

DT::STATUS MyDukeTopAna::setupOutput() {

  // setting up a ROOT file to output at the end of the job
  m_outputFile = new TFile("out.root","recreate");

  // we want to save a histogram to the file
  h_eventMass = new TH1D("eventMass",";Event Mass (TeV);Events/100 GeV",100,0,10);
  h_eventHt   = new TH1D("eventHt",  ";H_{T} (TeV);Events/40 GeV",50,0,2);
  
  return DT::STATUS::Good;
}

DT::STATUS MyDukeTopAna::execute() {
  m_eventCounter++;
  TLorentzVector total;
  for ( auto const& el : DT::zip(*(*el_pt),*(*el_eta),*(*el_phi)) ) {
    auto pt  = boost::get<0>(el);
    auto eta = boost::get<1>(el);
    auto phi = boost::get<2>(el);
    TLorentzVector tempv;
    tempv.SetPtEtaPhiM(pt,eta,phi,.511);
    total += tempv;
  }
  for ( auto const& mu : DT::zip(*(*mu_pt),*(*mu_eta),*(*mu_phi)) ) {
    auto pt  = boost::get<0>(mu);
    auto eta = boost::get<1>(mu);
    auto phi = boost::get<2>(mu);
    TLorentzVector tempv;
    tempv.SetPtEtaPhiM(pt,eta,phi,105.0);
    total += tempv;
  }
  for ( auto const& jet : DT::zip(*(*jet_pt),*(*jet_eta),*(*jet_phi),*(*jet_e)) ) {
    auto pt  = boost::get<0>(jet);
    auto eta = boost::get<1>(jet);
    auto phi = boost::get<2>(jet);
    auto e   = boost::get<3>(jet);
    TLorentzVector tempv;
    tempv.SetPtEtaPhiE(pt,eta,phi,e);
    total += tempv;
  }
  TLorentzVector tempmet;
  tempmet.SetPtEtaPhiM(*(*met_met),0.0,*(*met_phi),0.0);
  total += tempmet;
  h_eventMass->Fill(total.M()*DT::TeV);
  auto fillht = (*(*Ht))*DT::TeV;
  h_eventHt->Fill(fillht);
  if ( total.M() > 100.0e3 && m_eventCounter%8000 == 0 ) {
    DT::Info("execute()",
	     "leptons + jets + MET Invariant mass = "+std::to_string(total.M())+" GeV");
  }
  return DT::STATUS::Good;
}

DT::STATUS MyDukeTopAna::finish() {
  DT::Info("finish()","Total number of events = "+std::to_string(m_eventCounter));
  DT::Info("finish()","running finished()");

  // write histograms to output file
  h_eventMass->Write();
  h_eventHt->Write();
  
  // close the output file
  m_outputFile->Close();
  
  return DT::STATUS::Good;
}

// TL
#include <MyTopLoopAna/MyTopLoopAna.h>
#include <TopLoop/Utils.h>

// C++
#include <iostream>

// ROOT
#include <TLorentzVector.h>
#include <TFile.h>
#include <TH1D.h>

MyTopLoopAna::MyTopLoopAna() :
  TL::AnaBase() {
  m_singleTopNtuple = false;
}

MyTopLoopAna::~MyTopLoopAna() {}

TL::STATUS MyTopLoopAna::init() {
  TL::Info("init()","running init()");
  // ALWAYS MUST CALL THIS FUNCTION in init();
  init_core_vars();

  if ( m_singleTopNtuple ) {
    el_n   = new TTreeReaderValue<UInt_t>(*reader(),"el_n");
    mu_n   = new TTreeReaderValue<UInt_t>(*reader(),"mu_n");
    jet_n  = new TTreeReaderValue<UInt_t>(*reader(),"jet_n");

    met_px    = new TTreeReaderValue<float>(*reader(),"met_px");
    met_py    = new TTreeReaderValue<float>(*reader(),"met_py");
    met_sumet = new TTreeReaderValue<float>(*reader(),"met_sumet");

    Ht = new TTreeReaderValue<float>(*reader(),"Ht");
  }
  
  m_eventCounter = 0;
  
  return TL::STATUS::Good;
}

TL::STATUS MyTopLoopAna::setupOutput() {

  // setting up a ROOT file to output at the end of the job
  m_outputFile = new TFile("out.root","recreate");

  // we want to save a histogram to the file
  h_eventMass = new TH1D("eventMass",";Event Mass (TeV);Events/100 GeV",100,0,10);
  h_eventHt   = new TH1D("eventHt",  ";H_{T} (TeV);Events/40 GeV",50,0,2);
  
  return TL::STATUS::Good;
}

TL::STATUS MyTopLoopAna::execute() {
  m_eventCounter++;
  TLorentzVector total;
  // ATLAS Boost is old and lame doesn't include boost::combine.
  /*
  for ( auto const& el : TL::zip(*(*el_pt),*(*el_eta),*(*el_phi)) ) {
    auto pt  = boost::get<0>(el);
    auto eta = boost::get<1>(el);
    auto phi = boost::get<2>(el);
    TLorentzVector tempv;
    tempv.SetPtEtaPhiM(pt,eta,phi,0.511);
    total += tempv;
  }
  for ( auto const& mu : TL::zip(*(*mu_pt),*(*mu_eta),*(*mu_phi)) ) {
    auto pt  = boost::get<0>(mu);
    auto eta = boost::get<1>(mu);
    auto phi = boost::get<2>(mu);
    TLorentzVector tempv;
    tempv.SetPtEtaPhiM(pt,eta,phi,105.7);
    total += tempv;
  }
  for ( auto const& jet : TL::zip(*(*jet_pt),*(*jet_eta),*(*jet_phi),*(*jet_e)) ) {
    auto pt  = boost::get<0>(jet);
    auto eta = boost::get<1>(jet);
    auto phi = boost::get<2>(jet);
    auto e   = boost::get<3>(jet);
    TLorentzVector tempv;
    tempv.SetPtEtaPhiE(pt,eta,phi,e);
    total += tempv;
  }
  */
  for ( size_t i = 0; i < (*el_pt)->size(); ++ i ) {
    auto pt  = (*el_pt)->at(i);
    auto eta = (*el_eta)->at(i);
    auto phi = (*el_phi)->at(i);
    TLorentzVector tempv;
    tempv.SetPtEtaPhiM(pt,eta,phi,0.511);
    total += tempv;    
  }
  for ( size_t i = 0; i < (*mu_pt)->size(); ++ i ) {
    auto pt  = (*mu_pt)->at(i);
    auto eta = (*mu_eta)->at(i);
    auto phi = (*mu_phi)->at(i);
    TLorentzVector tempv;
    tempv.SetPtEtaPhiM(pt,eta,phi,105.7);
    total += tempv;    
  }
  for ( size_t i = 0; i < (*jet_pt)->size(); ++ i ) {
    auto pt  = (*jet_pt)->at(i);
    auto eta = (*jet_eta)->at(i);
    auto phi = (*jet_phi)->at(i);
    auto e   = (*jet_e)->at(i);
    TLorentzVector tempv;
    tempv.SetPtEtaPhiE(pt,eta,phi,e);
    total += tempv;
  }
  
  TLorentzVector tempmet;
  tempmet.SetPtEtaPhiM(*(*met_met),0.0,*(*met_phi),0.0);
  total += tempmet;
  h_eventMass->Fill(total.M()*TL::TeV);
  if ( m_singleTopNtuple ) {
    auto fillht = (*(*Ht))*TL::TeV;
    h_eventHt->Fill(fillht);
  }
  else {
    h_eventHt->Fill(0.0);
  }
  if ( total.M() > 100.0e3 && m_eventCounter%10 == 0 ) {
    TL::Info("execute()",
	     "leptons + jets + MET Invariant mass = "+std::to_string(total.M())+" GeV");
  }
  return TL::STATUS::Good;
}

TL::STATUS MyTopLoopAna::finish() {
  TL::Info("finish()","Total number of events = "+std::to_string(m_eventCounter));
  TL::Info("finish()","running finished()");

  // write histograms to output file
  h_eventMass->Write();
  h_eventHt->Write();
  
  // close the output file
  m_outputFile->Close();
  
  return TL::STATUS::Good;
}

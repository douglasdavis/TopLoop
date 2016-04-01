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
  TL::AnaBase() {}

MyTopLoopAna::~MyTopLoopAna() {}

TL::STATUS MyTopLoopAna::init() {
  TL::Info("init()","running init()");
  // ALWAYS MUST CALL THIS FUNCTION in init();
  init_core_vars();
  
  m_eventCounter = 0;
  
  return TL::STATUS::Good;
}

TL::STATUS MyTopLoopAna::setupOutput() {

  // setting up a ROOT file to output at the end of the job
  m_outputFile = new TFile("out.root","recreate");

  m_outTree = new TTree("nominal_EDM","nominal_EDM");
  m_outTree->Branch("FinalState",&m_finalState);
  
  return TL::STATUS::Good;
}

TL::STATUS MyTopLoopAna::execute() {
  m_eventCounter++;

  for ( size_t i = 0; i < (*el_pt)->size(); ++ i ) {
    auto pt  = (*el_pt)->at(i);
    auto eta = (*el_eta)->at(i);
    auto phi = (*el_phi)->at(i);
    TL::EDM::Lepton lep;
    lep.set_pdgId(11);
    lep.set_charge((*el_charge)->at(i));
    lep.p().SetPtEtaPhiM(pt,eta,phi,0.511);
    m_finalState.addLepton(lep);
  }
  for ( size_t i = 0; i < (*mu_pt)->size(); ++ i ) {
    auto pt  = (*mu_pt)->at(i);
    auto eta = (*mu_eta)->at(i);
    auto phi = (*mu_phi)->at(i);
    TL::EDM::Lepton lep;
    lep.set_pdgId(13);
    lep.set_charge((*mu_charge)->at(i));
    lep.p().SetPtEtaPhiM(pt,eta,phi,105.7);
    m_finalState.addLepton(lep);
  }
  for ( size_t i = 0; i < (*jet_pt)->size(); ++ i ) {
    auto pt  = (*jet_pt)->at(i);
    auto eta = (*jet_eta)->at(i);
    auto phi = (*jet_phi)->at(i);
    auto e   = (*jet_e)->at(i);
    TL::EDM::Jet jet;
    jet.p().SetPtEtaPhiE(pt,eta,phi,e);
    m_finalState.addJet(jet);
  }
  
  m_finalState.MET().p().SetPtEtaPhiM(*(*met_met),0.0,*(*met_phi),0.0);

  m_finalState.evaluateSelf();
  
  m_outTree->Fill();
  m_finalState.clear();

  return TL::STATUS::Good;
}

TL::STATUS MyTopLoopAna::finish() {
  TL::Info("finish()","running finished()");
  TL::Info("finish()","Total number of events =",m_eventCounter);

  m_outTree->Write();
  m_outputFile->Close();
  
  return TL::STATUS::Good;
}

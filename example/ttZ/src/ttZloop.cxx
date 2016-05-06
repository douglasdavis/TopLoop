// TL
#include <ttZloop/ttZloop.h>
#include <TopLoop/Utils.h>

// C++
#include <iostream>

// ROOT
#include <TLorentzVector.h>
#include <TFile.h>
#include <TH1D.h>

ttZloop::ttZloop() :
  TL::AnaBase() {}

ttZloop::~ttZloop() {}

TL::STATUS ttZloop::init() {
  TL::Info("init()","running init()");
  // ALWAYS MUST CALL THIS FUNCTION in init();
  init_core_vars();

  ht      = new TTreeReaderValue<float>(*reader(),"ht");
  eT_miss = new TTreeReaderValue<float>(*reader(),"eT_miss");
  
  m_eventCounter = 0;
  
  return TL::STATUS::Good;
}

TL::STATUS ttZloop::setupOutput() {
  
  // setting up a ROOT file to output at the end of the job
  m_outputFile = new TFile(m_outfileName.c_str(),"recreate");

  // we want to save a histogram to the file
  h_eventMass = new TH1D("eventMass",";Event Mass (TeV);Events/100 GeV",50,0,5);
  h_eventHt   = new TH1D("eventHt",  ";H_{T} (TeV);Events/40 GeV",25,0,1);

  m_nominalEDMTree = new TTree("nominal_EDM","nominal_EDM");
  m_nominalEDMTree->Branch("FinalState",&m_finalState);

  return TL::STATUS::Good;
}

TL::STATUS ttZloop::execute() {
  m_eventCounter++;

  auto nleps = (*el_pt)->size() + (*mu_pt)->size();
  
  if ( nleps > 3 || nleps < 2 ) {
    return TL::STATUS::Skip;
  }
  
  for ( size_t i = 0; i < (*el_pt)->size(); ++i ) {
    auto pt           = (*el_pt)->at(i);
    auto eta          = (*el_eta)->at(i);
    auto phi          = (*el_phi)->at(i);
    TL::EDM::Lepton lep;
    lep.set_pdgId(11);
    lep.p().SetPtEtaPhiM(pt,eta,phi,0.511);
    lep.set_charge((*el_charge)->at(i));
    lep.set_cl_eta((*el_cl_eta)->at(i));
    lep.set_topoetcone20((*el_topoetcone20)->at(i));
    lep.set_ptvarcone20((*el_ptvarcone20)->at(i));
    lep.set_d0sig((*el_d0sig)->at(i));
    lep.set_delta_z0_sintheta((*el_delta_z0_sintheta)->at(i));
    m_finalState.addLepton(lep);
  }
  for ( size_t i = 0; i < (*mu_pt)->size(); ++i ) {
    auto pt  = (*mu_pt)->at(i);
    auto eta = (*mu_eta)->at(i);
    auto phi = (*mu_phi)->at(i);
    TL::EDM::Lepton lep;
    lep.set_pdgId(13);
    lep.p().SetPtEtaPhiM(pt,eta,phi,105.7);
    lep.set_charge((*mu_charge)->at(i));
    lep.set_topoetcone20((*mu_topoetcone20)->at(i));
    lep.set_ptvarcone30((*mu_ptvarcone30)->at(i));
    lep.set_d0sig((*mu_d0sig)->at(i));
    lep.set_delta_z0_sintheta((*mu_delta_z0_sintheta)->at(i));
    m_finalState.addLepton(lep);
  }
  for ( size_t i = 0; i < (*jet_pt)->size(); ++i ) {
    auto pt  = (*jet_pt)->at(i);
    auto eta = (*jet_eta)->at(i);
    auto phi = (*jet_phi)->at(i);
    auto e   = (*jet_e)->at(i);
    TL::EDM::Jet jet;
    jet.p().SetPtEtaPhiE(pt,eta,phi,e);
    m_finalState.addJet(jet);
  }

  m_finalState.MET().p().SetPtEtaPhiM(*(*met_met),0.0,*(*met_phi),0.0);

  h_eventMass->Fill(m_finalState.M()/TL::TeV);
  m_finalState.evaluateSelf();

  auto dr_ht = *(*ht);

  m_nominalEDMTree->Fill();
  
  m_finalState.clear();

  return TL::STATUS::Good;
}

TL::STATUS ttZloop::finish() {
  TL::Info("finish()","running finished()");
  TL::Info("finish()","Total number of events =",m_eventCounter);

  // write histograms to output file
  h_eventMass->Write();
  h_eventHt->Write();

  m_nominalEDMTree->Write();

  // close the output file
  m_outputFile->Close();
  
  return TL::STATUS::Good;
}

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
  h_eventMass = new TH1D("eventMass",";Event Mass (TeV);Events/100 GeV",100,0,10);
  h_eventHt   = new TH1D("eventHt",  ";H_{T} (TeV);Events/40 GeV",50,0,2);

  m_outTree = new TTree("nominal_EDM","nominal_EDM");
  m_outTree->Branch("FinalState",&m_finalState);
  
  return TL::STATUS::Good;
}

TL::STATUS ttZloop::execute() {
  m_eventCounter++;
  // ATLAS Boost is old and lame doesn't include boost::combine.
  // Leaving this code here for one day if they do have it.
  /*
    for ( auto const& el : TL::zip(*(*el_pt),*(*el_eta),*(*el_phi)) ) {
    auto pt  = boost::get<0>(el);
    auto eta = boost::get<1>(el);
    auto phi = boost::get<2>(el);
    }
  */

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

  TL::EDM::MET met;
  met.p().SetPtEtaPhiM(*(*met_met),0.0,*(*met_phi),0.0);
  m_finalState.setMET(met);

  h_eventMass->Fill(m_finalState.M()*TL::TeV);
  m_finalState.evaluateSelf();

  if ( m_finalState.leptons().size() > 1  ) {
    if ( m_eventCounter%2000 == 0 ) {
      TL::Info("execute()",
	       met.pT()*TL::GeV - *(*eT_miss),
	       *(*eT_miss),
	       met.pT()*TL::GeV);
      TL::Info("execute()",
	       m_finalState.leptons().at(0).charge(),
	       m_finalState.leptons().at(1).charge(),
	       m_finalState.leptons().at(0).pdgId(),
	       m_finalState.leptons().at(1).pdgId(),"SS",
	       m_finalState.leptonPairs().at(0).SS(),"OS",
	       m_finalState.leptonPairs().at(0).OS(),"elel",
	       m_finalState.leptonPairs().at(0).elel(),"mumu",
	       m_finalState.leptonPairs().at(0).mumu(),"elmu",
	       m_finalState.leptonPairs().at(0).elmu());
    }
  }

  auto dr_ht = *(*ht);

  h_eventHt->Fill(dr_ht*TL::GeVtoTeV);
  
  m_outTree->Fill();
  m_finalState.clear();

  return TL::STATUS::Good;
}

TL::STATUS ttZloop::finish() {
  TL::Info("finish()","running finished()");
  TL::Info("finish()","Total number of events =",m_eventCounter);

  // write histograms to output file
  h_eventMass->Write();
  h_eventHt->Write();
  m_outTree->Write();
  // close the output file
  m_outputFile->Close();
  
  return TL::STATUS::Good;
}

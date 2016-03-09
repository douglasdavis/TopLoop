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

  // we want to save a histogram to the file
  h_eventMass = new TH1D("eventMass",";Event Mass (TeV);Events/100 GeV",100,0,10);
  h_eventHt   = new TH1D("eventHt",  ";H_{T} (TeV);Events/40 GeV",50,0,2);

  m_outTree = new TTree("nominal_EDM","nominal_EDM");
  m_outTree->Branch("FinalState",&m_finalState);
  
  return TL::STATUS::Good;
}

TL::STATUS MyTopLoopAna::execute() {
  m_eventCounter++;
  TLorentzVector total;
  total.SetPxPyPzE(0,0,0,0);
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
    total += lep.p();
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
    total += lep.p();
  }
  for ( size_t i = 0; i < (*jet_pt)->size(); ++ i ) {
    auto pt  = (*jet_pt)->at(i);
    auto eta = (*jet_eta)->at(i);
    auto phi = (*jet_phi)->at(i);
    auto e   = (*jet_e)->at(i);
    TL::EDM::Jet jet;
    jet.p().SetPtEtaPhiE(pt,eta,phi,e);
    m_finalState.addJet(jet);
    total += jet.p();
  }
  
  TL::EDM::LeptonPair lp(m_finalState.leptons().at(0),
			 m_finalState.leptons().at(1));
  m_finalState.addLeptonPair(lp);
  
  TL::EDM::MET met;
  met.p().SetPtEtaPhiM(*(*met_met),0.0,*(*met_phi),0.0);
  m_finalState.setMET(met);
  
  TLorentzVector tempmet;
  tempmet.SetPtEtaPhiM(*(*met_met),0.0,*(*met_phi),0.0);
  total += tempmet;
  h_eventMass->Fill(total.M()*TL::TeV);
  m_finalState.evaluateSelf();
  if ( total.M() > 100.0e3 && m_eventCounter%50 == 0 ) {
    TL::Info("execute()",
	     "leptons + jets + MET Invariant mass = "+std::to_string(total.M())+" GeV, "
	     +std::to_string(m_finalState.M()));
  }

  if ( m_eventCounter%20 ) {
    TL::Info("execute()",
	     std::to_string(m_finalState.leptons().at(0).charge())+" "+
	     std::to_string(m_finalState.leptons().at(1).charge())+" "+
	     std::to_string(m_finalState.leptons().at(0).pdgId()) +" "+
	     std::to_string(m_finalState.leptons().at(1).pdgId()) +" SS "+
	     std::to_string(m_finalState.leptonPairs().at(0).SS())+" OS "+
	     std::to_string(m_finalState.leptonPairs().at(0).OS())+" elel "+
	     std::to_string(m_finalState.leptonPairs().at(0).elel())+" mumu "+
	     std::to_string(m_finalState.leptonPairs().at(0).mumu())+" elmu "+
	     std::to_string(m_finalState.leptonPairs().at(0).elmu()));
  }
  
  m_outTree->Fill();
  m_finalState.clear();
  
  return TL::STATUS::Good;
}

TL::STATUS MyTopLoopAna::finish() {
  TL::Info("finish()","Total number of events = "+std::to_string(m_eventCounter));
  TL::Info("finish()","running finished()");

  // write histograms to output file
  h_eventMass->Write();
  h_eventHt->Write();
  m_outTree->Write();
  // close the output file
  m_outputFile->Close();
  
  return TL::STATUS::Good;
}

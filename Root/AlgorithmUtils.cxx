/** @file AlgorithmUtils.cxx
 *  @brief TL::Algorithm class utility function implementations
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/EDM/FinalState.h>
#include <TopLoop/EDM/Electron.h>
#include <TopLoop/EDM/Muon.h>

// C++
#include <cmath>

float TL::Algorithm::countSumWeights() {
  // sum up the weighted number of events in the metadata tree.  This
  // works for MC (to get the MC lumi) and data (perhaps as a
  // cross-check)
  float sumWeights = 0;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      logger()->critical("countSumWeights(): Tree reader does not return kEntryValid");
    }
    sumWeights += totalEventsWeighted();
  }
  m_weightsReader->Restart();
  //todo: cross-check the value with Ami, warn if different?

  logger()->debug("Value of countSumWeights(): {}",sumWeights);
  return sumWeights;
}

std::vector<float> TL::Algorithm::generatorVariedSumWeights() {
  std::size_t vsize = 0;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    vsize = totalEventsWeighted_mc_generator_weights().size();
    break;
  }
  std::vector<float> weights(vsize,0.0);
  m_weightsReader->Restart();

  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      logger()->critical("generatorVariedSumWeights(): Tree reader does not return kEntryValid");
    }
    // now get all the rest
    for ( std::size_t j = 0; j < totalEventsWeighted_mc_generator_weights().size(); ++j ) {
      weights[j] += totalEventsWeighted_mc_generator_weights().at(j);
    }
  }
  m_weightsReader->Restart();

  //todo: cross-check the value with Ami, warn if different?
  return weights;
}

std::vector<std::string> TL::Algorithm::generatorWeightNames() {
  m_weightsReader->Restart();
  std::vector<std::string> retvec;
  while ( m_weightsReader->Next() ) {
    retvec = names_mc_generator_weights();
    break;
  }
  m_weightsReader->Restart();
  return retvec;
}

unsigned int TL::Algorithm::get_dsid() {
  unsigned int ret_dsid = 999999;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    ret_dsid = dsid();
    break;
  }
  m_weightsReader->Restart();
  return ret_dsid;
}

void TL::Algorithm::printProgress(int n_prints) const {
  if ( m_totalEntries > n_prints ) {
    int gap = m_totalEntries/n_prints;
    if ( m_eventCounter%gap == 0 ) {
      auto progress = std::round(100.0*m_eventCounter/m_totalEntries);
      logger()->info("-- [{:3.0f}%] Event: {}",progress,m_eventCounter);
    }
  }
  return;
}

TL::StatusCode TL::Algorithm::addElectronsToFS(TL::EDM::FinalState& fs) const {
  for ( std::size_t i = 0; i < el_pt().size(); ++i ) {
    TL::EDM::Electron lep;
    float pt  = el_pt().at(i);
    float eta = el_eta().at(i);
    float phi = el_phi().at(i);
    lep.p().SetPtEtaPhiM(pt,eta,phi,0.510999);
    lep.set_charge(el_charge().at(i));
    lep.set_e_branch(el_e().at(i));
    fs.addElectron(lep);
  }
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::addMuonsToFS(TL::EDM::FinalState& fs) const {
  for ( std::size_t i = 0; i < mu_pt().size(); ++i ) {
    TL::EDM::Muon lep;
    float pt  = mu_pt().at(i);
    float eta = mu_eta().at(i);
    float phi = mu_phi().at(i);
    lep.p().SetPtEtaPhiM(pt,eta,phi,105.658);
    lep.set_charge(mu_charge().at(i));
    lep.set_e_branch(mu_e().at(i));
    fs.addMuon(lep);
  }
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::addJetsToFS(TL::EDM::FinalState& fs,
                                          const float ptcut,
                                          const float etacut) const {
  for ( std::size_t i = 0; i < jet_pt().size(); ++i ) {
    TL::EDM::Jet jet;
    float pt  = jet_pt().at(i);
    float eta = jet_eta().at(i);
    if ( (pt < ptcut) || (std::abs(eta) > etacut) ) continue;
    float phi = jet_phi().at(i);
    float ene = jet_e().at(i);
    jet.p().SetPtEtaPhiE(pt,eta,phi,ene);
    jet.set_isbtagged_MV2c10_77(jet_isbtagged_MV2c10_77().at(i));
    fs.addJet(jet);
  }
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::addMETtoFS(TL::EDM::FinalState& fs) const {
  fs.MET().p().SetPtEtaPhiM(met_met(),0.0,met_phi(),0.0);
  fs.MET().set_px(met_px());
  fs.MET().set_py(met_py());
  fs.MET().set_sumet(met_sumet());
  return TL::StatusCode::SUCCESS;
}

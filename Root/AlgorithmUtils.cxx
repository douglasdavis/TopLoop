/** @file AlgorithmUtils.cxx
 *  @brief TL::Algorithm class utility function implementations
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
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

TL::StatusCode TL::Algorithm::addElectronsToFS(TL::EDM::FinalState* fs) const {
  for ( std::size_t i = 0; i < el_pt().size(); ++i ) {
    TL::EDM::Electron lep;
    // four vector
    lep.p4().SetPtEtaPhiM(el_pt().at(i),el_eta().at(i),el_phi().at(i),0.510999);

    // shared lepton info
    lep.set_charge(el_charge().at(i));
    lep.set_topoetcone20(el_topoetcone20().at(i));
    lep.set_d0sig(el_d0sig().at(i));
    lep.set_delta_z0_sintheta(el_delta_z0_sintheta().at(i));
    if ( isMC() ) {
      lep.set_true_type(el_true_type().at(i));
      lep.set_true_origin(el_true_origin().at(i));
    }
    lep.set_e_branch(el_e().at(i));

    // electron only info
    lep.set_ptvarcone20(el_ptvarcone20().at(i));
    lep.set_CF(el_CF().at(i));
    lep.set_true_originbkg(el_true_originbkg().at(i));
    lep.set_true_typebkg(el_true_typebkg().at(i));

    fs->addElectron(lep);
  }
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::addMuonsToFS(TL::EDM::FinalState* fs) const {
  for ( std::size_t i = 0; i < mu_pt().size(); ++i ) {
    TL::EDM::Muon lep;
    // four vector
    lep.p4().SetPtEtaPhiM(mu_pt().at(i),mu_eta().at(i),mu_phi().at(i),0.510999);

    // shared lepton info
    lep.set_charge(mu_charge().at(i));
    lep.set_topoetcone20(mu_topoetcone20().at(i));
    lep.set_d0sig(mu_d0sig().at(i));
    lep.set_delta_z0_sintheta(mu_delta_z0_sintheta().at(i));
    if ( isMC() ) {
      lep.set_true_type(mu_true_type().at(i));
      lep.set_true_origin(mu_true_origin().at(i));
    }
    lep.set_e_branch(mu_e().at(i));

    // muon only info
    lep.set_ptvarcone30(mu_ptvarcone30().at(i));

    fs->addMuon(lep);
  }
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::addJetsToFS(TL::EDM::FinalState* fs,
                                          const float ptcut,
                                          const float etacut,
                                          const bool  do_all_ftagging) const {
  float pt, eta;
  for ( std::size_t i = 0; i < jet_pt().size(); ++i ) {
    TL::EDM::Jet jet;
    pt  = jet_pt().at(i);
    eta = jet_eta().at(i);
    if ( (pt < ptcut) || (std::abs(eta) > etacut) ) continue;
    jet.p4().SetPtEtaPhiE(pt,eta,jet_phi().at(i),jet_e().at(i));

    jet.set_mv2c00(jet_mv2c00().at(i));
    jet.set_mv2c10(jet_mv2c10().at(i));
    jet.set_mv2c20(jet_mv2c20().at(i));
    jet.set_passfjvt(jet_passfjvt().at(i));
    jet.set_isbtagged_MV2c10_70(jet_isbtagged_MV2c10_70().at(i));
    jet.set_isbtagged_MV2c10_77(jet_isbtagged_MV2c10_77().at(i));
    jet.set_isbtagged_MV2c10_85(jet_isbtagged_MV2c10_85().at(i));

    if ( do_all_ftagging ) {
      jet.set_MV2c10mu(jet_MV2c10mu().at(i));
      jet.set_MV2c10rnn(jet_MV2c10rnn().at(i));
      jet.set_DL1(jet_DL1().at(i));
      jet.set_DL1mu(jet_DL1mu().at(i));
      jet.set_DL1rnn(jet_DL1rnn().at(i));
      jet.set_MV2cl100(jet_MV2cl100().at(i));
      jet.set_MV2c100(jet_MV2c100().at(i));
      jet.set_DL1_pu(jet_DL1_pu().at(i));
      jet.set_DL1_pc(jet_DL1_pc().at(i));
      jet.set_DL1_pb(jet_DL1_pb().at(i));
      jet.set_DL1mu_pu(jet_DL1mu_pu().at(i));
      jet.set_DL1mu_pc(jet_DL1mu_pc().at(i));
      jet.set_DL1mu_pb(jet_DL1mu_pb().at(i));
      jet.set_DL1rnn_pu(jet_DL1rnn_pu().at(i));
      jet.set_DL1rnn_pc(jet_DL1rnn_pc().at(i));
      jet.set_DL1rnn_pb(jet_DL1rnn_pb().at(i));
    }

    fs->addJet(jet);
  }
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::addMissingETtoFS(TL::EDM::FinalState* fs) const {
  fs->MissingET().p4().SetPtEtaPhiM(met_met(),0.0,met_phi(),0.0);
  fs->MissingET().set_px(met_px());
  fs->MissingET().set_py(met_py());
  fs->MissingET().set_sumet(met_sumet());
  return TL::StatusCode::SUCCESS;
}

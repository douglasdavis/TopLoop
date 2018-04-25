/** @file AlgorithmVars.cxx
 *  @brief TL::Algorithm class TTreeReader variables implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/FileManager.h>

TL::StatusCode TL::Algorithm::init_core_vars() {

  if ( fileManager() == nullptr ) {
    logger()->error("Your algorithm has a null FileManager");
    return TL::StatusCode::FAILURE;
  }

  // this TChain::LoadTree()) call suppresses a warning from
  // TTreeReader about the entries being changed by multiple
  // controllers
  fileManager()->rootChain()->LoadTree(0);
  fileManager()->rootWeightsChain()->LoadTree(0);
  m_reader        = std::make_shared<TTreeReader>(fileManager()->rootChain());
  m_weightsReader = std::make_shared<TTreeReader>(fileManager()->rootWeightsChain());

  TL_CHECK(connect_default_branches());

  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::connect_default_branches() {

  CONNECT_BRANCH(dsid,Int_t,m_weightsReader);
  if ( isMC() ) {
    CONNECT_BRANCH(totalEventsWeighted,Float_t,m_weightsReader);
    CONNECT_BRANCH(totalEvents,ULong64_t,m_weightsReader);
    CONNECT_BRANCH(totalEventsWeighted_mc_generator_weights,std::vector<float>,m_weightsReader);
    CONNECT_BRANCH(names_mc_generator_weights,std::vector<std::string>,m_weightsReader);
  }

  CONNECT_BRANCH(PDFinfo_X1,std::vector<float>,m_reader);
  CONNECT_BRANCH(PDFinfo_X2,std::vector<float>,m_reader);
  CONNECT_BRANCH(PDFinfo_PDGID1,std::vector<int>,m_reader);
  CONNECT_BRANCH(PDFinfo_PDGID2,std::vector<int>,m_reader);
  CONNECT_BRANCH(PDFinfo_Q,std::vector<float>,m_reader);
  CONNECT_BRANCH(PDFinfo_XF1,std::vector<float>,m_reader);
  CONNECT_BRANCH(PDFinfo_XF2,std::vector<float>,m_reader);
  CONNECT_BRANCH(weights_mc,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_mc,Float_t,m_reader);
  CONNECT_BRANCH(weight_pileup,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85,Float_t,m_reader);
  CONNECT_BRANCH(weight_jvt,Float_t,m_reader);
  CONNECT_BRANCH(weight_pileup_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_pileup_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_EL_SF_Trigger_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_EL_SF_Trigger_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_EL_SF_Reco_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_EL_SF_Reco_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_EL_SF_ID_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_EL_SF_ID_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_EL_SF_Isol_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_EL_SF_Isol_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_Trigger_STAT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_Trigger_STAT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_Trigger_SYST_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_Trigger_SYST_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_ID_STAT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_ID_STAT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_ID_SYST_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_ID_SYST_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_ID_STAT_LOWPT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_ID_STAT_LOWPT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_ID_SYST_LOWPT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_ID_SYST_LOWPT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_Isol_STAT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_Isol_STAT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_Isol_SYST_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_Isol_SYST_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_TTVA_STAT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_TTVA_STAT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_TTVA_SYST_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_MU_SF_TTVA_SYST_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_jvt_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_jvt_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_B_up,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_C_up,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_Light_up,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_B_down,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_C_down,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_Light_down,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_extrapolation_up,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_extrapolation_down,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_extrapolation_from_charm_up,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_70_extrapolation_from_charm_down,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_B_up,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_C_up,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_Light_up,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_B_down,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_C_down,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_Light_down,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_extrapolation_up,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_extrapolation_down,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_extrapolation_from_charm_up,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_77_extrapolation_from_charm_down,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_B_up,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_C_up,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_Light_up,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_B_down,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_C_down,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_Light_down,std::vector<float>,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_extrapolation_up,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_extrapolation_down,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_extrapolation_from_charm_up,Float_t,m_reader);
  CONNECT_BRANCH(weight_bTagSF_MV2c10_85_extrapolation_from_charm_down,Float_t,m_reader);
  CONNECT_BRANCH(eventNumber,ULong64_t,m_reader);
  CONNECT_BRANCH(runNumber,UInt_t,m_reader);
  CONNECT_BRANCH(randomRunNumber,UInt_t,m_reader);
  CONNECT_BRANCH(mcChannelNumber,UInt_t,m_reader);
  CONNECT_BRANCH(mu,Float_t,m_reader);
  CONNECT_BRANCH(backgroundFlags,UInt_t,m_reader);
  CONNECT_BRANCH(hasBadMuon,UInt_t,m_reader);
  CONNECT_BRANCH(el_pt,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_eta,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_cl_eta,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_phi,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_e,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_charge,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_topoetcone20,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_ptvarcone20,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_isTight,std::vector<char>,m_reader);
  CONNECT_BRANCH(el_CF,std::vector<char>,m_reader);
  CONNECT_BRANCH(el_d0sig,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_delta_z0_sintheta,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_true_type,std::vector<int>,m_reader);
  CONNECT_BRANCH(el_true_origin,std::vector<int>,m_reader);
  CONNECT_BRANCH(mu_pt,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_eta,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_phi,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_e,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_charge,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_topoetcone20,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_ptvarcone30,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_isTight,std::vector<char>,m_reader);
  CONNECT_BRANCH(mu_d0sig,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_delta_z0_sintheta,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_true_type,std::vector<int>,m_reader);
  CONNECT_BRANCH(mu_true_origin,std::vector<int>,m_reader);
  CONNECT_BRANCH(jet_pt,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_eta,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_phi,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_e,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_mv2c00,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_mv2c10,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_mv2c20,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_passfjvt,std::vector<char>,m_reader);
  CONNECT_BRANCH(jet_truthflav,std::vector<int>,m_reader);
  CONNECT_BRANCH(jet_truthPartonLabel,std::vector<int>,m_reader);
  CONNECT_BRANCH(jet_isTrueHS,std::vector<char>,m_reader);
  CONNECT_BRANCH(jet_isbtagged_MV2c10_70,std::vector<char>,m_reader);
  CONNECT_BRANCH(jet_isbtagged_MV2c10_77,std::vector<char>,m_reader);
  CONNECT_BRANCH(jet_isbtagged_MV2c10_85,std::vector<char>,m_reader);
  CONNECT_BRANCH(jet_MV2c10mu,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_MV2c10rnn,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1mu,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1rnn,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_MV2cl100,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_MV2c100,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1_pu,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1_pc,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1_pb,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1mu_pu,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1mu_pc,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1mu_pb,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1rnn_pu,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1rnn_pc,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_DL1rnn_pb,std::vector<float>,m_reader);
  CONNECT_BRANCH(met_met,Float_t,m_reader);
  CONNECT_BRANCH(met_phi,Float_t,m_reader);
  CONNECT_BRANCH(all_particle,Int_t,m_reader);
  CONNECT_BRANCH(leptonic_2015,Int_t,m_reader);
  CONNECT_BRANCH(leptonic_2016,Int_t,m_reader);
  CONNECT_BRANCH(ee_2015,Int_t,m_reader);
  CONNECT_BRANCH(ee_2016,Int_t,m_reader);
  CONNECT_BRANCH(mumu_2015,Int_t,m_reader);
  CONNECT_BRANCH(mumu_2016,Int_t,m_reader);
  CONNECT_BRANCH(emu_2015,Int_t,m_reader);
  CONNECT_BRANCH(emu_2016,Int_t,m_reader);
  CONNECT_BRANCH(eee_2015,Int_t,m_reader);
  CONNECT_BRANCH(eee_2016,Int_t,m_reader);
  CONNECT_BRANCH(eemu_2015,Int_t,m_reader);
  CONNECT_BRANCH(eemu_2016,Int_t,m_reader);
  CONNECT_BRANCH(emumu_2015,Int_t,m_reader);
  CONNECT_BRANCH(emumu_2016,Int_t,m_reader);
  CONNECT_BRANCH(mumumu_2015,Int_t,m_reader);
  CONNECT_BRANCH(mumumu_2016,Int_t,m_reader);
  CONNECT_BRANCH(HLT_e60_lhmedium_nod0,Char_t,m_reader);
  CONNECT_BRANCH(HLT_mu26_ivarmedium,Char_t,m_reader);
  CONNECT_BRANCH(HLT_e26_lhtight_nod0_ivarloose,Char_t,m_reader);
  CONNECT_BRANCH(HLT_e140_lhloose_nod0,Char_t,m_reader);
  CONNECT_BRANCH(HLT_e120_lhloose,Char_t,m_reader);
  CONNECT_BRANCH(HLT_e24_lhmedium_L1EM20VH,Char_t,m_reader);
  CONNECT_BRANCH(HLT_mu50,Char_t,m_reader);
  CONNECT_BRANCH(HLT_e60_lhmedium,Char_t,m_reader);
  CONNECT_BRANCH(HLT_mu20_iloose_L1MU15,Char_t,m_reader);
  CONNECT_BRANCH(el_trigMatch_HLT_e60_lhmedium_nod0,std::vector<char>,m_reader);
  CONNECT_BRANCH(el_trigMatch_HLT_e120_lhloose,std::vector<char>,m_reader);
  CONNECT_BRANCH(el_trigMatch_HLT_e24_lhmedium_L1EM20VH,std::vector<char>,m_reader);
  CONNECT_BRANCH(el_trigMatch_HLT_e60_lhmedium,std::vector<char>,m_reader);
  CONNECT_BRANCH(el_trigMatch_HLT_e26_lhtight_nod0_ivarloose,std::vector<char>,m_reader);
  CONNECT_BRANCH(el_trigMatch_HLT_e140_lhloose_nod0,std::vector<char>,m_reader);
  CONNECT_BRANCH(mu_trigMatch_HLT_mu26_ivarmedium,std::vector<char>,m_reader);
  CONNECT_BRANCH(mu_trigMatch_HLT_mu50,std::vector<char>,m_reader);
  CONNECT_BRANCH(mu_trigMatch_HLT_mu20_iloose_L1MU15,std::vector<char>,m_reader);
  CONNECT_BRANCH(lbn,UInt_t,m_reader);
  CONNECT_BRANCH(Vtxz,Float_t,m_reader);
  CONNECT_BRANCH(npVtx,UInt_t,m_reader);
  CONNECT_BRANCH(el_d0pv,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_z0pv,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_d0sigpv,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_z0sigpv,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_tight,std::vector<bool>,m_reader);
  CONNECT_BRANCH(el_trigMatch,std::vector<bool>,m_reader);
  CONNECT_BRANCH(el_true_pdg,std::vector<int>,m_reader);
  CONNECT_BRANCH(el_true_pt,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_true_eta,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_d0pv,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_z0pv,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_d0sigpv,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_z0sigpv,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_tight,std::vector<bool>,m_reader);
  CONNECT_BRANCH(mu_trigMatch,std::vector<bool>,m_reader);
  CONNECT_BRANCH(mu_true_pdg,std::vector<int>,m_reader);
  CONNECT_BRANCH(mu_true_pt,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_true_eta,std::vector<float>,m_reader);
  CONNECT_BRANCH(jet_n,UInt_t,m_reader);
  CONNECT_BRANCH(jet_m,std::vector<float>,m_reader);
  CONNECT_BRANCH(met_px,Float_t,m_reader);
  CONNECT_BRANCH(met_py,Float_t,m_reader);
  CONNECT_BRANCH(met_sumet,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_EL_SF_Trigger_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_EL_SF_Trigger_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_EL_SF_Reco_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_EL_SF_Reco_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_EL_SF_ID_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_EL_SF_ID_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_EL_SF_Isol_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_EL_SF_Isol_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_STAT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_STAT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_SYST_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_SYST_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_ID_STAT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_ID_STAT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_ID_SYST_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_ID_SYST_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_ID_STAT_LOWPT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_ID_STAT_LOWPT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_ID_SYST_LOWPT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_ID_SYST_LOWPT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Isol_STAT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Isol_STAT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Isol_SYST_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_Isol_SYST_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_TTVA_STAT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_TTVA_STAT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_TTVA_SYST_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_leptonSF_tight_MU_SF_TTVA_SYST_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_triggerSF_tight,Float_t,m_reader);
  CONNECT_BRANCH(weight_triggerSF_tight_EL_SF_Trigger_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_triggerSF_tight_EL_SF_Trigger_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_STAT_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_STAT_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_SYST_UP,Float_t,m_reader);
  CONNECT_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_SYST_DOWN,Float_t,m_reader);
  CONNECT_BRANCH(el_weight_triggerSF_tight,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_triggerSF_tight_EL_SF_Trigger_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_triggerSF_tight_EL_SF_Trigger_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_recoIdSF_tight,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_recoIdSF_tight_EL_SF_Reco_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_recoIdSF_tight_EL_SF_Reco_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_recoIdSF_tight_EL_SF_ID_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_recoIdSF_tight_EL_SF_ID_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_isolSF_tight,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_isolSF_tight_EL_SF_Isol_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(el_weight_isolSF_tight_EL_SF_Isol_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_triggerSF_tight,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_STAT_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_STAT_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_SYST_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_SYST_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_STAT_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_STAT_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_SYST_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_SYST_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_STAT_LOWPT_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_STAT_LOWPT_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_SYST_LOWPT_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_SYST_LOWPT_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_TTVA_STAT_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_TTVA_STAT_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_TTVA_SYST_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_recoIdSF_tight_MU_SF_TTVA_SYST_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_isolSF_tight,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_isolSF_tight_MU_SF_Isol_STAT_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_isolSF_tight_MU_SF_Isol_STAT_DOWN,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_isolSF_tight_MU_SF_Isol_SYST_UP,std::vector<float>,m_reader);
  CONNECT_BRANCH(mu_weight_isolSF_tight_MU_SF_Isol_SYST_DOWN,std::vector<float>,m_reader);

  return TL::StatusCode::SUCCESS;
}

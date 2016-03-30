/** @file AnaBase.cxx
 *  @brief TL::AnaBase class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

// TL
#include <TopLoop/AnaBase.h>
#include <TopLoop/FileManager.h>

TL::AnaBase::AnaBase() :
  m_datasetName(),
  m_isMC(true),
  m_isNominal(true){
  core_init();
}

TL::AnaBase::~AnaBase() {}

void TL::AnaBase::core_init() {
  m_fm = std::make_shared<TL::FileManager>();
}

void TL::AnaBase::init_core_vars() {
  m_reader        = std::make_shared<TTreeReader>(fileManager()->rootChain());
  m_weightsReader = std::make_shared<TTreeReader>(fileManager()->rootWeightsChain());

  totalEventsWeighted = std::make_shared<TTRV_float>(*m_weightsReader,"totalEventsWeighted");

  if ( m_isMC ) {
    weight_mc          = std::make_shared<TTRV_float>(*m_reader,"weight_mc");
    weight_pileup      = std::make_shared<TTRV_float>(*m_reader,"weight_pileup");
    weight_leptonSF    = std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF");
    weight_bTagSF_77   = std::make_shared<TTRV_float>(*m_reader,"weight_bTagSF_77");
    weight_jvt         = std::make_shared<TTRV_float>(*m_reader,"weight_jvt");
    if ( m_isNominal ) {
      weight_pileup_UP    = std::make_shared<TTRV_float>(*m_reader,"weight_pileup_UP");
      weight_pileup_DOWN  = std::make_shared<TTRV_float>(*m_reader,"weight_pileup_DOWN");
      weightSyst_leptonSF = {
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_EL_SF_Trigger_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_EL_SF_Trigger_DOWN"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_EL_SF_Reco_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_EL_SF_Reco_DOWN"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_EL_SF_ID_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_EL_SF_ID_DOWN"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_EL_SF_Isol_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_EL_SF_Isol_DOWN"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_Trigger_STAT_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_Trigger_STAT_DOWN"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_Trigger_SYST_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_Trigger_SYST_DOWN"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_ID_STAT_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_ID_STAT_DOWN"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_ID_SYST_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_ID_SYST_DOWN"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_Isol_STAT_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_Isol_STAT_DOWN"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_Isol_SYST_UP"),
        std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF_MU_SF_Isol_SYST_DOWN"),
      };

      weightSyst_bTagSF_extrapolation = {
        std::make_shared<TTRV_float>(*m_reader,"weight_bTagSF_77_extrapolation_up"),
        std::make_shared<TTRV_float>(*m_reader,"weight_bTagSF_77_extrapolation_down"),
        std::make_shared<TTRV_float>(*m_reader,"weight_bTagSF_77_extrapolation_from_charm_up"),
        std::make_shared<TTRV_float>(*m_reader,"weight_bTagSF_77_extrapolation_from_charm_down"),
      };

      //jet eigenvector weights
      weightSyst_bTagSF_eigenvars = {
        std::make_shared<TTRV_vec_float>(*m_reader,"weight_bTagSF_77_eigenvars_B_up"),
        std::make_shared<TTRV_vec_float>(*m_reader,"weight_bTagSF_77_eigenvars_C_up"),
        std::make_shared<TTRV_vec_float>(*m_reader,"weight_bTagSF_77_eigenvars_Light_up"),
        std::make_shared<TTRV_vec_float>(*m_reader,"weight_bTagSF_77_eigenvars_B_down"),
        std::make_shared<TTRV_vec_float>(*m_reader,"weight_bTagSF_77_eigenvars_C_down"),
        std::make_shared<TTRV_vec_float>(*m_reader,"weight_bTagSF_77_eigenvars_Light_down"),
      };
    }
  }

  eventNumber     = std::make_shared<TTRV_ulongint>(*m_reader,"eventNumber");
  runNumber       = std::make_shared<TTRV_uint>    (*m_reader,"runNumber");
  mcChannelNumber = std::make_shared<TTRV_uint>    (*m_reader,"mcChannelNumber");
  mu              = std::make_shared<TTRV_float>   (*m_reader,"mu");
  
  el_pt     = std::make_shared<TTRV_vec_float>(*m_reader,"el_pt");
  el_eta    = std::make_shared<TTRV_vec_float>(*m_reader,"el_eta");
  el_phi    = std::make_shared<TTRV_vec_float>(*m_reader,"el_pt");
  el_e      = std::make_shared<TTRV_vec_float>(*m_reader,"el_e");
  el_charge = std::make_shared<TTRV_vec_float>(*m_reader,"el_charge");
  el_cl_eta = std::make_shared<TTRV_vec_float>(*m_reader,"el_cl_eta");

  el_topoetcone20      = std::make_shared<TTRV_vec_float>(*m_reader,"el_topoetcone20");
  el_ptvarcone20       = std::make_shared<TTRV_vec_float>(*m_reader,"el_ptvarcone20");
  el_d0sig             = std::make_shared<TTRV_vec_float>(*m_reader,"el_d0sig");
  el_delta_z0_sintheta = std::make_shared<TTRV_vec_float>(*m_reader,"el_delta_z0_sintheta");
  
  mu_pt     = std::make_shared<TTRV_vec_float>(*m_reader,"mu_pt");
  mu_eta    = std::make_shared<TTRV_vec_float>(*m_reader,"mu_eta");
  mu_phi    = std::make_shared<TTRV_vec_float>(*m_reader,"mu_pt");
  mu_e      = std::make_shared<TTRV_vec_float>(*m_reader,"mu_e");
  mu_charge = std::make_shared<TTRV_vec_float>(*m_reader,"mu_charge");

  mu_topoetcone20      = std::make_shared<TTRV_vec_float>(*m_reader,"mu_topoetcone20");
  mu_ptvarcone30       = std::make_shared<TTRV_vec_float>(*m_reader,"mu_ptvarcone30");
  mu_d0sig             = std::make_shared<TTRV_vec_float>(*m_reader,"mu_d0sig");
  mu_delta_z0_sintheta = std::make_shared<TTRV_vec_float>(*m_reader,"mu_delta_z0_sintheta");

  jet_pt  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_pt");
  jet_eta = std::make_shared<TTRV_vec_float>(*m_reader,"jet_eta");
  jet_phi = std::make_shared<TTRV_vec_float>(*m_reader,"jet_phi");
  jet_e   = std::make_shared<TTRV_vec_float>(*m_reader,"jet_e");

  jet_mv2c00  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv2c00");
  jet_mv2c10  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv2c10");
  jet_mv2c20  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv2c20");
  jet_ip3dsv1 = std::make_shared<TTRV_vec_float>(*m_reader,"jet_ip3dsv1");
  jet_jvt     = std::make_shared<TTRV_vec_float>(*m_reader,"jet_jvt");

  met_met = std::make_shared<TTRV_float>(*m_reader,"met_met");
  met_phi = std::make_shared<TTRV_float>(*m_reader,"met_phi");

  el_trigMatch_HLT_e60_lhmedium =
    std::make_shared<TTRV_vec_char>(*m_reader,"el_trigMatch_HLT_e60_lhmedium");
  el_trigMatch_HLT_e24_lhmedium_L1EM18VH =
    std::make_shared<TTRV_vec_char>(*m_reader,"el_trigMatch_HLT_e24_lhmedium_L1EM18VH");
  el_trigMatch_HLT_e120_lhloose =
    std::make_shared<TTRV_vec_char>(*m_reader,"el_trigMatch_HLT_e120_lhloose");
  mu_trigMatch_HLT_mu50 =
    std::make_shared<TTRV_vec_char>(*m_reader,"mu_trigMatch_HLT_mu50");
  mu_trigMatch_HLT_mu20_iloose_L1MU15 =
    std::make_shared<TTRV_vec_char>(*m_reader,"mu_trigMatch_HLT_mu20_iloose_L1MU15");
}

float TL::AnaBase::countSumWeights() {
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  float sumWeights = 0;
  while ( m_weightsReader->Next() ) {
    sumWeights += *(*totalEventsWeighted);
    //todo: add some protection for i/o problems?
  }

  //todo: cross-check the value with Ami, warn if different?
  return sumWeights;

}

TL::STATUS TL::AnaBase::init() {
  TL::Warning("init()","This is the base init() class, doesn't do anything!");
  init_core_vars();
  return TL::STATUS::Good;
}

TL::STATUS TL::AnaBase::setupOutput() {
  TL::Warning("setupOutput()","This is the base setupOutput() function, doesn't do anything!");
  return TL::STATUS::Good;
}

TL::STATUS TL::AnaBase::execute() {
  TL::Warning("execute()","This is the base execute() function, doesn't do anything!");
  return TL::STATUS::Good;
}

TL::STATUS TL::AnaBase::finish() {
  TL::Warning("finish()","This is the base finish() function, doesn't do anything!");
  return TL::STATUS::Good;
}

/** @file AnaBase.cxx
 *  @brief TL::AnaBase class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/AnaBase.h>
#include <TopLoop/FileManager.h>

TL::AnaBase::AnaBase() :
  m_datasetName(),
  m_isMC(true),
  m_isNominal(true) {
  core_init();
}

TL::AnaBase::~AnaBase() {}

void TL::AnaBase::core_init() {
  m_fm = std::make_shared<TL::FileManager>();
}

void TL::AnaBase::init_core_vars() {
  m_reader              = std::make_shared<TTreeReader>(fileManager()->rootChain());
  m_weightsReader       = std::make_shared<TTreeReader>(fileManager()->rootWeightsChain());
  m_particleLevelReader = std::make_shared<TTreeReader>(fileManager()->rootParticleLevelChain());

  totalEventsWeighted   = std::make_shared<TTRV_float>(*m_weightsReader,"totalEventsWeighted");
  dsid                  = std::make_shared<TTRV_int>  (*m_weightsReader,"dsid");
  
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
  if ( m_isMC ) { //use randomly generated MC run numbers to mimic data conditions
    runNumber       = std::make_shared<TTRV_uint>    (*m_reader,"randomRunNumber");
  } else {
    runNumber       = std::make_shared<TTRV_uint>    (*m_reader,"runNumber");
  }
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

  if ( m_isMC ) {
    el_truthMatched = std::make_shared<TTRV_vec_bool> (*m_reader,"el_truthMatched");
    el_true_pdg     = std::make_shared<TTRV_vec_int>  (*m_reader,"el_true_pdg");
    el_true_pt      = std::make_shared<TTRV_vec_float>(*m_reader,"el_true_pt");
    el_true_eta     = std::make_shared<TTRV_vec_float>(*m_reader,"el_true_eta");
  }

  mu_pt     = std::make_shared<TTRV_vec_float>(*m_reader,"mu_pt");
  mu_eta    = std::make_shared<TTRV_vec_float>(*m_reader,"mu_eta");
  mu_phi    = std::make_shared<TTRV_vec_float>(*m_reader,"mu_pt");
  mu_e      = std::make_shared<TTRV_vec_float>(*m_reader,"mu_e");
  mu_charge = std::make_shared<TTRV_vec_float>(*m_reader,"mu_charge");

  mu_topoetcone20      = std::make_shared<TTRV_vec_float>(*m_reader,"mu_topoetcone20");
  mu_ptvarcone30       = std::make_shared<TTRV_vec_float>(*m_reader,"mu_ptvarcone30");
  mu_d0sig             = std::make_shared<TTRV_vec_float>(*m_reader,"mu_d0sig");
  mu_delta_z0_sintheta = std::make_shared<TTRV_vec_float>(*m_reader,"mu_delta_z0_sintheta");

  if ( m_isMC ) {
    mu_truthMatched = std::make_shared<TTRV_vec_bool> (*m_reader,"mu_truthMatched");
    mu_true_pdg     = std::make_shared<TTRV_vec_int>  (*m_reader,"mu_true_pdg");
    mu_true_pt      = std::make_shared<TTRV_vec_float>(*m_reader,"mu_true_pt");
    mu_true_eta     = std::make_shared<TTRV_vec_float>(*m_reader,"mu_true_eta");
  }

  jet_pt  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_pt");
  jet_eta = std::make_shared<TTRV_vec_float>(*m_reader,"jet_eta");
  jet_phi = std::make_shared<TTRV_vec_float>(*m_reader,"jet_phi");
  jet_e   = std::make_shared<TTRV_vec_float>(*m_reader,"jet_e");

  jet_mv2c00  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv2c00");
  jet_mv2c10  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv2c10");
  jet_mv2c20  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv2c20");
  jet_ip3dsv1 = std::make_shared<TTRV_vec_float>(*m_reader,"jet_ip3dsv1");
  jet_jvt     = std::make_shared<TTRV_vec_float>(*m_reader,"jet_jvt");

  met_met   = std::make_shared<TTRV_float>(*m_reader,"met_met");
  met_phi   = std::make_shared<TTRV_float>(*m_reader,"met_phi");
  met_sumet = std::make_shared<TTRV_float>(*m_reader,"met_sumet");

  el_trigMatch_HLT_e60_lhmedium =
    std::make_shared<TTRV_vec_char>(*m_reader,"el_trigMatch_HLT_e60_lhmedium");
  el_trigMatch_HLT_e24_lhmedium_L1EM18VH =
    std::make_shared<TTRV_vec_char>(*m_reader,"el_trigMatch_HLT_e24_lhmedium_L1EM20VH");
  el_trigMatch_HLT_e120_lhloose =
    std::make_shared<TTRV_vec_char>(*m_reader,"el_trigMatch_HLT_e120_lhloose");
  mu_trigMatch_HLT_mu50 =
    std::make_shared<TTRV_vec_char>(*m_reader,"mu_trigMatch_HLT_mu50");
  mu_trigMatch_HLT_mu20_iloose_L1MU15 =
    std::make_shared<TTRV_vec_char>(*m_reader,"mu_trigMatch_HLT_mu20_iloose_L1MU15");

  // All of the truth (particleLevel tree, plt) variables {
  if ( m_isMC ) {
    plt_mu                  = std::make_shared<TTRV_float>(*m_particleLevelReader,"mu");
    plt_el_pt               = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"el_pt");
    plt_el_eta              = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"el_eta");
    plt_el_phi              = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"el_phi");
    plt_el_e                = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"el_e");
    plt_el_charge           = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"el_charge");
    plt_el_pt_bare          = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"el_pt_bare");
    plt_el_eta_bare         = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"el_eta_bare");
    plt_el_phi_bare         = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"el_phi_bare");
    plt_el_e_bare           = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"el_e_bare");
    plt_mu_pt               = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"mu_pt");
    plt_mu_eta              = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"mu_eta");
    plt_mu_phi              = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"mu_phi");
    plt_mu_e                = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"mu_e");
    plt_mu_charge           = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"mu_charge");
    plt_mu_pt_bare          = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"mu_pt_bare");
    plt_mu_eta_bare         = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"mu_eta_bare");
    plt_mu_phi_bare         = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"mu_phi_bare");
    plt_mu_e_bare           = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"mu_e_bare");
    plt_jet_pt              = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"jet_pt");
    plt_jet_eta             = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"jet_eta");
    plt_jet_phi             = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"jet_phi");
    plt_jet_e               = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"jet_e");
    plt_jet_nGhosts_bHadron = std::make_shared<TTRV_vec_int>(*m_particleLevelReader,"jet_nGhosts_bHadron");
    plt_met_met             = std::make_shared<TTRV_float>(*m_particleLevelReader,"met_met");
    plt_met_phi             = std::make_shared<TTRV_float>(*m_particleLevelReader,"met_phi");
    plt_PDFinfo_X1          = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"PDFinfo_X1");
    plt_PDFinfo_X2          = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"PDFinfo_X2");
    plt_PDFinfo_PDGID1      = std::make_shared<TTRV_vec_int>(*m_particleLevelReader,"PDFinfo_PDGID1");
    plt_PDFinfo_PDGID2      = std::make_shared<TTRV_vec_int>(*m_particleLevelReader,"PDFinfo_PDGID2");
    plt_PDFinfo_Q           = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"PDFinfo_Q");
    plt_PDFinfo_XF1         = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"PDFinfo_XF1");
    plt_PDFinfo_XF2         = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"PDFinfo_XF2");
    plt_emu_2015            = std::make_shared<TTRV_int>(*m_particleLevelReader,"emu_2015");
    plt_mumu_2016           = std::make_shared<TTRV_int>(*m_particleLevelReader,"mumu_2016");
    plt_mumu_2015           = std::make_shared<TTRV_int>(*m_particleLevelReader,"mumu_2015");
    plt_emu_2016            = std::make_shared<TTRV_int>(*m_particleLevelReader,"emu_2016");
    plt_ee_2016             = std::make_shared<TTRV_int>(*m_particleLevelReader,"ee_2016");
    plt_ee_2015             = std::make_shared<TTRV_int>(*m_particleLevelReader,"ee_2015");
    plt_nu_pt               = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"nu_pt");
    plt_nu_eta              = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"nu_eta");
    plt_nu_phi              = std::make_shared<TTRV_vec_float>(*m_particleLevelReader,"nu_phi");
    plt_nu_origin           = std::make_shared<TTRV_vec_int>(*m_particleLevelReader,"nu_origin");
  }
  // }
}

float TL::AnaBase::countSumWeights() {
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  float sumWeights = 0;
  while ( m_weightsReader->Next() ) {
    if (m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid) {
      TL::Fatal("countSumWeights()", "Tree reader does not return kEntryValid, I/O Error... terminating");
    }
    sumWeights += *(*totalEventsWeighted);
  }

  //todo: cross-check the value with Ami, warn if different?
  return sumWeights;
}

unsigned int TL::AnaBase::get_dsid() {
  m_weightsReader->SetEntry(0);
  auto ret_dsid = *(*dsid);

  // so TTreeReader::Next() can
  // be used again if desired
  m_weightsReader->SetEntry(-1);

  return ret_dsid;
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

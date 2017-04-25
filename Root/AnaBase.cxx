/** @file AnaBase.cxx
 *  @brief TL::AnaBase class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/AnaBase.h>
#include <TopLoop/Core/FileManager.h>

TL::AnaBase::AnaBase() :
  m_datasetName(),
  m_isMC(true),
  m_isNominal(true) {
  m_showTTRVwarning = true;
  m_verbose = false;
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

  totalEventsWeighted = setupTreeVar<TTRV_float>(m_weightsReader,"totalEventsWeighted");
  totalEventsWeighted_mc_generator_weights =
    setupTreeVar<TTRV_vec_float>(m_weightsReader,"totalEventsWeighted_mc_generator_weights");
  names_mc_generator_weights =
    setupTreeVar<TTRV_vec_str>(m_weightsReader,"names_mc_generator_weights");
  dsid = setupTreeVar<TTRV_int>  (m_weightsReader,"dsid");

  if ( m_isMC ) {
    weight_mc          = setupTreeVar<TTRV_float>(m_reader,"weight_mc");
    weight_pileup      = setupTreeVar<TTRV_float>(m_reader,"weight_pileup");
    weight_leptonSF    = setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF");
    weight_bTagSF_77   = setupTreeVar<TTRV_float>(m_reader,"weight_bTagSF_77");
    weight_jvt         = setupTreeVar<TTRV_float>(m_reader,"weight_jvt");

    if ( m_isNominal ) {
      weight_indiv_SF_EL_Trigger = setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_Trigger");
      weight_indiv_SF_EL_Reco    = setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_Reco");
      weight_indiv_SF_EL_ID      = setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_ID");
      weight_indiv_SF_EL_Isol    = setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_Isol");
      weight_indiv_SF_MU_Trigger = setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Trigger");
      weight_indiv_SF_MU_ID      = setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_ID");
      weight_indiv_SF_MU_Isol    = setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Isol");
      weight_indiv_SF_MU_TTVA    = setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_TTVA");
    }

    el_true_type      = setupTreeVar<TTRV_vec_int>(m_reader,"el_true_type");
    el_true_origin    = setupTreeVar<TTRV_vec_int>(m_reader,"el_true_origin");
    el_true_typebkg   = setupTreeVar<TTRV_vec_int>(m_reader,"el_true_typebkg");
    el_true_originbkg = setupTreeVar<TTRV_vec_int>(m_reader,"el_true_originbkg");

    mu_true_type   = setupTreeVar<TTRV_vec_int>(m_reader,"mu_true_type");
    mu_true_origin = setupTreeVar<TTRV_vec_int>(m_reader,"mu_true_origin");

    if ( m_isNominal ) {
      weightSyst_pileup = {
        { "pileup_UP"   , setupTreeVar<TTRV_float>(m_reader,"weight_pileup_UP")   } ,
        { "pileup_DOWN" , setupTreeVar<TTRV_float>(m_reader,"weight_pileup_DOWN") }
      };
      weightSyst_jvt = {
        { "jvt_UP"   , setupTreeVar<TTRV_float>(m_reader,"weight_jvt_UP")   } ,
        { "jvt_DOWN" , setupTreeVar<TTRV_float>(m_reader,"weight_jvt_DOWN") }
      };
      weightSyst_leptonSF = {
        { "leptonSF_EL_SF_Trigger_UP"        , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_EL_SF_Trigger_UP")        } ,
        { "leptonSF_EL_SF_Trigger_DOWN"      , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_EL_SF_Trigger_DOWN")      } ,
        { "leptonSF_EL_SF_Reco_UP"           , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_EL_SF_Reco_UP")           } ,
        { "leptonSF_EL_SF_Reco_DOWN"         , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_EL_SF_Reco_DOWN")         } ,
        { "leptonSF_EL_SF_ID_UP"             , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_EL_SF_ID_UP")             } ,
        { "leptonSF_EL_SF_ID_DOWN"           , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_EL_SF_ID_DOWN")           } ,
        { "leptonSF_EL_SF_Isol_UP"           , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_EL_SF_Isol_UP")           } ,
        { "leptonSF_EL_SF_Isol_DOWN"         , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_EL_SF_Isol_DOWN")         } ,
        { "leptonSF_MU_SF_Trigger_STAT_UP"   , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_Trigger_STAT_UP")   } ,
        { "leptonSF_MU_SF_Trigger_STAT_DOWN" , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_Trigger_STAT_DOWN") } ,
        { "leptonSF_MU_SF_Trigger_SYST_UP"   , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_Trigger_SYST_UP")   } ,
        { "leptonSF_MU_SF_Trigger_SYST_DOWN" , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_Trigger_SYST_DOWN") } ,
        { "leptonSF_MU_SF_ID_STAT_UP"        , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_ID_STAT_UP")        } ,
        { "leptonSF_MU_SF_ID_STAT_DOWN"      , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_ID_STAT_DOWN")      } ,
        { "leptonSF_MU_SF_ID_SYST_UP"        , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_ID_SYST_UP")        } ,
        { "leptonSF_MU_SF_ID_SYST_DOWN"      , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_ID_SYST_DOWN")      } ,
        { "leptonSF_MU_SF_Isol_STAT_UP"      , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_Isol_STAT_UP")      } ,
        { "leptonSF_MU_SF_Isol_STAT_DOWN"    , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_Isol_STAT_DOWN")    } ,
        { "leptonSF_MU_SF_Isol_SYST_UP"      , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_Isol_SYST_UP")      } ,
        { "leptonSF_MU_SF_Isol_SYST_DOWN"    , setupTreeVar<TTRV_float>(m_reader,"weight_leptonSF_MU_SF_Isol_SYST_DOWN")    }
      };
      weightSyst_indivSF = {
        { "indiv_SF_EL_Trigger_UP"           , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_Trigger_UP")         } ,
        { "indiv_SF_EL_Trigger_DOWN"         , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_Trigger_DOWN")       } ,
        { "indiv_SF_EL_Reco_UP"              , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_Reco_UP")            } ,
        { "indiv_SF_EL_Reco_DOWN"            , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_Reco_DOWN")          } ,
        { "indiv_SF_EL_ID_UP"                , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_ID_UP")              } ,
        { "indiv_SF_EL_ID_DOWN"              , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_ID_DOWN")            } ,
        { "indiv_SF_EL_Isol_UP"              , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_Isol_UP")            } ,
        { "indiv_SF_EL_Isol_DOWN"            , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_EL_Isol_DOWN")          } ,
        { "indiv_SF_MU_Trigger_STAT_UP"      , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Trigger_STAT_UP")    } ,
        { "indiv_SF_MU_Trigger_STAT_DOWN"    , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Trigger_STAT_DOWN")  } ,
        { "indiv_SF_MU_Trigger_SYST_UP"      , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Trigger_SYST_UP")    } ,
        { "indiv_SF_MU_Trigger_SYST_DOWN"    , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Trigger_SYST_DOWN")  } ,
        { "indiv_SF_MU_ID_STAT_UP"           , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_ID_STAT_UP")         } ,
        { "indiv_SF_MU_ID_STAT_DOWN"         , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_ID_STAT_DOWN")       } ,
        { "indiv_SF_MU_ID_SYST_UP"           , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_ID_SYST_UP")         } ,
        { "indiv_SF_MU_ID_SYST_DOWN"         , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_ID_SYST_DOWN")       } ,
        { "indiv_SF_MU_ID_STAT_LOWPT_UP"     , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_ID_STAT_LOWPT_UP")   } ,
        { "indiv_SF_MU_ID_STAT_LOWPT_DOWN"   , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_ID_STAT_LOWPT_DOWN") } ,
        { "indiv_SF_MU_ID_SYST_LOWPT_UP"     , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_ID_SYST_LOWPT_UP")   } ,
        { "indiv_SF_MU_ID_SYST_LOWPT_DOWN"   , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_ID_SYST_LOWPT_DOWN") } ,
        { "indiv_SF_MU_Isol_STAT_UP"         , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Isol_STAT_UP")       } ,
        { "indiv_SF_MU_Isol_STAT_DOWN"       , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Isol_STAT_DOWN")     } ,
        { "indiv_SF_MU_Isol_SYST_UP"         , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Isol_SYST_UP")       } ,
        { "indiv_SF_MU_Isol_SYST_DOWN"       , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_Isol_SYST_DOWN")     } ,
        { "indiv_SF_MU_TTVA_STAT_UP"         , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_TTVA_STAT_UP")       } ,
        { "indiv_SF_MU_TTVA_STAT_DOWN"       , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_TTVA_STAT_DOWN")     } ,
        { "indiv_SF_MU_TTVA_SYST_UP"         , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_TTVA_SYST_UP")       } ,
        { "indiv_SF_MU_TTVA_SYST_DOWN"       , setupTreeVar<TTRV_float>(m_reader,"weight_indiv_SF_MU_TTVA_SYST_DOWN")     }
      };
      weightSyst_bTagSF_extrapolation = {
        { "bTagSF_77_extrapolation_up"              , setupTreeVar<TTRV_float>(m_reader,"weight_bTagSF_77_extrapolation_up")              } ,
        { "bTagSF_77_extrapolation_down"            , setupTreeVar<TTRV_float>(m_reader,"weight_bTagSF_77_extrapolation_down")            } ,
        { "bTagSF_77_extrapolation_from_charm_up"   , setupTreeVar<TTRV_float>(m_reader,"weight_bTagSF_77_extrapolation_from_charm_up")   } ,
        { "bTagSF_77_extrapolation_from_charm_down" , setupTreeVar<TTRV_float>(m_reader,"weight_bTagSF_77_extrapolation_from_charm_down") }
      };

      //jet eigenvector weights
      weightSyst_bTagSF_eigenvars = {
        { "bTagSF_77_eigenvars_B_up"       , setupTreeVar<TTRV_vec_float>(m_reader,"weight_bTagSF_77_eigenvars_B_up")       } ,
        { "bTagSF_77_eigenvars_C_up"       , setupTreeVar<TTRV_vec_float>(m_reader,"weight_bTagSF_77_eigenvars_C_up")       } ,
        { "bTagSF_77_eigenvars_Light_up"   , setupTreeVar<TTRV_vec_float>(m_reader,"weight_bTagSF_77_eigenvars_Light_up")   } ,
        { "bTagSF_77_eigenvars_B_down"     , setupTreeVar<TTRV_vec_float>(m_reader,"weight_bTagSF_77_eigenvars_B_down")     } ,
        { "bTagSF_77_eigenvars_C_down"     , setupTreeVar<TTRV_vec_float>(m_reader,"weight_bTagSF_77_eigenvars_C_down")     } ,
        { "bTagSF_77_eigenvars_Light_down" , setupTreeVar<TTRV_vec_float>(m_reader,"weight_bTagSF_77_eigenvars_Light_down") }
      };
    }
  }

  eventNumber     = setupTreeVar<TTRV_ulongint>(m_reader,"eventNumber");
  if ( m_isMC ) { //use randomly generated MC run numbers to mimic data conditions
    runNumber       = setupTreeVar<TTRV_uint>    (m_reader,"randomRunNumber");
  } else {
    runNumber       = setupTreeVar<TTRV_uint>    (m_reader,"runNumber");
  }
  mcChannelNumber = setupTreeVar<TTRV_uint>    (m_reader,"mcChannelNumber");
  mu              = setupTreeVar<TTRV_float>   (m_reader,"mu");

  el_pt     = setupTreeVar<TTRV_vec_float>(m_reader,"el_pt");
  el_eta    = setupTreeVar<TTRV_vec_float>(m_reader,"el_eta");
  el_phi    = setupTreeVar<TTRV_vec_float>(m_reader,"el_pt");
  el_e      = setupTreeVar<TTRV_vec_float>(m_reader,"el_e");
  el_charge = setupTreeVar<TTRV_vec_float>(m_reader,"el_charge");
  el_cl_eta = setupTreeVar<TTRV_vec_float>(m_reader,"el_cl_eta");

  el_topoetcone20      = setupTreeVar<TTRV_vec_float>(m_reader,"el_topoetcone20");
  el_ptvarcone20       = setupTreeVar<TTRV_vec_float>(m_reader,"el_ptvarcone20");
  el_d0sig             = setupTreeVar<TTRV_vec_float>(m_reader,"el_d0sig");
  el_delta_z0_sintheta = setupTreeVar<TTRV_vec_float>(m_reader,"el_delta_z0_sintheta");
  el_CF                = setupTreeVar<TTRV_vec_char> (m_reader,"el_CF");

  mu_pt     = setupTreeVar<TTRV_vec_float>(m_reader,"mu_pt");
  mu_eta    = setupTreeVar<TTRV_vec_float>(m_reader,"mu_eta");
  mu_phi    = setupTreeVar<TTRV_vec_float>(m_reader,"mu_pt");
  mu_e      = setupTreeVar<TTRV_vec_float>(m_reader,"mu_e");
  mu_charge = setupTreeVar<TTRV_vec_float>(m_reader,"mu_charge");

  mu_topoetcone20      = setupTreeVar<TTRV_vec_float>(m_reader,"mu_topoetcone20");
  mu_ptvarcone30       = setupTreeVar<TTRV_vec_float>(m_reader,"mu_ptvarcone30");
  mu_d0sig             = setupTreeVar<TTRV_vec_float>(m_reader,"mu_d0sig");
  mu_delta_z0_sintheta = setupTreeVar<TTRV_vec_float>(m_reader,"mu_delta_z0_sintheta");

  jet_pt  = setupTreeVar<TTRV_vec_float>(m_reader,"jet_pt");
  jet_eta = setupTreeVar<TTRV_vec_float>(m_reader,"jet_eta");
  jet_phi = setupTreeVar<TTRV_vec_float>(m_reader,"jet_phi");
  jet_e   = setupTreeVar<TTRV_vec_float>(m_reader,"jet_e");

  jet_mv2c00       = setupTreeVar<TTRV_vec_float>(m_reader,"jet_mv2c00");
  jet_mv2c10       = setupTreeVar<TTRV_vec_float>(m_reader,"jet_mv2c10");
  jet_mv2c20       = setupTreeVar<TTRV_vec_float>(m_reader,"jet_mv2c20");
  jet_ip3dsv1      = setupTreeVar<TTRV_vec_float>(m_reader,"jet_ip3dsv1");
  jet_jvt          = setupTreeVar<TTRV_vec_float>(m_reader,"jet_jvt");
  jet_isbtagged_77 = setupTreeVar<TTRV_vec_char> (m_reader,"jet_isbtagged_77");

  emu_2015      = setupTreeVar<TTRV_int>(m_reader,"emu_2015");
  emu_2016      = setupTreeVar<TTRV_int>(m_reader,"emu_2016");
  emu_particle  = setupTreeVar<TTRV_int>(m_reader,"emu_particle");
  ee_2015       = setupTreeVar<TTRV_int>(m_reader,"ee_2015");
  ee_2016       = setupTreeVar<TTRV_int>(m_reader,"ee_2016");
  ee_particle   = setupTreeVar<TTRV_int>(m_reader,"ee_particle");
  mumu_2015     = setupTreeVar<TTRV_int>(m_reader,"mumu_2015");
  mumu_2016     = setupTreeVar<TTRV_int>(m_reader,"mumu_2016");
  mumu_particle = setupTreeVar<TTRV_int>(m_reader,"mumu_particle");

  met_met   = setupTreeVar<TTRV_float>(m_reader,"met_met");
  met_phi   = setupTreeVar<TTRV_float>(m_reader,"met_phi");

  HLT_e60_lhmedium_nod0          = setupTreeVar<TTRV_char>(m_reader,"HLT_e60_lhmedium_nod0");
  HLT_mu26_ivarmedium            = setupTreeVar<TTRV_char>(m_reader,"HLT_mu26_ivarmedium");
  HLT_e26_lhtight_nod0_ivarloose = setupTreeVar<TTRV_char>(m_reader,"HLT_e26_lhtight_nod0_ivarloose");
  HLT_e140_lhloose_nod0          = setupTreeVar<TTRV_char>(m_reader,"HLT_e140_lhloose_nod0");
  HLT_mu20_iloose_L1MU15         = setupTreeVar<TTRV_char>(m_reader,"HLT_mu20_iloose_L1MU15");
  HLT_mu50                       = setupTreeVar<TTRV_char>(m_reader,"HLT_mu50");
  HLT_e60_lhmedium               = setupTreeVar<TTRV_char>(m_reader,"HLT_e60_lhmedium");
  HLT_e24_lhmedium_L1EM20VH      = setupTreeVar<TTRV_char>(m_reader,"HLT_e24_lhmedium_L1EM20VH");
  HLT_e120_lhloose               = setupTreeVar<TTRV_char>(m_reader,"HLT_e120_lhloose");

  el_trigMatch_HLT_e60_lhmedium =
    setupTreeVar<TTRV_vec_char>(m_reader,"el_trigMatch_HLT_e60_lhmedium");
  el_trigMatch_HLT_e24_lhmedium_L1EM18VH =
    setupTreeVar<TTRV_vec_char>(m_reader,"el_trigMatch_HLT_e24_lhmedium_L1EM20VH");
  el_trigMatch_HLT_e120_lhloose =
    setupTreeVar<TTRV_vec_char>(m_reader,"el_trigMatch_HLT_e120_lhloose");
  mu_trigMatch_HLT_mu50 =
    setupTreeVar<TTRV_vec_char>(m_reader,"mu_trigMatch_HLT_mu50");
  mu_trigMatch_HLT_mu20_iloose_L1MU15 =
    setupTreeVar<TTRV_vec_char>(m_reader,"mu_trigMatch_HLT_mu20_iloose_L1MU15");

  // All of the truth (particleLevel tree, plt) variables {
  if ( m_isMC ) {
    plt_mu                  = setupTreeVar<TTRV_float>(m_particleLevelReader,"mu");
    plt_el_pt               = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"el_pt");
    plt_el_eta              = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"el_eta");
    plt_el_phi              = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"el_phi");
    plt_el_e                = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"el_e");
    plt_el_charge           = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"el_charge");
    plt_el_pt_bare          = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"el_pt_bare");
    plt_el_eta_bare         = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"el_eta_bare");
    plt_el_phi_bare         = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"el_phi_bare");
    plt_el_e_bare           = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"el_e_bare");
    plt_mu_pt               = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"mu_pt");
    plt_mu_eta              = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"mu_eta");
    plt_mu_phi              = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"mu_phi");
    plt_mu_e                = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"mu_e");
    plt_mu_charge           = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"mu_charge");
    plt_mu_pt_bare          = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"mu_pt_bare");
    plt_mu_eta_bare         = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"mu_eta_bare");
    plt_mu_phi_bare         = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"mu_phi_bare");
    plt_mu_e_bare           = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"mu_e_bare");
    plt_jet_pt              = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"jet_pt");
    plt_jet_eta             = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"jet_eta");
    plt_jet_phi             = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"jet_phi");
    plt_jet_e               = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"jet_e");
    plt_jet_nGhosts_bHadron = setupTreeVar<TTRV_vec_int>(m_particleLevelReader,"jet_nGhosts_bHadron");
    plt_met_met             = setupTreeVar<TTRV_float>(m_particleLevelReader,"met_met");
    plt_met_phi             = setupTreeVar<TTRV_float>(m_particleLevelReader,"met_phi");
    plt_PDFinfo_X1          = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"PDFinfo_X1");
    plt_PDFinfo_X2          = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"PDFinfo_X2");
    plt_PDFinfo_PDGID1      = setupTreeVar<TTRV_vec_int>(m_particleLevelReader,"PDFinfo_PDGID1");
    plt_PDFinfo_PDGID2      = setupTreeVar<TTRV_vec_int>(m_particleLevelReader,"PDFinfo_PDGID2");
    plt_PDFinfo_Q           = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"PDFinfo_Q");
    plt_PDFinfo_XF1         = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"PDFinfo_XF1");
    plt_PDFinfo_XF2         = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"PDFinfo_XF2");
    plt_emu_2015            = setupTreeVar<TTRV_int>(m_particleLevelReader,"emu_2015");
    plt_mumu_2016           = setupTreeVar<TTRV_int>(m_particleLevelReader,"mumu_2016");
    plt_mumu_2015           = setupTreeVar<TTRV_int>(m_particleLevelReader,"mumu_2015");
    plt_emu_2016            = setupTreeVar<TTRV_int>(m_particleLevelReader,"emu_2016");
    plt_ee_2016             = setupTreeVar<TTRV_int>(m_particleLevelReader,"ee_2016");
    plt_ee_2015             = setupTreeVar<TTRV_int>(m_particleLevelReader,"ee_2015");
    plt_nu_pt               = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"nu_pt");
    plt_nu_eta              = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"nu_eta");
    plt_nu_phi              = setupTreeVar<TTRV_vec_float>(m_particleLevelReader,"nu_phi");
    plt_nu_origin           = setupTreeVar<TTRV_vec_int>(m_particleLevelReader,"nu_origin");
  }
  // }

}

float TL::AnaBase::countSumWeights() {
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  float sumWeights = 0;

  //  while ( m_weightsReader->Next() ) { this didnt work WTF.
  for ( int i = 0; i < fileManager()->rootWeightsChain()->GetEntries(); ++i ) {
    m_weightsReader->SetEntry(i);
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      TL::Fatal("countSumWeights()", "Tree reader does not return kEntryValid");
    }
    sumWeights += *(*totalEventsWeighted);
  }
  m_weightsReader->SetEntry(-1);
  //todo: cross-check the value with Ami, warn if different?

  return sumWeights;
}

std::vector<float> TL::AnaBase::generatorVariedSumWeights() {
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  m_weightsReader->SetEntry(0);
  std::vector<float> weights((*totalEventsWeighted_mc_generator_weights)->size(),0.0);
  m_weightsReader->SetEntry(-1);

  for ( int i = 0; i < fileManager()->rootWeightsChain()->GetEntries(); ++i ) {
  //while ( m_weightsReader->Next() ) { //this didnt work WTF.
    m_weightsReader->SetEntry(i);
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      TL::Fatal("generatorVariedSumWeights()","Tree reader does not return kEntryValid");
    }
    // now get all the rest
    for ( std::size_t j = 0; j < (*totalEventsWeighted_mc_generator_weights)->size(); ++j ) {
      weights[j] += (*totalEventsWeighted_mc_generator_weights)->at(j);
    }
  }
  m_weightsReader->SetEntry(-1);

  //todo: cross-check the value with Ami, warn if different?
  return weights;
}

std::vector<std::string> TL::AnaBase::generatorWeightNames() {
  m_weightsReader->SetEntry(0);
  auto retvec = *(*names_mc_generator_weights);
  m_weightsReader->SetEntry(-1);
  return retvec;
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
  TL::Warning(__PRETTY_FUNCTION__,"This is the base init() class, doesn't do anything!");
  init_core_vars();
  return TL::STATUS::Good;
}

TL::STATUS TL::AnaBase::setupOutput() {
  TL::Warning(__PRETTY_FUNCTION__,"This is the base setupOutput() function, doesn't do anything!");
  return TL::STATUS::Good;
}

TL::STATUS TL::AnaBase::execute() {
  TL::Warning(__PRETTY_FUNCTION__,"This is the base execute() function, doesn't do anything!");
  return TL::STATUS::Good;
}

TL::STATUS TL::AnaBase::finish() {
  TL::Warning(__PRETTY_FUNCTION__,"This is the base finish() function, doesn't do anything!");
  return TL::STATUS::Good;
}

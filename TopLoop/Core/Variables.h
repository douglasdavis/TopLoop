/** @file  Variables.h
 *  @brief TL::Variables class header
 *  @class TL::Variables
 *  @brief Base class for all TTreeReaderValue variables for TopLoop
 *         analysis algorithms
 *
 *  A clean place to declare all TTreeReaderValues without saturating
 *  the main Algorithm class header with the TTreeReaderValues. This
 *  class also implements branch access features.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_Variables_h
#define TL_Variables_h

// C++
#include <vector>
#include <memory>

// ROOT
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

// TopLoop
#include <TopLoop/Core/Loggable.h>

#define DECLARE_BRANCH(NAME,TYPE)                                                     \
  protected:                                                                          \
    std::unique_ptr<TTreeReaderValue<TYPE>> bv__##NAME;                               \
  public:                                                                             \
    inline const TYPE& NAME() const {                                                 \
      if ( bv__##NAME ) return *(*bv__##NAME);                                        \
      spdlog::get("TopLoop non-existent branch")->critical("No {} branch!",#NAME);    \
      std::exit(EXIT_FAILURE);                                                        \
    }

#define DECLARE_PL_BRANCH(NAME,TYPE)                                                  \
  protected:                                                                          \
    std::unique_ptr<TTreeReaderValue<TYPE>> bv__pl__##NAME;                           \
  public:                                                                             \
    inline const TYPE& PL_##NAME() const {                                            \
      if ( bv__pl__##NAME ) return *(*bv__pl__##NAME);                                \
      spdlog::get("TopLoop non-existent branch")->critical("No PL_{} branch!",#NAME); \
      std::exit(EXIT_FAILURE);                                                        \
    }

#define CONNECT_BRANCH(NAME,TYPE,READER)                                   \
  bv__##NAME = TL::Variables::setupBranch<TTreeReaderValue<TYPE>>((READER),#NAME);

#define CONNECT_PL_BRANCH(NAME,TYPE,READER)                                \
  bv__pl__##NAME = TL::Variables::setupBranch<TTreeReaderValue<TYPE>>((READER),#NAME);

namespace TL {

  class Variables {
  private:

  public:
    /// default constructor
    Variables() = default;
    /// destructor
    virtual ~Variables() = default;

    /// disable copy constructor
    Variables(const Variables&) = delete;
    /// disable assignment operator
    Variables& operator=(const Variables&) = delete;

    /// Set up a variable as a TTreeReaderValue pointer
    /**
     *  This one liner checks to make sure that the variable is on the
     *  tree. If its not - you get a warning. If the variable isn't
     *  there your program will still work as long as you don't try to
     *  dereference the pointer.
     */
    template<typename T>
    inline static std::unique_ptr<T>
    setupBranch(std::shared_ptr<TTreeReader> reader, const char* name);

  protected:

    DECLARE_BRANCH(dsid                                     , Int_t                    );
    DECLARE_BRANCH(isAFII                                   , Int_t                    );
    DECLARE_BRANCH(totalEventsWeighted                      , Float_t                  );
    DECLARE_BRANCH(totalEvents                              , ULong64_t                );
    DECLARE_BRANCH(totalEventsWeighted_mc_generator_weights , std::vector<float>       );
    DECLARE_BRANCH(names_mc_generator_weights               , std::vector<std::string> );

    DECLARE_BRANCH(PDFinfo_X1                                            , std::vector<float> );
    DECLARE_BRANCH(PDFinfo_X2                                            , std::vector<float> );
    DECLARE_BRANCH(PDFinfo_PDGID1                                        , std::vector<int>   );
    DECLARE_BRANCH(PDFinfo_PDGID2                                        , std::vector<int>   );
    DECLARE_BRANCH(PDFinfo_Q                                             , std::vector<float> );
    DECLARE_BRANCH(PDFinfo_XF1                                           , std::vector<float> );
    DECLARE_BRANCH(PDFinfo_XF2                                           , std::vector<float> );
    DECLARE_BRANCH(mc_generator_weights                                  , std::vector<float> );
    DECLARE_BRANCH(weight_mc                                             , Float_t            );
    DECLARE_BRANCH(weight_pileup                                         , Float_t            );
    DECLARE_BRANCH(weight_leptonSF                                       , Float_t            );
    DECLARE_BRANCH(weight_globalLeptonTriggerSF                          , Float_t            );
    DECLARE_BRANCH(weight_globalLeptonTriggerSF_EL_Trigger_UP            , Float_t            );
    DECLARE_BRANCH(weight_globalLeptonTriggerSF_EL_Trigger_DOWN          , Float_t            );
    DECLARE_BRANCH(weight_globalLeptonTriggerSF_MU_Trigger_STAT_UP       , Float_t            );
    DECLARE_BRANCH(weight_globalLeptonTriggerSF_MU_Trigger_STAT_DOWN     , Float_t            );
    DECLARE_BRANCH(weight_globalLeptonTriggerSF_MU_Trigger_SYST_UP       , Float_t            );
    DECLARE_BRANCH(weight_globalLeptonTriggerSF_MU_Trigger_SYST_DOWN     , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70                               , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77                               , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85                               , Float_t            );
    DECLARE_BRANCH(weight_jvt                                            , Float_t            );
    DECLARE_BRANCH(weight_pileup_UP                                      , Float_t            );
    DECLARE_BRANCH(weight_pileup_DOWN                                    , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Trigger_UP                      , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Trigger_DOWN                    , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Reco_UP                         , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Reco_DOWN                       , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_EL_SF_ID_UP                           , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_EL_SF_ID_DOWN                         , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Isol_UP                         , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Isol_DOWN                       , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Trigger_STAT_UP                 , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Trigger_STAT_DOWN               , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Trigger_SYST_UP                 , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Trigger_SYST_DOWN               , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_STAT_UP                      , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_STAT_DOWN                    , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_SYST_UP                      , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_SYST_DOWN                    , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_STAT_LOWPT_UP                , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_STAT_LOWPT_DOWN              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_SYST_LOWPT_UP                , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_SYST_LOWPT_DOWN              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Isol_STAT_UP                    , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Isol_STAT_DOWN                  , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Isol_SYST_UP                    , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Isol_SYST_DOWN                  , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_TTVA_STAT_UP                    , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_TTVA_STAT_DOWN                  , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_TTVA_SYST_UP                    , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_MU_SF_TTVA_SYST_DOWN                  , Float_t            );
    DECLARE_BRANCH(weight_jvt_UP                                         , Float_t            );
    DECLARE_BRANCH(weight_jvt_DOWN                                       , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_B_up                , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_C_up                , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_Light_up            , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_B_down              , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_C_down              , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_eigenvars_Light_down          , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_extrapolation_up              , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_extrapolation_down            , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_extrapolation_from_charm_up   , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_70_extrapolation_from_charm_down , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_B_up                , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_C_up                , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_Light_up            , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_B_down              , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_C_down              , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_Light_down          , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_extrapolation_up              , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_extrapolation_down            , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_extrapolation_from_charm_up   , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77_extrapolation_from_charm_down , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_B_up                , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_C_up                , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_Light_up            , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_B_down              , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_C_down              , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_eigenvars_Light_down          , std::vector<float> );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_extrapolation_up              , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_extrapolation_down            , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_extrapolation_from_charm_up   , Float_t            );
    DECLARE_BRANCH(weight_bTagSF_MV2c10_85_extrapolation_from_charm_down , Float_t            );
    DECLARE_BRANCH(eventNumber                                           , ULong64_t          );
    DECLARE_BRANCH(runNumber                                             , UInt_t             );
    DECLARE_BRANCH(randomRunNumber                                       , UInt_t             );
    DECLARE_BRANCH(mcChannelNumber                                       , UInt_t             );
    DECLARE_BRANCH(mu                                                    , Float_t            );
    DECLARE_BRANCH(backgroundFlags                                       , UInt_t             );
    DECLARE_BRANCH(hasBadMuon                                            , UInt_t             );
    DECLARE_BRANCH(el_pt                                                 , std::vector<float> );
    DECLARE_BRANCH(el_eta                                                , std::vector<float> );
    DECLARE_BRANCH(el_cl_eta                                             , std::vector<float> );
    DECLARE_BRANCH(el_phi                                                , std::vector<float> );
    DECLARE_BRANCH(el_e                                                  , std::vector<float> );
    DECLARE_BRANCH(el_charge                                             , std::vector<float> );
    DECLARE_BRANCH(el_topoetcone20                                       , std::vector<float> );
    DECLARE_BRANCH(el_ptvarcone20                                        , std::vector<float> );
    DECLARE_BRANCH(el_isTight                                            , std::vector<char>  );
    DECLARE_BRANCH(el_CF                                                 , std::vector<char>  );
    DECLARE_BRANCH(el_d0sig                                              , std::vector<float> );
    DECLARE_BRANCH(el_delta_z0_sintheta                                  , std::vector<float> );
    DECLARE_BRANCH(el_true_type                                          , std::vector<int>   );
    DECLARE_BRANCH(el_true_origin                                        , std::vector<int>   );
    DECLARE_BRANCH(el_true_originbkg                                     , std::vector<int>   );
    DECLARE_BRANCH(el_true_typebkg                                       , std::vector<int>   );
    DECLARE_BRANCH(el_true_firstEgMotherTruthType                        , std::vector<int>   );
    DECLARE_BRANCH(el_true_firstEgMotherTruthOrigin                      , std::vector<int>   );
    DECLARE_BRANCH(el_true_isPrompt                                      , std::vector<char>  );
    DECLARE_BRANCH(mu_pt                                                 , std::vector<float> );
    DECLARE_BRANCH(mu_eta                                                , std::vector<float> );
    DECLARE_BRANCH(mu_phi                                                , std::vector<float> );
    DECLARE_BRANCH(mu_e                                                  , std::vector<float> );
    DECLARE_BRANCH(mu_charge                                             , std::vector<float> );
    DECLARE_BRANCH(mu_topoetcone20                                       , std::vector<float> );
    DECLARE_BRANCH(mu_ptvarcone30                                        , std::vector<float> );
    DECLARE_BRANCH(mu_isTight                                            , std::vector<char>  );
    DECLARE_BRANCH(mu_d0sig                                              , std::vector<float> );
    DECLARE_BRANCH(mu_delta_z0_sintheta                                  , std::vector<float> );
    DECLARE_BRANCH(mu_true_type                                          , std::vector<int>   );
    DECLARE_BRANCH(mu_true_origin                                        , std::vector<int>   );
    DECLARE_BRANCH(mu_true_isPrompt                                      , std::vector<char>  );
    DECLARE_BRANCH(jet_pt                                                , std::vector<float> );
    DECLARE_BRANCH(jet_eta                                               , std::vector<float> );
    DECLARE_BRANCH(jet_phi                                               , std::vector<float> );
    DECLARE_BRANCH(jet_e                                                 , std::vector<float> );
    DECLARE_BRANCH(jet_mv2c00                                            , std::vector<float> );
    DECLARE_BRANCH(jet_mv2c10                                            , std::vector<float> );
    DECLARE_BRANCH(jet_mv2c20                                            , std::vector<float> );
    DECLARE_BRANCH(jet_passfjvt                                          , std::vector<char>  );
    DECLARE_BRANCH(jet_truthflav                                         , std::vector<int>   );
    DECLARE_BRANCH(jet_truthPartonLabel                                  , std::vector<int>   );
    DECLARE_BRANCH(jet_isTrueHS                                          , std::vector<char>  );
    DECLARE_BRANCH(jet_isbtagged_MV2c10_70                               , std::vector<char>  );
    DECLARE_BRANCH(jet_isbtagged_MV2c10_77                               , std::vector<char>  );
    DECLARE_BRANCH(jet_isbtagged_MV2c10_85                               , std::vector<char>  );
    DECLARE_BRANCH(jet_MV2c10mu                                          , std::vector<float> );
    DECLARE_BRANCH(jet_MV2c10rnn                                         , std::vector<float> );
    DECLARE_BRANCH(jet_DL1                                               , std::vector<float> );
    DECLARE_BRANCH(jet_DL1mu                                             , std::vector<float> );
    DECLARE_BRANCH(jet_DL1rnn                                            , std::vector<float> );
    DECLARE_BRANCH(jet_MV2cl100                                          , std::vector<float> );
    DECLARE_BRANCH(jet_MV2c100                                           , std::vector<float> );
    DECLARE_BRANCH(jet_DL1_pu                                            , std::vector<float> );
    DECLARE_BRANCH(jet_DL1_pc                                            , std::vector<float> );
    DECLARE_BRANCH(jet_DL1_pb                                            , std::vector<float> );
    DECLARE_BRANCH(jet_DL1mu_pu                                          , std::vector<float> );
    DECLARE_BRANCH(jet_DL1mu_pc                                          , std::vector<float> );
    DECLARE_BRANCH(jet_DL1mu_pb                                          , std::vector<float> );
    DECLARE_BRANCH(jet_DL1rnn_pu                                         , std::vector<float> );
    DECLARE_BRANCH(jet_DL1rnn_pc                                         , std::vector<float> );
    DECLARE_BRANCH(jet_DL1rnn_pb                                         , std::vector<float> );
    DECLARE_BRANCH(met_met                                               , Float_t            );
    DECLARE_BRANCH(met_phi                                               , Float_t            );
    DECLARE_BRANCH(all_particle                                          , Int_t              );
    DECLARE_BRANCH(leptonic_2015                                         , Int_t              );
    DECLARE_BRANCH(leptonic_2016                                         , Int_t              );
    DECLARE_BRANCH(leptonic_2017                                         , Int_t              );
    DECLARE_BRANCH(leptonic_2018                                         , Int_t              );
    DECLARE_BRANCH(ee_2015                                               , Int_t              );
    DECLARE_BRANCH(ee_2016                                               , Int_t              );
    DECLARE_BRANCH(ee_2017                                               , Int_t              );
    DECLARE_BRANCH(ee_2018                                               , Int_t              );
    DECLARE_BRANCH(ejets_2015                                            , Int_t              );
    DECLARE_BRANCH(ejets_2016                                            , Int_t              );
    DECLARE_BRANCH(ejets_2017                                            , Int_t              );
    DECLARE_BRANCH(ejets_2018                                            , Int_t              );
    DECLARE_BRANCH(mumu_2015                                             , Int_t              );
    DECLARE_BRANCH(mumu_2016                                             , Int_t              );
    DECLARE_BRANCH(mumu_2017                                             , Int_t              );
    DECLARE_BRANCH(mumu_2018                                             , Int_t              );
    DECLARE_BRANCH(mujets_2015                                           , Int_t              );
    DECLARE_BRANCH(mujets_2016                                           , Int_t              );
    DECLARE_BRANCH(mujets_2017                                           , Int_t              );
    DECLARE_BRANCH(mujets_2018                                           , Int_t              );
    DECLARE_BRANCH(emu_2015                                              , Int_t              );
    DECLARE_BRANCH(emu_2016                                              , Int_t              );
    DECLARE_BRANCH(emu_2017                                              , Int_t              );
    DECLARE_BRANCH(emu_2018                                              , Int_t              );
    DECLARE_BRANCH(eee_2015                                              , Int_t              );
    DECLARE_BRANCH(eee_2016                                              , Int_t              );
    DECLARE_BRANCH(eee_2017                                              , Int_t              );
    DECLARE_BRANCH(eee_2018                                              , Int_t              );
    DECLARE_BRANCH(eemu_2015                                             , Int_t              );
    DECLARE_BRANCH(eemu_2016                                             , Int_t              );
    DECLARE_BRANCH(eemu_2017                                             , Int_t              );
    DECLARE_BRANCH(eemu_2018                                             , Int_t              );
    DECLARE_BRANCH(emumu_2015                                            , Int_t              );
    DECLARE_BRANCH(emumu_2016                                            , Int_t              );
    DECLARE_BRANCH(emumu_2017                                            , Int_t              );
    DECLARE_BRANCH(emumu_2018                                            , Int_t              );
    DECLARE_BRANCH(mumumu_2015                                           , Int_t              );
    DECLARE_BRANCH(mumumu_2016                                           , Int_t              );
    DECLARE_BRANCH(mumumu_2017                                           , Int_t              );
    DECLARE_BRANCH(mumumu_2018                                           , Int_t              );
    DECLARE_BRANCH(HLT_e60_lhmedium_nod0                                 , Char_t             );
    DECLARE_BRANCH(HLT_mu26_ivarmedium                                   , Char_t             );
    DECLARE_BRANCH(HLT_e26_lhtight_nod0_ivarloose                        , Char_t             );
    DECLARE_BRANCH(HLT_e140_lhloose_nod0                                 , Char_t             );
    DECLARE_BRANCH(HLT_e120_lhloose                                      , Char_t             );
    DECLARE_BRANCH(HLT_e24_lhmedium_L1EM20VH                             , Char_t             );
    DECLARE_BRANCH(HLT_e24_lhmedium_nod0_L1EM18VH                        , Char_t             );
    DECLARE_BRANCH(HLT_mu50                                              , Char_t             );
    DECLARE_BRANCH(HLT_mu24                                              , Char_t             );
    DECLARE_BRANCH(HLT_e60_lhmedium                                      , Char_t             );
    DECLARE_BRANCH(HLT_mu20_iloose_L1MU15                                , Char_t             );
    DECLARE_BRANCH(el_trigMatch_HLT_e60_lhmedium_nod0                    , std::vector<char>  );
    DECLARE_BRANCH(el_trigMatch_HLT_e120_lhloose                         , std::vector<char>  );
    DECLARE_BRANCH(el_trigMatch_HLT_e24_lhmedium_L1EM20VH                , std::vector<char>  );
    DECLARE_BRANCH(el_trigMatch_HLT_e24_lhmedium_nod0_L1EM18VH           , std::vector<char>  );
    DECLARE_BRANCH(el_trigMatch_HLT_e60_lhmedium                         , std::vector<char>  );
    DECLARE_BRANCH(el_trigMatch_HLT_e26_lhtight_nod0_ivarloose           , std::vector<char>  );
    DECLARE_BRANCH(el_trigMatch_HLT_e140_lhloose_nod0                    , std::vector<char>  );
    DECLARE_BRANCH(mu_trigMatch_HLT_mu26_ivarmedium                      , std::vector<char>  );
    DECLARE_BRANCH(mu_trigMatch_HLT_mu50                                 , std::vector<char>  );
    DECLARE_BRANCH(mu_trigMatch_HLT_mu24                                 , std::vector<char>  );
    DECLARE_BRANCH(mu_trigMatch_HLT_mu20_iloose_L1MU15                   , std::vector<char>  );
    DECLARE_BRANCH(lbn                                                   , UInt_t             );
    DECLARE_BRANCH(Vtxz                                                  , Float_t            );
    DECLARE_BRANCH(npVtx                                                 , UInt_t             );
    DECLARE_BRANCH(el_d0pv                                               , std::vector<float> );
    DECLARE_BRANCH(el_z0pv                                               , std::vector<float> );
    DECLARE_BRANCH(el_d0sigpv                                            , std::vector<float> );
    DECLARE_BRANCH(el_z0sigpv                                            , std::vector<float> );
    DECLARE_BRANCH(el_tight                                              , std::vector<bool>  );
    DECLARE_BRANCH(el_trigMatch                                          , std::vector<bool>  );
    DECLARE_BRANCH(el_true_pdg                                           , std::vector<int>   );
    DECLARE_BRANCH(el_true_pt                                            , std::vector<float> );
    DECLARE_BRANCH(el_true_eta                                           , std::vector<float> );
    DECLARE_BRANCH(mu_d0pv                                               , std::vector<float> );
    DECLARE_BRANCH(mu_z0pv                                               , std::vector<float> );
    DECLARE_BRANCH(mu_d0sigpv                                            , std::vector<float> );
    DECLARE_BRANCH(mu_z0sigpv                                            , std::vector<float> );
    DECLARE_BRANCH(mu_tight                                              , std::vector<bool>  );
    DECLARE_BRANCH(mu_trigMatch                                          , std::vector<bool>  );
    DECLARE_BRANCH(mu_true_pdg                                           , std::vector<int>   );
    DECLARE_BRANCH(mu_true_pt                                            , std::vector<float> );
    DECLARE_BRANCH(mu_true_eta                                           , std::vector<float> );
    DECLARE_BRANCH(jet_m                                                 , std::vector<float> );
    DECLARE_BRANCH(met_px                                                , Float_t            );
    DECLARE_BRANCH(met_py                                                , Float_t            );
    DECLARE_BRANCH(met_sumet                                             , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight                                 , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_EL_SF_Trigger_UP                , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_EL_SF_Trigger_DOWN              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_EL_SF_Reco_UP                   , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_EL_SF_Reco_DOWN                 , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_EL_SF_ID_UP                     , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_EL_SF_ID_DOWN                   , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_EL_SF_Isol_UP                   , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_EL_SF_Isol_DOWN                 , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_UP                , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_DOWN              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_STAT_UP           , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_STAT_DOWN         , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_SYST_UP           , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Trigger_SYST_DOWN         , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_ID_STAT_UP                , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_ID_STAT_DOWN              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_ID_SYST_UP                , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_ID_SYST_DOWN              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_ID_STAT_LOWPT_UP          , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_ID_STAT_LOWPT_DOWN        , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_ID_SYST_LOWPT_UP          , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_ID_SYST_LOWPT_DOWN        , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Isol_STAT_UP              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Isol_STAT_DOWN            , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Isol_SYST_UP              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_Isol_SYST_DOWN            , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_TTVA_STAT_UP              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_TTVA_STAT_DOWN            , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_TTVA_SYST_UP              , Float_t            );
    DECLARE_BRANCH(weight_leptonSF_tight_MU_SF_TTVA_SYST_DOWN            , Float_t            );
    DECLARE_BRANCH(weight_triggerSF_tight                                , Float_t            );
    DECLARE_BRANCH(weight_triggerSF_tight_EL_SF_Trigger_UP               , Float_t            );
    DECLARE_BRANCH(weight_triggerSF_tight_EL_SF_Trigger_DOWN             , Float_t            );
    DECLARE_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_UP               , Float_t            );
    DECLARE_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_DOWN             , Float_t            );
    DECLARE_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_STAT_UP          , Float_t            );
    DECLARE_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_STAT_DOWN        , Float_t            );
    DECLARE_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_SYST_UP          , Float_t            );
    DECLARE_BRANCH(weight_triggerSF_tight_MU_SF_Trigger_SYST_DOWN        , Float_t            );
    DECLARE_BRANCH(el_weight_triggerSF_tight                             , std::vector<float> );
    DECLARE_BRANCH(el_weight_triggerSF_tight_EL_SF_Trigger_UP            , std::vector<float> );
    DECLARE_BRANCH(el_weight_triggerSF_tight_EL_SF_Trigger_DOWN          , std::vector<float> );
    DECLARE_BRANCH(el_weight_recoIdSF_tight                              , std::vector<float> );
    DECLARE_BRANCH(el_weight_recoIdSF_tight_EL_SF_Reco_UP                , std::vector<float> );
    DECLARE_BRANCH(el_weight_recoIdSF_tight_EL_SF_Reco_DOWN              , std::vector<float> );
    DECLARE_BRANCH(el_weight_recoIdSF_tight_EL_SF_ID_UP                  , std::vector<float> );
    DECLARE_BRANCH(el_weight_recoIdSF_tight_EL_SF_ID_DOWN                , std::vector<float> );
    DECLARE_BRANCH(el_weight_isolSF_tight                                , std::vector<float> );
    DECLARE_BRANCH(el_weight_isolSF_tight_EL_SF_Isol_UP                  , std::vector<float> );
    DECLARE_BRANCH(el_weight_isolSF_tight_EL_SF_Isol_DOWN                , std::vector<float> );
    DECLARE_BRANCH(mu_weight_triggerSF_tight                             , std::vector<float> );
    DECLARE_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_UP            , std::vector<float> );
    DECLARE_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_DOWN          , std::vector<float> );
    DECLARE_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_STAT_UP       , std::vector<float> );
    DECLARE_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_STAT_DOWN     , std::vector<float> );
    DECLARE_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_SYST_UP       , std::vector<float> );
    DECLARE_BRANCH(mu_weight_triggerSF_tight_MU_SF_Trigger_SYST_DOWN     , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight                              , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_STAT_UP             , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_STAT_DOWN           , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_SYST_UP             , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_SYST_DOWN           , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_STAT_LOWPT_UP       , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_STAT_LOWPT_DOWN     , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_SYST_LOWPT_UP       , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_ID_SYST_LOWPT_DOWN     , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_TTVA_STAT_UP           , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_TTVA_STAT_DOWN         , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_TTVA_SYST_UP           , std::vector<float> );
    DECLARE_BRANCH(mu_weight_recoIdSF_tight_MU_SF_TTVA_SYST_DOWN         , std::vector<float> );
    DECLARE_BRANCH(mu_weight_isolSF_tight                                , std::vector<float> );
    DECLARE_BRANCH(mu_weight_isolSF_tight_MU_SF_Isol_STAT_UP             , std::vector<float> );
    DECLARE_BRANCH(mu_weight_isolSF_tight_MU_SF_Isol_STAT_DOWN           , std::vector<float> );
    DECLARE_BRANCH(mu_weight_isolSF_tight_MU_SF_Isol_SYST_UP             , std::vector<float> );
    DECLARE_BRANCH(mu_weight_isolSF_tight_MU_SF_Isol_SYST_DOWN           , std::vector<float> );

    DECLARE_PL_BRANCH(weight_mc            , Float_t            );
    DECLARE_PL_BRANCH(eventNumber          , ULong64_t          );
    DECLARE_PL_BRANCH(runNumber            , UInt_t             );
    DECLARE_PL_BRANCH(randomRunNumber      , UInt_t             );
    DECLARE_PL_BRANCH(mcChannelNumber      , UInt_t             );
    DECLARE_PL_BRANCH(mu                   , Float_t            );
    DECLARE_PL_BRANCH(weight_pileup        , Float_t            );
    DECLARE_PL_BRANCH(el_pt                , std::vector<float> );
    DECLARE_PL_BRANCH(el_eta               , std::vector<float> );
    DECLARE_PL_BRANCH(el_phi               , std::vector<float> );
    DECLARE_PL_BRANCH(el_e                 , std::vector<float> );
    DECLARE_PL_BRANCH(el_charge            , std::vector<float> );
    DECLARE_PL_BRANCH(el_pt_bare           , std::vector<float> );
    DECLARE_PL_BRANCH(el_eta_bare          , std::vector<float> );
    DECLARE_PL_BRANCH(el_phi_bare          , std::vector<float> );
    DECLARE_PL_BRANCH(el_e_bare            , std::vector<float> );
    DECLARE_PL_BRANCH(mu_pt                , std::vector<float> );
    DECLARE_PL_BRANCH(mu_eta               , std::vector<float> );
    DECLARE_PL_BRANCH(mu_phi               , std::vector<float> );
    DECLARE_PL_BRANCH(mu_e                 , std::vector<float> );
    DECLARE_PL_BRANCH(mu_charge            , std::vector<float> );
    DECLARE_PL_BRANCH(mu_pt_bare           , std::vector<float> );
    DECLARE_PL_BRANCH(mu_eta_bare          , std::vector<float> );
    DECLARE_PL_BRANCH(mu_phi_bare          , std::vector<float> );
    DECLARE_PL_BRANCH(mu_e_bare            , std::vector<float> );
    DECLARE_PL_BRANCH(jet_pt               , std::vector<float> );
    DECLARE_PL_BRANCH(jet_eta              , std::vector<float> );
    DECLARE_PL_BRANCH(jet_phi              , std::vector<float> );
    DECLARE_PL_BRANCH(jet_e                , std::vector<float> );
    DECLARE_PL_BRANCH(jet_nGhosts_bHadron  , std::vector<int>   );
    DECLARE_PL_BRANCH(jet_nGhosts_cHadron  , std::vector<int>   );
    DECLARE_PL_BRANCH(met_met              , Float_t            );
    DECLARE_PL_BRANCH(met_phi              , Float_t            );
    DECLARE_PL_BRANCH(PDFinfo_X1           , std::vector<float> );
    DECLARE_PL_BRANCH(PDFinfo_X2           , std::vector<float> );
    DECLARE_PL_BRANCH(PDFinfo_PDGID1       , std::vector<int>   );
    DECLARE_PL_BRANCH(PDFinfo_PDGID2       , std::vector<int>   );
    DECLARE_PL_BRANCH(PDFinfo_Q            , std::vector<float> );
    DECLARE_PL_BRANCH(PDFinfo_XF1          , std::vector<float> );
    DECLARE_PL_BRANCH(PDFinfo_XF2          , std::vector<float> );
    DECLARE_PL_BRANCH(mc_generator_weights , std::vector<float> );
    DECLARE_PL_BRANCH(all_particle         , Int_t              );
    DECLARE_PL_BRANCH(leptonic_2015        , Int_t              );
    DECLARE_PL_BRANCH(leptonic_2016        , Int_t              );
    DECLARE_PL_BRANCH(leptonic_2017        , Int_t              );
    DECLARE_PL_BRANCH(leptonic_2018        , Int_t              );
    DECLARE_PL_BRANCH(ee_2015              , Int_t              );
    DECLARE_PL_BRANCH(ee_2016              , Int_t              );
    DECLARE_PL_BRANCH(ee_2017              , Int_t              );
    DECLARE_PL_BRANCH(ee_2018              , Int_t              );
    DECLARE_PL_BRANCH(ejets_2015           , Int_t              );
    DECLARE_PL_BRANCH(ejets_2016           , Int_t              );
    DECLARE_PL_BRANCH(ejets_2017           , Int_t              );
    DECLARE_PL_BRANCH(ejets_2018           , Int_t              );
    DECLARE_PL_BRANCH(mumu_2015            , Int_t              );
    DECLARE_PL_BRANCH(mumu_2016            , Int_t              );
    DECLARE_PL_BRANCH(mumu_2017            , Int_t              );
    DECLARE_PL_BRANCH(mumu_2018            , Int_t              );
    DECLARE_PL_BRANCH(mujets_2015          , Int_t              );
    DECLARE_PL_BRANCH(mujets_2016          , Int_t              );
    DECLARE_PL_BRANCH(mujets_2017          , Int_t              );
    DECLARE_PL_BRANCH(mujets_2018          , Int_t              );
    DECLARE_PL_BRANCH(emu_2015             , Int_t              );
    DECLARE_PL_BRANCH(emu_2016             , Int_t              );
    DECLARE_PL_BRANCH(emu_2017             , Int_t              );
    DECLARE_PL_BRANCH(emu_2018             , Int_t              );
    DECLARE_PL_BRANCH(eee_2015             , Int_t              );
    DECLARE_PL_BRANCH(eee_2016             , Int_t              );
    DECLARE_PL_BRANCH(eee_2017             , Int_t              );
    DECLARE_PL_BRANCH(eee_2018             , Int_t              );
    DECLARE_PL_BRANCH(eemu_2015            , Int_t              );
    DECLARE_PL_BRANCH(eemu_2016            , Int_t              );
    DECLARE_PL_BRANCH(eemu_2017            , Int_t              );
    DECLARE_PL_BRANCH(eemu_2018            , Int_t              );
    DECLARE_PL_BRANCH(emumu_2015           , Int_t              );
    DECLARE_PL_BRANCH(emumu_2016           , Int_t              );
    DECLARE_PL_BRANCH(emumu_2017           , Int_t              );
    DECLARE_PL_BRANCH(emumu_2018           , Int_t              );
    DECLARE_PL_BRANCH(mumumu_2015          , Int_t              );
    DECLARE_PL_BRANCH(mumumu_2016          , Int_t              );
    DECLARE_PL_BRANCH(mumumu_2017          , Int_t              );
    DECLARE_PL_BRANCH(mumumu_2018          , Int_t              );


  };

}

template<typename T>
inline std::unique_ptr<T>
TL::Variables::setupBranch(std::shared_ptr<TTreeReader> reader,
                           const char* name) {
  if ( spdlog::get("TopLoop non-existent branch") == nullptr ) {
    spdlog::stdout_color_mt("TopLoop non-existent branch");
  }

  if ( reader->GetTree() == nullptr ) {
    spdlog::get("TopLoop non-existent branch")->debug("{} branch trying to link to a null tree! "
                                                      "TTreeReader name name: {}",
                                                      name, reader->GetTree()->GetName());
    return nullptr;
  }
  if ( reader->GetTree()->GetListOfBranches()->FindObject(name) != nullptr ) {
    return std::make_unique<T>(*reader,name);
  }
  else {
    spdlog::get("TopLoop non-existent branch")->debug("{} branch not found in the tree \"{}\"! "
                                                      "Using this branch will cause a painful death! ",
                                                      name,reader->GetTree()->GetName());
  }
  return nullptr;
}

#endif

/** @file  Variables.h
 *  @brief TL::Variables class header
 *  @class TL::Variables
 *  @brief Base class for all TTreeReaderValue variables for TopLoop
 *         analysis algorithms
 *
 *  A clean place to declare all TTreeReaderValues without saturating
 *  the main Algorithm class header with the TTreeReaderValues.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef TL_Variables_h
#define TL_Variables_h

// C++
#include <vector>
#include <string>
#include <memory>

// ROOT
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

#define DECLARE_BRANCH(NAME,TYPE)        std::unique_ptr<TTreeReaderValue<TYPE>> NAME;
#define CONNECT_BRANCH(NAME,TYPE,READER) NAME = setupBranch<TTreeReaderValue<TYPE>>(READER,#NAME)

namespace TL {

  class Variables {
  protected:
    std::vector<std::string> m_ignoreList;

  public:
    /// default constructor
    Variables();
    /// destructor
    virtual ~Variables();

    /// disable copy constructor
    Variables(const Variables&) = delete;
    /// disable assignment operator
    Variables& operator=(const Variables&) = delete;

    //! Create a list of branches to ignore from a plain text file
    /*!
      The list of strings in the txt file given will be ignores
      when setting up the TTreeReader variables.  WARNING: If you try
      to access a file in the ignore list you will get a glorious
      crash.
    */
    void ignoreListFile(const std::string& filename);

    /// Return the ignore list.
    const std::vector<std::string>& ignoreList() const { return m_ignoreList; }

  protected:

    DECLARE_BRANCH(dsid                                     , Int_t                    )
    DECLARE_BRANCH(totalEventsWeighted                      , Float_t                  )
    DECLARE_BRANCH(totalEvents                              , ULong64_t                )
    DECLARE_BRANCH(totalEventsWeighted_mc_generator_weights , std::vector<float>       )
    DECLARE_BRANCH(names_mc_generator_weights               , std::vector<std::string> )

    // Variables shared by all trees (Data, Nominal MC, Systematic MC)
    DECLARE_BRANCH(eventNumber                                 , ULong64_t          )
    DECLARE_BRANCH(runNumber                                   , UInt_t             )
    DECLARE_BRANCH(mcChannelNumber                             , UInt_t             )
    DECLARE_BRANCH(mu                                          , Float_t            )
    DECLARE_BRANCH(backgroundFlags                             , UInt_t             )
    DECLARE_BRANCH(hasBadMuon                                  , UInt_t             )
    DECLARE_BRANCH(el_pt                                       , std::vector<float> )
    DECLARE_BRANCH(el_eta                                      , std::vector<float> )
    DECLARE_BRANCH(el_cl_eta                                   , std::vector<float> )
    DECLARE_BRANCH(el_phi                                      , std::vector<float> )
    DECLARE_BRANCH(el_e                                        , std::vector<float> )
    DECLARE_BRANCH(el_charge                                   , std::vector<float> )
    DECLARE_BRANCH(el_topoetcone20                             , std::vector<float> )
    DECLARE_BRANCH(el_ptvarcone20                              , std::vector<float> )
    DECLARE_BRANCH(el_CF                                       , std::vector<char>  )
    DECLARE_BRANCH(el_d0sig                                    , std::vector<float> )
    DECLARE_BRANCH(el_delta_z0_sintheta                        , std::vector<float> )
    DECLARE_BRANCH(mu_pt                                       , std::vector<float> )
    DECLARE_BRANCH(mu_eta                                      , std::vector<float> )
    DECLARE_BRANCH(mu_phi                                      , std::vector<float> )
    DECLARE_BRANCH(mu_e                                        , std::vector<float> )
    DECLARE_BRANCH(mu_charge                                   , std::vector<float> )
    DECLARE_BRANCH(mu_topoetcone20                             , std::vector<float> )
    DECLARE_BRANCH(mu_ptvarcone30                              , std::vector<float> )
    DECLARE_BRANCH(mu_d0sig                                    , std::vector<float> )
    DECLARE_BRANCH(mu_delta_z0_sintheta                        , std::vector<float> )
    DECLARE_BRANCH(jet_pt                                      , std::vector<float> )
    DECLARE_BRANCH(jet_eta                                     , std::vector<float> )
    DECLARE_BRANCH(jet_phi                                     , std::vector<float> )
    DECLARE_BRANCH(jet_e                                       , std::vector<float> )
    DECLARE_BRANCH(jet_mv2c00                                  , std::vector<float> )
    DECLARE_BRANCH(jet_mv2c10                                  , std::vector<float> )
    DECLARE_BRANCH(jet_mv2c20                                  , std::vector<float> )
    DECLARE_BRANCH(jet_ip3dsv1                                 , std::vector<float> )
    DECLARE_BRANCH(jet_jvt                                     , std::vector<float> )
    DECLARE_BRANCH(jet_passfjvt                                , std::vector<char>  )
    DECLARE_BRANCH(jet_isbtagged_MV2c10_77                     , std::vector<char>  )
    DECLARE_BRANCH(met_met                                     , Float_t            )
    DECLARE_BRANCH(met_phi                                     , Float_t            )
    DECLARE_BRANCH(emu_2015                                    , Int_t              )
    DECLARE_BRANCH(emu_2016                                    , Int_t              )
    DECLARE_BRANCH(emu_particle                                , Int_t              )
    DECLARE_BRANCH(ee_2015                                     , Int_t              )
    DECLARE_BRANCH(ee_2016                                     , Int_t              )
    DECLARE_BRANCH(ee_particle                                 , Int_t              )
    DECLARE_BRANCH(mumu_2015                                   , Int_t              )
    DECLARE_BRANCH(mumu_2016                                   , Int_t              )
    DECLARE_BRANCH(mumu_particle                               , Int_t              )
    DECLARE_BRANCH(HLT_e60_lhmedium_nod0                       , Char_t             )
    DECLARE_BRANCH(HLT_mu26_ivarmedium                         , Char_t             )
    DECLARE_BRANCH(HLT_e26_lhtight_nod0_ivarloose              , Char_t             )
    DECLARE_BRANCH(HLT_e140_lhloose_nod0                       , Char_t             )
    DECLARE_BRANCH(HLT_e120_lhloose                            , Char_t             )
    DECLARE_BRANCH(HLT_e24_lhmedium_L1EM20VH                   , Char_t             )
    DECLARE_BRANCH(HLT_mu50                                    , Char_t             )
    DECLARE_BRANCH(HLT_e60_lhmedium                            , Char_t             )
    DECLARE_BRANCH(HLT_mu20_iloose_L1MU15                      , Char_t             )
    DECLARE_BRANCH(el_trigMatch_HLT_e60_lhmedium_nod0          , std::vector<char>  )
    DECLARE_BRANCH(el_trigMatch_HLT_e120_lhloose               , std::vector<char>  )
    DECLARE_BRANCH(el_trigMatch_HLT_e24_lhmedium_L1EM20VH      , std::vector<char>  )
    DECLARE_BRANCH(el_trigMatch_HLT_e60_lhmedium               , std::vector<char>  )
    DECLARE_BRANCH(el_trigMatch_HLT_e26_lhtight_nod0_ivarloose , std::vector<char>  )
    DECLARE_BRANCH(el_trigMatch_HLT_e140_lhloose_nod0          , std::vector<char>  )
    DECLARE_BRANCH(mu_trigMatch_HLT_mu26_ivarmedium            , std::vector<char>  )
    DECLARE_BRANCH(mu_trigMatch_HLT_mu50                       , std::vector<char>  )
    DECLARE_BRANCH(mu_trigMatch_HLT_mu20_iloose_L1MU15         , std::vector<char>  )

    // variables only belonging to MC trees
    DECLARE_BRANCH(weight_mc               , Float_t           )
    DECLARE_BRANCH(weight_pileup           , Float_t           )
    DECLARE_BRANCH(weight_leptonSF         , Float_t           )
    DECLARE_BRANCH(weight_bTagSF_MV2c10_77 , Float_t           )
    DECLARE_BRANCH(weight_bTagSF_77        , Float_t           )
    DECLARE_BRANCH(weight_jvt              , Float_t           )
    DECLARE_BRANCH(randomRunNumber         , UInt_t            )
    DECLARE_BRANCH(el_true_type            , std::vector<int>  )
    DECLARE_BRANCH(el_true_origin          , std::vector<int>  )
    DECLARE_BRANCH(el_true_typebkg         , std::vector<int>  )
    DECLARE_BRANCH(el_true_originbkg       , std::vector<int>  )
    DECLARE_BRANCH(mu_true_type            , std::vector<int>  )
    DECLARE_BRANCH(mu_true_origin          , std::vector<int>  )
    DECLARE_BRANCH(jet_truthflav           , std::vector<int>  )
    DECLARE_BRANCH(jet_truthPartonLabel    , std::vector<int>  )
    DECLARE_BRANCH(jet_isTrueHS            , std::vector<char> )

    // variables only belonging to nominal MC tree
    DECLARE_BRANCH(weight_pileup_UP                         , Float_t )
    DECLARE_BRANCH(weight_pileup_DOWN                       , Float_t )
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Trigger_UP         , Float_t )
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Trigger_DOWN       , Float_t )
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Reco_UP            , Float_t )
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Reco_DOWN          , Float_t )
    DECLARE_BRANCH(weight_leptonSF_EL_SF_ID_UP              , Float_t )
    DECLARE_BRANCH(weight_leptonSF_EL_SF_ID_DOWN            , Float_t )
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Isol_UP            , Float_t )
    DECLARE_BRANCH(weight_leptonSF_EL_SF_Isol_DOWN          , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Trigger_STAT_UP    , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Trigger_STAT_DOWN  , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Trigger_SYST_UP    , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Trigger_SYST_DOWN  , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_STAT_UP         , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_STAT_DOWN       , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_SYST_UP         , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_SYST_DOWN       , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_STAT_LOWPT_UP   , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_STAT_LOWPT_DOWN , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_SYST_LOWPT_UP   , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_ID_SYST_LOWPT_DOWN , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Isol_STAT_UP       , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Isol_STAT_DOWN     , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Isol_SYST_UP       , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_Isol_SYST_DOWN     , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_TTVA_STAT_UP       , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_TTVA_STAT_DOWN     , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_TTVA_SYST_UP       , Float_t )
    DECLARE_BRANCH(weight_leptonSF_MU_SF_TTVA_SYST_DOWN     , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_Trigger               , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_Trigger_UP            , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_Trigger_DOWN          , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_Reco                  , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_Reco_UP               , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_Reco_DOWN             , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ID                    , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ID_UP                 , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ID_DOWN               , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_Isol                  , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_Isol_UP               , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_Isol_DOWN             , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ChargeID              , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ChargeID_UP           , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ChargeID_DOWN         , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ChargeMisID           , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ChargeMisID_STAT_UP   , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ChargeMisID_STAT_DOWN , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ChargeMisID_SYST_UP   , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_EL_ChargeMisID_SYST_DOWN , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Trigger               , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Trigger_STAT_UP       , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Trigger_STAT_DOWN     , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Trigger_SYST_UP       , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Trigger_SYST_DOWN     , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_ID                    , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_ID_STAT_UP            , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_ID_STAT_DOWN          , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_ID_SYST_UP            , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_ID_SYST_DOWN          , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_ID_STAT_LOWPT_UP      , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_ID_STAT_LOWPT_DOWN    , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_ID_SYST_LOWPT_UP      , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_ID_SYST_LOWPT_DOWN    , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Isol                  , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Isol_STAT_UP          , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Isol_STAT_DOWN        , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Isol_SYST_UP          , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_Isol_SYST_DOWN        , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_TTVA                  , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_TTVA_STAT_UP          , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_TTVA_STAT_DOWN        , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_TTVA_SYST_UP          , Float_t )
    DECLARE_BRANCH(weight_indiv_SF_MU_TTVA_SYST_DOWN        , Float_t )
    DECLARE_BRANCH(weight_jvt_UP                            , Float_t )
    DECLARE_BRANCH(weight_jvt_DOWN                          , Float_t )

  };

}

#endif

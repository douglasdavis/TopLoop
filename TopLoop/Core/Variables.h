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

using TTRV_vec_str   = TTreeReaderValue<std::vector<std::string>>;
using TTRV_vec_float = TTreeReaderValue<std::vector<float>>;
using TTRV_vec_char  = TTreeReaderValue<std::vector<char>>;
using TTRV_vec_bool  = TTreeReaderValue<std::vector<bool>>;
using TTRV_vec_int   = TTreeReaderValue<std::vector<int>>;
using TTRV_float     = TTreeReaderValue<Float_t>;
using TTRV_uint      = TTreeReaderValue<UInt_t>;
using TTRV_int       = TTreeReaderValue<Int_t>;
using TTRV_ulongint  = TTreeReaderValue<ULong64_t>;
using TTRV_char      = TTreeReaderValue<Char_t>;

namespace TL {

  class Variables {

  public:
    Variables(const Variables&) = delete;
    Variables& operator=(const Variables&) = delete;
    Variables() {}
    virtual ~Variables() {}

  protected:
    std::shared_ptr<TTRV_float>     totalEventsWeighted;
    std::shared_ptr<TTRV_vec_float> totalEventsWeighted_mc_generator_weights;
    std::shared_ptr<TTRV_vec_str>   names_mc_generator_weights;
    std::shared_ptr<TTRV_int>       dsid;

    std::shared_ptr<TTRV_float>   weight_mc;
    std::shared_ptr<TTRV_float>   weight_pileup;
    std::shared_ptr<TTRV_float>   weight_leptonSF;
    std::shared_ptr<TTRV_float>   weight_bTagSF_77;
    std::shared_ptr<TTRV_float>   weight_jvt;

    std::shared_ptr<TTRV_float>   weight_indiv_SF_EL_Trigger;
    std::shared_ptr<TTRV_float>   weight_indiv_SF_EL_Reco;
    std::shared_ptr<TTRV_float>   weight_indiv_SF_EL_ID;
    std::shared_ptr<TTRV_float>   weight_indiv_SF_EL_Isol;
    std::shared_ptr<TTRV_float>   weight_indiv_SF_MU_Trigger;
    std::shared_ptr<TTRV_float>   weight_indiv_SF_MU_ID;
    std::shared_ptr<TTRV_float>   weight_indiv_SF_MU_Isol;
    std::shared_ptr<TTRV_float>   weight_indiv_SF_MU_TTVA;

    std::map<std::string,std::shared_ptr<TTRV_float>>     weightSyst_pileup;
    std::map<std::string,std::shared_ptr<TTRV_float>>     weightSyst_jvt;
    std::map<std::string,std::shared_ptr<TTRV_float>>     weightSyst_leptonSF;
    std::map<std::string,std::shared_ptr<TTRV_float>>     weightSyst_indivSF;
    std::map<std::string,std::shared_ptr<TTRV_float>>     weightSyst_bTagSF_extrapolation;
    std::map<std::string,std::shared_ptr<TTRV_vec_float>> weightSyst_bTagSF_eigenvars;

    std::shared_ptr<TTRV_ulongint>  eventNumber;
    std::shared_ptr<TTRV_uint>      runNumber;
    std::shared_ptr<TTRV_uint>      mcChannelNumber;
    std::shared_ptr<TTRV_float>     mu;

    std::shared_ptr<TTRV_vec_float> el_pt;
    std::shared_ptr<TTRV_vec_float> el_phi;
    std::shared_ptr<TTRV_vec_float> el_eta;
    std::shared_ptr<TTRV_vec_float> el_e;
    std::shared_ptr<TTRV_vec_float> el_cl_eta;
    std::shared_ptr<TTRV_vec_float> el_charge;
    std::shared_ptr<TTRV_vec_float> el_topoetcone20;
    std::shared_ptr<TTRV_vec_float> el_ptvarcone20;
    std::shared_ptr<TTRV_vec_float> el_d0sig;
    std::shared_ptr<TTRV_vec_float> el_delta_z0_sintheta;
    std::shared_ptr<TTRV_vec_char>  el_CF;
    std::shared_ptr<TTRV_vec_int> el_true_type;
    std::shared_ptr<TTRV_vec_int> el_true_origin;
    std::shared_ptr<TTRV_vec_int> el_true_typebkg;
    std::shared_ptr<TTRV_vec_int> el_true_originbkg;

    std::shared_ptr<TTRV_vec_float> mu_pt;
    std::shared_ptr<TTRV_vec_float> mu_phi;
    std::shared_ptr<TTRV_vec_float> mu_eta;
    std::shared_ptr<TTRV_vec_float> mu_e;
    std::shared_ptr<TTRV_vec_float> mu_charge;
    std::shared_ptr<TTRV_vec_float> mu_topoetcone20;
    std::shared_ptr<TTRV_vec_float> mu_ptvarcone30;
    std::shared_ptr<TTRV_vec_float> mu_d0sig;
    std::shared_ptr<TTRV_vec_float> mu_delta_z0_sintheta;

    std::shared_ptr<TTRV_vec_int> mu_true_type;
    std::shared_ptr<TTRV_vec_int> mu_true_origin;

    std::shared_ptr<TTRV_vec_float> jet_pt;
    std::shared_ptr<TTRV_vec_float> jet_eta;
    std::shared_ptr<TTRV_vec_float> jet_phi;
    std::shared_ptr<TTRV_vec_float> jet_e;
    std::shared_ptr<TTRV_vec_float> jet_mv2c00;
    std::shared_ptr<TTRV_vec_float> jet_mv2c10;
    std::shared_ptr<TTRV_vec_float> jet_mv2c20; 
    std::shared_ptr<TTRV_vec_float> jet_ip3dsv1;
    std::shared_ptr<TTRV_vec_float> jet_jvt;
    std::shared_ptr<TTRV_vec_char>  jet_isbtagged_77;

    std::shared_ptr<TTRV_int> emu_2015;
    std::shared_ptr<TTRV_int> emu_2016;
    std::shared_ptr<TTRV_int> emu_particle;
    std::shared_ptr<TTRV_int> ee_2015;
    std::shared_ptr<TTRV_int> ee_2016;
    std::shared_ptr<TTRV_int> ee_particle;
    std::shared_ptr<TTRV_int> mumu_2015;
    std::shared_ptr<TTRV_int> mumu_2016;
    std::shared_ptr<TTRV_int> mumu_particle;

    std::shared_ptr<TTRV_float> met_met;
    std::shared_ptr<TTRV_float> met_phi;

    std::shared_ptr<TTRV_char> HLT_e60_lhmedium_nod0;
    std::shared_ptr<TTRV_char> HLT_mu26_ivarmedium;
    std::shared_ptr<TTRV_char> HLT_e26_lhtight_nod0_ivarloose;
    std::shared_ptr<TTRV_char> HLT_e140_lhloose_nod0;
    std::shared_ptr<TTRV_char> HLT_mu20_iloose_L1MU15;
    std::shared_ptr<TTRV_char> HLT_mu50;
    std::shared_ptr<TTRV_char> HLT_e60_lhmedium;
    std::shared_ptr<TTRV_char> HLT_e24_lhmedium_L1EM20VH;
    std::shared_ptr<TTRV_char> HLT_e120_lhloose;

    std::shared_ptr<TTRV_vec_char> el_trigMatch_HLT_e60_lhmedium;
    std::shared_ptr<TTRV_vec_char> el_trigMatch_HLT_e24_lhmedium_L1EM18VH;
    std::shared_ptr<TTRV_vec_char> el_trigMatch_HLT_e120_lhloose;
    std::shared_ptr<TTRV_vec_char> mu_trigMatch_HLT_mu50;
    std::shared_ptr<TTRV_vec_char> mu_trigMatch_HLT_mu20_iloose_L1MU15;

  };

}

#endif

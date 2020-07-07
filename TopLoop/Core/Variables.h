/*! @file  Variables.h
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
#include <memory>
#include <vector>

// ROOT
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

// TopLoop
#include <TopLoop/Core/Loggable.h>

#define DECLARE_BRANCH(NAME, TYPE)                                 \
 protected:                                                        \
  std::unique_ptr<TTreeReaderValue<TYPE>> bv__##NAME;              \
                                                                   \
 public:                                                           \
  const TYPE& NAME() const {                                       \
    if (bv__##NAME) return *(*bv__##NAME);                         \
    spdlog::get("BranchAccess")->critical("No {} branch!", #NAME); \
    std::exit(EXIT_FAILURE);                                       \
  }

#define DECLARE_BRANCH_PRIMITIVE(NAME, TYPE)                       \
 protected:                                                        \
  std::unique_ptr<TTreeReaderValue<TYPE>> bv__##NAME;              \
                                                                   \
 public:                                                           \
  TYPE NAME() const {                                              \
    if (bv__##NAME) return *(*bv__##NAME);                         \
    spdlog::get("BranchAccess")->critical("No {} branch!", #NAME); \
    std::exit(EXIT_FAILURE);                                       \
  }

#define DECLARE_PL_BRANCH(NAME, TYPE)                                 \
 protected:                                                           \
  std::unique_ptr<TTreeReaderValue<TYPE>> bv__pl__##NAME;             \
                                                                      \
 public:                                                              \
  const TYPE& PL_##NAME() const {                                     \
    if (bv__pl__##NAME) return *(*bv__pl__##NAME);                    \
    spdlog::get("BranchAccess")->critical("No PL_{} branch!", #NAME); \
    std::exit(EXIT_FAILURE);                                          \
  }

#define DECLARE_PL_BRANCH_PRIMITIVE(NAME, TYPE)                       \
 protected:                                                           \
  std::unique_ptr<TTreeReaderValue<TYPE>> bv__pl__##NAME;             \
                                                                      \
 public:                                                              \
  TYPE PL_##NAME() const {                                            \
    if (bv__pl__##NAME) return *(*bv__pl__##NAME);                    \
    spdlog::get("BranchAccess")->critical("No PL_{} branch!", #NAME); \
    std::exit(EXIT_FAILURE);                                          \
  }

#define DECLARE_TRUTH_BRANCH(NAME, TYPE)                                 \
 protected:                                                              \
  std::unique_ptr<TTreeReaderValue<TYPE>> bv__truth__##NAME;             \
                                                                         \
 public:                                                                 \
  TYPE truth_##NAME() const {                                            \
    if (bv__truth__##NAME) return *(*bv__truth__##NAME);                 \
    spdlog::get("BranchAccess")->critical("No truth_{} branch!", #NAME); \
    std::exit(EXIT_FAILURE);                                             \
  }

#define CONNECT_BRANCH(NAME, TYPE, READER) \
  bv__##NAME = TL::Variables::setupBranch<TTreeReaderValue<TYPE>>((READER), #NAME);

#define CONNECT_PL_BRANCH(NAME, TYPE, READER) \
  bv__pl__##NAME = TL::Variables::setupBranch<TTreeReaderValue<TYPE>>((READER), #NAME);

#define CONNECT_TRUTH_BRANCH(NAME, TYPE, READER) \
  bv__truth__##NAME = TL::Variables::setupBranch<TTreeReaderValue<TYPE>>((READER), #NAME);

namespace TL {

class Variables {
 public:
  /// default constructor
  Variables() { m_brlogger = TL::Loggable::setupLogger("BranchAccess"); }
  /// destructor
  virtual ~Variables() = default;

  /// disable copy constructor
  Variables(const Variables&) = delete;
  /// disable assignment operator
  Variables& operator=(const Variables&) = delete;

  /// Set up a variable as a TTreeReaderValue pointer
  /*!
   *  This one liner checks to make sure that the variable is on the
   *  tree. If its not - you get a warning. If the variable isn't
   *  there your program will still work as long as you don't try to
   *  dereference the pointer.
   */
  template <typename T>
  std::unique_ptr<T> setupBranch(std::shared_ptr<TTreeReader> reader,
                                 const char* name) const {
    if (reader->GetTree() == nullptr) {
      m_brlogger->debug(
          "{} branch trying to link to a null tree! "
          "TTreeReader name name: {}",
          name, reader->GetTree()->GetName());
      return nullptr;
    }
    if (reader->GetTree()->GetListOfBranches()->FindObject(name) != nullptr) {
      return std::make_unique<T>(*reader, name);
    }
    else {
      m_brlogger->debug(
          "{} branch not found in the tree \"{}\"! "
          "Using this branch will cause a painful death! ",
          name, reader->GetTree()->GetName());
    }
    return nullptr;
  }

 private:
  std::shared_ptr<spdlog::logger> m_brlogger{nullptr};

 protected:
  std::unique_ptr<TTreeReaderValue<Int_t>> bv__dsid;
  std::unique_ptr<TTreeReaderValue<Int_t>> bv__isAFII;

 public:
  [[deprecated("Don't use dsid branch, use fileManager->dsid()")]] Int_t dsid() const {
    if (bv__dsid) return *(*bv__dsid);
    m_brlogger->critical("No {} branch!", "dsid");
    std::exit(EXIT_FAILURE);
  }
  [[deprecated("Don't use isAFII branch, use fileManager->isAFII()")]] Int_t isAFII()
      const {
    if (bv__isAFII) return *(*bv__isAFII);
    m_brlogger->critical("No {} branch!", "isAFII");
    std::exit(EXIT_FAILURE);
  }

 protected:
  DECLARE_BRANCH(totalEventsWeighted, Float_t);
  DECLARE_BRANCH(totalEvents, ULong64_t);
  DECLARE_BRANCH(totalEventsWeighted_mc_generator_weights, std::vector<float>);
  DECLARE_BRANCH(names_mc_generator_weights, std::vector<std::string>);

  DECLARE_BRANCH(PDFinfo_X1, std::vector<float>);
  DECLARE_BRANCH(PDFinfo_X2, std::vector<float>);
  DECLARE_BRANCH(PDFinfo_PDGID1, std::vector<int>);
  DECLARE_BRANCH(PDFinfo_PDGID2, std::vector<int>);
  DECLARE_BRANCH(PDFinfo_Q, std::vector<float>);
  DECLARE_BRANCH(PDFinfo_XF1, std::vector<float>);
  DECLARE_BRANCH(PDFinfo_XF2, std::vector<float>);
  DECLARE_BRANCH(mc_generator_weights, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(weight_mc, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_pileup, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_globalLeptonTriggerSF, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_globalLeptonTriggerSF_EL_Trigger_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_globalLeptonTriggerSF_EL_Trigger_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_globalLeptonTriggerSF_MU_Trigger_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_globalLeptonTriggerSF_MU_Trigger_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_globalLeptonTriggerSF_MU_Trigger_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_globalLeptonTriggerSF_MU_Trigger_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_ELEOLR_TOTAL_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_ELEOLR_TOTAL_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEOLR_HIGHMU_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEOLR_HIGHMU_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEOLR_LOWMU_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEOLR_LOWMU_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEOLR_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEOLR_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEBDT_MC16A_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEBDT_MC16A_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEBDT_MC16D_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEBDT_MC16D_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEBDT_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_TRUEELECTRON_ELEBDT_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_1P2025_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_1P2025_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_1P2530_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_1P2530_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_1P3040_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_1P3040_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_1PGE40_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_1PGE40_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_3P2030_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_3P2030_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_3PGE30_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_3PGE30_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_HIGHPT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_HIGHPT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_AF2_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_JETID_AF2_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_RECO_TOTAL_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_RECO_TOTAL_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_RECO_HIGHPT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_RECO_HIGHPT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_RECO_AF2_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_tauSF_RECO_AF2_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_MV2c10_70, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_MV2c10_77, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_MV2c10_85, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_MV2c10_Continuous, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1_HybBEff_60, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_85, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_77, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_70, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_60, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_Continuous, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_jvt, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_forwardjvt, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_pileup_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_pileup_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_EL_SF_Trigger_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_EL_SF_Trigger_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_EL_SF_Reco_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_EL_SF_Reco_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_EL_SF_ID_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_EL_SF_ID_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_EL_SF_Isol_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_EL_SF_Isol_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_Trigger_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_Trigger_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_Trigger_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_Trigger_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_ID_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_ID_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_ID_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_ID_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_ID_STAT_LOWPT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_ID_STAT_LOWPT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_ID_SYST_LOWPT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_ID_SYST_LOWPT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_Isol_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_Isol_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_Isol_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_Isol_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_TTVA_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_TTVA_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_TTVA_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_MU_SF_TTVA_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_jvt_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_jvt_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_forwardjvt_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_forwardjvt_DOWN, Float_t);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_B_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_C_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_Light_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_B_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_C_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_77_eigenvars_Light_down, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_MV2c10_77_extrapolation_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_MV2c10_77_extrapolation_down, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_MV2c10_77_extrapolation_from_charm_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_MV2c10_77_extrapolation_from_charm_down, Float_t);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_Continuous_eigenvars_B_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_Continuous_eigenvars_C_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_Continuous_eigenvars_Light_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_Continuous_eigenvars_B_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_Continuous_eigenvars_C_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_MV2c10_Continuous_eigenvars_Light_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1_HybBEff_60_eigenvars_B_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1_HybBEff_60_eigenvars_C_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1_HybBEff_60_eigenvars_Light_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1_HybBEff_60_eigenvars_B_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1_HybBEff_60_eigenvars_C_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1_HybBEff_60_eigenvars_Light_down, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1_HybBEff_60_extrapolation_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1_HybBEff_60_extrapolation_down, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1_HybBEff_60_extrapolation_from_charm_up,
                           Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1_HybBEff_60_extrapolation_from_charm_down,
                           Float_t);
  DECLARE_BRANCH(weight_bTagSF_DL1r_85_eigenvars_B_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_85_eigenvars_C_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_85_eigenvars_Light_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_85_eigenvars_B_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_85_eigenvars_C_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_85_eigenvars_Light_down, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_85_extrapolation_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_85_extrapolation_down, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_85_extrapolation_from_charm_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_85_extrapolation_from_charm_down, Float_t);
  DECLARE_BRANCH(weight_bTagSF_DL1r_77_eigenvars_B_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_77_eigenvars_C_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_77_eigenvars_Light_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_77_eigenvars_B_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_77_eigenvars_C_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_77_eigenvars_Light_down, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_77_extrapolation_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_77_extrapolation_down, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_77_extrapolation_from_charm_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_77_extrapolation_from_charm_down, Float_t);
  DECLARE_BRANCH(weight_bTagSF_DL1r_70_eigenvars_B_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_70_eigenvars_C_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_70_eigenvars_Light_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_70_eigenvars_B_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_70_eigenvars_C_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_70_eigenvars_Light_down, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_70_extrapolation_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_70_extrapolation_down, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_70_extrapolation_from_charm_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_70_extrapolation_from_charm_down, Float_t);
  DECLARE_BRANCH(weight_bTagSF_DL1r_60_eigenvars_B_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_60_eigenvars_C_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_60_eigenvars_Light_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_60_eigenvars_B_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_60_eigenvars_C_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_60_eigenvars_Light_down, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_60_extrapolation_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_60_extrapolation_down, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_60_extrapolation_from_charm_up, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_bTagSF_DL1r_60_extrapolation_from_charm_down, Float_t);
  DECLARE_BRANCH(weight_bTagSF_DL1r_Continuous_eigenvars_B_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_Continuous_eigenvars_C_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_Continuous_eigenvars_Light_up, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_Continuous_eigenvars_B_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_Continuous_eigenvars_C_down, std::vector<float>);
  DECLARE_BRANCH(weight_bTagSF_DL1r_Continuous_eigenvars_Light_down, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(eventNumber, ULong64_t);
  DECLARE_BRANCH_PRIMITIVE(runNumber, UInt_t);
  DECLARE_BRANCH_PRIMITIVE(randomRunNumber, UInt_t);
  DECLARE_BRANCH_PRIMITIVE(mcChannelNumber, UInt_t);
  DECLARE_BRANCH_PRIMITIVE(mu, Float_t);
  DECLARE_BRANCH_PRIMITIVE(backgroundFlags, UInt_t);
  DECLARE_BRANCH_PRIMITIVE(hasBadMuon, UInt_t);
  DECLARE_BRANCH(el_pt, std::vector<float>);
  DECLARE_BRANCH(el_eta, std::vector<float>);
  DECLARE_BRANCH(el_cl_eta, std::vector<float>);
  DECLARE_BRANCH(el_phi, std::vector<float>);
  DECLARE_BRANCH(el_e, std::vector<float>);
  DECLARE_BRANCH(el_charge, std::vector<float>);
  DECLARE_BRANCH(el_topoetcone20, std::vector<float>);
  DECLARE_BRANCH(el_ptvarcone20, std::vector<float>);
  DECLARE_BRANCH(el_isTight, std::vector<char>);
  DECLARE_BRANCH(el_Isol_Gradient, std::vector<char>);
  DECLARE_BRANCH(el_Isol_FCLoose, std::vector<char>);
  DECLARE_BRANCH(el_Isol_FCTight, std::vector<char>);
  DECLARE_BRANCH(el_Isol_FCHighPtCaloOnly, std::vector<char>);
  DECLARE_BRANCH(el_Isol_HighPtCaloOnly, std::vector<char>);
  DECLARE_BRANCH(el_Isol_Loose, std::vector<char>);
  DECLARE_BRANCH(el_Isol_Tight, std::vector<char>);
  DECLARE_BRANCH(el_Isol_TightTrackOnly, std::vector<char>);
  DECLARE_BRANCH(el_Isol_TightTrackOnly_FixedRad, std::vector<char>);
  DECLARE_BRANCH(el_Isol_PLVTight, std::vector<char>);
  DECLARE_BRANCH(el_Isol_PLVLoose, std::vector<char>);
  DECLARE_BRANCH(el_Isol_PflowTight, std::vector<char>);
  DECLARE_BRANCH(el_Isol_PflowLoose, std::vector<char>);
  DECLARE_BRANCH(el_CF, std::vector<char>);
  DECLARE_BRANCH(el_d0sig, std::vector<float>);
  DECLARE_BRANCH(el_delta_z0_sintheta, std::vector<float>);
  DECLARE_BRANCH(el_true_type, std::vector<int>);
  DECLARE_BRANCH(el_true_origin, std::vector<int>);
  DECLARE_BRANCH(el_true_originbkg, std::vector<int>);
  DECLARE_BRANCH(el_true_typebkg, std::vector<int>);
  DECLARE_BRANCH(el_true_firstEgMotherTruthType, std::vector<int>);
  DECLARE_BRANCH(el_true_firstEgMotherTruthOrigin, std::vector<int>);
  DECLARE_BRANCH(el_true_firstEgMotherPdgId, std::vector<int>);
  DECLARE_BRANCH(el_true_isPrompt, std::vector<char>);
  DECLARE_BRANCH(el_true_isChargeFl, std::vector<char>);
  DECLARE_BRANCH(mu_pt, std::vector<float>);
  DECLARE_BRANCH(mu_eta, std::vector<float>);
  DECLARE_BRANCH(mu_phi, std::vector<float>);
  DECLARE_BRANCH(mu_e, std::vector<float>);
  DECLARE_BRANCH(mu_charge, std::vector<float>);
  DECLARE_BRANCH(mu_topoetcone20, std::vector<float>);
  DECLARE_BRANCH(mu_ptvarcone30, std::vector<float>);
  DECLARE_BRANCH(mu_isTight, std::vector<char>);
  DECLARE_BRANCH(mu_Isol_FCTight, std::vector<char>);
  DECLARE_BRANCH(mu_Isol_FCLoose, std::vector<char>);
  DECLARE_BRANCH(mu_Isol_FCTightTrackOnly, std::vector<char>);
  DECLARE_BRANCH(mu_Isol_FCTightTrackOnly_FixedRad, std::vector<char>);
  DECLARE_BRANCH(mu_Isol_FCLoose_FixedRad, std::vector<char>);
  DECLARE_BRANCH(mu_Isol_FCTight_FixedRad, std::vector<char>);
  DECLARE_BRANCH(mu_Isol_FixedCutPflowTight, std::vector<char>);
  DECLARE_BRANCH(mu_Isol_FixedCutPflowLoose, std::vector<char>);
  DECLARE_BRANCH(mu_d0sig, std::vector<float>);
  DECLARE_BRANCH(mu_delta_z0_sintheta, std::vector<float>);
  DECLARE_BRANCH(mu_true_type, std::vector<int>);
  DECLARE_BRANCH(mu_true_origin, std::vector<int>);
  DECLARE_BRANCH(mu_true_isPrompt, std::vector<char>);
  DECLARE_BRANCH(tau_pt, std::vector<float>);
  DECLARE_BRANCH(tau_eta, std::vector<float>);
  DECLARE_BRANCH(tau_phi, std::vector<float>);
  DECLARE_BRANCH(tau_charge, std::vector<float>);
  DECLARE_BRANCH(tau_nTrack, std::vector<float>);
  DECLARE_BRANCH(tau_true_pdg, std::vector<int>);
  DECLARE_BRANCH(tau_true_pt, std::vector<float>);
  DECLARE_BRANCH(tau_true_eta, std::vector<float>);
  DECLARE_BRANCH(tau_tight, std::vector<float>);
  DECLARE_BRANCH(tau_RNNScore, std::vector<float>);
  DECLARE_BRANCH(tau_BDTScore, std::vector<float>);
  DECLARE_BRANCH(jet_pt, std::vector<float>);
  DECLARE_BRANCH(jet_eta, std::vector<float>);
  DECLARE_BRANCH(jet_phi, std::vector<float>);
  DECLARE_BRANCH(jet_e, std::vector<float>);
  DECLARE_BRANCH(jet_mv2c00, std::vector<float>);
  DECLARE_BRANCH(jet_mv2c10, std::vector<float>);
  DECLARE_BRANCH(jet_mv2c20, std::vector<float>);
  DECLARE_BRANCH(jet_forwardjvt, std::vector<float>);
  DECLARE_BRANCH(jet_passfjvt, std::vector<char>); // supersede by jet_passforwardjvt
  DECLARE_BRANCH(jet_passforwardjvt, std::vector<char>);
  DECLARE_BRANCH(jet_truthflav, std::vector<int>);
  DECLARE_BRANCH(jet_truthPartonLabel, std::vector<int>);
  DECLARE_BRANCH(jet_isTrueHS, std::vector<char>);
  DECLARE_BRANCH(jet_isbtagged_MV2c10_70, std::vector<char>);
  DECLARE_BRANCH(jet_isbtagged_MV2c10_77, std::vector<char>);
  DECLARE_BRANCH(jet_isbtagged_MV2c10_85, std::vector<char>);
  DECLARE_BRANCH(jet_tagWeightBin_MV2c10_Continuous, std::vector<int>);
  DECLARE_BRANCH(jet_isbtagged_DL1_HybBEff_60, std::vector<char>);
  DECLARE_BRANCH(jet_truthflavExtended, std::vector<int>);
  DECLARE_BRANCH(jet_isbtagged_DL1r_60, std::vector<char>);
  DECLARE_BRANCH(jet_isbtagged_DL1r_70, std::vector<char>);
  DECLARE_BRANCH(jet_isbtagged_DL1r_77, std::vector<char>);
  DECLARE_BRANCH(jet_isbtagged_DL1r_85, std::vector<char>);
  DECLARE_BRANCH(jet_tagWeightBin_DL1r_Continuous, std::vector<int>);
  DECLARE_BRANCH(jet_MV2c10mu, std::vector<float>);
  DECLARE_BRANCH(jet_MV2c10rnn, std::vector<float>);
  DECLARE_BRANCH(jet_DL1, std::vector<float>);
  DECLARE_BRANCH(jet_DL1r, std::vector<float>);
  DECLARE_BRANCH(jet_DL1rmu, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(met_met, Float_t);
  DECLARE_BRANCH_PRIMITIVE(met_phi, Float_t);
  DECLARE_BRANCH_PRIMITIVE(all_particle, Int_t);
  DECLARE_BRANCH_PRIMITIVE(leptonic_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(leptonic_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(leptonic_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(leptonic_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ee_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ee_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ee_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ee_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ejets_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ejets_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ejets_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ejets_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mumu_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mumu_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mumu_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mumu_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mujets_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mujets_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mujets_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mujets_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emu_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emu_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emu_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emu_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eee_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eee_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eee_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eee_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eemu_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eemu_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eemu_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eemu_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emumu_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emumu_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emumu_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emumu_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mumumu_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mumumu_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mumumu_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mumumu_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(et_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(et_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(et_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(et_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mt_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mt_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mt_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mt_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ett_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ett_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ett_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(ett_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eet_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eet_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eet_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(eet_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mtt_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mtt_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mtt_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mtt_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mmt_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mmt_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mmt_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(mmt_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emt_2015, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emt_2016, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emt_2017, Int_t);
  DECLARE_BRANCH_PRIMITIVE(emt_2018, Int_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_e60_lhmedium_nod0, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_mu26_ivarmedium, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_e26_lhtight_nod0_ivarloose, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_e140_lhloose_nod0, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_e120_lhloose, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_e24_lhmedium_L1EM20VH, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_e24_lhmedium_nod0_L1EM18VH, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_mu50, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_mu24, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_e60_lhmedium, Char_t);
  DECLARE_BRANCH_PRIMITIVE(HLT_mu20_iloose_L1MU15, Char_t);
  DECLARE_BRANCH(el_trigMatch_HLT_e60_lhmedium_nod0, std::vector<char>);
  DECLARE_BRANCH(el_trigMatch_HLT_e120_lhloose, std::vector<char>);
  DECLARE_BRANCH(el_trigMatch_HLT_e24_lhmedium_L1EM20VH, std::vector<char>);
  DECLARE_BRANCH(el_trigMatch_HLT_e24_lhmedium_nod0_L1EM18VH, std::vector<char>);
  DECLARE_BRANCH(el_trigMatch_HLT_e60_lhmedium, std::vector<char>);
  DECLARE_BRANCH(el_trigMatch_HLT_e26_lhtight_nod0_ivarloose, std::vector<char>);
  DECLARE_BRANCH(el_trigMatch_HLT_e140_lhloose_nod0, std::vector<char>);
  DECLARE_BRANCH(mu_trigMatch_HLT_mu26_ivarmedium, std::vector<char>);
  DECLARE_BRANCH(mu_trigMatch_HLT_mu50, std::vector<char>);
  DECLARE_BRANCH(mu_trigMatch_HLT_mu24, std::vector<char>);
  DECLARE_BRANCH(mu_trigMatch_HLT_mu20_iloose_L1MU15, std::vector<char>);
  DECLARE_BRANCH_PRIMITIVE(lbn, UInt_t);
  DECLARE_BRANCH_PRIMITIVE(Vtxz, Float_t);
  DECLARE_BRANCH_PRIMITIVE(npVtx, UInt_t);
  DECLARE_BRANCH(el_d0pv, std::vector<float>);
  DECLARE_BRANCH(el_z0pv, std::vector<float>);
  DECLARE_BRANCH(el_d0sigpv, std::vector<float>);
  DECLARE_BRANCH(el_z0sigpv, std::vector<float>);
  DECLARE_BRANCH(el_tight_old, std::vector<bool>);
  DECLARE_BRANCH(el_trigMatch_old, std::vector<bool>);
  DECLARE_BRANCH(el_tight, std::vector<char>);
  DECLARE_BRANCH(el_trigMatch, std::vector<char>);
  DECLARE_BRANCH(el_true_pdg, std::vector<int>);
  DECLARE_BRANCH(el_true_pt, std::vector<float>);
  DECLARE_BRANCH(el_true_eta, std::vector<float>);
  DECLARE_BRANCH(el_truthIFFClass, std::vector<int>);
  DECLARE_BRANCH(mu_d0pv, std::vector<float>);
  DECLARE_BRANCH(mu_z0pv, std::vector<float>);
  DECLARE_BRANCH(mu_d0sigpv, std::vector<float>);
  DECLARE_BRANCH(mu_z0sigpv, std::vector<float>);
  DECLARE_BRANCH(mu_tight_old, std::vector<bool>);
  DECLARE_BRANCH(mu_trigMatch_old, std::vector<bool>);
  DECLARE_BRANCH(mu_tight, std::vector<char>);
  DECLARE_BRANCH(mu_trigMatch, std::vector<char>);
  DECLARE_BRANCH(mu_true_pdg, std::vector<int>);
  DECLARE_BRANCH(mu_true_pt, std::vector<float>);
  DECLARE_BRANCH(mu_true_eta, std::vector<float>);
  DECLARE_BRANCH(mu_truthIFFClass, std::vector<int>);
  DECLARE_BRANCH(jet_m, std::vector<float>);
  DECLARE_BRANCH(jet_OLTau, std::vector<float>);
  DECLARE_BRANCH_PRIMITIVE(met_px, Float_t);
  DECLARE_BRANCH_PRIMITIVE(met_py, Float_t);
  DECLARE_BRANCH_PRIMITIVE(met_sumet, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_EL_SF_Trigger_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_EL_SF_Trigger_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_EL_SF_Reco_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_EL_SF_Reco_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_EL_SF_ID_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_EL_SF_ID_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_EL_SF_Isol_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_EL_SF_Isol_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Trigger_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Trigger_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Trigger_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Trigger_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Trigger_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Trigger_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_ID_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_ID_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_ID_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_ID_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_ID_STAT_LOWPT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_ID_STAT_LOWPT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_ID_SYST_LOWPT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_ID_SYST_LOWPT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Isol_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Isol_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Isol_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_Isol_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_TTVA_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_TTVA_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_TTVA_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_leptonSF_tight_MU_SF_TTVA_SYST_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_triggerSF_tight, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_triggerSF_tight_EL_SF_Trigger_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_triggerSF_tight_EL_SF_Trigger_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_triggerSF_tight_MU_SF_Trigger_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_triggerSF_tight_MU_SF_Trigger_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_triggerSF_tight_MU_SF_Trigger_STAT_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_triggerSF_tight_MU_SF_Trigger_STAT_DOWN, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_triggerSF_tight_MU_SF_Trigger_SYST_UP, Float_t);
  DECLARE_BRANCH_PRIMITIVE(weight_triggerSF_tight_MU_SF_Trigger_SYST_DOWN, Float_t);

  DECLARE_PL_BRANCH_PRIMITIVE(weight_mc, Float_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eventNumber, ULong64_t);
  DECLARE_PL_BRANCH_PRIMITIVE(runNumber, UInt_t);
  DECLARE_PL_BRANCH_PRIMITIVE(randomRunNumber, UInt_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mcChannelNumber, UInt_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mu, Float_t);
  DECLARE_PL_BRANCH_PRIMITIVE(weight_pileup, Float_t);
  DECLARE_PL_BRANCH(nu_pt, std::vector<float>);
  DECLARE_PL_BRANCH(nu_eta, std::vector<float>);
  DECLARE_PL_BRANCH(nu_phi, std::vector<float>);
  DECLARE_PL_BRANCH(nu_origin, std::vector<int>);
  DECLARE_PL_BRANCH(el_pt, std::vector<float>);
  DECLARE_PL_BRANCH(el_eta, std::vector<float>);
  DECLARE_PL_BRANCH(el_phi, std::vector<float>);
  DECLARE_PL_BRANCH(el_e, std::vector<float>);
  DECLARE_PL_BRANCH(el_charge, std::vector<float>);
  DECLARE_PL_BRANCH(el_pt_bare, std::vector<float>);
  DECLARE_PL_BRANCH(el_eta_bare, std::vector<float>);
  DECLARE_PL_BRANCH(el_phi_bare, std::vector<float>);
  DECLARE_PL_BRANCH(el_e_bare, std::vector<float>);
  DECLARE_PL_BRANCH(mu_pt, std::vector<float>);
  DECLARE_PL_BRANCH(mu_eta, std::vector<float>);
  DECLARE_PL_BRANCH(mu_phi, std::vector<float>);
  DECLARE_PL_BRANCH(mu_e, std::vector<float>);
  DECLARE_PL_BRANCH(mu_charge, std::vector<float>);
  DECLARE_PL_BRANCH(mu_pt_bare, std::vector<float>);
  DECLARE_PL_BRANCH(mu_eta_bare, std::vector<float>);
  DECLARE_PL_BRANCH(mu_phi_bare, std::vector<float>);
  DECLARE_PL_BRANCH(mu_e_bare, std::vector<float>);
  DECLARE_PL_BRANCH(jet_pt, std::vector<float>);
  DECLARE_PL_BRANCH(jet_eta, std::vector<float>);
  DECLARE_PL_BRANCH(jet_phi, std::vector<float>);
  DECLARE_PL_BRANCH(jet_e, std::vector<float>);
  DECLARE_PL_BRANCH(jet_nGhosts_bHadron, std::vector<int>);
  DECLARE_PL_BRANCH(jet_nGhosts_cHadron, std::vector<int>);
  DECLARE_PL_BRANCH_PRIMITIVE(met_met, Float_t);
  DECLARE_PL_BRANCH_PRIMITIVE(met_phi, Float_t);
  DECLARE_PL_BRANCH(PDFinfo_X1, std::vector<float>);
  DECLARE_PL_BRANCH(PDFinfo_X2, std::vector<float>);
  DECLARE_PL_BRANCH(PDFinfo_PDGID1, std::vector<int>);
  DECLARE_PL_BRANCH(PDFinfo_PDGID2, std::vector<int>);
  DECLARE_PL_BRANCH(PDFinfo_Q, std::vector<float>);
  DECLARE_PL_BRANCH(PDFinfo_XF1, std::vector<float>);
  DECLARE_PL_BRANCH(PDFinfo_XF2, std::vector<float>);
  DECLARE_PL_BRANCH(mc_generator_weights, std::vector<float>);
  DECLARE_PL_BRANCH_PRIMITIVE(all_particle, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(leptonic_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(leptonic_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(leptonic_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(leptonic_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ee_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ee_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ee_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ee_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ejets_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ejets_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ejets_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ejets_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mumu_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mumu_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mumu_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mumu_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mujets_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mujets_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mujets_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mujets_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emu_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emu_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emu_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emu_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eee_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eee_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eee_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eee_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eemu_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eemu_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eemu_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eemu_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emumu_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emumu_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emumu_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emumu_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mumumu_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mumumu_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mumumu_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mumumu_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(et_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(et_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(et_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(et_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mt_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mt_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mt_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mt_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ett_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ett_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ett_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(ett_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eet_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eet_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eet_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(eet_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mtt_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mtt_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mtt_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mtt_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mmt_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mmt_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mmt_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(mmt_2018, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emt_2015, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emt_2016, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emt_2017, Int_t);
  DECLARE_PL_BRANCH_PRIMITIVE(emt_2018, Int_t);

  DECLARE_TRUTH_BRANCH(weight_mc, Float_t);
  DECLARE_TRUTH_BRANCH(eventNumber, ULong64_t);
  DECLARE_TRUTH_BRANCH(runNumber, UInt_t);
  DECLARE_TRUTH_BRANCH(mu, Float_t);
  DECLARE_TRUTH_BRANCH(weight_pileup, Float_t);
  DECLARE_TRUTH_BRANCH(randomRunNumber, UInt_t);
  DECLARE_TRUTH_BRANCH(mcChannelNumber, UInt_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_tbar_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_t_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_tbar_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_t_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_tbar_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_t_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_t_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_tbar_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_tbar_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_tbar_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_t_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_t_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_tbar_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_from_tbar_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_from_tbar_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_t_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_from_t_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_afterFSR_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_tbar_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_beforeFSR_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_afterFSR_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_afterFSR_beforeDecay_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_beforeFSR_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_afterFSR_beforeDecay_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_afterFSR_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_beforeFSR_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_beforeFSR_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_afterFSR_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_beforeFSR_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_beforeFSR_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_afterFSR_SC_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_from_t_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_from_t_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_afterFSR_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_afterFSR_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_tbar_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_beforeFSR_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_afterFSR_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_afterFSR_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_afterFSR_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_afterFSR_beforeDecay_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_ttbar_afterFSR_beforeDecay_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_from_t_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_afterFSR_SC_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_beforeFSR_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_afterFSR_SC_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_afterFSR_SC_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_tbar_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_beforeFSR_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_afterFSR_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_t_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_beforeFSR_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_afterFSR_SC_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_from_tbar_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_from_t_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_afterFSR_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_from_t_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_beforeFSR_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_from_tbar_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_afterFSR_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay1_from_t_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_afterFSR_SC_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_from_t_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_afterFSR_SC_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_from_tbar_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_tbar_afterFSR_SC_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_from_t_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Wdecay2_from_t_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_from_tbar_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_from_tbar_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_t_beforeFSR_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_from_tbar_phi, Float_t);

  DECLARE_TRUTH_BRANCH(MC_b_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_b_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_b_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay2_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay1_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay1_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Higgs_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay1_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Higgs_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Higgs_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay2_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay1_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay2_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay2_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay2_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_H_decay1_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Higgs_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W1_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W1_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W2_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W2_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W2_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W1_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W2_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W2_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W1_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W2_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W2_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W2_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W1_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W1_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W1_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W2_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W1_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W1_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_W1_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_W2_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z2_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z2_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z1_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z1_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z2_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z2_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z2_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z2_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z2_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z2_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z2_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z2_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z1_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z1_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z1_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton2_from_Z1_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z1_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z1_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z1_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_Z_Lepton1_from_Z1_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_hadr_Tau_Jet1, Int_t);
  DECLARE_TRUTH_BRANCH(MC_hadr_Tau_Jet2, Int_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau1_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau1_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau1_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau2_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau2_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau2_pdgId, Int_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau2_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau1_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau1_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau2_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau1_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau2_eta, Float_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau1_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau2_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau2_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau2_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau1_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau1_phi, Float_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau2_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau1_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau1_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau1_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau2_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau2_m, Float_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau1_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau1_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau1_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay1_from_Tau2_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_nu_from_Tau2_pt, Float_t);
  DECLARE_TRUTH_BRANCH(MC_W_decay2_from_Tau2_pt, Float_t);
};

}  // namespace TL

#endif

/** @file  AlgBase.h
 *  @brief TL::AlgBase class header
 *  @class TL::AlgBase
 *  @brief Base class for running a
 *         TopLoop analysis algorithm
 *
 *  This class is a skeleton for the steps needed
 *  to run a TopLoop algorithm. 
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

#ifndef TL_AlgBase_h
#define TL_AlgBase_h

// ATLAS
#include <AsgTools/StatusCode.h>
#include <AsgTools/MessageCheck.h>

// TL
#include <TopLoop/Core/Utils.h>

// C++
#include <string>
#include <memory>

// ROOT
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

ANA_MSG_HEADER(msgAlgBase)
//ANA_MSG_HEADER(msgAlgBaseVars)

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
  class FileManager;
}

namespace TL {

  class AlgBase : public TNamed {

  protected:
    std::string   m_datasetName;
    bool          m_isMC, m_isNominal;
    bool          m_showTTRVwarning;

    std::shared_ptr<TL::FileManager> m_fm;
    std::shared_ptr<TTreeReader>     m_reader;
    std::shared_ptr<TTreeReader>     m_weightsReader;
    std::shared_ptr<TTreeReader>     m_particleLevelReader;

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

    //! Set up a variable as a TTreeReaderValue pointer
    /*!
      This one liner checks to make sure that the variable is on
      the tree. If its not - you get a warning. If the variable isn't
      there your program will still work as long as you don't try to
      dereference the pointer.
    */
    template<typename T>
    std::shared_ptr<T>
    setupTreeVar(std::shared_ptr<TTreeReader> reader, const char* name, const char* tree_name = "");

    void progress(long cur, long total, int range = 10) const;

  public:
    AlgBase();
    virtual ~AlgBase();

    //! Set the file manager
    /*! 
      This is a requirement of all TopLoop algorithms
    */
    void setFileManager(std::shared_ptr<TL::FileManager> fm);

    //! Count the sumWeights from all input trees
    /*!
      This can be called in the init() function if info about the
      sum of weights is desired; s the nominal sum of weights
    */
    float countSumWeights();

    //! Generator based changes in the sum of weights
    /*!
      Generator "on the fly" weight variations stored in a vector
      The first entry (as of April 2017) is the same as nominal.
    */
    std::vector<float> generatorVariedSumWeights();

    //! Get names of Generator based weights
    /*!
      Can be called to retrieve the list of strings corresponding
      to the name of the generator based weights.
    */
    std::vector<std::string> generatorWeightNames();

    //! Get the dataset id
    /*!
      This can be called in the init() function if
      the dataset ID is desired
    */
    unsigned int get_dsid();

    //! Initialize the variables for the TTreeReader
    /*!
      This function sets the TTreeReader variables up.
    */
    StatusCode init_core_vars();

    //! Initialize the algorithm properties
    /*!
      The point of this function is to initialize various properties
      of the algorithm, e.g. setting user specific member variables.
    */
    virtual StatusCode init();

    //! The function which is called after init(), for output.
    /*!
      This function is meant for declaring files, histograms, trees, etc.
      to be output by the histogram.
    */
    virtual StatusCode setupOutput();

    //! The function which is called in a loop over all events
    /*! 
      This function is meant to be where the actual analysis
      happens.  All variables which are initialized for the
      TTreeReader are updated at the beginning of execute and all of
      the event information is available.
    */
    virtual StatusCode execute();

    //! The function which is called at the end.
    /*! 
      This function is meant to wrap up the algorithm, e.g.  write
      histograms and trees to a file, close the file.
    */
    virtual StatusCode finish();

    //! Function to tell algorithm it's analyzing data
    /*! 
      Some variables don't exist for data, so the algorithm must
      know if it's touching data, so it knows which variables to avoid
      initializing.
    */
    void setIsData();

    //! Function to tell algorithm it's analyzing a systematic variation
    /*!
      Some variables in the nominal tree do not exist in the systematic
      trees; so we need to avoid setting them up for systematic runs.
    */
    void setIsSystematic();

    //! Function to turn off warning message for missing variables
    /*!
      Before the we setup TTreeReaderVariable pointers, we make sure
      that the variable actually exists in the tree, if it doesn't a warning
      is displayed by default. This turns off that warning.
    */
    virtual void turnOffTTRVWarning();

    std::shared_ptr<TL::FileManager> fileManager();         //!< get pointer to file manager

    std::shared_ptr<TTreeReader>     reader();              //!< get pointer to TTreeReader

    std::shared_ptr<TTreeReader>     weightsReader();       //!< get pointer to TTreeReader for sumweights

    std::shared_ptr<TTreeReader>     particleLevelReader(); //!< get pointer to TTreeReader for PL info
    
  private:
    ClassDef(AlgBase, 1);
  };

}

inline void TL::AlgBase::setIsData()          { m_isMC            = false; }
inline void TL::AlgBase::setIsSystematic()    { m_isNominal       = false; }
inline void TL::AlgBase::turnOffTTRVWarning() { m_showTTRVwarning = false; }

inline std::shared_ptr<TL::FileManager> TL::AlgBase::fileManager()         { return m_fm;                  }
inline std::shared_ptr<TTreeReader>     TL::AlgBase::reader()              { return m_reader;              }
inline std::shared_ptr<TTreeReader>     TL::AlgBase::weightsReader()       { return m_weightsReader;       }
inline std::shared_ptr<TTreeReader>     TL::AlgBase::particleLevelReader() { return m_particleLevelReader; }

inline void TL::AlgBase::setFileManager(std::shared_ptr<TL::FileManager> fm) { m_fm = fm; }

template<typename T>
inline std::shared_ptr<T>
TL::AlgBase::setupTreeVar(std::shared_ptr<TTreeReader> reader, const char* name, const char* tree_name) {
  //ANA_MSG_SOURCE(msgAlgBase,"TL::AlgBase");
  using namespace msgAlgBase;
  if ( reader->GetTree() == nullptr ) {
    if ( m_showTTRVwarning ) {
      ANA_MSG_WARNING(name << " branch trying to link to a null tree! tree_name: " << tree_name);
    }
    return nullptr;
  }
  if ( reader->GetTree()->GetListOfBranches()->FindObject(name) != nullptr ) {
    return std::make_shared<T>(*reader,name);
  }
  else {
    if ( m_showTTRVwarning ) {
      ANA_MSG_WARNING(name << " branch not found in the tree! If you try to access it, you will crash");
    }
    return nullptr;
  }
}

#endif

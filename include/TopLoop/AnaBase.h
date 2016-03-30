/** @file  AnaBase.h
 *  @brief TL::AnaBase class header
 *  @class TL::AnaBase
 *  @brief Base class for running a
 *         TopLoop analysis algorithm
 *
 *  This class is a skeleton for the steps needed
 *  to run a TopLoop algorithm. 
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_AnaBase_h
#define TL_AnaBase_h

// TL
#include <TopLoop/Utils.h>

// C++
#include <string>
#include <memory>

// ROOT
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

using TTRV_vec_float = TTreeReaderValue<std::vector<float> >;
using TTRV_vec_char  = TTreeReaderValue<std::vector<char> >;
using TTRV_float     = TTreeReaderValue<Float_t>;
using TTRV_uint      = TTreeReaderValue<UInt_t>;
using TTRV_ulongint  = TTreeReaderValue<ULong64_t>;

namespace TL {
  class FileManager;
}

namespace TL {

  class AnaBase {

  protected:
    std::string      m_datasetName;
    bool             m_isMC, m_isNominal;
    
    std::shared_ptr<TL::FileManager> m_fm;
    std::shared_ptr<TTreeReader>     m_reader;
    std::shared_ptr<TTreeReader>     m_weightsReader;

    std::shared_ptr<TTRV_float>     totalEventsWeighted;

    std::shared_ptr<TTRV_float>     weight_mc;
    std::shared_ptr<TTRV_float>     weight_pileup;
    std::shared_ptr<TTRV_float>     weight_leptonSF;
    std::shared_ptr<TTRV_float>     weight_bTagSF_77;
    std::shared_ptr<TTRV_float>     weight_jvt;

    std::shared_ptr<TTRV_float>     weight_pileup_UP;
    std::shared_ptr<TTRV_float>     weight_pileup_DOWN;
    std::vector<std::shared_ptr<TTRV_float> > weightSyst_leptonSF;
    std::vector<std::shared_ptr<TTRV_float> > weightSyst_bTagSF_extrapolation;
    std::vector<std::shared_ptr<TTRV_vec_float> > weightSyst_bTagSF_eigenvars;

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
    
    std::shared_ptr<TTRV_vec_float> mu_pt;
    std::shared_ptr<TTRV_vec_float> mu_phi;
    std::shared_ptr<TTRV_vec_float> mu_eta;
    std::shared_ptr<TTRV_vec_float> mu_e;
    std::shared_ptr<TTRV_vec_float> mu_charge;
    std::shared_ptr<TTRV_vec_float> mu_topoetcone20;
    std::shared_ptr<TTRV_vec_float> mu_ptvarcone30;
    std::shared_ptr<TTRV_vec_float> mu_d0sig;
    std::shared_ptr<TTRV_vec_float> mu_delta_z0_sintheta;
    
    std::shared_ptr<TTRV_vec_float> jet_pt;
    std::shared_ptr<TTRV_vec_float> jet_eta;
    std::shared_ptr<TTRV_vec_float> jet_phi;
    std::shared_ptr<TTRV_vec_float> jet_e;
    std::shared_ptr<TTRV_vec_float> jet_mv2c00;
    std::shared_ptr<TTRV_vec_float> jet_mv2c10;
    std::shared_ptr<TTRV_vec_float> jet_mv2c20; 
    std::shared_ptr<TTRV_vec_float> jet_ip3dsv1;
    std::shared_ptr<TTRV_vec_float> jet_jvt;

    std::shared_ptr<TTRV_float> met_met;
    std::shared_ptr<TTRV_float> met_phi;

    std::shared_ptr<TTRV_vec_char> el_trigMatch_HLT_e60_lhmedium;
    std::shared_ptr<TTRV_vec_char> el_trigMatch_HLT_e24_lhmedium_L1EM18VH;
    std::shared_ptr<TTRV_vec_char> el_trigMatch_HLT_e120_lhloose;
    std::shared_ptr<TTRV_vec_char> mu_trigMatch_HLT_mu50;
    std::shared_ptr<TTRV_vec_char> mu_trigMatch_HLT_mu20_iloose_L1MU15;

  public:
    AnaBase();
    virtual ~AnaBase();

    //! Initialize core members
    /*!
      This function is always called by the constructor.
      It initializes the file manager.
    */
    void core_init();

    //! Initialize the variables for the TTreeReader
    /*!
      This function sets the TTreeReader variables up.
      It *MUST* be called in the base init file which is
      user modifiable.
    */
    void init_core_vars();

    //! Count the sumWeights from all input trees
    /*!
      This can be called in the init() function if 
      info about the sum of weights is desired.
    */
    float countSumWeights();

    //! Initialize the algorithm properties
    /*!
      The point of this function is to initialize various properties
      of the algorithm, e.g. setting user specific member variables.
    */
    virtual TL::STATUS init();

    
    //! The function which is called after init(), for output.
    /*!
      This function is meant for declaring files, histograms, trees, etc.
      to be output by the histogram.
    */
    virtual TL::STATUS setupOutput();

    //! The function which is called in a loop over all events
    /*! 
      This function is meant to be where the actual analysis
      happens.  All variables which are initialized for the
      TTreeReader are updated at the beginning of execute and all of
      the event information is available.
    */
    virtual TL::STATUS execute();

    //! The function which is called at the end.
    /*! 
      This function is meant to wrap up the algorithm, e.g.  write
      histograms and trees to a file, close the file.
    */
    virtual TL::STATUS finish();

    //! Function to tell algorithm it's analyzing data
    /*! 
      Some variables don't exist for data, so the algorithm must
      know if it's touching data, so it knows which variables to avoid
      initializing.
    */
    virtual void isData();
    virtual void isSystematic();
    
    std::shared_ptr<TL::FileManager> fileManager();   //!< get pointer to file manager
    std::shared_ptr<TTreeReader>     reader();        //!< get pointer to TTreeReader
    std::shared_ptr<TTreeReader>     weightsReader(); //!< get pointer to TTreeReader for sumweights
    
  };

}

inline void TL::AnaBase::isData() { m_isMC = false; }
inline void TL::AnaBase::isSystematic() { m_isNominal = false; }

inline std::shared_ptr<TL::FileManager> TL::AnaBase::fileManager()   { return m_fm;     }
inline std::shared_ptr<TTreeReader>     TL::AnaBase::reader()        { return m_reader; }
inline std::shared_ptr<TTreeReader>     TL::AnaBase::weightsReader() { return m_weightsReader; }

#endif

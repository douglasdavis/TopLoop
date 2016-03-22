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

// ROOT
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

namespace TL {
  class FileManager;
}

namespace TL {

  class AnaBase {

  protected:
    std::string      m_datasetName;
    TL::FileManager* m_fm;

    TTreeReader* m_reader;
    
    TTreeReaderValue<std::vector<float> >* el_pt;
    TTreeReaderValue<std::vector<float> >* el_phi;
    TTreeReaderValue<std::vector<float> >* el_eta;
    TTreeReaderValue<std::vector<float> >* el_e;
    TTreeReaderValue<std::vector<float> >* el_cl_eta;
    TTreeReaderValue<std::vector<float> >* el_charge;
    TTreeReaderValue<std::vector<float> >* el_topoetcone20;
    TTreeReaderValue<std::vector<float> >* el_ptvarcone20;
    TTreeReaderValue<std::vector<float> >* el_d0sig;
    TTreeReaderValue<std::vector<float> >* el_delta_z0_sintheta;
    
    TTreeReaderValue<std::vector<float> >* mu_pt;
    TTreeReaderValue<std::vector<float> >* mu_phi;
    TTreeReaderValue<std::vector<float> >* mu_eta;
    TTreeReaderValue<std::vector<float> >* mu_e;
    TTreeReaderValue<std::vector<float> >* mu_charge;
    TTreeReaderValue<std::vector<float> >* mu_topoetcone20;
    TTreeReaderValue<std::vector<float> >* mu_ptvarcone30;
    TTreeReaderValue<std::vector<float> >* mu_d0sig;
    TTreeReaderValue<std::vector<float> >* mu_delta_z0_sintheta;
    
    TTreeReaderValue<std::vector<float> >* jet_pt;
    TTreeReaderValue<std::vector<float> >* jet_eta;
    TTreeReaderValue<std::vector<float> >* jet_phi;
    TTreeReaderValue<std::vector<float> >* jet_e;
    TTreeReaderValue<std::vector<float> >* jet_mv1;
    TTreeReaderValue<std::vector<float> >* jet_mvb;

    TTreeReaderValue<float>* met_met;
    TTreeReaderValue<float>* met_phi;

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
      This function is meant to wrap up the algorithm, e.g.
      write histograms and trees to a file, close the file.
     */
    virtual TL::STATUS finish();

    TL::FileManager* fileManager(); //!< A getter for the file manager

    TTreeReader* reader(); //!< A getter for the TTreeReader
    
  };

}

inline TL::FileManager* TL::AnaBase::fileManager() { return m_fm;     }
inline TTreeReader*     TL::AnaBase::reader()      { return m_reader; }

#endif

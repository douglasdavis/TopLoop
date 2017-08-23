/** @file  Algorithm.h
 *  @brief TL::Algorithm class header
 *  @class TL::Algorithm
 *  @brief Base class for running a
 *         TopLoop analysis algorithm
 *
 *  This class is a skeleton for the steps needed
 *  to run a TopLoop algorithm. 
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

#ifndef TL_Algorithm_h
#define TL_Algorithm_h

// ATLAS
#include <AsgTools/StatusCode.h>
#include <AsgTools/MessageCheck.h>

// TL
#include <TopLoop/Core/Utils.h>
#include <TopLoop/Core/Variables.h>
#include <TopLoop/Core/FileManager.h>

ANA_MSG_HEADER(msgAlgorithm)

namespace TL {

  class Algorithm : public TNamed, public TL::Variables {

  protected:
    std::string   m_datasetName;
    bool          m_isMC, m_isNominal;
    bool          m_showTTRVwarning;

    long m_eventCounter;
    long m_totalEntries;

    std::shared_ptr<TL::FileManager> m_fm;
    std::shared_ptr<TTreeReader>     m_reader;
    std::shared_ptr<TTreeReader>     m_weightsReader;

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

    //! Print the progess of the event loop (percent done)
    void progress(int percent_base = 10) const;

  public:
    Algorithm();
    virtual ~Algorithm();

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
    
  private:
    ClassDef(Algorithm, 1);

  };

}

#include "Algorithm.icc"

#endif

/** @file  Algorithm.h
 *  @brief TL::Algorithm class header
 *  @class TL::Algorithm
 *  @brief Base class for running a
 *         TopLoop analysis algorithm
 *
 *  This class is a skeleton for the steps needed
 *  to run a TopLoop algorithm.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
 */

#ifndef TL_Algorithm_h
#define TL_Algorithm_h

// TL
#include <TopLoop/Core/Utils.h>
#include <TopLoop/Core/Variables.h>
#include <TopLoop/Core/FileManager.h>
#include <TopLoop/Core/Loggable.h>
#include <TopLoop/Core/WeightTool.h>

// ROOT
#include <TChain.h>

namespace TL {
  namespace EDM {
    class FinalState;
  }
}

namespace TL {

  class Algorithm : public TL::Loggable, public TL::Variables {

  private:
    std::string   m_datasetName{""};
    bool          m_isMC{true};
    bool          m_isNominal{true};
    bool          m_isNominal_Loose{false};
    bool          m_initCalled{false};

    long m_totalEntries{0};
    long m_eventCounter{0};

    std::unique_ptr<TL::FileManager> m_fm{nullptr};
    std::shared_ptr<TTreeReader>     m_reader{nullptr};
    std::shared_ptr<TTreeReader>     m_weightsReader{nullptr};

    TL::WeightTool m_weightTool{this};

  public:
    /// default constructor
    Algorithm();
    /// destructor
    virtual ~Algorithm() = default;

    /// default move constructor
    Algorithm(Algorithm&&) = default;
    /// default move assignemnt
    Algorithm& operator=(Algorithm&&) = default;

  private:
    /// delete copy constructor
    Algorithm(const Algorithm&) = delete;
    /// delete assignment operator
    Algorithm& operator=(const Algorithm&) = delete;

    /// for use in TL::Job to ensure main init() function is called.
    bool initCalled() const;

    /// Set the file manager
    /**
     *  This is a requirement of all TopLoop algorithms Must be called
     *  before feeding to the TL::Job object.
     */
    TL::StatusCode setFileManager(std::unique_ptr<TL::FileManager> fm);

  private:
    /// connect all of the default branches
    /**
     *  Uses the CONNECT_BRANCH macro to finish the setup of all
     *  default branches.
     */
    TL::StatusCode connect_default_branches();

  public:

    /// @name Main sample processing API functions
    /// @{

    /// Initialize the algorithm properties
    /**
     *  The point of this function is to initialize various properties
     *  of the algorithm, e.g. setting user specific member variables.
     */
    virtual TL::StatusCode init();

    /// The function which is called after init(), for output.
    /**
     *  This function is meant for declaring files, histograms, trees,
     *  etc.  to be output by the algorithm. Anything a user puts in
     *  this function can technically just be included in the init()
     *  function - but for organizational purposes this function is
     *  included.
     */
    virtual TL::StatusCode setupOutput();

    /// The function which is called in a loop over all events
    /**
     *  This function is meant to be where the actual analysis
     *  happens.  All variables which are initialized for the
     *  TTreeReader are updated at the beginning of execute and all of
     *  the event's information is available.
     */
    virtual TL::StatusCode execute();

    /// The function which is called at the end.
    /**
     *  This function is meant to wrap up the algorithm, e.g.  write
     *  histograms and trees to a file, close the file.
     */
    virtual TL::StatusCode finish();

    /// @}

  private:
    /// Initialize the variables for the TTreeReader
    /**
     *  This function sets the TTreeReader variables up. Gets called
     *  in init()
     */
    TL::StatusCode init_core_vars();

  public:

    /// @name Sample property setting utilities
    /// @{

    /// Function to tell algorithm it's analyzing data
    /**
     *  Some variables don't exist for data, so the algorithm must
     *  know if it's touching data, so it knows which variables to
     *  avoid initializing.
     */
    void setIsData();

    /// @}

  protected:
    /// @name Sample property utilities
    /// @{

    /// get if sample is MC
    bool isMC()            const;
    /// get if sample is data (for readability, opposite of isMC())
    bool isData()          const;
    /// get is nominal
    bool isNominal()       const;
    /// get is nominal loose
    bool isNominal_Loose() const;
    /// get is systematic (for readability, true if not nominal or nominal_Loose)
    bool isSystematic()    const;
    /// get the current event counter (not eventNumber!)
    long eventCount()      const;

    /// @}

  protected:

    /// @name Generator and weight utilities
    /// @{

    TL::WeightTool& weightTool() { return m_weightTool; }

    [[deprecated("use weightTool().generatorSumWeights()")]]
    float countSumWeights() { return weightTool().generatorSumWeights(); }

    [[deprecated("use weightTool().generatorSumWeights()")]]
    float generatorSumWeights() { return weightTool().generatorSumWeights(); }

    [[deprecated("use weightTool().generatorVariedSumWeights()")]]
    const std::vector<float>& generatorVariedSumWeights() {
      return weightTool().generatorVariedSumWeights();
    }

    [[deprecated("use weightTool().generatorVariedWeightsNames()")]]
    const std::map<std::string,std::size_t>& generatorVariedWeightsNames() {
      return weightTool().generatorVariedWeightsNames();
    }

    [[deprecated("use weightTool().sumOfVariation()")]]
    float sumOfVariation(const std::string& variation_name) {
      return weightTool().sumOfVariation(variation_name);
    }

    [[deprecated("use weightTool().currentWeightOfVariation()")]]
    float currentWeightOfVariation(const std::string& variation_name) {
      return weightTool().currentWeightOfVariation(variation_name);
    }

    /// Get the dataset id
    /**
     *  This can be called in the init() function if the dataset ID is
     *  desired
     */
    unsigned int get_dsid();

    /// get if the sample is AFII (determined from the sumWeights tree)
    bool sampleIsAFII();

    /// @}

  protected:
    /// @name Sample pointer getters
    /// @{

    /// get the raw pointer to the file manager
    const TL::FileManager*              fileManager()   const;
    /// get pointer to the main reader
    const std::shared_ptr<TTreeReader>& reader()        const;
    /// get pointer to the weights reader
    const std::shared_ptr<TTreeReader>& weightsReader() const;
    /// get non const pointer to weights reader
    std::shared_ptr<TTreeReader>& weightsReader();

    /// @}

    /// Print the progess of the event loop (percent done)
    void printProgress(int n_prints = 10) const;

  private:

    friend class Job;
    friend class WeightTool;

  };

}

inline void TL::Algorithm::setIsData()             { m_isMC = false;                             }
inline bool TL::Algorithm::initCalled()      const { return m_initCalled;                        }
inline bool TL::Algorithm::isMC()            const { return m_isMC;                              }
inline bool TL::Algorithm::isData()          const { return !m_isMC;                             }
inline bool TL::Algorithm::isNominal()       const { return m_isNominal;                         }
inline bool TL::Algorithm::isNominal_Loose() const { return m_isNominal_Loose;                   }
inline bool TL::Algorithm::isSystematic()    const { return !(m_isNominal || m_isNominal_Loose); }
inline long TL::Algorithm::eventCount()      const { return m_eventCounter;                      }

#endif

/** @file  FileManager.h
 *  @brief TL::FileManager class header
 *  @class TL::FileManager
 *  @brief Handles ROOT files.
 *
 *  This class does the necessary file handling for a TopLoop
 *  Algorithim.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_FileManager_h
#define TL_FileManager_h

// TopLoop
#include <TopLoop/Core/Loggable.h>
#include <TopLoop/Core/Utils.h>
#include <TopLoop/Core/SampleMetaSvc.h>

// ROOT
class TChain;

namespace TL {
  class FileManager : public TL::Loggable {
  private:
    bool                     m_doParticleLevel    {false};
    std::vector<std::string> m_fileNames          {};
    std::string              m_plTreeName         {"particleLevel"};
    std::string              m_treeName           {"nominal"};
    std::string              m_weightsTreeName    {"sumWeights"};
    std::unique_ptr<TChain>  m_rootChain          {nullptr};
    std::unique_ptr<TChain>  m_particleLevelChain {nullptr};
    std::unique_ptr<TChain>  m_rootWeightsChain   {nullptr};
    std::string              m_rucioDirName       {"none"};
    unsigned int             m_dsid               {0};
    TL::kSgTopNtup           m_sgtopNtupVersion   {};

    /// initialize the ROOT TChain pointers
    TL::StatusCode initChain();

  public:

    /// default constructor
    FileManager();

    /// destructor
    virtual ~FileManager() = default;

    /// enable particle level tree setup
    void enableParticleLevel();

    /// determine if particle level has been enabled
    bool particleLevelEnabled() const { return m_doParticleLevel; }

    /// @name Sample tree naming setup functions
    /**
     *  By default, the main tree name will be "nominal" and the
     *  weights tree name will be "sumWeights". These functions are
     *  used for modifying the names. They must be used *before*
     *  calling any of the feeding functions.
     */
    ///@{

    /// set the tree name to something other than "nominal"
    /**
     *  If this function is to be used - it must be called before any
     *  feeds!
     */
    void setTreeName(const std::string& tn);

    /// set the weight tree name to something other than "sumWeights"
    /**
     *  If this function is to be used - it must be called before any
     *  feeds!
     */
    void setWeightsTreeName(const std::string& tn);

    /// set the particle level tree name to something other than "particleLevel"
    /**
     *  If this function is to be used - it must be called before any
     *  feeds!
     */
    void setParticleLevelTreeName(const std::string& tn);

    /// @}

    /// @name Feeding functions
    /**
     *  Various ways to feed your FileManager with samples
     */
    /// @{

    /// Feed the chains with files in a directory
    /**
     *  By default this function will only look for files ending in
     *  "*.root" the take_all param set to true will eat _all_ files!
     *  (e.g. "*.root.2")
     *
     *  @param dirpath path of the directory containing files to
     *  process
     *  @param max_files maximum number of files to find before breaking
     */
    void feedDir(const std::string& dirpath, const unsigned int max_files = 10000);

    /// Feed the chains with files listed in a text file.
    void feedTxt(const std::string& txtfilename);

    /// Feed the chains with a single file
    void feedSingle(const char* fileName);

    /// @}

    /// @name Simple getters related to naming
    /// @{

    /// getter for the file names which have been fed to the chains
    const std::vector<std::string>& fileNames() const { return m_fileNames; }
    /// the name of the main tree being read
    const std::string& treeName() const { return m_treeName; }
    /// the name of the weights tree being read
    const std::string& weightsTreeName() const { return m_weightsTreeName; }
    /// the name of the particle level tree being read
    const std::string& particleLevelTreeName() const { return m_plTreeName; }
    /// the name of the rucio dataset fed to feedDir
    const std::string& rucioDir() const { return m_rucioDirName; }
    /// the dsid as determined from a regex search on the rucio dir
    unsigned int dsid() const { return m_dsid; }
    /// the single top ntuple version
    TL::kSgTopNtup getSgTopNtupVersion() const { return m_sgtopNtupVersion; }

    /// @}

    /// @name ROOT object getters
    /// @{

    /// getter for the main chain raw pointer
    TChain* rootChain() const { return m_rootChain.get(); }
    /// getter for the particle level chain raw pointer
    TChain* particleLevelChain() const { return m_particleLevelChain.get(); }
    /// getter for the weights chain raw pointer
    TChain* rootWeightsChain() const { return m_rootWeightsChain.get(); }

    /// @}
  };

}

#endif

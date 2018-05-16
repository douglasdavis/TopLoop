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

// ROOT
class TChain;

namespace TL {
  class FileManager : public TL::Loggable {
  private:
    std::vector<std::string> m_fileNames        {};
    std::string              m_treeName         {"nominal"};
    std::string              m_weightsTreeName  {"sumWeights"};
    std::unique_ptr<TChain>  m_rootChain        {nullptr};
    std::unique_ptr<TChain>  m_rootWeightsChain {nullptr};
    std::string              m_rucioDirName     {"none"};
    unsigned int             m_dsid             {0};

    /// make files ending in ".root.N" their old name again
    std::map<std::string,std::string> m_renames{};

    /// initialize the ROOT TChain pointers
    TL::StatusCode initChain();

  public:

    /// default constructor
    FileManager();

    /// destructor
    virtual ~FileManager();

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
     *  @param take_all take all files no matter what the extension is
     */
    void feedDir(const std::string& dirpath, const bool take_all=false);

    /// Feed the chains with files listed in a text file.
    void feedTxt(const std::string& txtfilename);

    /// Feed the chains with a single file
    void feedSingle(const char* fileName);

    /// @}

    /// @name Simple getters related to naming
    /// @{

    /// getter for the file names which have been fed to the chains
    const std::vector<std::string>& fileNames()       const;
    /// the name of the main tree being read
    const std::string&              treeName()        const;
    /// the name of the weights tree being read
    const std::string&              weightsTreeName() const;
    /// the name of the rucio dataset fed to feedDir
    const std::string&              rucioDir()        const;
    /// the dsid as determined from a regex search on the rucio dir
    unsigned int                    dsid()            const;

    /// @}

    /// @name ROOT object getters
    /// @{

    /// getter for the main chain raw pointer
    TChain* rootChain()        const;
    /// getter for the weights chain raw pointer
    TChain* rootWeightsChain() const;

    /// @}
  };

}

inline const std::vector<std::string>& TL::FileManager::fileNames() const { return m_fileNames; }

inline const std::string& TL::FileManager::weightsTreeName() const { return m_weightsTreeName; }
inline const std::string& TL::FileManager::treeName()        const { return m_treeName;        }
inline const std::string& TL::FileManager::rucioDir()        const { return m_rucioDirName;    }
inline       unsigned int TL::FileManager::dsid()            const { return m_dsid;            }

inline       TChain* TL::FileManager::rootChain()        const { return m_rootChain.get();        }
inline       TChain* TL::FileManager::rootWeightsChain() const { return m_rootWeightsChain.get(); }

#endif

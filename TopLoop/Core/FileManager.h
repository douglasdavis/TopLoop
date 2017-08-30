/** @file  FileManager.h
 *  @brief TL::FileManager class header
 *  @class TL::FileManager
 *  @brief Handles ROOT files.
 *
 *  This class does the necessary file handling for TL::AnaBase
 *  algorithms.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

#ifndef TL_FileManager_h
#define TL_FileManager_h

// TopLoop
#include <TopLoop/Core/Logable.h>

// ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

// C++
#include <vector>
#include <string>

namespace TL {
  class FileManager : public TL::Logable {
  private:
    std::vector<std::string> m_fileNames;
    std::string              m_treeName;
    std::string              m_weightsTreeName;
    TChain*                  m_rootChain;
    TChain*                  m_rootWeightsChain;

  public:
    FileManager();
    virtual ~FileManager();

    void setTreeName(const std::string& tn);
    void setWeightsTreeName(const std::string& tn);
    void initChain();

    void feedDir(const std::string& dirpath, const bool take_all=false);
    void feedTxt(const std::string& txtfilename);
    void feedSingle(const char* fileName);

    const std::vector<std::string>& fileNames()       const;
    const std::string&              treeName()        const;
    const std::string&              weightsTreeName() const;

    TChain* rootChain();
    TChain* rootWeightsChain();
  };
}

inline const std::vector<std::string>& TL::FileManager::fileNames() const { return m_fileNames; }

inline const std::string&   TL::FileManager::weightsTreeName() const { return m_weightsTreeName; }
inline const std::string&   TL::FileManager::treeName()        const { return m_treeName;        }

inline       TChain*   TL::FileManager::rootChain()        { return m_rootChain;        }
inline       TChain*   TL::FileManager::rootWeightsChain() { return m_rootWeightsChain; }

#endif

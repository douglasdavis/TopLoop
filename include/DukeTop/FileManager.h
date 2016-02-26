/** @file  FileManager.h
 *  @brief DT::FileManager class header
 *  @class DT::FileManager
 *  @brief Handles ROOT files.
 *
 *  This class does the necessary file handling for DT::AnaBase
 *  algorithms.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef DT_FileManager_h
#define DT_FileManager_h

// ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

// C++
#include <vector>
#include <string>

namespace DT {
  class FileManager {
  private:
    std::vector<std::string> m_fileNames;
    std::string              m_treeName;
    TChain*                  m_rootChain;
    
  public:
    FileManager();
    virtual ~FileManager();

    void setTreeName(const std::string& tn);
    
    void feedDir(const std::string& dirpath, const bool take_all=false);
    void feedTxt(const std::string& txtfilename);
    
    const std::vector<std::string>& fileNames() const;
    const std::string&              treeName()  const;
    
    TChain* rootChain();
  };
}

inline const std::vector<std::string>& DT::FileManager::fileNames() const { return m_fileNames; }
inline const std::string&              DT::FileManager::treeName()  const { return m_treeName;  }

inline       TChain*                   DT::FileManager::rootChain()       { return m_rootChain; }

#endif

/** @file  FileManager.h
 *  @brief TL::FileManager class header
 *  @class TL::FileManager
 *  @brief Handles ROOT files.
 *
 *  This class does the necessary file handling for TL::AnaBase
 *  algorithms.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_FileManager_h
#define TL_FileManager_h

// ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

// C++
#include <vector>
#include <string>

namespace TL {
  class FileManager {
  private:
    std::vector<std::string> m_fileNames;
    std::string              m_treeName;
    TChain*                  m_rootChain;
    
  public:
    FileManager();
    virtual ~FileManager();

    void setTreeName(const std::string& tn);
    void initChain();
    
    void feedDir(const std::string& dirpath, const bool take_all=false);
    void feedTxt(const std::string& txtfilename);
    void feedSingle(const char* fileName);
    
    const std::vector<std::string>& fileNames() const;
    const std::string&              treeName()  const;
    
    TChain* rootChain();
  };
}

inline const std::vector<std::string>& TL::FileManager::fileNames() const { return m_fileNames; }
inline const std::string&              TL::FileManager::treeName()  const { return m_treeName;  }
inline       TChain*                   TL::FileManager::rootChain()       { return m_rootChain; }

#endif

/** @file FileManager.cxx
 *  @brief TL::FileManager class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/FileManager.h>
#include <TopLoop/Core/Utils.h>

// ATLAS
//

// boost
#include <boost/filesystem.hpp>

// C++
#include <iostream>
#include <cstddef>
#include <fstream>
#include <sstream>

// ROOT
#include <TROOT.h>

TL::FileManager::FileManager() : TL::Loggable("TL::FileManager"),
  m_fileNames(),
  m_treeName("nominal"),
  m_weightsTreeName("sumWeights"),
  m_rootChain(nullptr),
  m_rootWeightsChain(nullptr) {
}

TL::FileManager::~FileManager() {}

void TL::FileManager::setTreeName(const std::string& tn) {
  m_treeName = tn;
}

void TL::FileManager::setWeightsTreeName(const std::string& tn) {
  m_weightsTreeName = tn;
}

TL::StatusCode TL::FileManager::initChain() {
  if ( !m_rootChain ) {
    m_rootChain = std::make_unique<TChain>(m_treeName.c_str());
  }
  if ( !m_rootWeightsChain ) {
    m_rootWeightsChain = std::make_unique<TChain>(m_weightsTreeName.c_str());
  }
  if ( !m_rootChain || !m_rootWeightsChain ) {
    return TL::StatusCode::FAILURE;
  }
  return TL::StatusCode::SUCCESS;
}

void TL::FileManager::feedDir(const std::string& dirpath, const bool take_all) {
  TL_CHECK(initChain());
  logger()->info("Feeding {}", dirpath);
  boost::filesystem::path p(dirpath);
  auto i = boost::filesystem::directory_iterator(p);
  for ( ; i != boost::filesystem::directory_iterator(); ++i ) {
    if ( !boost::filesystem::is_directory(i->path()) ) {
      auto split_version = TL::string_split(i->path().filename().string(),'.');
      if ( split_version.back() != "root" && !take_all ) {
        continue;
      }
      else {
        std::string final_path = i->path().filename().string();
        logger()->info("Adding file: {}",final_path);
        m_fileNames.emplace_back(dirpath+(final_path));
        m_rootChain->Add((dirpath+"/"+final_path).c_str());
        m_rootWeightsChain->Add((dirpath+"/"+final_path).c_str());
      }
    }
    else {
      continue;
    }
  }
}

void TL::FileManager::feedTxt(const std::string& txtfilename) {
  TL_CHECK(initChain());
  std::string line;
  std::ifstream infile(txtfilename);
  while ( std::getline(infile,line) ) {
    if ( !line.empty() ) {
      logger()->info("Adding file {}",line);
      m_fileNames.emplace_back(line);
      m_rootChain->Add(line.c_str());
      m_rootWeightsChain->Add(line.c_str());
    }
  }
}

void ::TL::FileManager::feedSingle(const char* fileName) {
  TL_CHECK(initChain());
  m_fileNames.emplace_back(std::string(fileName));
  m_rootChain->Add(fileName);
  m_rootWeightsChain->Add(fileName);
}

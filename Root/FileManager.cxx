/** @file FileManager.cxx
 *  @brief TL::FileManager class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
 */

// TL
#include <TopLoop/Core/FileManager.h>
#include <TopLoop/Core/Utils.h>

// ATLAS
//

// boost
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

// C++
#include <iostream>
#include <cstddef>
#include <fstream>
#include <sstream>

// ROOT
#include <TROOT.h>

TL::FileManager::FileManager() : TL::Loggable("TL::FileManager") {}

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
  logger()->info("Feeding from {}", dirpath);
  fs::path p(dirpath);
  for ( const auto& i : fs::directory_iterator(p) ) {
    if ( !fs::is_directory(i.path()) ) {
      auto whole_path = i.path();
      if ( whole_path.filename().string().find(".root") == std::string::npos && !take_all ) {
        continue;
      }
      else {
        logger()->info("Adding file: {}",whole_path.filename().string());
        auto final_path = whole_path.string().c_str();
        m_fileNames.emplace_back(final_path);
        m_rootChain->Add(final_path);
        m_rootWeightsChain->Add(final_path);
      }
    }
    else {
      continue;
    }
  }
  if ( m_fileNames.empty() ) {
    logger()->critical("Directory {} doesn't contain any files!", dirpath);
  }
  return;
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

void TL::FileManager::feedSingle(const char* fileName) {
  TL_CHECK(initChain());
  m_fileNames.emplace_back(std::string(fileName));
  m_rootChain->Add(fileName);
  m_rootWeightsChain->Add(fileName);
}

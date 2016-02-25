/** @file FileManager.cxx
 *  @brief DT::FileManager class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

// DT
#include <DukeTop/FileManager.h>
#include <DukeTop/Utils.h>

// boost
#include <boost/filesystem.hpp>

// C++
#include <iostream>
#include <cstddef>

DT::FileManager::FileManager() :
  m_fileNames(), m_rootFiles(), m_rootChain(nullptr) {
}

DT::FileManager::~FileManager() {}

void DT::FileManager::setTreeName(const std::string& tn) {
  m_treeName  = tn;
  m_rootChain = new TChain(tn.c_str());
}

void DT::FileManager::feedDir(const std::string& dirpath, const bool take_all) {
  DT::Info("FileManager::feedDir()","feeding");
  boost::filesystem::path p(dirpath);
  auto i =boost::filesystem::directory_iterator(p);  
  for ( ; i != boost::filesystem::directory_iterator(); ++i ) {
    if ( !boost::filesystem::is_directory(i->path()) ) {
      auto split_version = DT::string_split(i->path().filename().string(),'.');
      if ( split_version.back() != "root" && !take_all ) {
	continue;
      }
      else {
	m_fileNames.emplace_back(dirpath+(i->path().filename().string()));
	m_rootChain->Add((dirpath+i->path().filename().string()).c_str());
      }
    }
    else
      continue;
  }
}

void DT::FileManager::feedTxt(const std::string& txtfilename) {
  DT::Fatal("DT::FileManager::feedTxt not supported yet");
}

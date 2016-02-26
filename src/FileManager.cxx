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
#include <fstream>
#include <sstream>

DT::FileManager::FileManager() :
  m_fileNames(), m_treeName(),  m_rootChain(nullptr) {
}

DT::FileManager::~FileManager() {}

void DT::FileManager::setTreeName(const std::string& tn) {
  m_treeName  = tn;
  m_rootChain = new TChain(tn.c_str());
}

void DT::FileManager::feedDir(const std::string& dirpath, const bool take_all) {
  DT::Info("FileManager::feedDir()","feeding");
  boost::filesystem::path p(dirpath);
  auto i = boost::filesystem::directory_iterator(p);  
  for ( ; i != boost::filesystem::directory_iterator(); ++i ) {
    if ( !boost::filesystem::is_directory(i->path()) ) {
      auto split_version = DT::string_split(i->path().filename().string(),'.');
      if ( split_version.back() != "root" && !take_all ) {
	continue;
      }
      else {
	DT::Info("feedDir()","Adding file "+i->path().filename().string());
	m_fileNames.emplace_back(dirpath+(i->path().filename().string()));
	m_rootChain->Add((dirpath+i->path().filename().string()).c_str());
      }
    }
    else {
      continue;
    }
  }
}

void DT::FileManager::feedTxt(const std::string& txtfilename) {
  std::string line;
  std::ifstream infile(txtfilename);
  while ( std::getline(infile,line) ) {
    if ( !line.empty() ) {
      DT::Info("feedTxt()","Adding file "+line);
      m_fileNames.emplace_back(line);
      m_rootChain->Add(line.c_str());
    }
  }
}

/** @file FileManager.cxx
 *  @brief TL::FileManager class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/FileManager.h>
#include <TopLoop/Core/Utils.h>

// boost
#include <boost/filesystem.hpp>

// C++
#include <iostream>
#include <cstddef>
#include <fstream>
#include <sstream>

// ROOT
#include <TROOT.h>

TL::FileManager::FileManager() :
  m_fileNames(), m_treeName("nominal"), m_weightsTreeName("sumWeights"),
  m_particleLevelTreeName("particleLevel"), m_rootChain(nullptr),
  m_rootWeightsChain(nullptr), m_particleLevelChain(nullptr) {
}

TL::FileManager::~FileManager() {
  delete m_rootChain;
  delete m_rootWeightsChain;
  delete m_particleLevelChain;
  gROOT->GetListOfFiles()->Remove(m_rootChain);
  gROOT->GetListOfFiles()->Remove(m_rootWeightsChain);
  gROOT->GetListOfFiles()->Remove(m_particleLevelChain);
}

void TL::FileManager::setTreeName(const std::string& tn) {
  m_treeName = tn;
}

void TL::FileManager::setWeightsTreeName(const std::string& tn) {
  m_weightsTreeName = tn;
}

void TL::FileManager::setParticleLevelTreeName(const std::string& tn) {
  m_particleLevelTreeName = tn;
}

void TL::FileManager::initChain() {
  m_rootChain          = new TChain(m_treeName.c_str());
  m_rootWeightsChain   = new TChain(m_weightsTreeName.c_str());
  //m_particleLevelChain = new TChain(m_particleLevelTreeName.c_str());
}

void TL::FileManager::feedDir(const std::string& dirpath, const bool take_all) {
  this->initChain();
  TL::Info(__PRETTY_FUNCTION__,"feeding");
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
        TL::Info(__PRETTY_FUNCTION__,"Adding file",final_path);
        m_fileNames.emplace_back(dirpath+(final_path));
        m_rootChain->         Add((dirpath+"/"+final_path).c_str());
        m_rootWeightsChain->  Add((dirpath+"/"+final_path).c_str());
        //m_particleLevelChain->Add((dirpath+"/"+final_path).c_str());
      }
    }
    else {
      continue;
    }
  }
}

void TL::FileManager::feedTxt(const std::string& txtfilename) {
  this->initChain();
  std::string line;
  std::ifstream infile(txtfilename);
  while ( std::getline(infile,line) ) {
    if ( !line.empty() ) {
      TL::Info(__PRETTY_FUNCTION__,"Adding file",line);
      m_fileNames.emplace_back(line);
      m_rootChain->         Add(line.c_str());
      m_rootWeightsChain->  Add(line.c_str());
      //m_particleLevelChain->Add(line.c_str());
    }
  }
}

void ::TL::FileManager::feedSingle(const char* fileName) {
  this->initChain();
  m_fileNames.emplace_back(std::string(fileName));
  m_rootChain->         Add(fileName);
  m_rootWeightsChain->  Add(fileName);
  //m_particleLevelChain->Add(fileName);
}

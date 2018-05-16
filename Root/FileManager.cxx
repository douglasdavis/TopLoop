/** @file FileManager.cxx
 *  @brief TL::FileManager class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
 */

// TL
#include <TopLoop/Core/FileManager.h>
#include <TopLoop/Core/Utils.h>

// ROOT
#include <TChain.h>

// boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
namespace fs = boost::filesystem;

// C++
#include <fstream>
#include <regex>
#include <string>

TL::FileManager::FileManager() : TL::Loggable("TL::FileManager") {}

TL::FileManager::~FileManager() {
  // rename the renamed files back to original names
  for ( const auto& entry : m_renames ) {
    logger()->debug("Renaming {} back to {}",entry.second,entry.first);
    fs::rename(entry.second,entry.first);
  }
}

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
  std::string dp{dirpath};
  if ( boost::algorithm::ends_with(dp,"/") ) {
    dp.pop_back();
  }
  logger()->info("Feeding from {}", dp);
  fs::path p(dp);

  std::vector<std::string> splits;
  boost::algorithm::split(splits,dp,boost::is_any_of("/"));
  m_rucioDirName = splits.back();

  std::regex rgx("(.[0-9]{6}.)");
  std::smatch match;
  if (std::regex_search(m_rucioDirName, match, rgx)) {
    std::string matchstr = match[1].str();
    matchstr = matchstr.substr(1, matchstr.size() - 2);
    m_dsid = std::stoi(matchstr);
    logger()->info("Determined DSID: {}", m_dsid);
  }

  for ( const auto& i : fs::directory_iterator(p) ) {
    if ( !fs::is_directory(i.path()) ) {
      auto whole_path = i.path();
      if ( whole_path.filename().string().find(".root") == std::string::npos && !take_all ) {
        continue;
      }
      logger()->info("Adding file: {}",whole_path.string());
      std::string final_path = whole_path.string();
      // if the file doesn't end in .root, make it end in .root
      // because ROOT is insane.
      if ( not boost::algorithm::ends_with(final_path,"root") ) {
        m_renames.emplace(final_path,final_path+".root");
        logger()->debug("Temporarily renaming {} to {} because ROOT is insane",
                        final_path,final_path+".root");
        fs::rename(final_path,final_path+".root");
        final_path = final_path+".root";
      }
      m_fileNames.emplace_back(final_path);
      m_rootChain->AddFile(final_path.c_str());
      m_rootWeightsChain->AddFile(final_path.c_str());
    }
    else {
      continue;
    }
  }
  if ( m_fileNames.empty() ) {
    logger()->error("Directory {} doesn't contain any files!", dp);
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

void TL::FileManager::feedSingle(const char* fileName) {
  TL_CHECK(initChain());
  m_fileNames.emplace_back(std::string(fileName));
  m_rootChain->Add(fileName);
  m_rootWeightsChain->Add(fileName);
}

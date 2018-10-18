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

void TL::FileManager::enableParticleLevel() {
  m_doParticleLevel = true;
}

void TL::FileManager::setTreeName(const std::string& tn) {
  m_treeName = tn;
}

void TL::FileManager::setWeightsTreeName(const std::string& tn) {
  m_weightsTreeName = tn;
}

void TL::FileManager::setParticleLevelTreeName(const std::string& tn) {
  m_plTreeName = tn;
}

void TL::FileManager::setTruthTreeName(const std::string& tn) {
  m_truthTreeName = tn;
}

TL::StatusCode TL::FileManager::initChain() {
  if ( !m_rootChain ) {
    m_rootChain = std::make_unique<TChain>(m_treeName.c_str());
  }
  if ( !m_rootWeightsChain ) {
    m_rootWeightsChain = std::make_unique<TChain>(m_weightsTreeName.c_str());
  }
  if ( m_doParticleLevel && !m_particleLevelChain && !m_truthChain ) {
    m_particleLevelChain = std::make_unique<TChain>(m_plTreeName.c_str());
    m_truthChain         = std::make_unique<TChain>(m_truthTreeName.c_str());
  }
  if ( !m_rootChain || !m_rootWeightsChain ) {
    return TL::StatusCode::FAILURE;
  }
  return TL::StatusCode::SUCCESS;
}

void TL::FileManager::feedDir(const std::string& dirpath, const unsigned int max_files) {
  TL_CHECK(initChain());
  std::string dp{dirpath};
  if ( boost::algorithm::ends_with(dp,"/") ) {
    dp.pop_back();
  }
  fs::path p(dp);
  logger()->info("Feeding from {}", p.string());

  // the "rucio directory name" or "rucio dataset name" is assumed to
  // be the path fed to this function.
  std::vector<std::string> splits;
  boost::algorithm::split(splits,dp,boost::is_any_of("/"));
  m_rucioDirName = splits.back();

  // were going to loop over the files in a rucio download
  // directory... nominally just the directory path given to this
  // function... which we've defined as a path object p above
  fs::path loop_over = p;

  // loop over the dataset files to determine if we need a symbolic link
  // we need one if a file doesn't end in exactly ".root"
  bool need_symlink = false;
  for ( const auto& i : fs::directory_iterator(p) ) {
    if ( fs::is_directory(i.path()) ) continue;
    auto ipathname = i.path().string();
    if ( not boost::algorithm::ends_with(ipathname,"root") ) {
      need_symlink = true;
      break;
    }
  }

  // if the dataset name ends in ".root" and a file inside
  // doesn't... we hit the TChain::Add bug. Our workaround is to
  // create symbolic link to the rucio dataset but remove the ".root"
  // at the end of the dataset name.
  if ( boost::algorithm::ends_with(p.string(),"root") && need_symlink ) {
    fs::path abs_dsd     = fs::absolute(p);
    fs::path abs_parent  = abs_dsd.parent_path();
    std::string lose_root = m_rucioDirName;
    boost::replace_all(lose_root,".root","");
    std::string symlink_name = abs_parent.string() + "/.TL_FileManager_symlinks/" + lose_root;
    // if hidden symlink holder doesn't exist.. make it
    if ( !fs::exists(abs_parent.string() + "/.TL_FileManager_symlinks") ) {
      fs::create_directory(abs_parent.string() + "/.TL_FileManager_symlinks");
    }
    // if the required symlink doesn't already exist.. make it
    if ( !fs::exists(symlink_name) ) {
      logger()->info("Creating symlink {} to avoid TChain::Add bug",symlink_name);
      fs::create_symlink(abs_dsd,symlink_name);
    }
    else {
      logger()->info("Using existing symlink {} to avoid TChain::Add bug",symlink_name);
    }
    // if we ended up in this if block, we want to loop over the
    // symlink directory.. not the original one.
    loop_over = fs::path(symlink_name);
  }

  std::vector<std::string> checkForDupes;

  // loop_over is the symobolic link if necessary. nominally it's
  // just the standard rucio dataset directory
  for ( const auto& i : fs::directory_iterator(loop_over) ) {
    if ( m_fileNames.size() >= max_files ) {
      logger()->warn("Breaking file feeding loop at {} files",max_files);
      break;
    }
    if ( !fs::is_directory(i.path()) ) {
      auto whole_path = i.path();
      if ( whole_path.filename().string().find(".root") == std::string::npos ) {
        continue;
      }
      logger()->info("Adding file: {}",whole_path.filename().string());

      std::vector<std::string> dupeSplits;
      boost::algorithm::split(dupeSplits,i.path().filename().string(),boost::is_any_of("."));
      checkForDupes.emplace_back(dupeSplits.at(2)+dupeSplits.at(3));

      std::string final_path = whole_path.string();
      m_fileNames.emplace_back(final_path);
      m_rootChain->AddFile(final_path.c_str());
      m_rootWeightsChain->AddFile(final_path.c_str());
      if ( m_doParticleLevel ) {
        m_particleLevelChain->AddFile(final_path.c_str());
        m_truthChain->AddFile(final_path.c_str());
      }
    }
    else {
      continue;
    }
  }
  if ( m_fileNames.empty() ) {
    logger()->error("Directory {} doesn't contain any files!", dp);
  }

  // check for duplicate {[job number].[file number]} combos
  std::sort(std::begin(checkForDupes),std::end(checkForDupes));
  auto uniq = std::unique(std::begin(checkForDupes),std::end(checkForDupes));
  if ( uniq != std::end(checkForDupes) ) {
    logger()->error("You have duplicate files in your dataset!");
    return;
  }

  // check for exact ducplicate filenames
  if ( std::unique(std::begin(m_fileNames),std::end(m_fileNames)) != std::end(m_fileNames) ) {
    logger()->error("You have duplicate files in your dataset!");
    return;
  }

  // try to determine dsid from rucio directory name
  std::regex rgx("(.[0-9]{6}.)");
  std::smatch match;
  if (std::regex_search(m_rucioDirName, match, rgx)) {
    std::string matchstr = match[1].str();
    matchstr = matchstr.substr(1, matchstr.size() - 2);
    m_dsid = std::stoi(matchstr);
    logger()->info("Determined DSID: {}", m_dsid);
    TL::SampleMetaSvc::get().printInfo(m_dsid);
  }
  m_sgtopNtupVersion = TL::SampleMetaSvc::get().getNtupleVersion(m_rucioDirName);

}

void TL::FileManager::feedTxt(const std::string& txtfilename) {
  TL_CHECK(initChain());

  fs::path p(txtfilename);
  if ( p.extension().string() != ".txt" ) {
    logger()->warn("feedTxt given file without .txt extension");
    logger()->warn("Logic to determine rucio info might fail");
  }

  auto txtpos = txtfilename.find_last_of(".");
  m_rucioDirName = txtfilename.substr(0,txtpos);
  logger()->info("feedTxt determined rucio dataset name:");
  logger()->info("{}", m_rucioDirName);

  std::string line;
  std::ifstream infile(txtfilename);
  while ( std::getline(infile,line) ) {
    if ( !line.empty() ) {
      logger()->info("Adding file {}",line);
      m_fileNames.emplace_back(line);
      m_rootChain->Add(line.c_str());
      m_rootWeightsChain->Add(line.c_str());
      if ( m_doParticleLevel ) {
        m_particleLevelChain->AddFile(line.c_str());
        m_truthChain->AddFile(line.c_str());
      }
    }
  }

  // try to determine dsid from rucio directory name
  std::regex rgx("(.[0-9]{6}.)");
  std::smatch match;
  if (std::regex_search(m_rucioDirName, match, rgx)) {
    std::string matchstr = match[1].str();
    matchstr = matchstr.substr(1, matchstr.size() - 2);
    m_dsid = std::stoi(matchstr);
    logger()->info("Determined DSID: {}", m_dsid);
    TL::SampleMetaSvc::get().printInfo(m_dsid);
  }
  m_sgtopNtupVersion = TL::SampleMetaSvc::get().getNtupleVersion(m_rucioDirName);

}

void TL::FileManager::feedSingle(const char* fileName) {
  TL_CHECK(initChain());
  m_fileNames.emplace_back(std::string(fileName));
  m_rootChain->Add(fileName);
  m_rootWeightsChain->Add(fileName);
  if ( m_doParticleLevel ) {
    m_particleLevelChain->AddFile(fileName);
    m_truthChain->AddFile(fileName);
  }
}

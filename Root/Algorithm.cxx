/** @file Algorithm.cxx
 *  @brief TL::Algorithm class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/SampleMetaSvc.h>

// Boost
#include <boost/algorithm/string/predicate.hpp>

TL::Algorithm::Algorithm() : TL::Loggable("TL::Algorithm") {}

TL::StatusCode TL::Algorithm::init() {
  TL_CHECK(init_core_vars());
  m_initCalled = true;
  m_eventCounter = 0;
  std::string treename(fileManager()->rootChain()->GetName());

  std::string data_or_mc = "MC";
  std::string mode = "nominal";
  if (isSystematic()) {
    mode = "systematic";
  }
  if (isData()) {
    data_or_mc = "Data";
  }
  logger()->info("Processing tree {} in mode {} as a {} sample", treename, mode,
                 data_or_mc);

  checkRelease();

  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::setupOutput() { return TL::StatusCode::SUCCESS; }

TL::StatusCode TL::Algorithm::execute() {
  m_eventCounter++;
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::finish() { return TL::StatusCode::SUCCESS; }

const TL::FileManager* TL::Algorithm::fileManager() const { return m_fm.get(); }

const std::shared_ptr<TTreeReader>& TL::Algorithm::reader() const { return m_reader; }

const std::shared_ptr<TTreeReader>& TL::Algorithm::particleLevelReader() const {
  return m_particleLevelReader;
}

const std::shared_ptr<TTreeReader>& TL::Algorithm::weightsReader() const {
  return m_weightsReader;
}

const std::shared_ptr<TTreeReader>& TL::Algorithm::truthReader() const {
  return m_truthReader;
}

std::shared_ptr<TTreeReader>& TL::Algorithm::weightsReader() { return m_weightsReader; }

TL::StatusCode TL::Algorithm::setFileManager(std::unique_ptr<TL::FileManager> fm) {
  if (fm == nullptr) {
    logger()->error("TL::EDM::FileManager is nullptr");
    return TL::StatusCode::FAILURE;
  }
  m_fm = std::move(fm);
  m_totalEntries = m_fm->rootChain()->GetEntries();
  m_isNominal = m_fm->treeName() == "nominal";
  m_isNominal_Loose = m_fm->treeName() == "nominal_Loose";
  auto camp = m_fm->getCampaign();
  if (camp == TL::kCampaign::Unknown) {
    logger()->warn("Unknown campaign, TL::Algorithm is going to assume this is data");
    m_isMC = false;
  }
  else {
    m_isMC = camp != TL::kCampaign::Data;
  }

  if (m_fm->particleLevelEnabled()) {
    m_totalParticleLevelEntries = m_fm->particleLevelChain()->GetEntries();
  }
  return TL::StatusCode::SUCCESS;
}

void TL::Algorithm::checkRelease() {
  /*** Figuring out if we're using release 20.7 sample ***/
  auto rucioDirStr = fileManager()->rucioDir();
  bool dataCouldBeRel207 = boost::algorithm::contains(rucioDirStr, "p2950");
  auto camp = TL::SampleMetaSvc::get().getCampaign(rucioDirStr);
  if (isMC() && (camp == TL::kCampaign::MC15c)) {
    m_isRel207 = true;
  }
  else if (isData() && dataCouldBeRel207) {
    m_isRel207 = true;
  }
  else {
    m_isRel207 = false;
  }
  if (m_isRel207) {
    logger()->warn(
        "********************************************************************************"
        "*");
    logger()->warn(
        "* TopLoop was designed with release 21 as a first class citizen, not 20.7!      "
        "*");
    logger()->warn(
        "* It _looks_ like you're using release 20.7, I hope you know what you're doing! "
        "*");
    logger()->warn(
        "* I'm not 100% sure you're using 20.7, it's an educated guess.. see the code!   "
        "*");
    logger()->warn(
        "********************************************************************************"
        "*");
  }
}

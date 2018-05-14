/** @file Algorithm.cxx
 *  @brief TL::Algorithm class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>

TL::Algorithm::Algorithm() :
  TL::Loggable("TL::Algorithm") {}

TL::StatusCode TL::Algorithm::init() {
  TL_CHECK(init_core_vars());
  m_initCalled   = true;
  m_eventCounter = 0;
  std::string treename(fileManager()->rootChain()->GetName());

  std::string mode = "nominal";
  if ( isSystematic() ) {
    mode = "systematic";
  }

  logger()->info("Processing tree {} in mode {}",treename,mode);

  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::setupOutput() {
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::execute() {
  m_eventCounter++;
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Algorithm::finish() {
  return TL::StatusCode::SUCCESS;
}

const TL::FileManager* TL::Algorithm::fileManager() const {
  return m_fm.get();
}

const std::shared_ptr<TTreeReader>& TL::Algorithm::reader() const {
  return m_reader;
}

const std::shared_ptr<TTreeReader>& TL::Algorithm::weightsReader() const {
  return m_weightsReader;
}

TL::StatusCode TL::Algorithm::setFileManager(std::unique_ptr<TL::FileManager> fm) {
  if ( fm == nullptr ) {
    logger()->error("TL::EDM::FileManager is nullptr");
    return TL::StatusCode::FAILURE;
  }
  m_fm = std::move(fm);
  m_totalEntries    = m_fm->rootChain()->GetEntries();
  m_isNominal       = m_fm->treeName() == "nominal";
  m_isNominal_Loose = m_fm->treeName() == "nominal_Loose";
  return TL::StatusCode::SUCCESS;
}

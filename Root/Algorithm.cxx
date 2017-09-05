/** @file Algorithm.cxx
 *  @brief TL::Algorithm class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/Algorithm.h>

TL::Algorithm::Algorithm() :
  TL::Loggable("TL::Algorithm"),
  m_datasetName(),
  m_isMC(true),
  m_isNominal(true)
{
  SetName("TopLoop");
}

TL::Algorithm::~Algorithm() {}

TL::StatusCode TL::Algorithm::init() {
  TL_CHECK(init_core_vars());
  m_eventCounter = 0;
  m_totalEntries = fileManager()->rootChain()->GetEntries();

  if ( fileManager()->treeName() != "nominal" ) {
    setIsSystematic();
  }

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

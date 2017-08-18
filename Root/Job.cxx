/** @file Job.cxx
 *  @brief TL::Job class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

// TL
#include <TopLoop/Core/Job.h>
#include <TopLoop/Core/AlgBase.h>
#include <TopLoop/Core/Utils.h>
#include <TopLoop/Core/FileManager.h>

ANA_MSG_SOURCE(msgTopLoop,"TL::Job")

StatusCode TL::Job::run() {
  using namespace msgTopLoop;
  StatusCode::enableFailure();
  ANA_CHECK_SET_TYPE(StatusCode);
  ANA_CHECK(m_analysis->init());
  ANA_CHECK(m_analysis->setupOutput());
  m_analysis->reader()->Restart();
  while ( m_analysis->reader()->Next() ) {
    ANA_CHECK(m_analysis->execute());
  }
  ANA_CHECK(m_analysis->finish());
  return StatusCode::SUCCESS;
}

/** @file Job.cxx
 *  @brief TL::Job class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

// TL
#include <TopLoop/Core/Job.h>
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/Utils.h>
#include <TopLoop/Core/FileManager.h>

TL::StatusCode TL::Job::run() {
  m_analysis->init();
  m_analysis->setupOutput();
  m_analysis->reader()->Restart();
  while ( m_analysis->reader()->Next() ) {
    m_analysis->execute();
  }
  m_analysis->finish();
  return TL::StatusCode::SUCCESS;
}

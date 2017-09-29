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
  TL_CHECK(m_analysis->init());
  if ( not m_analysis->initCalled() ) {
    logger()->critical("You didn't call TL::Algorithm::init()");
    logger()->critical("in your algorithm's init() function");
    logger()->critical("This is a required line!");
    return TL::StatusCode::FAILURE;
  }
  TL_CHECK(m_analysis->setupOutput());
  m_analysis->reader()->Restart();
  while ( m_analysis->reader()->Next() ) {
    TL_CHECK(m_analysis->execute());
  }
  TL_CHECK(m_analysis->finish());
  return TL::StatusCode::SUCCESS;
}

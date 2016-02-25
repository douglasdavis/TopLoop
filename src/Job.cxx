/** @file Job.cxx
 *  @brief DT::Job class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

// DT
#include <DukeTop/Job.h>
#include <DukeTop/AnaBase.h>

void DT::Job::run() {
  m_analysis->init();
  m_analysis->setupOutput();
  while ( m_analysis->reader()->Next() ) {
    m_analysis->execute();
  }
  m_analysis->finish();
}

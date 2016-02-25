/** @file Job.cxx
 *  @brief DT::Job class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

// DT
#include <DukeTop/Job.h>
#include <DukeTop/AnaBase.h>

void DT::Job::run() {

  if ( m_analysis->init() == DT::STATUS::Good ) {}
  else
    DT::Fatal("your init() returned DT::STATUS::Fail");

  if ( m_analysis->setupOutput() == DT::STATUS::Good ) {}
  else
    DT::Fatal("your setupOutput() returned DT::STATUS::Fail");
  
  while ( m_analysis->reader()->Next() ) {
    if ( m_analysis->execute() == DT::STATUS::Good ) {}
    else
      DT::Fatal("your execute() returned DT::STATUS::Fail");
  }

  if ( m_analysis->finish() == DT::STATUS::Good ) {}
  else
    DT::Fatal("your finish() returned DT::STATUS::Fail");

}

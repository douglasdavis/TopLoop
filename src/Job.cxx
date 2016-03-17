/** @file Job.cxx
 *  @brief TL::Job class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

// TL
#include <TopLoop/Job.h>
#include <TopLoop/AnaBase.h>

void TL::Job::run() {

  if ( m_analysis->init() == TL::STATUS::Good ) {}
  else
    TL::Fatal("your init() returned TL::STATUS::Fail");

  if ( m_analysis->setupOutput() == TL::STATUS::Good ) {}
  else
    TL::Fatal("your setupOutput() returned TL::STATUS::Fail");
  
  while ( m_analysis->reader()->Next() ) {
    if ( m_analysis->execute() == TL::STATUS::Good ) {}
    else
      TL::Fatal("your execute() returned TL::STATUS::Fail");
  }

  if ( m_analysis->finish() == TL::STATUS::Good ) {}
  else {
    TL::Fatal("your finish() returned TL::STATUS::Fail");
  }

}

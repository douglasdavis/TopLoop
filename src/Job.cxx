/** @file Job.cxx
 *  @brief TL::Job class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Job.h>
#include <TopLoop/AnaBase.h>
#include <TopLoop/Utils.h>

void TL::Job::run() {
  if ( m_analysis->init() == TL::STATUS::Good ) {}
  else {
    TL::Fatal("your init() returned TL::STATUS::Fail");
  }

  if ( m_analysis->setupOutput() == TL::STATUS::Good ) {}
  else {
    TL::Fatal("your setupOutput() returned TL::STATUS::Fail");
  }

  auto current_reader = m_analysis->reader();
  if ( m_particleLevelRun ) {
    TL::Info("Job::run()","particle level run!");
    current_reader = m_analysis->particleLevelReader();
  }

  current_reader->SetEntry(-1); // always start from beginning, TTreeReader::Next() will go to 0.
  TL::Info("Job::run()","made it to execute");
  while ( current_reader->Next() ) {
    auto eventResult = m_analysis->execute();
    if ( eventResult == TL::STATUS::Good ) {
      // good event
    }
    else if ( eventResult == TL::STATUS::Skip ) {
      continue;
    }
    else {
      TL::Fatal("your execute() returned TL::STATUS::Fail");
    }
  }

  if ( m_analysis->finish() == TL::STATUS::Good ) {}
  else {
    TL::Fatal("your finish() returned TL::STATUS::Fail");
  }
}

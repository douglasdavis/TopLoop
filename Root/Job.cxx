/** @file Job.cxx
 *  @brief TL::Job class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/Job.h>
#include <TopLoop/Core/AlgBase.h>
#include <TopLoop/Core/Utils.h>
#include <TopLoop/Core/FileManager.h>

void TL::Job::run() {
  if ( m_analysis->init() == TL::STATUS::Good ) {}
  else {
    TL::Fatal(__PRETTY_FUNCTION__,"your init() returned TL::STATUS::Fail");
  }

  if ( m_analysis->setupOutput() == TL::STATUS::Good ) {}
  else {
    TL::Fatal(__PRETTY_FUNCTION__,"your setupOutput() returned TL::STATUS::Fail");
  }

  auto current_reader = m_analysis->reader();
  //if ( m_particleLevelRun ) {
  //  TL::Info(__PRETTY_FUNCTION__,"particle level run!");
  //  current_reader = m_analysis->particleLevelReader();
  //  current_chain  = m_analysis->fileManager()->rootParticleLevelChain();
  //}

  current_reader->Restart();
  while ( current_reader->Next() ) {
    auto eventResult = m_analysis->execute();
    if ( eventResult == TL::STATUS::Good ) {
      // good event
    }
    else if ( eventResult == TL::STATUS::Skip ) {
      continue;
    }
    else {
      TL::Fatal(__PRETTY_FUNCTION__,"your execute() returned TL::STATUS::Fail");
    }
  }

  if ( m_analysis->finish() == TL::STATUS::Good ) {}
  else {
    TL::Fatal(__PRETTY_FUNCTION__,"your finish() returned TL::STATUS::Fail");
  }
}

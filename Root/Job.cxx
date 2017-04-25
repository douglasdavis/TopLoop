/** @file Job.cxx
 *  @brief TL::Job class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/Job.h>
#include <TopLoop/Core/AnaBase.h>
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
  TChain *current_chain = m_analysis->fileManager()->rootChain();
  auto current_reader = m_analysis->reader();
  if ( m_particleLevelRun ) {
    TL::Info(__PRETTY_FUNCTION__,"particle level run!");
    current_reader = m_analysis->particleLevelReader();
    current_chain  = m_analysis->fileManager()->rootParticleLevelChain();
  }

  //current_reader->SetEntry(-1); // always start from beginning, TTreeReader::Next() will go to 0.
  //while ( current_reader->Next() ) {
  for ( Long64_t i = 0; i < current_chain->GetEntries(); ++i ) {
    current_reader->SetEntry(i);
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

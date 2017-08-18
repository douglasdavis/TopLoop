/** @file AlgBase.cxx
 *  @brief TL::AlgBase class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/AlgBase.h>
#include <TopLoop/Core/FileManager.h>

TL::AlgBase::AlgBase() :
  m_datasetName(),
  m_isMC(true),
  m_isNominal(true),
  m_showTTRVwarning(true),
  m_verbose(false)
{}

TL::AlgBase::~AlgBase() {}

float TL::AlgBase::countSumWeights() {
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  float sumWeights = 0;

  while ( m_weightsReader->Next() ) { // this didnt work WTF.
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      TL::Fatal("countSumWeights()", "Tree reader does not return kEntryValid");
    }
    sumWeights += *(*totalEventsWeighted);
  }
  m_weightsReader->Restart();
  //todo: cross-check the value with Ami, warn if different?

  return sumWeights;
}

std::vector<float> TL::AlgBase::generatorVariedSumWeights() {
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  std::size_t vsize = 0;
  while ( m_weightsReader->Next() ) {
    vsize = (*totalEventsWeighted_mc_generator_weights)->size();
    break;
  }
  std::vector<float> weights(vsize,0.0);
  m_weightsReader->Restart();

  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      TL::Fatal("generatorVariedSumWeights()","Tree reader does not return kEntryValid");
    }
    // now get all the rest
    for ( std::size_t j = 0; j < (*totalEventsWeighted_mc_generator_weights)->size(); ++j ) {
      weights[j] += (*totalEventsWeighted_mc_generator_weights)->at(j);
    }
  }
  m_weightsReader->Restart();

  //todo: cross-check the value with Ami, warn if different?
  return weights;
}

std::vector<std::string> TL::AlgBase::generatorWeightNames() {
  std::vector<std::string> retvec;
  while ( m_weightsReader->Next() ) {
    retvec = *(*names_mc_generator_weights);
    break;
  }
  m_weightsReader->Restart();
  return retvec;
}

unsigned int TL::AlgBase::get_dsid() {
  unsigned int ret_dsid = 999999;
  while ( m_weightsReader->Next() ) {
    ret_dsid = *(*dsid);
    break;
  }
  m_weightsReader->Restart();

  return ret_dsid;
}

TL::STATUS TL::AlgBase::init() {
  init_core_vars();
  return TL::STATUS::Good;
}

TL::STATUS TL::AlgBase::setupOutput() {
  return TL::STATUS::Good;
}

TL::STATUS TL::AlgBase::execute() {
  return TL::STATUS::Good;
}

TL::STATUS TL::AlgBase::finish() {
  return TL::STATUS::Good;
}

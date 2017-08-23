/** @file Algorithm.cxx
 *  @brief TL::Algorithm class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/FileManager.h>

ANA_MSG_SOURCE(msgAlgorithm,"TL::Algorithm")

TL::Algorithm::Algorithm() :
m_datasetName(),
  m_isMC(true),
  m_isNominal(true),
  m_showTTRVwarning(true)
{
  SetName("TopLoop");
  m_totalEntries = 0;
  m_eventCounter = 0;
}

TL::Algorithm::~Algorithm() {}

float TL::Algorithm::countSumWeights() {
  using namespace msgAlgorithm;
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  float sumWeights = 0;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      FATAL("countSumWeights(): Tree reader does not return kEntryValid");
    }
    sumWeights += *(*totalEventsWeighted);
  }
  m_weightsReader->Restart();
  //todo: cross-check the value with Ami, warn if different?

  return sumWeights;
}

std::vector<float> TL::Algorithm::generatorVariedSumWeights() {
  using namespace msgAlgorithm;
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  std::size_t vsize = 0;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    vsize = (*totalEventsWeighted_mc_generator_weights)->size();
    break;
  }
  std::vector<float> weights(vsize,0.0);
  m_weightsReader->Restart();

  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      FATAL("generatorVariedSumWeights(): Tree reader does not return kEntryValid");
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

std::vector<std::string> TL::Algorithm::generatorWeightNames() {
  m_weightsReader->Restart();
  std::vector<std::string> retvec;
  while ( m_weightsReader->Next() ) {
    retvec = *(*names_mc_generator_weights);
    break;
  }
  m_weightsReader->Restart();
  return retvec;
}

unsigned int TL::Algorithm::get_dsid() {
  unsigned int ret_dsid = 999999;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    ret_dsid = *(*dsid);
    break;
  }
  m_weightsReader->Restart();

  return ret_dsid;
}

StatusCode TL::Algorithm::init() {
  using namespace msgAlgorithm;
  ANA_CHECK_SET_TYPE(StatusCode);
  ANA_CHECK(init_core_vars());
  return StatusCode::SUCCESS;
}

StatusCode TL::Algorithm::setupOutput() {
  ANA_CHECK_SET_TYPE(StatusCode);
  return StatusCode::SUCCESS;
}

StatusCode TL::Algorithm::execute() {
  ANA_CHECK_SET_TYPE(StatusCode);
  m_eventCounter++;
  return StatusCode::SUCCESS;
}

StatusCode TL::Algorithm::finish() {
  ANA_CHECK_SET_TYPE(StatusCode);
  return StatusCode::SUCCESS;
}

void TL::Algorithm::progress(int percent_base) const {
  using namespace msgAlgorithm;
  if ( m_totalEntries > percent_base ) {
    int gap = m_totalEntries/percent_base;
    if ( m_eventCounter%gap == 0 ) {
      auto progress = 100.0*m_eventCounter/m_totalEntries;
      ANA_MSG_INFO("Event " << m_eventCounter << ", " << std::round(progress) << "%");
    }
  }
  return;
}
  

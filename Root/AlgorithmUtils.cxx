/** @file AlgorithmUtils.cxx
 *  @brief TL::Algorithm class utility function implementations
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/Algorithm.h>

float TL::Algorithm::countSumWeights() {
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  float sumWeights = 0;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      logger()->critical("countSumWeights(): Tree reader does not return kEntryValid");
    }
    sumWeights += *(*totalEventsWeighted);
  }
  m_weightsReader->Restart();
  //todo: cross-check the value with Ami, warn if different?

  return sumWeights;
}

std::vector<float> TL::Algorithm::generatorVariedSumWeights() {
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
      logger()->critical("generatorVariedSumWeights(): Tree reader does not return kEntryValid");
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

void TL::Algorithm::progress(int n_prints) const {
  if ( m_totalEntries > n_prints ) {
    int gap = m_totalEntries/n_prints;
    if ( m_eventCounter%gap == 0 ) {
      auto progress = 100.0*m_eventCounter/m_totalEntries;
      logger()->info("-- [{:3.0f}%] Event: {}",std::round(progress),m_eventCounter);
    }
  }
  return;
}

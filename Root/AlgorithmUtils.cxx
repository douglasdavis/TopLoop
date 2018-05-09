/** @file AlgorithmUtils.cxx
 *  @brief TL::Algorithm class utility function implementations
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/EDM/FinalState.h>
#include <TopLoop/EDM/Electron.h>
#include <TopLoop/EDM/Muon.h>

// C++
#include <cmath>

// Boost
#include <boost/range/adaptor/indexed.hpp>

float TL::Algorithm::countSumWeights() {
  // sum up the weighted number of events in the metadata tree.  This
  // works for MC (to get the MC lumi) and data (perhaps as a
  // cross-check)
  float sumWeights = 0;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      logger()->error("countSumWeights(): Tree reader does not return kEntryValid");
    }
    sumWeights += totalEventsWeighted();
  }
  m_weightsReader->Restart();
  //todo: cross-check the value with Ami, warn if different?

  logger()->debug("Value of countSumWeights(): {}",sumWeights);
  return sumWeights;
}

std::vector<float> TL::Algorithm::generatorVariedSumWeights() {
  std::size_t vsize = 0;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    vsize = totalEventsWeighted_mc_generator_weights().size();
    break;
  }
  std::vector<float> variedSumWeights;
  variedSumWeights.reserve(vsize);
  m_weightsReader->Restart();

  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      logger()->error("generatorVariedSumWeights(): Tree reader does not return kEntryValid");
    }
    // now get all the rest
    for ( std::size_t j = 0; j < totalEventsWeighted_mc_generator_weights().size(); ++j ) {
      variedSumWeights[j] += totalEventsWeighted_mc_generator_weights().at(j);
    }
  }
  m_weightsReader->Restart();

  //todo: cross-check the value with Ami, warn if different?
  return variedSumWeights;
}

//std::vector<std::string> TL::Algorithm::generatorWeightNames() {
std::map<std::string,std::size_t> TL::Algorithm::generatorVariedWeightNames() {
  m_weightsReader->Restart();
  std::map<std::string,std::size_t> retmap;
  while ( m_weightsReader->Next() ) {
    for ( const auto& nameitr : names_mc_generator_weights() | boost::adaptors::indexed() ) {
      logger()->info("{}\t{}",nameitr.index(),nameitr.value());
      retmap.emplace(nameitr.value(),nameitr.index());
    }
    break;
  }
  m_weightsReader->Restart();
  return retmap;
}

unsigned int TL::Algorithm::get_dsid() {
  unsigned int ret_dsid = 999999;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    ret_dsid = dsid();
    break;
  }
  m_weightsReader->Restart();
  return ret_dsid;
}

bool TL::Algorithm::sampleIsAFII() {
  int branch_value = 0;
  m_weightsReader->Restart();
  while ( m_weightsReader->Next() ) {
    branch_value = isAFII();
    break;
  }
  m_weightsReader->Restart();
  return (branch_value > 0);
}

void TL::Algorithm::printProgress(int n_prints) const {
  if ( m_totalEntries > n_prints ) {
    int gap = m_totalEntries/n_prints;
    if ( m_eventCounter%gap == 0 ) {
      auto progress = std::round(100.0*m_eventCounter/m_totalEntries);
      logger()->info("-- [{:3.0f}%] Event: {}",progress,m_eventCounter);
    }
  }
}

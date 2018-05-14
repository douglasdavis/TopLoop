/** @file AlgorithmUtils.cxx
 *  @brief TL::Algorithm class utility function implementations
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/EDM/Electron.h>
#include <TopLoop/EDM/FinalState.h>
#include <TopLoop/EDM/Muon.h>

// C++
#include <cmath>

// Boost
#include <boost/range/adaptor/indexed.hpp>

float TL::Algorithm::generatorSumWeights() {
  if (std::get<0>(m_weightCache) > 0) {
    return std::get<0>(m_weightCache);
  }
  std::get<0>(m_weightCache) = 0;
  m_weightsReader->Restart();
  while (m_weightsReader->Next()) {
    if (m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid) {
      logger()->error(
        "countSumWeights(): Tree reader does not return kEntryValid");
    }
    std::get<0>(m_weightCache) += totalEventsWeighted();
  }
  m_weightsReader->Restart();

  logger()->debug("Value of countSumWeights(): {}", std::get<0>(m_weightCache));
  return std::get<0>(m_weightCache);
}

const std::vector<float> &TL::Algorithm::generatorVariedSumWeights() {
  if (not std::get<1>(m_weightCache).empty()) {
    return std::get<1>(m_weightCache);
  }
  std::size_t vsize = 0;
  m_weightsReader->Restart();
  while (m_weightsReader->Next()) {
    vsize = totalEventsWeighted_mc_generator_weights().size();
    break;
  }
  std::get<1>(m_weightCache).resize(vsize, 0.0);
  m_weightsReader->Restart();

  while (m_weightsReader->Next()) {
    if (m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid) {
      logger()->error("generatorVariedSumWeights(): Tree reader does not "
                      "return kEntryValid");
    }
    // now get all the rest
    for (std::size_t j = 0; j < vsize; ++j) {
      auto jsum = totalEventsWeighted_mc_generator_weights().at(j);
      std::get<1>(m_weightCache).at(j) += jsum;
    }
  }
  m_weightsReader->Restart();

  // todo: cross-check the value with Ami, warn if different?
  return std::get<1>(m_weightCache);
}

const std::map<std::string, std::size_t> &
TL::Algorithm::generatorVariedWeightsNames() {
  if (not std::get<2>(m_weightCache).empty()) {
    return std::get<2>(m_weightCache);
  }
  m_weightsReader->Restart();
  while (m_weightsReader->Next()) {
    for (const auto &nameitr :
         names_mc_generator_weights() | boost::adaptors::indexed()) {
      std::get<2>(m_weightCache).emplace(nameitr.value(), nameitr.index());
    }
    break;
  }
  m_weightsReader->Restart();
  return std::get<2>(m_weightCache);
}

float TL::Algorithm::sumOfVariation(const std::string &variation_name) {
  auto itr = generatorVariedWeightsNames().find(variation_name);
  if (itr == std::end(generatorVariedWeightsNames())) {
    logger()->error("Cannot find variation named {}, returning nominal!",
                    variation_name);
    return generatorSumWeights();
  }
  return generatorVariedSumWeights().at(itr->second);
}

float TL::Algorithm::currentWeightOfVariation(
  const std::string &variation_name) {
  auto itr = generatorVariedWeightsNames().find(variation_name);
  if (itr == std::end(generatorVariedWeightsNames())) {
    logger()->error("Cannot find variation named {}, returning 0!",
                    variation_name);
    return 0;
  }
  return mc_generator_weights().at(itr->second);
}

unsigned int TL::Algorithm::get_dsid() {
  unsigned int ret_dsid = 999999;
  m_weightsReader->Restart();
  while (m_weightsReader->Next()) {
    ret_dsid = dsid();
    break;
  }
  m_weightsReader->Restart();
  return ret_dsid;
}

bool TL::Algorithm::sampleIsAFII() {
  int branch_value = 0;
  m_weightsReader->Restart();
  while (m_weightsReader->Next()) {
    branch_value = isAFII();
    break;
  }
  m_weightsReader->Restart();
  return (branch_value > 0);
}

void TL::Algorithm::printProgress(int n_prints) const {
  if (m_totalEntries > n_prints) {
    int gap = m_totalEntries / n_prints;
    if (m_eventCounter % gap == 0) {
      auto progress = std::round(100.0 * m_eventCounter / m_totalEntries);
      logger()->info("-- [{:3.0f}%] Event: {}", progress, m_eventCounter);
    }
  }
}

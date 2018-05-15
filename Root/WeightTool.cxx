/** @file WeightTool.cxx
 *  @brief TL::WeightTool class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/WeightTool.h>

// Boost
#include <boost/range/adaptor/indexed.hpp>

TL::WeightTool::WeightTool(TL::Algorithm* algorithm) :
  TL::Loggable("TL::WeightTool"),
  m_algorithm(algorithm)
{}

float TL::WeightTool::generatorSumWeights() {
  if (std::get<0>(m_weightCache) > 0) {
    return std::get<0>(m_weightCache);
  }
  std::get<0>(m_weightCache) = 0;
  alg()->weightsReader()->Restart();
  while (alg()->weightsReader()->Next()) {
    if (alg()->weightsReader()->GetEntryStatus() != TTreeReader::kEntryValid) {
      logger()->error(
        "countSumWeights(): Tree reader does not return kEntryValid");
    }
    std::get<0>(m_weightCache) += alg()->totalEventsWeighted();
  }
  alg()->weightsReader()->Restart();

  logger()->debug("Value of countSumWeights(): {}", std::get<0>(m_weightCache));
  return std::get<0>(m_weightCache);
}

const std::vector<float> &TL::WeightTool::generatorVariedSumWeights() {
  if (not std::get<1>(m_weightCache).empty()) {
    return std::get<1>(m_weightCache);
  }
  std::size_t vsize = 0;
  alg()->weightsReader()->Restart();
  while (alg()->weightsReader()->Next()) {
    vsize = alg()->totalEventsWeighted_mc_generator_weights().size();
    break;
  }
  std::get<1>(m_weightCache).resize(vsize, 0.0);
  alg()->weightsReader()->Restart();

  while (alg()->weightsReader()->Next()) {
    if (alg()->weightsReader()->GetEntryStatus() != TTreeReader::kEntryValid) {
      logger()->error("generatorVariedSumWeights(): Tree reader does not "
                      "return kEntryValid");
    }
    // now get all the rest
    for (std::size_t j = 0; j < vsize; ++j) {
      auto jsum = alg()->totalEventsWeighted_mc_generator_weights().at(j);
      std::get<1>(m_weightCache).at(j) += jsum;
    }
  }
  alg()->weightsReader()->Restart();

  // todo: cross-check the value with Ami, warn if different?
  return std::get<1>(m_weightCache);
}

const std::map<std::string, std::size_t> &
TL::WeightTool::generatorVariedWeightsNames() {
  if (not std::get<2>(m_weightCache).empty()) {
    return std::get<2>(m_weightCache);
  }
  alg()->weightsReader()->Restart();
  while (alg()->weightsReader()->Next()) {
    for (const auto &nameitr :
         alg()->names_mc_generator_weights() | boost::adaptors::indexed()) {
      std::get<2>(m_weightCache).emplace(nameitr.value(), nameitr.index());
    }
    break;
  }
  alg()->weightsReader()->Restart();
  return std::get<2>(m_weightCache);
}

float TL::WeightTool::sumOfVariation(const std::string &variation_name) {
  auto itr = generatorVariedWeightsNames().find(variation_name);
  if (itr == std::end(generatorVariedWeightsNames())) {
    logger()->error("Cannot find variation named {}, returning nominal!",
                    variation_name);
    return generatorSumWeights();
  }
  return generatorVariedSumWeights().at(itr->second);
}

float TL::WeightTool::currentWeightOfVariation(const std::string &variation_name) {
  auto itr = generatorVariedWeightsNames().find(variation_name);
  if (itr == std::end(generatorVariedWeightsNames())) {
    logger()->error("Cannot find variation named {}, returning 0!",
                    variation_name);
    return 0;
  }
  return alg()->mc_generator_weights().at(itr->second);
}

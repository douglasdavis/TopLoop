/** @file WeightTool.cxx
 *  @brief TL::WeightTool class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/WeightTool.h>
#include <TopLoop/Core/SampleMetaSvc.h>

// C++
#include <cmath>

// Boost
#include <boost/range/adaptor/indexed.hpp>

// ATLAS
#include <TopDataPreparation/SampleXsectionSvc.h>
#include <PathResolver/PathResolver.h>

TL::WeightTool::WeightTool(TL::Algorithm* algorithm) :
  TL::Loggable("TL::WeightTool"),
  m_alg(algorithm)
{
  std::string XsecFile = PathResolverFindCalibFile
    ("dev/AnalysisTop/TopDataPreparation/XSection-MC15-13TeV.data");
  m_xsec = SampleXsectionSvc::svc(XsecFile)->sampleXsection();
}

float TL::WeightTool::generatorSumWeights() {
  if (std::get<0>(m_weightCache) > 0) {
    return std::get<0>(m_weightCache);
  }
  std::get<0>(m_weightCache) = 0;
  m_alg->weightsReader()->Restart();
  while (m_alg->weightsReader()->Next()) {
    if (m_alg->weightsReader()->GetEntryStatus() != TTreeReader::kEntryValid) {
      logger()->error(
        "countSumWeights(): Tree reader does not return kEntryValid");
    }
    std::get<0>(m_weightCache) += m_alg->totalEventsWeighted();
  }
  m_alg->weightsReader()->Restart();

  logger()->debug("Value of countSumWeights(): {}", std::get<0>(m_weightCache));
  return std::get<0>(m_weightCache);
}

const std::vector<float> &TL::WeightTool::generatorVariedSumWeights() {
  if (not std::get<1>(m_weightCache).empty()) {
    return std::get<1>(m_weightCache);
  }
  std::size_t vsize = 0;
  m_alg->weightsReader()->Restart();
  while (m_alg->weightsReader()->Next()) {
    vsize = m_alg->totalEventsWeighted_mc_generator_weights().size();
    break;
  }
  std::get<1>(m_weightCache).resize(vsize, 0.0);
  m_alg->weightsReader()->Restart();

  while (m_alg->weightsReader()->Next()) {
    if (m_alg->weightsReader()->GetEntryStatus() != TTreeReader::kEntryValid) {
      logger()->error("generatorVariedSumWeights(): Tree reader does not "
                      "return kEntryValid");
    }
    // now get all the rest
    for (std::size_t j = 0; j < vsize; ++j) {
      auto jsum = m_alg->totalEventsWeighted_mc_generator_weights().at(j);
      std::get<1>(m_weightCache).at(j) += jsum;
    }
  }
  m_alg->weightsReader()->Restart();

  // todo: cross-check the value with Ami, warn if different?
  return std::get<1>(m_weightCache);
}

const std::map<std::string, std::size_t> &
TL::WeightTool::generatorVariedWeightsNames() {
  if (not std::get<2>(m_weightCache).empty()) {
    return std::get<2>(m_weightCache);
  }
  m_alg->weightsReader()->Restart();
  while (m_alg->weightsReader()->Next()) {
    for (const auto &nameitr :
         m_alg->names_mc_generator_weights() | boost::adaptors::indexed()) {
      std::get<2>(m_weightCache).emplace(nameitr.value(), nameitr.index());
    }
    break;
  }
  m_alg->weightsReader()->Restart();
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
  return m_alg->mc_generator_weights().at(itr->second);
}

float TL::WeightTool::currentPDF4LHCsumQuadVariations() {
  float sumSq = 0.0;
  float w_c = currentWeightOfVariation("PDFset=90900");
  float n_c = sumOfVariation("PDFset=90900");
  for ( unsigned int i = 90901; i <= 90930; ++i ) {
    std::string vname = "PDFset="+std::to_string(i);
    float w_i  = currentWeightOfVariation(vname);
    float n_i  = sumOfVariation(vname);
    float term = (w_c*n_i - w_i*n_c)/n_i;
    sumSq += term*term;
  }
  return (1.0/n_c)*std::sqrt(sumSq);
}

float TL::WeightTool::sampleCrossSection() const {
  auto dsid = m_alg->get_dsid();
  auto xsec = m_xsec->getXsection(dsid);
  logger()->debug("Retreiving cross section for sample {}: {} pb",dsid,xsec);
  return xsec;
}

float TL::WeightTool::luminosityWeight(const std::vector<TL::kCampaign>& campaigns,
                                       const float lumi) {
  auto xs    = sampleCrossSection();
  auto sumW  = generatorSumWeights();
  auto campW = TL::SampleMetaSvc::get().getCampaignWeight(m_alg->fileManager()->rucioDir(),
                                                          campaigns);
  float finalW = (xs * lumi / sumW) * campW;
  logger()->debug("Retreiving luminosity weight (for 1/fb): {}",finalW);
  return finalW;
}

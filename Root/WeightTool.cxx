
/** @file WeightTool.cxx
 *  @brief TL::WeightTool class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/SampleMetaSvc.h>
#include <TopLoop/Core/WeightTool.h>

// C++
#include <cmath>

// ATLAS
#include <PathResolver/PathResolver.h>
#include <TopDataPreparation/SampleXsectionSvc.h>

TL::WeightTool::WeightTool(TL::Algorithm* algorithm)
    : TL::Loggable("TL::WeightTool"), m_alg(algorithm) {
  PathResolverSetOutputLevel(5);
  std::string xsFile = PathResolverFindCalibFile(
      "dev/AnalysisTop/TopDataPreparation/XSection-MC15-13TeV.data");
  m_xsec = SampleXsectionSvc::svc(xsFile)->sampleXsection();
  logger()->info("Cross section file: {}", xsFile);
}

float TL::WeightTool::generatorSumWeights() {
  if (m_generatorSumWeights > 0) {
    return m_generatorSumWeights;
  }
  m_generatorSumWeights = 0;
  m_alg->weightsReader()->Restart();
  while (m_alg->weightsReader()->Next()) {
    if (m_alg->weightsReader()->GetEntryStatus() != TTreeReader::kEntryValid) {
      logger()->error("countSumWeights(): Tree reader does not return kEntryValid");
    }
    m_generatorSumWeights += m_alg->totalEventsWeighted();
  }
  m_alg->weightsReader()->Restart();

  logger()->debug("Value of countSumWeights(): {}", m_generatorSumWeights);
  return m_generatorSumWeights;
}

const std::vector<float>& TL::WeightTool::generatorVariedSumWeights() {
  if (!m_generatorVariedSumWeights.empty()) {
    return m_generatorVariedSumWeights;
  }
  std::size_t vsize = 0;
  m_alg->weightsReader()->Restart();
  while (m_alg->weightsReader()->Next()) {
    vsize = m_alg->totalEventsWeighted_mc_generator_weights().size();
    break;
  }
  m_generatorVariedSumWeights.resize(vsize, 0.0);
  m_alg->weightsReader()->Restart();

  while (m_alg->weightsReader()->Next()) {
    if (m_alg->weightsReader()->GetEntryStatus() != TTreeReader::kEntryValid) {
      logger()->error(
          "generatorVariedSumWeights(): Tree reader does not "
          "return kEntryValid");
    }
    // now get all the rest
    for (std::size_t j = 0; j < vsize; ++j) {
      auto jsum = m_alg->totalEventsWeighted_mc_generator_weights().at(j);
      m_generatorVariedSumWeights.at(j) += jsum;
    }
  }
  m_alg->weightsReader()->Restart();

  return m_generatorVariedSumWeights;
}

const std::map<std::string, std::size_t>& TL::WeightTool::generatorVariedWeightsNames() {
  if (!m_generatorVariedWeightsNames.empty()) {
    return m_generatorVariedWeightsNames;
  }
  m_alg->weightsReader()->Restart();
  while (m_alg->weightsReader()->Next()) {
    std::size_t name_counter = 0;
    for (const std::string& name : m_alg->names_mc_generator_weights()) {
      m_generatorVariedWeightsNames.emplace(name, name_counter++);
    }
    break;
  }
  m_alg->weightsReader()->Restart();
  return m_generatorVariedWeightsNames;
}

float TL::WeightTool::sumOfVariation(const std::string& variation_name) {
  auto itr = generatorVariedWeightsNames().find(variation_name);
  if (itr == std::end(generatorVariedWeightsNames())) {
    logger()->error("Cannot find variation named {}, returning nominal!", variation_name);
    return generatorSumWeights();
  }
  return generatorVariedSumWeights().at(itr->second);
}

float TL::WeightTool::currentWeightOfVariation(const std::string& variation_name) {
  auto itr = generatorVariedWeightsNames().find(variation_name);
  if (itr == std::end(generatorVariedWeightsNames())) {
    logger()->error("Cannot find variation named {}, returning 0!", variation_name);
    return 0;
  }
  return m_alg->mc_generator_weights().at(itr->second);
}

std::pair<float, float> TL::WeightTool::currentPDF4LHCsumQuadVariations() {
  float sumSq = 0.0;
  float w_c = currentWeightOfVariation("PDFset=90900");
  float n_c = sumOfVariation("PDFset=90900");
  for (unsigned int i = 90901; i <= 90930; ++i) {
    std::string vname = "PDFset=" + std::to_string(i);
    float w_i = currentWeightOfVariation(vname);
    float n_i = sumOfVariation(vname);
    float term = (w_c * n_i - w_i * n_c) / n_i;
    sumSq += term * term;
  }
  float final_val = (1.0 / n_c) * std::sqrt(sumSq);
  float percent = final_val / w_c * n_c * 100.0;
  return std::make_pair(final_val, percent);
}

float TL::WeightTool::sampleCrossSection() const {
  auto dsid = m_alg->fileManager()->dsid();
  auto xsec = m_xsec->getXsection(dsid);
  logger()->debug("Retreiving cross section for sample {}: {} pb", dsid, xsec);
  return xsec;
}

float TL::WeightTool::sampleRawCrossSection() const {
  auto dsid = m_alg->fileManager()->dsid();
  auto rxsec = m_xsec->getRawXsection(dsid);
  logger()->debug("Retrieving raw cross section for sample {}: {} pb", dsid, rxsec);
  return rxsec;
}

float TL::WeightTool::sampleKfactor() const {
  auto dsid = m_alg->fileManager()->dsid();
  auto kf = m_xsec->getKfactor(dsid);
  logger()->debug("Retrieving k-factor for sample {}: {}", dsid, kf);
  return kf;
}

float TL::WeightTool::luminosityWeight(const std::vector<TL::kCampaign>& campaigns,
                                       const float lumi) {
  auto xs = sampleCrossSection();
  auto sumW = generatorSumWeights();
  auto campW = TL::SampleMetaSvc::get().getCampaignWeight(m_alg->fileManager()->rucioDir(),
                                                          campaigns);
  float finalW = (xs * lumi / sumW) * campW;
  logger()->debug("Retreiving luminosity weight (for 1/fb): {}", finalW);
  return finalW;
}

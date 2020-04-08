/*! @file WeightTool.cxx
 *  @brief TL::WeightTool class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/SampleMetaSvc.h>
#include <TopLoop/Core/WeightTool.h>

// ATLAS
#include <PathResolver/PathResolver.h>
#include <TopDataPreparation/SampleXsectionSvc.h>

// C++
#include <cmath>

TL::WeightTool::WeightTool(TL::Algorithm* algorithm)
    : TL::Loggable("TL::WeightTool"), m_alg(algorithm) {
  PathResolverSetOutputLevel(5);
  std::string xsFile = PathResolverFindCalibFile(
      "dev/AnalysisTop/TopDataPreparation/XSection-MC15-13TeV.data");
  m_xsec = SampleXsectionSvc::svc(xsFile)->sampleXsection();
  logger()->info("Cross section file: {}", xsFile);
}

void TL::WeightTool::initialize() {
  TL_CHECK(determineScheme());
  TL_CHECK(determine_muRmuF_names());
}

TL::StatusCode TL::WeightTool::determineScheme() {
  logger()->info("Determining auxiliary weights scheme");
  std::vector<std::string> names;
  for (const auto& entry : generatorVariedWeightsNames()) {
    names.push_back(entry.first);
  }
  auto vec_contains = [](const std::vector<std::string>& v, const std::string& s) {
    if (std::find(std::begin(v), std::end(v), s) != std::end(v)) {
      return true;
    }
    return false;
  };
  if (vec_contains(names, "muR=20,muF=10")) {
    m_scheme = TL::AuxWeightScheme::ttbar_v29;
    logger()->info("using AuxWeightScheme ttbar_v29");
  }
  else if (vec_contains(names, "muR=200,muF=200")) {
    m_scheme = TL::AuxWeightScheme::tW_v29;
    logger()->info("using AuxWeightScheme tW_v29");
  }
  else if (vec_contains(names, " muR = 1.0, muF = 2.0 ")) {
    m_scheme = TL::AuxWeightScheme::ttbar_v30;
    logger()->info("using AuxWeightScheme ttbar_v30");
  }
  else if (vec_contains(names, " muR = 1.00, muF = 2.00 ")) {
    m_scheme = TL::AuxWeightScheme::tW_v30;
    logger()->info("using AuxWeightScheme tW_v30");
  }
  else {
    logger()->warn("Scheme undetermined");
    m_scheme = TL::AuxWeightScheme::unknown;
    return TL::StatusCode::SUCCESS;
  }
  if (m_scheme == TL::AuxWeightScheme::ttbar_v29 || m_scheme == TL::AuxWeightScheme::tW_v29) {
    bool has_pdf = vec_contains(names, "PDFset=90900");
    if (!has_pdf) {
      logger()->critical("AuxWeightScheme logic failed");
      return TL::StatusCode::FAILURE;
    }
  }
  else if (m_scheme == TL::AuxWeightScheme::ttbar_v30 ||
           m_scheme == TL::AuxWeightScheme::tW_v30) {
    bool has_pdf = vec_contains(names, " PDF set = 90900 ");
    if (!has_pdf) {
      logger()->critical("AuxWeightScheme logic failed");
      return TL::StatusCode::FAILURE;
    }
  }
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::WeightTool::determine_muRmuF_names() {
  m_name_Var3cUp = "Var3cUp";
  m_name_Var3cDown = "Var3cDown";
  if (m_scheme == TL::AuxWeightScheme::tW_v29) {
    m_name_scale_muR_20 = "muR=200,muF=100";
    m_name_scale_muR_05 = "muR=050,muF=100";
    m_name_scale_muF_20 = "muR=100,muF=200";
    m_name_scale_muF_05 = "muR=100,muF=050";
    m_name_fsr_muR_20 = "isr:muRfac=10_fsr:muRfac=20";
    m_name_fsr_muR_05 = "isr:muRfac=10_fsr:muRfac=05";
  }
  else if (m_scheme == TL::AuxWeightScheme::ttbar_v29) {
    m_name_scale_muR_20 = "muR=20,muF=10";
    m_name_scale_muR_05 = "muR=05,muF=10";
    m_name_scale_muF_20 = "muR=10,muF=20";
    m_name_scale_muF_05 = "muR=10,muF=05";
    m_name_fsr_muR_20 = "isr:muRfac=10_fsr:muRfac=20";
    m_name_fsr_muR_05 = "isr:muRfac=10_fsr:muRfac=05";
  }
  else if (m_scheme == TL::AuxWeightScheme::tW_v30) {
    m_name_scale_muR_20 = " muR = 2.00, muF = 1.00 ";
    m_name_scale_muR_05 = " muR = 0.50, muF = 1.00 ";
    m_name_scale_muF_20 = " muR = 1.00, muF = 2.00 ";
    m_name_scale_muF_05 = " muR = 1.00, muF = 0.50 ";
    m_name_fsr_muR_20 = "isr:muRfac=1.0_fsr:muRfac=2.0";
    m_name_fsr_muR_05 = "isr:muRfac=1.0_fsr:muRfac=0.5";
  }
  else if (m_scheme == TL::AuxWeightScheme::ttbar_v30) {
    m_name_scale_muR_20 = " muR = 2.0, muF = 1.0 ";
    m_name_scale_muR_05 = " muR = 0.5, muF = 1.0 ";
    m_name_scale_muF_20 = " muR = 1.0, muF = 2.0 ";
    m_name_scale_muF_05 = " muR = 1.0, muF = 0.5 ";
    m_name_fsr_muR_20 = "isr:muRfac=1.0_fsr:muRfac=2.0";
    m_name_fsr_muR_05 = "isr:muRfac=1.0_fsr:muRfac=0.5";
  }

  m_idx_Var3cUp = getIndexOfVariation(m_name_Var3cUp);
  m_idx_Var3cDown = getIndexOfVariation(m_name_Var3cDown);
  m_idx_scale_muR_20 = getIndexOfVariation(m_name_scale_muR_20);
  m_idx_scale_muR_05 = getIndexOfVariation(m_name_scale_muR_05);
  m_idx_scale_muF_20 = getIndexOfVariation(m_name_scale_muF_20);
  m_idx_scale_muF_05 = getIndexOfVariation(m_name_scale_muF_05);
  m_idx_fsr_muR_20 = getIndexOfVariation(m_name_fsr_muR_20);
  m_idx_fsr_muR_05 = getIndexOfVariation(m_name_fsr_muR_05);

  return TL::StatusCode::SUCCESS;
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

std::size_t TL::WeightTool::getIndexOfVariation(const std::string& variation_name) {
  auto itr = generatorVariedWeightsNames().find(variation_name);
  if (itr == std::end(generatorVariedWeightsNames())) {
    logger()->error("Cannot find variation named {}", variation_name);
    return 0;
  }
  return itr->second;
}

float TL::WeightTool::sumOfVariation(const std::string& variation_name) {
  auto idx = getIndexOfVariation(variation_name);
  return sumOfVariation(idx);
}

float TL::WeightTool::sumOfVariation(const std::size_t idx) {
  return generatorVariedSumWeights()[idx];
}

float TL::WeightTool::currentWeightOfVariation(const std::string& variation_name) {
  auto idx = getIndexOfVariation(variation_name);
  return currentWeightOfVariation(idx);
}

float TL::WeightTool::currentWeightOfVariation(const std::size_t idx) {
  return m_alg->mc_generator_weights()[idx];
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

const std::array<std::string, 31>& TL::WeightTool::PDFWeightNames() {
  if (!m_PDFWeightNames[0].empty()) {
    return m_PDFWeightNames;
  }

  // no spaces in the name
  if (m_scheme == TL::AuxWeightScheme::tW_v29 || m_scheme == TL::AuxWeightScheme::ttbar_v29) {
    for (int i = 0; i < 10; ++i) {
      m_PDFWeightNames[i] = fmt::format("PDFset=9090{}", i);
    }
    for (int i = 10; i < 31; ++i) {
      m_PDFWeightNames[i] = fmt::format("PDFset=909{}", i);
    }
  }

  // spaces in the name
  if (m_scheme == TL::AuxWeightScheme::tW_v30 || m_scheme == TL::AuxWeightScheme::ttbar_v30) {
    for (int i = 0; i < 10; ++i) {
      m_PDFWeightNames[i] = fmt::format(" PDF set = 9090{} ", i);
    }
    for (int i = 10; i < 31; ++i) {
      m_PDFWeightNames[i] = fmt::format(" PDF set = 909{} ", i);
    }
  }

  return m_PDFWeightNames;
}

/*! @file  WeightTool.h
 *  @brief TL::WeightTool class header
 *  @class TL::WeightTool
 *  @brief A class to handle weight access
 *
 *  This class is intimately connects to the TL::Algorithm class and
 *  uses its features to provide a somewhat isolated object to cache
 *  and retrieve sample information for calculating various weights.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_WeightTool_h
#define TL_WeightTool_h

#include <TopLoop/Core/Loggable.h>
#include <TopLoop/Core/Utils.h>

#include <map>
#include <memory>
#include <tuple>
#include <vector>

class SampleXsection;

namespace TL {
class Algorithm;
enum class kCampaign;
}  // namespace TL

namespace TL {

enum class AuxWeightScheme { ttbar_v1, ttbar_v2, tW_v1, tW_v2, unknown };

class WeightTool : public TL::Loggable {
 private:
  TL::Algorithm* m_alg;
  const SampleXsection* m_xsec;

  AuxWeightScheme m_scheme{AuxWeightScheme::unknown};
  std::array<std::string, 31> m_PDFWeightNames{};
  float m_generatorSumWeights{-1};
  std::vector<float> m_generatorVariedSumWeights{};
  std::map<std::string, std::size_t> m_generatorVariedWeightsNames{};

  TL::StatusCode determine_muRmuF_names();
  TL::StatusCode determineScheme();

  std::string m_name_scale_muR_20;
  std::string m_name_scale_muR_05;
  std::string m_name_scale_muF_20;
  std::string m_name_scale_muF_05;
  std::string m_name_Var3cUp;
  std::string m_name_Var3cDown;
  std::string m_name_fsr_muR_20;
  std::string m_name_fsr_muR_05;
  std::size_t m_idx_scale_muR_20;
  std::size_t m_idx_scale_muR_05;
  std::size_t m_idx_scale_muF_20;
  std::size_t m_idx_scale_muF_05;
  std::size_t m_idx_Var3cUp;
  std::size_t m_idx_Var3cDown;
  std::size_t m_idx_fsr_muR_20;
  std::size_t m_idx_fsr_muR_05;

  bool m_initialized{false};

 public:

  /// @name Constructors
  /// @{

  /// Only usable constructor - must include algorithm pointer
  explicit WeightTool(TL::Algorithm* algorithm);

  WeightTool() = delete;
  WeightTool(const WeightTool&) = delete;
  WeightTool(WeightTool&&) = delete;
  WeightTool& operator=(const WeightTool&) = delete;
  WeightTool& operator=(WeightTool&&) = delete;

  /// @}

  /// @name required for use
  /// @{

  /// must be called before any use
  /*!
   *  The tool does some parsing of the sumWeights tree to determine
   *  how it should operate. This function executes that logic to
   *  ensure the right weight names and used.
   */
  void initialize();

  /// @}

  /// @name generic API
  /// @{

  /// Count the sumWeights from all input trees
  /*!
   *  This can be called in the init() function if info about the sum
   *  of weights is desired; is the nominal sum of weights
   */
  float generatorSumWeights();

  /// Generator based changes in the sum of weights
  /*!
   *  Generator "on the fly" weight variations stored in a vector
   *  The first entry (as of May 2018) is the same as nominal.
   *
   *  This vector of sums of weights is required for properly
   *  normalizing events when using alternative weights from the
   *  generator. Recommended to be used with
   *  `generatorVariedWeightsNames()` See the documentation of that
   *  function for more.
   */
  const std::vector<float>& generatorVariedSumWeights();

  /// Get names of Generator based weights
  /*!
   *  Can be called to retrieve the list of strings corresponding to
   *  the name of the generator based weights. The key in the map is
   *  the index in `mc_generator_weights` vector branch as well as the
   *  vector returned by `generatorVariedSumWeights()`. These can be
   *  used to find the normalization associated with a specific
   *  generator's varied weight... example usage in a TL::Algorithm
   *  based class:
   *
   *  @code{.cpp}
   *  auto genWeightNames = weightTool().generatorVariedSumWeightNames();
   *  std::size_t foo_idx = genWeightNames.at("foo");
   *
   *  //// .....
   *
   *  @endcode
   *  @code{.cpp}
   *  float weight_foo = mc_generator_weights().at(foo_idx);
   *  float sumWeights_foo = weightTool().generatorVariedSumWeights().at(foo_idx);
   *  @endcode
   *
   *  To get the event weight for "foo".. if you haven't
   *  already multiplied by the nominal sum of weights...
   *
   *  @code{.cpp}
   *  float event_weight_foo = (xsec / sumWeights_foo) * weight_foo * other_weights;
   *  @endcode
   *
   *  or if you already have a "nominal" luminosity weight which
   *  took into account the nominal sum weights, just multiply it
   *  out...
   *
   *  @code{.cpp}
   *  float event_weight_foo = lumiWeight * (sumWeights_nominal / sumWeights_foo) *
   *                           weight_foo * other_weights;
   *
   *  @endcode
   *
   *  See also the convenience functions `sumOfVariation()` and
   *  `currentWeightOfVariation()`.
   *
   */
  const std::map<std::string, std::size_t>& generatorVariedWeightsNames();

  /// Get the sum of weights required to normalize the given variation
  /*!
   *  Convienence function to grab the sum of weights required to
   *  given variation. See `currentWeightOfVariation()` for more.
   *
   *  @param variation_name the name of the generator variation
   */
  float sumOfVariation(const std::string& variation_name);

  /// Get the sum of weights required to normalize a variation based on the index
  /*!
   *  This is similar to the string overload of the same function but
   *  expects the user to have manually determined the necessary index
   *  ahead of calling.
   *
   *  @param idx the index in the mc_generator_weights vector
   */
  float sumOfVariation(const std::size_t idx);

  /// Get the weight of the generator variation for the event (use in execute())
  /*!
   *  This function is for convenience. It may not be the most
   *  performant way to calculate the generator varied weights!  See
   *  `generatorVariedWeightsNames()` and
   *  `generatorVariedSumWeights()` to think about designing a more
   *  performant access method. Which doesn't require searching a map
   *  on each event...
   *
   *  Example:
   *
   *  @code{.cpp}
   *  TL::StatusCode MyAlgorithm::execute() {
   *    // ...
   *    float nom_weight = xsec * (weight_mc() / weightTool().generatorSumWeight()) *
   *                       other_weights;
   *
   *    float foo_weight = xsec * (weightTool().currentWeightOfVariation("foo") /
   *                       weightTool().sumOfVariation("foo")) * other_weights;
   *    // ...
   *  }
   *  @endcode
   *
   *  @param variation_name the name of the generator variation
   */
  float currentWeightOfVariation(const std::string& variation_name);

  /// get the weight of the generator variation for the  current event based on index
  /*!
   *  This is similar to the string overload of the same function but
   *  expects the user to have manually determined the necessary index
   *  ahead of calling.
   *
   *  @param idx the vector
   */
  float currentWeightOfVariation(const std::size_t idx);

  /// get the index (in the mc_generator_weight_names vector) of the variation name
  /*!
   *  if one knows the index of a variation, one can access the
   *  entries in the mc_generator_weight_names branch via direct index
   *  access instead of figuring it out from the map, this can provide
   *  some performance improvements over doing the lookup in the map.
   *
   *  @param variation_name the name of the generator variation
   */
  std::size_t getIndexOfVariation(const std::string& variation_name);

  /// @}

  /// @name cross section helpers
  /// @{

  /// get the cross section of the sample the algorithm is processing
  /*!
   *  This function uses the TopDataPreparation SampleXsection class
   *  to retrieve the cross section for the DSID.
   */
  float sampleCrossSection() const;

  /// Raw cross section from TopDataPreparation
  float sampleRawCrossSection() const;

  /// k-factor from TopDataPreparation
  float sampleKfactor() const;

  /// get a nominal luminosity weight associated with the sample
  /*!
   *  This function uses the cross section and sum of weights to
   *  calculate nominal luminosity weight. We scale such that the
   *  integrated luminosity of the dataset will be in inverse
   *  femtobarns.
   *
   *  \f[
   *     w_{\mathcal{L}} = \frac{\sigma\mathcal{L}}{N_w}\times w_c
   *  \f]
   *
   *  where \f$\sigma\f$ is the cross section in picobarns,
   *  \f$\mathcal{L} = \f$ your input (default 1000.0 pb), \f$N_w\f$
   *  is the total number of weights before cuts, and \f$w_c\f$ is
   *  the campaign weight determined from the campaigns
   *  argument. See the TL::SampleMetaSvc::getCampaignWeight()
   *  documentation for more information about the campaign weight.
   *
   *  @param campaigns the list of campaigns the output is meant to
   *  be used with.
   *  @param lumi the integrated luminosity (in pb) to generate the
   *  weight.
   */
  float luminosityWeight(const std::vector<TL::kCampaign>& campaigns,
                         const float lumi = 1000.0);

  /// retreive the TopDataPreparation provided cross section class
  const SampleXsection* sampleXsection() const { return m_xsec; }

  /// @}

  /// @name specific string and index weight getters
  /// @{

  /// retrieve the list of PDF weight names
  /*!
   *  Unfortunately these are sample dependent, so we determine at the
   *  beginning of run time what they are and store them in a vector
   *  for later retrieval.
   */
  const std::array<std::string, 31>& PDFWeightNames();

  /// the name for the muR scale variation to 2.0
  const std::string& name_scale_muR_20() const { return m_name_scale_muR_20; }
  /// the name for the muR scale variation to 0.5
  const std::string& name_scale_muR_05() const { return m_name_scale_muR_05; }
  /// the name for the muF scale variation to 2.0
  const std::string& name_scale_muF_20() const { return m_name_scale_muF_20; }
  /// the name for the muF scale variation to 0.5
  const std::string& name_scale_muF_05() const { return m_name_scale_muF_05; }
  /// the name for the Var3cUp ISR variation
  const std::string& name_Var3cUp() const { return m_name_Var3cUp; }
  /// the name for the Var3cDown ISR variation
  const std::string& name_Var3cDown() const { return m_name_Var3cDown; }
  /// the name for the FSR muR variation to 2.0
  const std::string& name_fsr_muR_20() const { return m_name_fsr_muR_20; }
  /// the name for the FSR muR variation to 0.5
  const std::string& name_fsr_muR_05() const { return m_name_fsr_muR_05; }

  /// the index for the muR scale variation to 2.0
  std::size_t idx_scale_muR_20() const { return m_idx_scale_muR_20; }
  /// the index for the muR scale variation to 0.5
  std::size_t idx_scale_muR_05() const { return m_idx_scale_muR_05; }
  /// the index for the muF scale variation to 2.0
  std::size_t idx_scale_muF_20() const { return m_idx_scale_muF_20; }
  /// the index for the muF scale variation to 0.5
  std::size_t idx_scale_muF_05() const { return m_idx_scale_muF_05; }
  /// the index for the Var3cUp ISR variation
  std::size_t idx_Var3cUp() const { return m_idx_Var3cUp; }
  /// the index for the Var3cDown ISR variation
  std::size_t idx_Var3cDown() const { return m_idx_Var3cDown; }
  /// the index for the FSR muR variation to 2.0
  std::size_t idx_fsr_muR_20() const { return m_idx_fsr_muR_20; }
  /// the index for the FSR muR variation to 0.5
  std::size_t idx_fsr_muR_05() const { return m_idx_fsr_muR_05; }

  /// @}
};
}  // namespace TL

#endif

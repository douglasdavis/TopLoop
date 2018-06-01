/** @file  WeightTool.h
 *  @brief TL::WeightTool class header
 *  @class TL::WeightTool
 *  @brief A class to handle weight access
 *
 *  This class is intimately connected to the TL::Algorithm class and
 *  uses its features to provide a somewhat isolated object to cache
 *  and retrieve sample information for calculating various weights.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */


#ifndef TL_WeightTool_h
#define TL_WeightTool_h

#include <memory>
#include <vector>
#include <TopLoop/Core/Loggable.h>

class SampleXsection;

namespace TL {
  class Algorithm;
  enum class kCampaign;
}

namespace TL {
  class WeightTool : public TL::Loggable {
  private:
    TL::Algorithm* m_alg;

    std::tuple<float,
               std::vector<float>,
               std::map<std::string,std::size_t>>
    m_weightCache{-1,{},{}};

    const SampleXsection* m_xsec;

  public:
    WeightTool() = delete;//fault;
    virtual ~WeightTool() = default;

    WeightTool(const WeightTool&) = delete;
    WeightTool(WeightTool&&) = delete;
    WeightTool& operator=(const WeightTool&) = delete;
    WeightTool& operator=(WeightTool&&) = delete;

    /// Only constructor - must include algorithm pointer
    WeightTool(TL::Algorithm* algorithm);

    /// Count the sumWeights from all input trees
    /**
     *  This can be called in the init() function if info about the
     *  sum of weights is desired; s the nominal sum of weights
     */
    float generatorSumWeights();

    /// Generator based changes in the sum of weights
    /**
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
    /**
     *  Can be called to retrieve the list of strings corresponding to
     *  the name of the generator based weights. The key in the map is
     *  the index in `mc_generator_weights` vector branch as well as
     *  the vector returned by `generatorVariedSumWeights()`. These
     *  can be used to find the normalization associated with a
     *  specific generator's varied weight... example usage in a
     *  TL::Algorithm based class:
     *
     *  @code{.cpp}
     *  auto genWeightNames = weightTool().generatorVariedSumWeightNames();
     *  std::size_t foo_idx = genWeightNames.at("foo");
     *
     *  //.....
     *
     *  float weight_foo     = mc_generator_weights().at(foo_idx);
     *  float sumWeights_foo = weightTool().generatorVariedSumWeights().at(foo_idx);
     *
     *  // To get the event weight for "foo".. if you haven't
     *  // already multiplied by the nominal sum of weights...
     *  float event_weight_foo = (xsec / sumWeights_foo) *
     *    weight_foo * other_weights;
     *
     *  // or if you already have a "nominal" luminosity weight which
     *  // took into account the nominal sum weights, just multiply it
     *  // out...
     *  float event_weight_foo = lumiWeight *
     *    (sumWeights_nominal / sumWeights_foo) *
     *    weight_foo * other_weights;
     *
     *  @endcode
     *
     *  See also the convenience functions `sumOfVariation()` and
     *  `currentWeightOfVariation()`.
     *
     */
    const std::map<std::string,std::size_t>& generatorVariedWeightsNames();

    /// Get the sum of weights required to normalize the given variation
    /**
     *  Convienence function to grab the sum of weights required to
     *  given variation. See `currentWeightOfVariation()` for more.
     *
     *  @param variation_name the name of the generator variation
     */
    float sumOfVariation(const std::string& variation_name);

    /// Get the weight of the generator variation for the event (use in execute())
    /**
     *  This function is for convenience. It may not be the most
     *  performant way to calculate the generator varied weights!  See
     *  `generatorVariedWeightsNames()` and
     *  `generatorVariedSumWeights()` to think about designing a more
     *  performant access method. Which doesn't require searching a
     *  map on each event...
     *
     *  Example:
     *
     *  @code{.cpp}
     *  TL::StatusCode MyAlgorithm::execute() {
     *    // ...
     *    auto nom_weight = xsec *
     *      (weight_mc() / weightTool().generatorSumWeight()) *
     *      (other_weights;
     *    auto foo_weight = xsec *
     *      (weightTool().currentWeightOfVariation("foo") / weightTool().sumOfVariation("foo")) *
     *      other_weights;
     *    // ...
     *  }
     *  @endcode
     *
     *  @param variation_name the name of the generator variation
     */
    float currentWeightOfVariation(const std::string& variation_name);

    /// generate "sum in quadrature" of the PDF4LHC variations (use in execute()).
    /**
     *  This function will return the following calculation:
     *
     *  \f[
     *     w_{\mathrm{PDF4LHC}} =
     *       \sqrt{ \sum_{i=0}^{30} \left(w_c - w_i\right)^2}
     *  \f]
     *
     *  where \f$w_c\f$ is the central value for PDF4LHC treatment
     *  ("PDFset=90900") and \f$\{w_i\}\f$ are the 30 variations
     *  ("PDFset=909{01-30}"). This function also takes into account
     *  the "sum of weights" associated with each variation such that
     *  the normalization is treated correctly.
     */
    float currentPDF4LHCsumQuadVariations();

    /// get the cross section of the sample the algorithm is processing
    /**
     *  This function uses the TopDataPreparation SampleXsection class
     *  to retrieve the cross section for the DSID.
     */
    float sampleCrossSection() const;

    /// get a luminosity weight associated with the sample
    /**
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
     *  the campaign weight determined from the campaigns argument.
     *
     *  @param campaigns the list of campaigns the output is meant to
     *  be used with.
     *  @param lumi the integrated luminosity (in pb) to generate the
     *  weight.
     */
    float luminosityWeight(const std::vector<TL::kCampaign>& campaigns,
                           const float lumi = 1000.0);

  };
}

#endif

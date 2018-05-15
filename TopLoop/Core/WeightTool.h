/** @file  WeightTool.h
 *  @brief TL::WeightTool class header
 *  @class TL::WeightTool
 *  @brief A class to handle weight access
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */


#ifndef TL_WeightTool_h
#define TL_WeightTool_h

#include <memory>
#include <TopLoop/Core/Loggable.h>

namespace TL {
  class Algorithm;
}

namespace TL {
  class WeightTool : public TL::Loggable {
  private:
    TL::Algorithm* m_algorithm;

    std::tuple<float,std::vector<float>,
               std::map<std::string,std::size_t>>
    m_weightCache{-1,{},{}};

    inline TL::Algorithm* alg() { return m_algorithm; }

  public:
    WeightTool() = default;
    virtual ~WeightTool() = default;

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
     *  The first entry (as of April 2017) is the same as nominal.
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
     *  specific generator's varied weight... example usage:
     *
     *  @code{.cpp}
     *  auto genWeightNames = generatorVariedSumWeightNames();
     *  std::size_t foo_idx = genWeightNames.at("foo");
     *
     *  //.....
     *
     *  float weight_foo     = mc_generator_weights().at(foo_idx);
     *  float sumWeights_foo = generatorVariedSumWeights().at(foo_idx);
     *
     *  // To get the event weight for "foo".. if you haven't
     *  // already multiplied by the nominal sum of weights...
     *  float event_weight_foo = (xsec / sumWeights_foo) * weight_foo * other_weights;
     *
     *  // or if you already have a "nominal" luminosity weight which
     *  // took into account the nominal sum weights, just multiply it
     *  // out...
     *  float event_weight_foo = lumiWeight * sumWeights_nominal / sumWeights_foo * weight_foo * other_weights;
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
     *    auto nom_weight = xsec * (weight_mc() / generatorSumWeight()) * other_weights;
     *    auto foo_weight = xsec * (currentWeightOfVariation("foo") / sumOfVariation("foo")) * other_weights;
     *    // ...
     *  }
     *  @endcode
     *
     *  @param variation_name the name of the generator variation
     */
    float currentWeightOfVariation(const std::string& variation_name);

  };
}

#endif

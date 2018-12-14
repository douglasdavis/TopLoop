/** @file Jet.cxx
 *  @brief TL::EDM::Jet class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/EDM/Jet.h>

bool TL::EDM::Jet::isbtaggedContinuous(const TL::EDM::BTagBin bin_requirement) const {
  auto req = static_cast<std::uint32_t>(bin_requirement);
  auto bin = static_cast<std::uint32_t>(m_tagWeightBin_MV2c10_Continuous);
  return (bin >= req);
}

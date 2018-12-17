/** @file Jet.cxx
 *  @brief TL::EDM::Jet class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/EDM/Jet.h>

std::map<TL::EDM::BTagBin, std::string> TL::EDM::BTagBinDict = {
  { TL::EDM::BTagBin::eff_100_85, "eff_100_85" },
  { TL::EDM::BTagBin::eff_85_77, "eff_85_77" },
  { TL::EDM::BTagBin::eff_77_70, "eff_77_70" },
  { TL::EDM::BTagBin::eff_70_60, "eff_70_60" },
  { TL::EDM::BTagBin::eff_60_0, "eff_60_0" }
};

std::map<TL::EDM::BTagWP, std::string> TL::EDM::BTagWPDict = {
  { TL::EDM::BTagWP::mv2c10_70, "mv2c10_70" },
  { TL::EDM::BTagWP::mv2c10_77, "mv2c10_77" },
  { TL::EDM::BTagWP::mv2c10_85, "mv2c10_85" },
  { TL::EDM::BTagWP::mv2c10_PC, "mv2c10_PC" }
};

bool TL::EDM::Jet::isbtaggedContinuous(const TL::EDM::BTagBin bin_requirement) const {
  auto req = static_cast<std::uint32_t>(bin_requirement);
  auto bin = static_cast<std::uint32_t>(m_tagWeightBin_MV2c10_Continuous);
  return (bin >= req);
}

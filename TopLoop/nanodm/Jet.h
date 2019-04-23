/** @file  nanodm/Jet.h
 *  @brief nanodm::Jet class header utilities
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef nanodm_Jet_h
#define nanodm_Jet_h

// TL nanodm
#include <TopLoop/nanodm/PhysicsObject.h>

namespace nanodm {

/// Fixed b-tagging WP identifiers
enum class BTagWP {
  mv2c10_70 = 1,
  mv2c10_77 = 2,
  mv2c10_85 = 3,
  mv2c10_PC = 4,
  unknown = 999
};

/// Pseudocontinuous b-tagging WP indentifiers
/**
 *  The numbers are lowerBoundEff_upperBoundEff. (tightest WP
 *  would be 60_0).
 */
enum class BTagBin {
  eff_100_85 = 1,
  eff_85_77 = 2,
  eff_77_70 = 3,
  eff_70_60 = 4,
  eff_60_0 = 5,
  unknown = 999
};

/// convenience working point enum to string function
inline std::string to_string(const nanodm::BTagWP wp) {
  switch (wp) {
    case BTagWP::mv2c10_70:
      return "mv2c10_70";
    case BTagWP::mv2c10_77:
      return "mv2c10_77";
    case BTagWP::mv2c10_85:
      return "mv2c10_85";
    case BTagWP::mv2c10_PC:
      return "mv2c10_PC";
    default:
      return "unknown";
  };
}

/// convenience working point string to enum
inline nanodm::BTagWP from_WP_string(const std::string& wp) {
  if (wp == "mv2c10_70") return BTagWP::mv2c10_70;
  if (wp == "mv2c10_77") return BTagWP::mv2c10_77;
  if (wp == "mv2c10_85") return BTagWP::mv2c10_85;
  if (wp == "mv2c10_PC") return BTagWP::mv2c10_PC;
  return BTagWP::unknown;
}

/// convenience bin enum to string function
inline std::string to_string(const nanodm::BTagBin bin) {
  switch (bin) {
    case BTagBin::eff_100_85:
      return "eff_100_85";
    case BTagBin::eff_85_77:
      return "eff_85_77";
    case BTagBin::eff_77_70:
      return "eff_77_70";
    case BTagBin::eff_70_60:
      return "eff_70_60";
    case BTagBin::eff_60_0:
      return "eff_60_0";
    default:
      return "unknown";
  };
}

inline nanodm::BTagBin from_Bin_string(const std::string& bin) {
  if (bin == "eff_100_85") return BTagBin::eff_100_85;
  if (bin == "eff_85_77") return BTagBin::eff_85_77;
  if (bin == "eff_77_70") return BTagBin::eff_77_70;
  if (bin == "eff_70_60") return BTagBin::eff_70_60;
  if (bin == "eff_60_0") return BTagBin::eff_60_0;
  return BTagBin::unknown;
}

/**
 *  @class nanodm::Jet
 *  @brief A class for jet information
 */
class Jet : public nanodm::PhysicsObject {
 private:
  char m_passfjvt;
  int m_tagWeightBin_MV2c10_Continuous;

 public:
  /// default constructor
  Jet() = default;
  /// default destructor
  virtual ~Jet() = default;
  /// default copy constructor
  Jet(const Jet&) = default;
  /// default assignment operator
  Jet& operator=(const Jet&) = default;
  /// default move copy constructor
  Jet(Jet&&) = default;
  /// default move assignment constructor
  Jet& operator=(Jet&&) = default;

  /// @name settings
  /// @{

  void set_passfjvt(const char val) { m_passfjvt = val; }
  void set_tagWeightBin_MV2c10_Continuous(const int val) {
    m_tagWeightBin_MV2c10_Continuous = val;
  }

  /// @}

  /// @name getters

  char passfjvt() const { return m_passfjvt; }
  int tagWeightBin_MV2c10_Continuous() const { return m_tagWeightBin_MV2c10_Continuous; }

  /// test if jet is b-tagged based on the given minimum bin
  /**
   *  @param bin_requirement the minimum PC b-tagging bin the jet must
   *  pass to be considered tagged.
   */
  bool isbtaggedContinuous(const nanodm::BTagBin bin_requirement) const {
    auto req = static_cast<std::uint32_t>(bin_requirement);
    auto bin = static_cast<std::uint32_t>(m_tagWeightBin_MV2c10_Continuous);
    return (bin >= req);
  }

  /// @}

  /// construct a jet from \f$(p_\mathrm{T}, \eta, \phi, E)\f$
  static std::unique_ptr<Jet> make(float pt, float eta, float phi, float energy) {
    auto jet = std::make_unique<Jet>();
    jet->p4().SetPtEtaPhiE(pt, eta, phi, energy);
    return jet;
  }
};
}  // namespace nanodm

#endif

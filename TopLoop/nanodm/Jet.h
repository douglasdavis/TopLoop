/*! @file  nanodm/Jet.h
 *  @brief nanodm::Jet class header utilities
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef nanodm_Jet_h
#define nanodm_Jet_h

// TL nanodm
#include "TopLoop/nanodm/IPhysicsObject.h"

namespace nanodm {

/// Fixed b-tagging WP identifiers
enum class BTagWP {
  /// mv2c10 70% WP
  mv2c10_70 = 1,
  /// mv2c10 77% WP
  mv2c10_77 = 2,
  /// mv2c10 85% WP
  mv2c10_85 = 3,
  /// mv2c10 psuedo continuous
  mv2c10_PC = 4,
  /// DL1r 60% WP
  DL1r_60 = 5,
  /// DL1r 70% WP
  DL1r_70 = 6,
  /// DL1r 77% WP
  DL1r_77 = 7,
  /// DL1r 85% WP
  DL1r_85 = 8,
  /// DL1r psuedo continuous
  DL1r_PC = 9,
  /// backup unknown identifier
  unknown = 999
};

/// Pseudocontinuous b-tagging WP indentifiers
/*!
 *  The numbers are lowerBoundEff_upperBoundEff. (tightest WP would be 60_0).
 */
enum class BTagBin {
  /// bin associated with 100% efficiency (to 85%)
  eff_100_85 = 1,
  /// bin associated with at least 85% efficiency (to 77%)
  eff_85_77 = 2,
  /// bin associated with at least 77% efficiency (to 70%)
  eff_77_70 = 3,
  /// bin associated with at least 70% efficiency (to 60%)
  eff_70_60 = 4,
  /// bin associated with at least 60% efficiency (to 0%)
  eff_60_0 = 5,
  /// backup unknown identifier
  unknown = 999
};

/// convenience working point enum to string function
/*!
 *  @param wp the working point to convert to string
 */
inline std::string to_string(const BTagWP wp) {
  switch (wp) {
    case BTagWP::mv2c10_70:
      return "mv2c10_70";
    case BTagWP::mv2c10_77:
      return "mv2c10_77";
    case BTagWP::mv2c10_85:
      return "mv2c10_85";
    case BTagWP::mv2c10_PC:
      return "mv2c10_PC";
    case BTagWP::DL1r_60:
      return "DL1r_60";
    case BTagWP::DL1r_70:
      return "DL1r_70";
    case BTagWP::DL1r_77:
      return "DL1r_77";
    case BTagWP::DL1r_85:
      return "DL1r_85";
    case BTagWP::DL1r_PC:
      return "DL1r_PC";
    default:
      return "unknown";
  };
}

/// convenience working point string to enum
/*!
 *  @param wp the string to convert to enum entry
 */
inline BTagWP from_WP_string(const std::string& wp) {
  if (wp == "mv2c10_70") return BTagWP::mv2c10_70;
  if (wp == "mv2c10_77") return BTagWP::mv2c10_77;
  if (wp == "mv2c10_85") return BTagWP::mv2c10_85;
  if (wp == "mv2c10_PC") return BTagWP::mv2c10_PC;
  if (wp == "DL1r_60") return BTagWP::DL1r_60;
  if (wp == "DL1r_70") return BTagWP::DL1r_70;
  if (wp == "DL1r_77") return BTagWP::DL1r_77;
  if (wp == "DL1r_85") return BTagWP::DL1r_85;
  if (wp == "DL1r_PC") return BTagWP::DL1r_PC;
  return BTagWP::unknown;
}

/// convenience bin enum to string function
/*!
 *  @param wp the bin to convert to string
 */
inline std::string to_string(const BTagBin bin) {
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

/// convenience working point string to enum
/*!
 *  @param wp the string to convert to enum entry
 */
inline BTagBin from_Bin_string(const std::string& bin) {
  if (bin == "eff_100_85") return BTagBin::eff_100_85;
  if (bin == "eff_85_77") return BTagBin::eff_85_77;
  if (bin == "eff_77_70") return BTagBin::eff_77_70;
  if (bin == "eff_70_60") return BTagBin::eff_70_60;
  if (bin == "eff_60_0") return BTagBin::eff_60_0;
  return BTagBin::unknown;
}

/*!
 *  @class nanodm::Jet
 *  @brief A class to describe a jet
 */
class Jet : public IPhysicsObject<CoordJet> {
 private:
  // kinematics
  FourVec<CoordJet> m_p4;
  // jvt
  char m_passfjvt;
  // raw tagging information
  float m_mv2c10;
  float m_DL1r;
  // PC tagging information
  int m_tagWeightBin_MV2c10_Continuous;
  int m_tagWeightBin_DL1r_Continuous;
  // fixed tagging information
  char m_isbtagged_DL1r_60;
  char m_isbtagged_DL1r_70;
  char m_isbtagged_DL1r_77;
  char m_isbtagged_DL1r_85;

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

  /// retrieve four vector
  virtual FourVec<CoordJet>& p4() override { return m_p4; }
  /// retrieve const four vector
  virtual const FourVec<CoordJet>& p4() const override { return m_p4; }

  /// get the \f$p_\mathrm{T}\f$.
  virtual float pt() const override { return m_p4.pt(); }
  /// get the pseudorapidity, \f$\eta\f$.
  virtual float eta() const override { return m_p4.eta(); }
  /// get the \f$|\eta|\f$.
  virtual float abseta() const override { return std::abs(m_p4.eta()); }
  /// get the \f$\phi\f$ (angle in the transerve plane).
  virtual float phi() const override { return m_p4.phi(); }
  /// get the energy *from the ROOT four vector*.
  virtual float energy() const override { return m_p4.energy(); }
  /// get the mass *from the ROOT four vector*.
  virtual float mass() const override { return m_p4.mass(); }
  /// get the x-component of the momentum.
  virtual float px() const override { return m_p4.px(); }
  /// get the y-compnent of the momentum.
  virtual float py() const override { return m_p4.py(); }
  /// get the z-compnent of the momentum.
  virtual float pz() const override { return m_p4.pz(); }

  /// @name setters
  /// @{

  /// set the fjvt flag
  void set_passfjvt(const char val) { m_passfjvt = val; }
  /// set the DL1r 60% flag
  void set_isbtagged_DL1r_60(const char val) { m_isbtagged_DL1r_60 = val; }
  /// set the DL1r 70% flag
  void set_isbtagged_DL1r_70(const char val) { m_isbtagged_DL1r_70 = val; }
  /// set the DL1r 77% flag
  void set_isbtagged_DL1r_77(const char val) { m_isbtagged_DL1r_77 = val; }
  /// set the DL1r 85% flag
  void set_isbtagged_DL1r_85(const char val) { m_isbtagged_DL1r_85 = val; }
  /// set the MV2c10 continuous WP bin
  void set_tagWeightBin_MV2c10_Continuous(const int val) {
    m_tagWeightBin_MV2c10_Continuous = val;
  }
  /// set the DL1r continuous WP bin
  void set_tagWeightBin_DL1r_Continuous(const int val) {
    m_tagWeightBin_DL1r_Continuous = val;
  }
  /// set the raw MV2c10 score
  void set_MV2c10_score(const float val) { m_mv2c10 = val; }
  /// set the DL1r score
  void set_DL1r_score(const float val) { m_DL1r = val; }

  /// @}

  /// @name getters
  /// @{

  /// the fJVT flag
  char passfjvt() const { return m_passfjvt; }
  /// the raw MV2c10 score
  float MV2c10() const { return m_mv2c10; }
  /// the raw DL1r score
  float DL1r() const { return m_DL1r; }
  /// the 60% WP flag
  char isbtagged_DL1r_60() const { return m_isbtagged_DL1r_60; }
  /// the 70% WP flag
  char isbtagged_DL1r_70() const { return m_isbtagged_DL1r_70; }
  /// the 77% WP flag
  char isbtagged_DL1r_77() const { return m_isbtagged_DL1r_77; }
  /// the 85% WP flag
  char isbtagged_DL1r_85() const { return m_isbtagged_DL1r_85; }
  /// the DL1r continuous b-tagging bin
  int tagWeightBin_DL1r_Continous() const { return m_tagWeightBin_DL1r_Continuous; }
  /// the MV2c10 continuous b-tagging bin
  int tagWeightBin_MV2c10_Continuous() const { return m_tagWeightBin_MV2c10_Continuous; }

  /// test if jet is b-tagged based on the given minimum bin
  /*!
   *  @param bin_requirement the minimum PC b-tagging bin the jet must
   *  pass to be considered tagged.
   */
  bool isbtaggedContinuous_MV2c10(const BTagBin bin_requirement) const {
    auto req = static_cast<std::uint32_t>(bin_requirement);
    auto bin = static_cast<std::uint32_t>(m_tagWeightBin_MV2c10_Continuous);
    return (bin >= req);
  }

  /// test if jet is b-tagged based on the given minimum bin
  /*!
   *  @param bin_requirement the minimum PC b-tagging bin the jet must
   *  pass to be considered tagged.
   */
  bool isbtaggedContinuous_DL1r(const BTagBin bin_requirement) const {
    auto req = static_cast<std::uint32_t>(bin_requirement);
    auto bin = static_cast<std::uint32_t>(m_tagWeightBin_DL1r_Continuous);
    return (bin >= req);
  }

  /// test if jet is b-tagged based on the given minimum bin
  /*!
   *  This version is deprecated and returns the
   *  isbtaggedContinuous_MV2c10 version.
   *
   *  @param bin_requirement the minimum PC b-tagging bin the jet must
   *  pass to be considered tagged.
   */
  // clang-format off
  [[deprecated("Use the tagger specific isbtaggedContinuous_TAGGER test functions")]]
  bool isbtaggedContinuous(const BTagBin bin_requirement) const {
    return isbtaggedContinuous_MV2c10(bin_requirement);
  }
  // clang-format on

  /// @}

  /// @name factory
  /// @{

  /// construct a jet from \f$(p_\mathrm{T}, \eta, \phi, E)\f$
  static std::unique_ptr<Jet> make(float pt, float eta, float phi, float energy) {
    auto jet = std::make_unique<Jet>();
    jet->p4().SetCoordinates(pt, eta, phi, energy);
    return jet;
  }

  /// @}
};
}  // namespace nanodm

#endif

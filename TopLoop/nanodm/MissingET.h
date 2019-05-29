/** @file  nanodm/MissingET.h
 *  @brief nanodm::MissingET class header
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef nanodm_MissingET_h
#define nanodm_MissingET_h

#include "TopLoop/nanodm/IPhysicsObject.h"

namespace nanodm {

/**
 *  @class nanodm::MissingET
 *  @brief a class to describe \f$E_\mathrm{T}^\mathrm{miss}\f$.
 */
class MissingET : public IPhysicsObject<CoordMET> {
 private:
  float m_sumet;
  float m_px;
  float m_py;

  FourVec<CoordMET> m_p4;

 public:
  /// default constructor
  MissingET() = default;
  /// default destructor
  virtual ~MissingET() = default;
  /// default copy constructor
  MissingET(const MissingET&) = default;
  /// default assignment operator
  MissingET& operator=(const MissingET&) = default;
  /// default move copy constructor
  MissingET(MissingET&&) = default;
  /// default move assignment operator
  MissingET& operator=(MissingET&&) = default;

  /// @name setters
  /// @{

  /// set the \f$\sum E_\mathrm{T}\f$ of all objects constributing to \f$E_\mathrm{T}^\mathrm{miss}\f$.
  void set_sumet(const float val) { m_sumet = val; }
  /// set the \f$x\f$ component of the \f$E_\mathrm{T}^\mathrm{miss}\f$ value.
  void set_px(const float val) { m_px = val; }
  /// set \f$y\f$ component of the \f$E_\mathrm{T}^\mathrm{miss}\f$ value.
  void set_py(const float val) { m_py = val; }

  /// @}

  /// @name getters
  /// @{

  /// the \f$\sum E_\mathrm{T}\f$ (sumet from the sgtop ntuple tree)
  /**
   *  the sum of transverse energies of all hard objects constributing
   *  to \f$E_\mathrm{T}^\mathrm{miss}\f$.
   */
  float sumet() const { return m_sumet; }

  /// @}

  /// retrieve four vector
  virtual FourVec<CoordMET>& p4() override { return m_p4; }
  /// retrieve const four vector
  virtual const FourVec<CoordMET>& p4() const override { return m_p4; }

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
  virtual float px() const override { return m_px; }
  /// get the y-compnent of the momentum.
  virtual float py() const override { return m_py; }
  /// get the z-compnent of the momentum.
  virtual float pz() const override { return m_p4.pz(); }

  /// @name creation utilities
  /// @{

  /// construct a MissingET object from its met (pt) and phi valus
  static std::unique_ptr<MissingET> make(float pt, float phi) {
    auto m = std::make_unique<MissingET>();
    m->p4().SetCoordinates(pt, 0, phi, 0);
    return m;
  }

  /// @}
};
}  // namespace nanodm

#endif

/** @file  nanodm/MissingET.h
 *  @brief nanodm::MissingET class header
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef nanodm_MissingET_h
#define nanodm_MissingET_h

#include <TopLoop/nanodm/PhysicsObject.h>

namespace nanodm {

/**
 *  @class nanodm::MissingET
 *  @brief a class to describe \f$E_\mathrm{T}^\mathrm{miss}\f$.
 */
class MissingET : public nanodm::PhysicsObject {
 private:
  float m_sumet;
  float m_px;
  float m_py;

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

  /// the \f$\sum E_\mathrm{T}\f$ of all objects constributing to \f$E_\mathrm{T}^\mathrm{miss}\f$. (sumet from the sgtop ntuple tree)
  float sumet() const { return m_sumet; }
  /// the \f$x\f$ component of the \f$E_\mathrm{T}^\mathrm{miss}\f$ value. (met_px from the sgtop ntuple tree)
  float px() const final { return m_px; }
  /// set \f$y\f$ component of the \f$E_\mathrm{T}^\mathrm{miss}\f$ value. (met_py from the sgtop ntuple tree)
  float py() const final { return m_py; }

  /// @}

  /// @name creation utilities
  /// @{

  /// construct a MissingET object from its met (pt) and phi valus
  static std::unique_ptr<MissingET> make(float pt, float phi) {
    auto m = std::make_unique<MissingET>();
    m->p4().SetPtEtaPhiM(pt, 0, phi, 0);
    return m;
  }

  /// @}

};
}  // namespace nanodm

#endif

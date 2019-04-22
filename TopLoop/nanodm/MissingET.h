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
 *  @brief A container for electron/muon information.
 */
class MissingET : public nanodm::PhysicsObject {
 private:
  float m_sumet;
  float m_px;
  float m_py;

 public:
  MissingET() = default;
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

  void set_sumet(const float val) { m_sumet = val; }
  void set_px(const float val) { m_px = val; }
  void set_py(const float val) { m_py = val; }

  /// @}

  /// @name getters
  /// @{

  /// get sumet from the sgtop ntuple tree
  float sumet() const { return m_sumet; }
  /// get met_px from the sgtop ntuple tree
  float px() const final { return m_px; }
  /// get met_py from the sgtop ntuple tree
  float py() const final { return m_py; }

  static std::unique_ptr<MissingET> make(float pt, float phi) {
    auto m = std::make_unique<MissingET>();
    m->p4().SetPtEtaPhiM(pt, 0, phi, 0);
    return m;
  }

  /// @}
};
}  // namespace nanodm

#endif

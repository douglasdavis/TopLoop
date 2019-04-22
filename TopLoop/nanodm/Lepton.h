/** @file  nanodm/Lepton.h
 *  @brief nanodm::Lepton class header
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef nanodm_Lepton_h
#define nanodm_Lepton_h

#include <TopLoop/nanodm/PhysicsObject.h>

namespace nanodm {

/**
 *  @class nanodm::Lepton
 *  @brief A container for electron/muon information.
 */
class Lepton : public nanodm::PhysicsObject {
 protected:
  int m_pdgId;
  float m_charge;
  bool m_isMCNonPrompt{false};

 public:
  Lepton() = delete;
  virtual ~Lepton() = default;
  /// default copy constructor
  Lepton(const Lepton&) = default;
  /// default assignment operator
  Lepton& operator=(const Lepton&) = default;
  /// default move copy constructor
  Lepton(Lepton&&) = default;
  /// default move assignment operator
  Lepton& operator=(Lepton&&) = default;

  /// constructor defining pdgId
  /**
   *  This is for the Electron and Muon classes which
   *  inherit from Lepton
   *
   *  @param pdgId the PDG code
   */
  Lepton(const unsigned int pdgId) : m_pdgId(pdgId) {}

  /// @name setters
  /// @{

  void set_pdgId(const int val) { m_pdgId = val; }
  void set_charge(const float val) { m_charge = val; }
  void set_isMCNonPrompt(const bool val) { m_isMCNonPrompt = val; }

  /// @}

  /// @name getters
  /// @{

  int pdgId() const { return m_pdgId; }
  float charge() const { return m_charge; }
  float isMCNonPrompt() const { return m_isMCNonPrompt; }

  /// @}
};

/**
 *  @class nanodm::Muon
 *  @brief A class for muon information
 */
class Muon : public nanodm::Lepton {
 public:
  Muon() : nanodm::Lepton(13) {}
  /// default destructor
  virtual ~Muon() = default;
  /// default copy constructor
  Muon(const Muon&) = default;
  /// default assignment operator
  Muon& operator=(const Muon&) = default;
  /// default move constructor
  Muon(Muon&&) = default;
  /// default move assignment operator
  Muon& operator=(Muon&&) = default;

  /// construct a muon from \f$(p_\mathrm{T}, \eta, \phi)\f$
  static std::unique_ptr<Muon> make(float pt, float eta, float phi) {
    auto muon = std::make_unique<Muon>();
    muon->p4().SetPtEtaPhiM(pt, eta, phi, 105.6583745);
    return muon;
  }
};

/**
 *  @class nanodm::Electron
 *  @brief A class for electron information
 */
class Electron : public nanodm::Lepton {
 public:
  Electron() : nanodm::Lepton(11) {}
  /// default destructor
  virtual ~Electron() = default;
  /// default copy constructor
  Electron(const Electron&) = default;
  /// default assignment operator
  Electron& operator=(const Electron&) = default;
  /// default move constructor
  Electron(Electron&&) = default;
  /// default move assignment operator
  Electron& operator=(Electron&&) = default;

  /// construct an electron from \f$(p_\mathrm{T}, \eta, \phi)\f$
  static std::unique_ptr<Electron> make(float pt, float eta, float phi) {
    auto electron = std::make_unique<Electron>();
    electron->p4().SetPtEtaPhiM(pt, eta, phi, 0.5109989461);
    return electron;
  }
};

}  // namespace nanodm

#endif

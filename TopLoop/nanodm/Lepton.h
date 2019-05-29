/** @file  nanodm/Lepton.h
 *  @brief nanodm::Lepton class header
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef nanodm_Lepton_h
#define nanodm_Lepton_h

#include "TopLoop/nanodm/IPhysicsObject.h"

namespace nanodm {

/**
 *  @class nanodm::Lepton
 *  @brief A base class to describe electrons or muon.
 */
class Lepton : public IPhysicsObject<CoordLep> {
 protected:
  int m_pdgId;
  float m_charge;
  bool m_isMCNonPrompt{false};

  FourVec<CoordLep> m_p4;

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
  explicit Lepton(const unsigned int pdgId) : m_pdgId(pdgId) {}

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

  /// retrieve four vector
  virtual FourVec<CoordLep>& p4() override { return m_p4; }
  /// retrieve const four vector
  virtual const FourVec<CoordLep>& p4() const override { return m_p4; }

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
};

/**
 *  @class nanodm::Muon
 *  @brief A class for muon information
 */
class Muon : public Lepton {
 public:
  Muon() : Lepton(13) {}
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

  /// @name creation utilities
  /// @{

  /// construct a muon from \f$(p_\mathrm{T}, \eta, \phi)\f$
  static std::unique_ptr<Muon> make(float pt, float eta, float phi) {
    auto muon = std::make_unique<Muon>();
    muon->p4().SetCoordinates(pt, eta, phi, 105.6583745);
    return muon;
  }

  /// @}
};

/**
 *  @class nanodm::Electron
 *  @brief A class for electron information
 */
class Electron : public Lepton {
 public:
  Electron() : Lepton(11) {}
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

  /// @name creation utilities
  /// @{

  /// construct an electron from \f$(p_\mathrm{T}, \eta, \phi)\f$
  static std::unique_ptr<Electron> make(float pt, float eta, float phi) {
    auto electron = std::make_unique<Electron>();
    electron->p4().SetCoordinates(pt, eta, phi, 0.5109989461);
    return electron;
  }

  /// @}
};

}  // namespace nanodm

#endif

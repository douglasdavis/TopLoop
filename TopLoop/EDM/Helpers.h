/** @file Helpers.h
 *  @brief TL::EDM Helper functions
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_EDM_Helpers_h
#define TL_EDM_Helpers_h

#include <TopLoop/EDM/PhysicsObject.h>

namespace TL {
  namespace EDM {

    using PhysObjList = std::initializer_list<TL::EDM::PhysicsObject>;

    /// @addtogroup EDM EDM Helper functions
    /// @brief Helper functions for making calculations using EDM
    /// classes.
    ///
    /// Many of these functions take
    /// `std::initializer_list<TL::EDM::PhysicsObject>` function
    /// parameters. An alias to that has been created called
    /// `PhysObjList`. For use, just feed a braced-init-list of
    /// objects inheriting from TL::EDM::PhysicsObject to the
    /// function. For example:
    ///
    /// @code{.cpp}
    ///
    /// // assuming the existense of finalState as a TL::EDM::FinalState object.
    /// auto lep1  = finalState.leptons().at(0);
    /// auto lep2  = finalState.leptons().at(1);
    /// auto jet1  = finalState.leptons().at(0);
    /// auto jet2  = finalState.leptons().at(1);
    /// auto sumet = finalState.MissingET().sumet();
    /// double sigma_pTsys_lep1lep2jet1jet2 = TL::EDM::sigma_pTsys({lep1,lep2,jet1,jet2},sumet);
    /// double deltapT_lep1lep2_jet1jet2    = TL::EDM::deltapT({lep1,lep2},{jet1,jet2});
    /// double eom_lep1lep2                 = TL::EDM::energyMassRatio({lep1,lep2});
    /// // example not taking lists:
    /// double mT_lep1jet1 = TL::EDM::transverseMass(lep1,jet1);
    ///
    /// @endcode
    ///
    /// @{

    /// calculate the \f$p_{\mathrm{T}}\f$ of the system of objects.
    /**
     *  @param objects the list of objects inheriting from
     *  TL::EDM::PhysicsObject to use in the calculation.
     */
    double pTsys(const PhysObjList objects);

    /// calculate the \f$\sigma(p^\mathrm{sys}_{\mathrm{T}})\f$ of a system in the final state
    /**
     *  The calculation is defined as
     *  \f[
     *     \sigma(p_\mathrm{T}^\mathrm{sys}(o_1,\ldots,o_n)) =
     *        \frac{p_\mathrm{T}^\mathrm{sys}(o_1,\ldots,o_n)}
                 {\sqrt{H_\mathrm{T}(o_1,\ldots,o_n)+\sum E_\mathrm{T}}}.
     *  \f]
     *
     *  \f$H_\mathrm{T}\f$ is calculated from the same system of
     *  objects that is used to calculate
     *  \f$p_\mathrm{T}^\mathrm{sys}\f$, where \f$\sum E_\mathrm{T}\f$
     *  is an event property.
     *
     *  Has units of \f$\sqrt{\mathrm{[Energy]}}\f$.
     *
     *  @param objects the list of objects inheriting from
     *  TL::EDM::PhysicsObject to use in the calculation.
     *  @param sumet the \f$\sum E_{\mathrm{T}}\f$ of the event. (sum
     *  of \f$p_\mathrm{T}\f$ of all hard objects contributing to the
     *  \f$E_\mathrm{T}^\mathrm{miss}\f$ calculation).
     */
    double sigma_pTsys(const PhysObjList objects, const float sumet);

    /// Calculate the \f$H_{\mathrm{T}}\f$ of the system of objects
    /**
     *  The \f$H_\mathrm{T}\f$ is the scalar sum of the
     *  \f$p_\mathrm{T}\f$ of all objects.
     *
     *  @param objects the list of objects inhjeriting from
     *  TL::EDM::PhysicsObject to use in the calculation.
     */
    double HTsys(const PhysObjList objects);

    /// Calculate the centrality of the system of objects
    /**
     *  Centrality is defined as
     *  \f[
     *     C(o_1,\ldots,o_n) =
             \frac{\sum_\limits{i=1}^{n}p_\mathrm{T}^{(i)}}{\sum_\limits{i=1}^{n}E^{(i)}}
     *  \f]
     *
     *  @param objects the list of objects inhjeriting from
     *  TL::EDM::PhysicsObject to use in the calculation.
     */
    double centrality(const PhysObjList objects);

    /// Calculate \f$\Delta R\f$ between systems of objects
    /**
     *  This function uses the ROOT function `TLorentzVector::DeltaR` to calculate:
     *
     *  \f[
     *     \Delta R \equiv \sqrt{(\Delta\eta)^2 + (\Delta\phi)^2}.
     *  \f]
     *
     *  @param system1 list of objects (inheriting from TL::EDM::PhysicsObject) in system 1.
     *  @param system2 list of objects (inheriting from TL::EDM::PhysicsObject) in system 2.
     */
    double deltaR(const PhysObjList system1, const PhysObjList system2);

    /// Calculate \f$\Delta p_{\mathrm{T}}\f$ between systems of objects.
    /**
     *  @param system1 list of objects (inheriting from TL::EDM::PhysicsObject) in system 1.
     *  @param system2 list of objects (inheriting from TL::EDM::PhysicsObject) in system 2.
     */
    double deltapT(const PhysObjList system1, const PhysObjList system2);

    /// Calculate \f$\Delta \phi\f$ between systems of objects.
    /**
     *  The result is wrapped to the interval \f$[-\pi, \pi)\f$.
     *
     *  @param system1 list of objects (inheriting from TL::EDM::PhysicsObject) in system 1.
     *  @param system2 list of objects (inheriting from TL::EDM::PhysicsObject) in system 2.
     */
    double deltaphi(const PhysObjList system1, const PhysObjList system2);

    /// Calculate the transverse mass (\f$m_{\mathrm{T}}\f$) of two object system.
    /**
     *  The transverse mass is defined as
     *  \f[
     *     m_{\mathrm{T}} =
     *        \sqrt{2p_{\mathrm{T}}(o_1)p_{\mathrm{T}}(o_2)(1-\cos\Delta\phi)}.
     *  \f]
     *
     *  @param obj1 first object.
     *  @param obj2 second object.
     */
    double transverseMass(const PhysicsObject& obj1, const PhysicsObject& obj2);

    /// Calculate the energy to mass ratio of a system.
    /**
     *  @param objects the list of objects inhjeriting from
     *  TL::EDM::PhysicsObject to use in the calculation.
     */
    double energyMassRatio(const PhysObjList objects);

    /// @}

  }
}

#endif

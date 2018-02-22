/** @file Helpers.h
 *  @brief TL::EDM Helper functions
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef TL_EDM_Helpers_h
#define TL_EDM_Helpers_h

#include <TopLoop/EDM/PhysicsObject.h>

using ObjList_t = std::initializer_list<TL::EDM::PhysicsObject>;

namespace TL {
  namespace EDM {

    /// @name EDM Helper functions
    /// @{

    /// calculate the \f$p_{\mathrm{T}}\f$ of the system of objects
    /**
     *  @param objects the list of objects inheriting from
     *  TL::EDM::PhysicsObject to use in the calculation.
     */
    double pTsys(const ObjList_t& objects);

    /// calculate the \f$\sigma_{p_{\mathrm{T}}}\f$ system of objects
    /**
     *  @param objects the list of objects inheriting from
     *  TL::EDM::PhysicsObject to use in the calculation.
     *  @param sumet the \f$\sum E_{\mathrm{T}}\f$ of the event.
     */
    double sigma_pTsys(const ObjList_t& objects, const float sumet);

    /// Calculate the \f$H_{\mathrm{T}}\f$ of the system of objects
    /**
     *  @param objects the list of objects inhjeriting from
     *  TL::EDM::PhysicsObject to use in the calculation
     */
    double HTsys(const ObjList_t& objects);

    /// Calculate the centrality of the system of objects
    /**
     *  @param objects the list of objects inhjeriting from
     *  TL::EDM::PhysicsObject to use in the calculation
     */
    double centrality(const ObjList_t& objects);

    /// Calculate \f$\Delta R\f$ between systems of objects
    /**
     *  @param s1 list of objects in system 1
     *  @param s2 list of objects in system 2
     */
    double deltaR(const ObjList_t& s1, const ObjList_t& s2);

    /// Calculate \f$\Delta p_{\mathrm{T}}\f$ between systems of objects
    /**
     *  @param s1 list of objects in system 1
     *  @param s2 list of objects in system 2
     */
    double deltapT(const ObjList_t& s1, const ObjList_t& s2);

    /// Calculate the transverse mass (\f$m_{\mathrm{T}}\f$) of two object system
    /**
     *  The transverse mass is calculated as
     *  \f$ m_{\mathrm{T}} = \sqrt{2p_{\mathrm{T}}(o_1)p_{\mathrm{T}}(o_2)(1-\cos\Delta\phi)}\f$.
     *
     *  @param o1 first object
     *  @param o2 second object
     */
    double transverseMass(const PhysicsObject& o1, const PhysicsObject& o2);

    /// Calculate the energy to mass ratio of a system
    /**
     *  @param objects the list of objects inhjeriting from
     *  TL::EDM::PhysicsObject to use in the calculation
     */
    double energyMassRatio(const ObjList_t& objects);

    /// @}

  }
}

#endif

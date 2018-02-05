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

    /// calculate the pT of the system of objects
    /**
     *  @param objects the list of objects inheriting from
     *  TL::EDM::PhysicsObject to use in the calculation.
     */
    double pTsys(const ObjList_t& objects);

    /// calculate the sigma pT system of objects
    /**
     *  @param objects the list of objects inheriting from
     *  TL::EDM::PhysicsObject to use in the calculation.
     *  @param sumet the sum(E_T) of the event
     */
    double sigma_pTsys(const ObjList_t& objects, const float sumet);

    /// Calculate the HT of the system of objects
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

    /// Calculate delta R between systems of objects
    /*
     *  @param s1 list of objects in system 1
     *  @param s2 list of objects in system 2
     */
    double deltaR(const ObjList_t& s1, const ObjList_t& s2);

    /// Calculate delta pT between systems of objects
    /*
     *  @param s1 list of objects in system 1
     *  @param s2 list of objects in system 2
     */
    double deltapT(const ObjList_t& s1, const ObjList_t& s2);

    /// @}

  }
}

#endif

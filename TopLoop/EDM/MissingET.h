/** @file  MissingET.h
 *  @brief TL::EDM::MissingET class header
 *  @class TL::EDM::MissingET
 *  @brief A class for \f$E_\mathrm{T}^\mathrm{miss}\f$ information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of the missing tranvserse energy.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_EDM_MissingET_h
#define TL_EDM_MissingET_h

// TL
#include <TopLoop/EDM/PhysicsObject.h>

namespace TL {
  namespace EDM {

    class MissingET : public TL::EDM::PhysicsObject {
    private:
      float m_px{0};
      float m_py{0};
      float m_sumet{0};

    public:
      MissingET() = default;
      virtual ~MissingET() = default;

      MissingET(const MissingET&) = default;
      MissingET& operator=(const MissingET&) = default;

      /// @name setters
      /// @{

      void set_px(const float val)    { m_px = val;    }
      void set_py(const float val)    { m_py = val;    }
      void set_sumet(const float val) { m_sumet = val; }

      /// @}

      /// @name getters
      /// @{

      float px()    const { return m_px;    }
      float py()    const { return m_py;    }
      float sumet() const { return m_sumet; }

      /// @}

      /// @name utilities
      /// @{

      void reset() {
        m_px = 0;
        m_py = 0;
        m_sumet = 0;
        p4().SetPtEtaPhiM(0,0,0,0);
      }

      /// @}

    };

  }
}

#endif

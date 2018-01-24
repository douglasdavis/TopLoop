/** @file  MET.h
 *  @brief TL::EDM::MET class header
 *  @class TL::EDM::MET
 *  @brief A container for MET information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of the missing tranvserse energy.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef TL_EDM_MET_h
#define TL_EDM_MET_h

// TL
#include <TopLoop/EDM/PhysicsObject.h>

namespace TL {
  namespace EDM {

    class MET : public TL::EDM::PhysicsObject {
    private:
      float m_px;
      float m_py;
      float m_sumet;

    public:
      MET() = default;
      virtual ~MET() = default;

      MET(const MET&) = default;
      MET& operator=(const MET&) = default;

      /// @name setters
      /// @{

      void set_px(const float val);
      void set_py(const float val);
      void set_sumet(const float val);

      /// @}

      /// @name getters
      /// @{

      float px()    const;
      float py()    const;
      float sumet() const;

      /// @}

    };

  }
}

inline void TL::EDM::MET::set_px(const float val)    { m_px    = val; }
inline void TL::EDM::MET::set_py(const float val)    { m_py    = val; }
inline void TL::EDM::MET::set_sumet(const float val) { m_sumet = val; }

inline float TL::EDM::MET::px()    const { return m_px;    }
inline float TL::EDM::MET::py()    const { return m_py;    }
inline float TL::EDM::MET::sumet() const { return m_sumet; }

#endif

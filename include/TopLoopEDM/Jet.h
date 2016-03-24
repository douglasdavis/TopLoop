/** @file  Jet.h
 *  @brief TL::EDM::Jet class header
 *  @class TL::EDM::Jet
 *  @brief A container for jet information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a jet.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_EDM_Jet_h
#define TL_EDM_Jet_h

// TL
#include <TopLoopEDM/PhysicsObject.h>

namespace TL {
  namespace EDM {

    class Jet : public TL::EDM::PhysicsObject {
    private:
      float m_mv1;
      float m_mvb;
      float m_mv1c;
      float m_mv2c00;
      float m_mv2c10;
      float m_mv2c20;
      float m_ip3dsv1;
      float m_jvt;
      
      ClassDef(Jet,1);
      
    public:
      Jet() : TL::EDM::PhysicsObject() {}
      virtual ~Jet() {}

      void set_mv1(const float val);
      void set_mvb(const float val);
      void set_mv1c(const float val);
      void set_mv2c00(const float val);
      void set_mv2c10(const float val);
      void set_mv2c20(const float val);
      void set_ip3dsv1(const float val);
      void set_jvt(const float val);

      float mv1() const;
      float mvb() const;
      float mv1c() const;
      float mv2c00() const;
      float mv2c10() const;
      float mv2c20() const;
      float ip3dsv1() const;
      float jvt() const;      
    };

  }
}

inline void TL::EDM::Jet::set_mv1(const float val)      { m_mv1     = val; }
inline void TL::EDM::Jet::set_mvb(const float val)      { m_mvb     = val; }
inline void TL::EDM::Jet::set_mv1c(const float val)     { m_mv1c    = val; }
inline void TL::EDM::Jet::set_mv2c00(const float val)   { m_mv2c00  = val; }
inline void TL::EDM::Jet::set_mv2c10(const float val)   { m_mv2c10  = val; }
inline void TL::EDM::Jet::set_mv2c20(const float val)   { m_mv2c20  = val; }
inline void TL::EDM::Jet::set_ip3dsv1(const float val)  { m_ip3dsv1 = val; }
inline void TL::EDM::Jet::set_jvt(const float val)      { m_jvt     = val; }

inline float TL::EDM::Jet::mv1()     const { return m_mv1;     }
inline float TL::EDM::Jet::mvb()     const { return m_mvb;     }
inline float TL::EDM::Jet::mv1c()    const { return m_mv1c;    }
inline float TL::EDM::Jet::mv2c00()  const { return m_mv2c00;  }
inline float TL::EDM::Jet::mv2c10()  const { return m_mv2c10;  }
inline float TL::EDM::Jet::mv2c20()  const { return m_mv2c20;  }
inline float TL::EDM::Jet::ip3dsv1() const { return m_ip3dsv1; }
inline float TL::EDM::Jet::jvt()     const { return m_jvt;     }

#endif

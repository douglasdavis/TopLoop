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
#include <map>

namespace TL {
  namespace EDM {

    class Jet : public TL::EDM::PhysicsObject {
    private:
      float m_mv2c00;
      float m_mv2c10;
      float m_mv2c20;
      float m_ip3dsv1;
      float m_jvt;
      bool  m_tagged;
      
      ClassDef(Jet,1);
      
    public:
      Jet() : TL::EDM::PhysicsObject() {}
      virtual ~Jet() {}

      enum WP_mv2c20 { kEFF_60, kEFF_70, kEFF_77, kEFF_85 };

      void set_mv2c00(const float val);
      void set_mv2c10(const float val);
      void set_mv2c20(const float val);
      void set_ip3dsv1(const float val);
      void set_jvt(const float val);
      void set_tagged();

      float mv2c00() const;
      float mv2c10() const;
      float mv2c20() const;
      float ip3dsv1() const;
      float jvt() const;

      bool isTagged(WP_mv2c20 wp = WP_mv2c20::kEFF_77) const;
    };

  }
}

namespace TL {
  namespace EDM {
    const std::map<TL::EDM::Jet::WP_mv2c20,float> kBTAGCUTS = {
      { TL::EDM::Jet::WP_mv2c20::kEFF_60,  0.4496 },
      { TL::EDM::Jet::WP_mv2c20::kEFF_70, -0.0436 },
      { TL::EDM::Jet::WP_mv2c20::kEFF_77, -0.4434 },
      { TL::EDM::Jet::WP_mv2c20::kEFF_85, -0.7887 }
    };
  }
}

inline void TL::EDM::Jet::set_mv2c00(const float val)   { m_mv2c00  = val; }
inline void TL::EDM::Jet::set_mv2c10(const float val)   { m_mv2c10  = val; }
inline void TL::EDM::Jet::set_mv2c20(const float val)   { m_mv2c20  = val; }
inline void TL::EDM::Jet::set_ip3dsv1(const float val)  { m_ip3dsv1 = val; }
inline void TL::EDM::Jet::set_jvt(const float val)      { m_jvt     = val; }

inline float TL::EDM::Jet::mv2c00()  const { return m_mv2c00;  }
inline float TL::EDM::Jet::mv2c10()  const { return m_mv2c10;  }
inline float TL::EDM::Jet::mv2c20()  const { return m_mv2c20;  }
inline float TL::EDM::Jet::ip3dsv1() const { return m_ip3dsv1; }
inline float TL::EDM::Jet::jvt()     const { return m_jvt;     }

inline bool  TL::EDM::Jet::isTagged(TL::EDM::Jet::WP_mv2c20 wp) const {
  return (m_mv2c20 > TL::EDM::kBTAGCUTS.at(wp));
}

#endif

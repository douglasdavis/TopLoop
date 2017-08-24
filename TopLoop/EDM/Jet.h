/** @file  Jet.h
 *  @brief TL::EDM::Jet class header
 *  @class TL::EDM::Jet
 *  @brief A container for jet information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a jet.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef TL_EDM_Jet_h
#define TL_EDM_Jet_h

// TL
#include <TopLoop/EDM/PhysicsObject.h>

// C++
#include <map>

namespace TL {
  namespace EDM {
    // MV2c b-tagging benchmarks
    enum WP_mv2cbm { kEFF_60, kEFF_70, kEFF_77, kEFF_85 };
  }
}

namespace TL {
  namespace EDM {

    class Jet : public TL::EDM::PhysicsObject {
    private:
      float m_mv2c00;
      float m_mv2c10;
      float m_mv2c20;
      float m_ip3dsv1;
      float m_jvt;
      bool  m_isbtagged_70;
      bool  m_isbtagged_77;
      bool  m_isbtagged_85;

      ClassDef(Jet,1);
      
    public:
      Jet() : TL::EDM::PhysicsObject() {}
      virtual ~Jet() {}

      void set_mv2c00(const float val);
      void set_mv2c10(const float val);
      void set_mv2c20(const float val);
      void set_ip3dsv1(const float val);
      void set_jvt(const float val);
      void set_isbtagged_70(const bool val);
      void set_isbtagged_77(const bool val);
      void set_isbtagged_85(const bool val);

      float mv2c00()       const;
      float mv2c10()       const;
      float mv2c20()       const;
      float ip3dsv1()      const;
      float jvt()          const;
      bool  isbtagged_70() const;
      bool  isbtagged_77() const;
      bool  isbtagged_85() const;

      bool isTagged_c20(const TL::EDM::WP_mv2cbm& wp = WP_mv2cbm::kEFF_77) const;
      bool isTagged_c10(const TL::EDM::WP_mv2cbm& wp = WP_mv2cbm::kEFF_77) const;
    };

  }
}

// b-tagging benchmarks, MV2c10 and MC2c20
// these are current for MC15c
// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#b_tagging_Benchmarks_for_mc15c_s
// The current (as of May 2016) recommendation is to use MV2c10.
namespace TL {
  namespace EDM {
    const std::map<TL::EDM::WP_mv2cbm,float> kBTAGCUTS_c20 = {
      { TL::EDM::WP_mv2cbm::kEFF_60, 0.8867 },
      { TL::EDM::WP_mv2cbm::kEFF_70, 0.7110 },
      { TL::EDM::WP_mv2cbm::kEFF_77, 0.4803 },
      { TL::EDM::WP_mv2cbm::kEFF_85, 0.0206 }
    };
    const std::map<TL::EDM::WP_mv2cbm,float> kBTAGCUTS_c10 = {
      { TL::EDM::WP_mv2cbm::kEFF_60, 0.9349 },
      { TL::EDM::WP_mv2cbm::kEFF_70, 0.8244 },
      { TL::EDM::WP_mv2cbm::kEFF_77, 0.6459 },
      { TL::EDM::WP_mv2cbm::kEFF_85, 0.1758 }
    };
  }
}

inline void TL::EDM::Jet::set_mv2c00(const float val)      { m_mv2c00       = val; }
inline void TL::EDM::Jet::set_mv2c10(const float val)      { m_mv2c10       = val; }
inline void TL::EDM::Jet::set_mv2c20(const float val)      { m_mv2c20       = val; }
inline void TL::EDM::Jet::set_ip3dsv1(const float val)     { m_ip3dsv1      = val; }
inline void TL::EDM::Jet::set_jvt(const float val)         { m_jvt          = val; }
inline void TL::EDM::Jet::set_isbtagged_70(const bool val) { m_isbtagged_70 = val; }
inline void TL::EDM::Jet::set_isbtagged_77(const bool val) { m_isbtagged_77 = val; }
inline void TL::EDM::Jet::set_isbtagged_85(const bool val) { m_isbtagged_85 = val; }

inline float TL::EDM::Jet::mv2c00()       const { return m_mv2c00;       }
inline float TL::EDM::Jet::mv2c10()       const { return m_mv2c10;       }
inline float TL::EDM::Jet::mv2c20()       const { return m_mv2c20;       }
inline float TL::EDM::Jet::ip3dsv1()      const { return m_ip3dsv1;      }
inline float TL::EDM::Jet::jvt()          const { return m_jvt;          }
inline bool  TL::EDM::Jet::isbtagged_70() const { return m_isbtagged_70; }
inline bool  TL::EDM::Jet::isbtagged_77() const { return m_isbtagged_77; }
inline bool  TL::EDM::Jet::isbtagged_85() const { return m_isbtagged_85; }

inline bool TL::EDM::Jet::isTagged_c20(const TL::EDM::WP_mv2cbm& wp) const {
  return (m_mv2c20 > TL::EDM::kBTAGCUTS_c20.at(wp));
}

inline bool TL::EDM::Jet::isTagged_c10(const TL::EDM::WP_mv2cbm& wp) const {
  return (m_mv2c10 > TL::EDM::kBTAGCUTS_c10.at(wp));
}

#endif

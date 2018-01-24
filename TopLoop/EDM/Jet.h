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

    class Jet : public TL::EDM::PhysicsObject {
    private:
      float m_mv2c00;
      float m_mv2c10;
      float m_mv2c20;
      float m_passfjvt;
      char  m_isbtagged_MV2c10_70;
      char  m_isbtagged_MV2c10_77;
      char  m_isbtagged_MV2c10_85;
      float m_MV2c10mu;
      float m_MV2c10rnn;
      float m_DL1;
      float m_DL1mu;
      float m_DL1rnn;
      float m_MV2cl100;
      float m_MV2c100;
      float m_DL1_pu;
      float m_DL1_pc;
      float m_DL1_pb;
      float m_DL1mu_pu;
      float m_DL1mu_pc;
      float m_DL1mu_pb;
      float m_DL1rnn_pu;
      float m_DL1rnn_pc;
      float m_DL1rnn_pb;

    public:
      Jet() = default;
      virtual ~Jet() = default;

      Jet& operator=(const Jet&) = default;
      Jet(const Jet&) = default;

      /// @name setters
      /// @{

      void set_mv2c00(const float val);
      void set_mv2c10(const float val);
      void set_mv2c20(const float val);
      void set_passfjvt(const float val);
      void set_isbtagged_MV2c10_70(const char val);
      void set_isbtagged_MV2c10_77(const char val);
      void set_isbtagged_MV2c10_85(const char val);
      void set_MV2c10mu(const float val);
      void set_MV2c10rnn(const float val);
      void set_DL1(const float val);
      void set_DL1mu(const float val);
      void set_DL1rnn(const float val);
      void set_MV2cl100(const float val);
      void set_MV2c100(const float val);
      void set_DL1_pu(const float val);
      void set_DL1_pc(const float val);
      void set_DL1_pb(const float val);
      void set_DL1mu_pu(const float val);
      void set_DL1mu_pc(const float val);
      void set_DL1mu_pb(const float val);
      void set_DL1rnn_pu(const float val);
      void set_DL1rnn_pc(const float val);
      void set_DL1rnn_pb(const float val);

      /// @}

      /// @name getters
      /// @{

      float mv2c00()              const;
      float mv2c10()              const;
      float mv2c20()              const;
      float passfjvt()            const;
      char  isbtagged_MV2c10_70() const;
      char  isbtagged_MV2c10_77() const;
      char  isbtagged_MV2c10_85() const;
      float MV2c10mu()            const;
      float MV2c10rnn()           const;
      float DL1()                 const;
      float DL1mu()               const;
      float DL1rnn()              const;
      float MV2cl100()            const;
      float MV2c100()             const;
      float DL1_pu()              const;
      float DL1_pc()              const;
      float DL1_pb()              const;
      float DL1mu_pu()            const;
      float DL1mu_pc()            const;
      float DL1mu_pb()            const;
      float DL1rnn_pu()           const;
      float DL1rnn_pc()           const;
      float DL1rnn_pb()           const;

      /// @}

    };

  }
}

inline void TL::EDM::Jet::set_mv2c00(const float val)             { m_mv2c00              = val; }
inline void TL::EDM::Jet::set_mv2c10(const float val)             { m_mv2c10              = val; }
inline void TL::EDM::Jet::set_mv2c20(const float val)             { m_mv2c20              = val; }
inline void TL::EDM::Jet::set_passfjvt(const float val)           { m_passfjvt            = val; }
inline void TL::EDM::Jet::set_isbtagged_MV2c10_70(const char val) { m_isbtagged_MV2c10_70 = val; }
inline void TL::EDM::Jet::set_isbtagged_MV2c10_77(const char val) { m_isbtagged_MV2c10_77 = val; }
inline void TL::EDM::Jet::set_isbtagged_MV2c10_85(const char val) { m_isbtagged_MV2c10_85 = val; }
inline void TL::EDM::Jet::set_MV2c10mu(const float val)           { m_MV2c10mu            = val; }
inline void TL::EDM::Jet::set_MV2c10rnn(const float val)          { m_MV2c10rnn           = val; }
inline void TL::EDM::Jet::set_DL1(const float val)                { m_DL1                 = val; }
inline void TL::EDM::Jet::set_DL1mu(const float val)              { m_DL1mu               = val; }
inline void TL::EDM::Jet::set_DL1rnn(const float val)             { m_DL1rnn              = val; }
inline void TL::EDM::Jet::set_MV2cl100(const float val)           { m_MV2cl100            = val; }
inline void TL::EDM::Jet::set_MV2c100(const float val)            { m_MV2c100             = val; }
inline void TL::EDM::Jet::set_DL1_pu(const float val)             { m_DL1_pu              = val; }
inline void TL::EDM::Jet::set_DL1_pc(const float val)             { m_DL1_pc              = val; }
inline void TL::EDM::Jet::set_DL1_pb(const float val)             { m_DL1_pb              = val; }
inline void TL::EDM::Jet::set_DL1mu_pu(const float val)           { m_DL1mu_pu            = val; }
inline void TL::EDM::Jet::set_DL1mu_pc(const float val)           { m_DL1mu_pc            = val; }
inline void TL::EDM::Jet::set_DL1mu_pb(const float val)           { m_DL1mu_pb            = val; }
inline void TL::EDM::Jet::set_DL1rnn_pu(const float val)          { m_DL1rnn_pu           = val; }
inline void TL::EDM::Jet::set_DL1rnn_pc(const float val)          { m_DL1rnn_pc           = val; }
inline void TL::EDM::Jet::set_DL1rnn_pb(const float val)          { m_DL1rnn_pb           = val; }


inline float TL::EDM::Jet::mv2c00()              const { return m_mv2c00;              }
inline float TL::EDM::Jet::mv2c10()              const { return m_mv2c10;              }
inline float TL::EDM::Jet::mv2c20()              const { return m_mv2c20;              }
inline float TL::EDM::Jet::passfjvt()            const { return m_passfjvt;            }
inline char  TL::EDM::Jet::isbtagged_MV2c10_70() const { return m_isbtagged_MV2c10_70; }
inline char  TL::EDM::Jet::isbtagged_MV2c10_77() const { return m_isbtagged_MV2c10_77; }
inline char  TL::EDM::Jet::isbtagged_MV2c10_85() const { return m_isbtagged_MV2c10_85; }
inline float TL::EDM::Jet::MV2c10mu()            const { return m_MV2c10mu;            }
inline float TL::EDM::Jet::MV2c10rnn()           const { return m_MV2c10rnn;           }
inline float TL::EDM::Jet::DL1()                 const { return m_DL1;                 }
inline float TL::EDM::Jet::DL1mu()               const { return m_DL1mu;               }
inline float TL::EDM::Jet::DL1rnn()              const { return m_DL1rnn;              }
inline float TL::EDM::Jet::MV2cl100()            const { return m_MV2cl100;            }
inline float TL::EDM::Jet::MV2c100()             const { return m_MV2c100;             }
inline float TL::EDM::Jet::DL1_pu()              const { return m_DL1_pu;              }
inline float TL::EDM::Jet::DL1_pc()              const { return m_DL1_pc;              }
inline float TL::EDM::Jet::DL1_pb()              const { return m_DL1_pb;              }
inline float TL::EDM::Jet::DL1mu_pu()            const { return m_DL1mu_pu;            }
inline float TL::EDM::Jet::DL1mu_pc()            const { return m_DL1mu_pc;            }
inline float TL::EDM::Jet::DL1mu_pb()            const { return m_DL1mu_pb;            }
inline float TL::EDM::Jet::DL1rnn_pu()           const { return m_DL1rnn_pu;           }
inline float TL::EDM::Jet::DL1rnn_pc()           const { return m_DL1rnn_pc;           }
inline float TL::EDM::Jet::DL1rnn_pb()           const { return m_DL1rnn_pb;           }

#endif

/** @file  Jet.h
 *  @brief TL::EDM::Jet class header
 *  @class TL::EDM::Jet
 *  @brief A class for jet information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a jet.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_EDM_Jet_h
#define TL_EDM_Jet_h

// TL
#include <TopLoop/EDM/PhysicsObject.h>

// C++
#include <map>

namespace TL {
  namespace EDM {

    /// b-tagging working point identifiers
    enum class BTagWP { mv2c10_70, mv2c10_77, mv2c10_85 };

    class Jet : public TL::EDM::PhysicsObject {
    private:
      float m_mv2c00;
      float m_mv2c10;
      float m_mv2c20;
      float m_passfjvt;
      int   m_truthflav;
      int   m_truthPartonLabel;
      char  m_isTrueHS;
      char  m_isbtagged_MV2c10_70;
      char  m_isbtagged_MV2c10_77;
      char  m_isbtagged_MV2c10_85;
      int   m_tagWeightBin_MV2c10_Continuous;
      char  m_isbtagged_DL1_HybBEff_60;
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
      /// default constructor
      Jet() = default;
      /// default destructor
      virtual ~Jet() = default;
      /// default copy constructor
      Jet(const Jet&) = default;
      /// default assignment operator
      Jet& operator=(const Jet&) = default;
      /// default move copy constructor
      Jet(Jet&&) = default;
      /// default move assignment constructor
      Jet& operator=(Jet&&) = default;

      void set_mv2c00(const float val)             { m_mv2c00              = val; }
      void set_mv2c10(const float val)             { m_mv2c10              = val; }
      void set_mv2c20(const float val)             { m_mv2c20              = val; }
      void set_passfjvt(const float val)           { m_passfjvt            = val; }
      void set_truthflav(const int val)            { m_truthflav           = val; }
      void set_truthPartonLabel(const int val)     { m_truthPartonLabel    = val; }
      void set_isTrueHS(const char val)            { m_isTrueHS            = val; }
      void set_isbtagged_MV2c10_70(const char val) { m_isbtagged_MV2c10_70 = val; }
      void set_isbtagged_MV2c10_77(const char val) { m_isbtagged_MV2c10_77 = val; }
      void set_isbtagged_MV2c10_85(const char val) { m_isbtagged_MV2c10_85 = val; }
      void set_tagWeightBin_MV2c10_Continuous(const int val) { m_tagWeightBin_MV2c10_Continuous = val; }
      void set_isbtagged_DL1_HybBEff_60(const char val) { m_isbtagged_DL1_HybBEff_60 = val; }
      void set_MV2c10mu(const float val)           { m_MV2c10mu            = val; }
      void set_MV2c10rnn(const float val)          { m_MV2c10rnn           = val; }
      void set_DL1(const float val)                { m_DL1                 = val; }
      void set_DL1mu(const float val)              { m_DL1mu               = val; }
      void set_DL1rnn(const float val)             { m_DL1rnn              = val; }
      void set_MV2cl100(const float val)           { m_MV2cl100            = val; }
      void set_MV2c100(const float val)            { m_MV2c100             = val; }
      void set_DL1_pu(const float val)             { m_DL1_pu              = val; }
      void set_DL1_pc(const float val)             { m_DL1_pc              = val; }
      void set_DL1_pb(const float val)             { m_DL1_pb              = val; }
      void set_DL1mu_pu(const float val)           { m_DL1mu_pu            = val; }
      void set_DL1mu_pc(const float val)           { m_DL1mu_pc            = val; }
      void set_DL1mu_pb(const float val)           { m_DL1mu_pb            = val; }
      void set_DL1rnn_pu(const float val)          { m_DL1rnn_pu           = val; }
      void set_DL1rnn_pc(const float val)          { m_DL1rnn_pc           = val; }
      void set_DL1rnn_pb(const float val)          { m_DL1rnn_pb           = val; }


      float mv2c00()              const { return m_mv2c00;              }
      float mv2c10()              const { return m_mv2c10;              }
      float mv2c20()              const { return m_mv2c20;              }
      float passfjvt()            const { return m_passfjvt;            }
      int   truthflav()           const { return m_truthflav;           }
      int   truthPartonLabel()    const { return m_truthPartonLabel;    }
      char  isTrueHS()            const { return m_isTrueHS;            }
      char  isbtagged_MV2c10_70() const { return m_isbtagged_MV2c10_70; }
      char  isbtagged_MV2c10_77() const { return m_isbtagged_MV2c10_77; }
      char  isbtagged_MV2c10_85() const { return m_isbtagged_MV2c10_85; }
      int   tagWeightBin_MV2c10_Continuous() const { return m_tagWeightBin_MV2c10_Continuous; }
      char  isbtagged_DL1_HybBEff_60() const { return m_isbtagged_DL1_HybBEff_60; }
      float MV2c10mu()            const { return m_MV2c10mu;            }
      float MV2c10rnn()           const { return m_MV2c10rnn;           }
      float DL1()                 const { return m_DL1;                 }
      float DL1mu()               const { return m_DL1mu;               }
      float DL1rnn()              const { return m_DL1rnn;              }
      float MV2cl100()            const { return m_MV2cl100;            }
      float MV2c100()             const { return m_MV2c100;             }
      float DL1_pu()              const { return m_DL1_pu;              }
      float DL1_pc()              const { return m_DL1_pc;              }
      float DL1_pb()              const { return m_DL1_pb;              }
      float DL1mu_pu()            const { return m_DL1mu_pu;            }
      float DL1mu_pc()            const { return m_DL1mu_pc;            }
      float DL1mu_pb()            const { return m_DL1mu_pb;            }
      float DL1rnn_pu()           const { return m_DL1rnn_pu;           }
      float DL1rnn_pc()           const { return m_DL1rnn_pc;           }

    };

  }
}

#endif

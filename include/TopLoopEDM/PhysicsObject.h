/** @file  PhysicsObject.h
 *  @brief TL::EDM::PhysicsObject class header
 *  @class TL::EDM::PhysicsObject
 *  @brief A container for physics object information.
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a physics object. This includes
 *  leptons, jets, MET..
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

#ifndef TL_EDM_PhysicsObject_h
#define TL_EDM_PhysicsObject_h

// ROOT
#include <TLorentzVector.h>

namespace TL {
  namespace EDM {

    class PhysicsObject : public TObject {
    protected:
      TLorentzVector m_p;
      bool           m_isParticleLevel;
      bool           m_partLevPassedSelection;

      ClassDef(PhysicsObject,1);

    public:
      PhysicsObject() :
	m_p(), m_isParticleLevel(false), m_partLevPassedSelection(false)
      {}
      virtual ~PhysicsObject() {}

      TLorentzVector& p();

      const TLorentzVector& p() const;

      void set_isParticleLevel(const bool val);
      void set_partLevPassedSelection(const bool val);

      bool isParticleLevel() const;
      bool partLevPassedSelection() const;

      float pT()  const;
      float eta() const;
      float phi() const;
      float E()   const;
      float m()   const;
      float px()  const;
      float py()  const;
      float pz()  const;
      
    };

  }
}

inline const TLorentzVector& TL::EDM::PhysicsObject::p() const { return m_p; }
inline       TLorentzVector& TL::EDM::PhysicsObject::p()       { return m_p; }

inline void TL::EDM::PhysicsObject::set_isParticleLevel(const bool val)        { m_isParticleLevel        = val; }
inline void TL::EDM::PhysicsObject::set_partLevPassedSelection(const bool val) { m_partLevPassedSelection = val; }

inline bool TL::EDM::PhysicsObject::isParticleLevel()        const { return m_isParticleLevel;        }
inline bool TL::EDM::PhysicsObject::partLevPassedSelection() const { return m_partLevPassedSelection; }

inline float TL::EDM::PhysicsObject::pT()  const { return m_p.Pt();  }
inline float TL::EDM::PhysicsObject::eta() const { return m_p.Eta(); }
inline float TL::EDM::PhysicsObject::phi() const { return m_p.Phi(); }
inline float TL::EDM::PhysicsObject::E()   const { return m_p.E();   }
inline float TL::EDM::PhysicsObject::m()   const { return m_p.M();   }
inline float TL::EDM::PhysicsObject::px()  const { return m_p.Px();  }
inline float TL::EDM::PhysicsObject::py()  const { return m_p.Py();  }
inline float TL::EDM::PhysicsObject::pz()  const { return m_p.Pz();  }

#endif

/** @file  Muon.h
 *  @brief TL::EDM::Muon class header
 *  @class TL::EDM::Muon
 *  @brief A container for muon information
 *
 *  This class is part of the TopLoop event data model (EDM). It
 *  contains the properties of a muon
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef TL_EDM_Muon_h
#define TL_EDM_Muon_h

#include <TopLoop/EDM/Lepton.h>

namespace TL {
  namespace EDM {

    class Muon : public TL::EDM::Lepton {
    private:
      float m_ptvarcone30;

    public:
      Muon() : TL::EDM::Lepton() {
        set_pdgId(13);
      }
      virtual ~Muon() = default;

      Muon& operator=(const Muon&) = default;
      Muon(const Muon&) = default;

      /// @name setters
      /// @{

      void set_ptvarcone30(const float val);

      /// @}

      /// @name getters
      /// @{

      float ptvarcone30() const;

      /// @}

    };

  }
}

inline void TL::EDM::Muon::set_ptvarcone30(const float val) { m_ptvarcone30 = val; }

inline float TL::EDM::Muon::ptvarcone30() const { return m_ptvarcone30; }

#endif

/** @file  SampleMetaSvc.h
 *  @brief TL::SampleMetaSvc class header
 *
 *  A utility for connect a DSID with meta data, which includes the
 *  initial state, the generator, and the type (nominal or
 *  systematic).
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef TL_SampleMetaSvc_h
#define TL_SampleMetaSvc_h

// C++
#include <sstream>
#include <fstream>
#include <map>
#include <tuple>

// TL
#include <TopLoop/Utils.h>

// ROOT
#include <TObject.h>

namespace TL {
  // these enums are directly related to the samplemeta.txt file. If a
  // new intial state, generator, or type is added to that file (that
  // doesn't already exist in it), these enums must be related. The
  // setupMaps() function must also be updated to be compatible with
  // the new entry(ies).
  enum kInitialState {
    Data_1  , ttbar,  Wt      ,
    Zjets   , Wjets,  WW      ,
    WZ      , ZZ,     Diboson ,
    ttbarZ  , ttbarW
  };
  enum kGenerator {
    Data_2           , PowhegPythia    , PowhegPythia8  ,
    PowhegHerwig     , PowhegHerwigpp  , Sherpa21       ,
    Sherpa22         , Sherpa221       , MadgraphPythia ,
    MadgraphPythia8  , aMCatNLOPythia8 , aMCatNLOHerwig ,
    aMCatNLOHerwigpp , Unknown_2
  };
  enum kSampleType {
    Data_3 , Nominal , Systematic , Unknown_3
  };
}

namespace TL {
  class SampleMetaSvc : public TObject {
  private:
    std::map<std::string,TL::kInitialState> m_s2eInitialState;
    std::map<std::string,TL::kGenerator>    m_s2eGenerator;
    std::map<std::string,TL::kSampleType>   m_s2eSampleType;
    std::map<TL::kInitialState,std::string> m_e2sInitialState;
    std::map<TL::kGenerator,   std::string> m_e2sGenerator;
    std::map<TL::kSampleType,  std::string> m_e2sSampleType;

    std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>> m_table;

    void setupMaps();

    ClassDef(TL::SampleMetaSvc,1);

  public:
    SampleMetaSvc();
    virtual ~SampleMetaSvc();

    const std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>>& table() const;

    TL::kInitialState getInitialState(const unsigned int dsid) const;
    TL::kGenerator    getGenerator(const unsigned int dsid)    const;
    TL::kSampleType   getSampleType(const unsigned int dsid)   const;

    const std::string initialStateString(const TL::kInitialState istate) const;
    const std::string generatorString(const TL::kGenerator igen)         const;
    const std::string sampleTypeString(const TL::kSampleType ist)        const;

    const std::string initialStateString(const unsigned int dsid) const;
    const std::string generatorString(const unsigned int dsid)    const;
    const std::string sampleTypeString(const unsigned int dsid)   const;
  };
}

inline const std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>>& TL::SampleMetaSvc::table() const {
  return m_table;
}

inline TL::kInitialState TL::SampleMetaSvc::getInitialState(const unsigned int dsid) const {
  return std::get<0>(m_table.at(dsid));
}

inline TL::kGenerator TL::SampleMetaSvc::getGenerator(const unsigned int dsid) const {
  return std::get<1>(m_table.at(dsid));
}

inline TL::kSampleType TL::SampleMetaSvc::getSampleType(const unsigned int dsid) const {
  return std::get<2>(m_table.at(dsid));
}

inline const std::string TL::SampleMetaSvc::initialStateString(const TL::kInitialState istat)  const {
  if ( m_e2sInitialState.find(istat) == m_e2sInitialState.end() ) {
    TL::Fatal(__PRETTY_FUNCTION__,"bad TL::kInitialState",istat);
  }
  return m_e2sInitialState.at(istat);
}

inline const std::string TL::SampleMetaSvc::generatorString(const TL::kGenerator igen) const {
  if ( m_e2sGenerator.find(igen) == m_e2sGenerator.end() ) {
    TL::Fatal(__PRETTY_FUNCTION__,"bad TL::kGenerator",igen);
  }
  return m_e2sGenerator.at(igen);
}

inline const std::string TL::SampleMetaSvc::sampleTypeString(const TL::kSampleType ist) const {
  if ( m_e2sSampleType.find(ist) == m_e2sSampleType.end() ) {
    TL::Fatal(__PRETTY_FUNCTION__,"bad TL::kSampleType",ist);
  }
  return m_e2sSampleType.at(ist);
}

inline const std::string TL::SampleMetaSvc::initialStateString(const unsigned int dsid) const {
  return initialStateString(getInitialState(dsid));
}

inline const std::string TL::SampleMetaSvc::generatorString(const unsigned int dsid) const {
  return generatorString(getGenerator(dsid));
}

inline const std::string TL::SampleMetaSvc::sampleTypeString(const unsigned int dsid) const {
  return sampleTypeString(getSampleType(dsid));
}

#endif

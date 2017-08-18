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
#include <TopLoop/Core/Utils.h>

// ROOT
#include <TObject.h>

namespace TL {
  // these enums are directly related to the samplemeta.txt file. If a
  // new intial state, generator, or type is added to that file (that
  // doesn't already exist in it), these enums must be related. The
  // setupMaps() function must also be updated to be compatible with
  // the new entry(ies).
  enum kMeta {
    Unknown ,
    //
    Data , Nominal , Systematic ,
    //
    ttbar  , Wt      , Zjets    ,
    Wjets  , WW      , WZ       ,
    ZZ     , Diboson , ttbarZ   ,
    //
    ttbarW , ttbarll , ttbarphi ,
    PowhegPythia6     , PowhegPythia8     , PowhegHerwig      ,
    PowhegHerwigpp    , Sherpa21          , Sherpa22          ,
    Sherpa221         , MadgraphPythia    , MadgraphPythia8   ,
    //
    aMCatNLOPythia8   , aMCatNLOHerwig    , aMCatNLOHerwigpp  ,
    PowhegPythia8_dil , PowhegPythia6_dil , PowhegHerwig7     ,
  };
}

namespace TL {
  class SampleMetaSvc : public TObject {
  private:

    std::map<std::string,TL::kMeta> m_s2e;
    typedef std::map<std::string,TL::kMeta>::const_iterator s2eIter_t;

    std::map<TL::kMeta,std::string> m_e2s;
    typedef std::map<TL::kMeta,std::string>::const_iterator e2sIter_t;

    //std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>> m_table;
    // init state ,, generator ,, sample type
    std::map<int,std::tuple<TL::kMeta,TL::kMeta,TL::kMeta>> m_table;
    typedef std::map<int,std::tuple<TL::kMeta,TL::kMeta,TL::kMeta>>::const_iterator TableIter_t;

    void setupMaps();
    const TableIter_t checkTable(const unsigned int dsid) const;

    ClassDef(TL::SampleMetaSvc,1);

  public:
    SampleMetaSvc();
    virtual ~SampleMetaSvc();

    const std::map<int,std::tuple<TL::kMeta,TL::kMeta,TL::kMeta>>& table() const;

    TL::kMeta getInitialState(const unsigned int dsid) const;
    TL::kMeta getGenerator(const unsigned int dsid)    const;
    TL::kMeta getSampleType(const unsigned int dsid)   const;

    const std::string stringFromEnum(const TL::kMeta ienum) const;

    const std::string initialStateString(const unsigned int dsid) const;
    const std::string generatorString(const unsigned int dsid)    const;
    const std::string sampleTypeString(const unsigned int dsid)   const;
  };
}

inline const std::map<int,std::tuple<TL::kMeta,TL::kMeta,TL::kMeta>>& TL::SampleMetaSvc::table() const {
  return m_table;
}

inline const TL::SampleMetaSvc::TableIter_t TL::SampleMetaSvc::checkTable(const unsigned int dsid) const {
  const TableIter_t itr = m_table.find(dsid);
  if ( itr == m_table.end() ) {
    TL::Fatal(__PRETTY_FUNCTION__,"can't find DSID!",dsid,"not in SampleMetaSvc table");
  }
  return itr;
}

inline TL::kMeta TL::SampleMetaSvc::getInitialState(const unsigned int dsid) const {
  auto itr = checkTable(dsid);
  return std::get<0>(itr->second);
}

inline TL::kMeta TL::SampleMetaSvc::getGenerator(const unsigned int dsid) const {
  auto itr = checkTable(dsid);
  return std::get<1>(itr->second);
}

inline TL::kMeta TL::SampleMetaSvc::getSampleType(const unsigned int dsid) const {
  auto itr = checkTable(dsid);
  return std::get<2>(itr->second);
}

inline const std::string TL::SampleMetaSvc::stringFromEnum(const TL::kMeta ienum) const {
  e2sIter_t itr = m_e2s.find(ienum);
  if ( itr == m_e2s.end() ) {
    TL::Fatal(__PRETTY_FUNCTION__,"bad TL::kMeta",ienum);
  }
  return itr->second;
}

inline const std::string TL::SampleMetaSvc::initialStateString(const unsigned int dsid) const {
  return stringFromEnum(getInitialState(dsid));
}

inline const std::string TL::SampleMetaSvc::generatorString(const unsigned int dsid) const {
  return stringFromEnum(getGenerator(dsid));
}

inline const std::string TL::SampleMetaSvc::sampleTypeString(const unsigned int dsid) const {
  return stringFromEnum(getSampleType(dsid));
}

#endif

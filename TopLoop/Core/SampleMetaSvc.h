/** @file  SampleMetaSvc.h
 *  @brief TL::SampleMetaSvc class header
 *  @class TL::SampleMetaSvc
 *  @brief C++11 singleton class for handling sample meta data
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
#include <TopLoop/Core/Loggable.h>

// ROOT
#include <TObject.h>

namespace TL {

  /*!
    \enum kMeta

    this enum is directly related to the samplemeta.json file. If a
    new intial state, generator, or type is added to that file (that
    doesn't already exist in it), this enum must be updated. The
    setupMap() function must also be updated to be compatible with
    the new entry(ies)!
  */

  enum kMeta {
    Unknown ,
    // sample types
    Data , Nominal , Systematic ,
    // initial states
    ttbar  , Wt      , Zjets    ,
    Wjets  , WW      , WZ       ,
    ZZ     , Diboson , ttbarZ   ,
    ttbarW , ttbarll , ttbarphi , tchan ,
    // generators
    PowhegPythia6     , PowhegPythia8     , PowhegHerwig      ,
    PowhegHerwigpp    , Sherpa21          , Sherpa22          ,
    Sherpa221         , MadgraphPythia    , MadgraphPythia8   ,
    aMCatNLOPythia8   , aMCatNLOHerwig    , aMCatNLOHerwigpp  ,
    PowhegPythia8_dil , PowhegPythia6_dil , PowhegHerwig7     ,
  };
}

namespace TL {
  class SampleMetaSvc final : public TL::Loggable {
  private:

    SampleMetaSvc();
    ~SampleMetaSvc();

    SampleMetaSvc(const SampleMetaSvc&) = delete;
    SampleMetaSvc& operator=(const SampleMetaSvc&) = delete;
    SampleMetaSvc(SampleMetaSvc&&) = delete;
    SampleMetaSvc& operator=(SampleMetaSvc&&) = delete;

    std::map<std::string,TL::kMeta> m_s2e;
    std::map<TL::kMeta,std::string> m_e2s;

    std::map<int,std::tuple<TL::kMeta,TL::kMeta,TL::kMeta>> m_table;
    typedef std::map<int,std::tuple<TL::kMeta,TL::kMeta,TL::kMeta>>::const_iterator TableIter_t;

    void setupMap();
    const TableIter_t checkTable(const unsigned int dsid) const;

  public:

    /// get the instance of the singleton class
    static SampleMetaSvc& get();

    /// get the table of the form (DSID,(initial state, generator, sample type))
    const std::map<int,std::tuple<TL::kMeta,TL::kMeta,TL::kMeta>>& table() const;

    /// retrieve a enum value corresponding to the initial state based on a DSID
    TL::kMeta getInitialState(const unsigned int dsid) const;
    /// retrieve a enum value corresponding to the generator based on a DSID    
    TL::kMeta getGenerator(const unsigned int dsid)    const;
    /// retrieve a enum value corresponding to the sample type based on a DSID
    TL::kMeta getSampleType(const unsigned int dsid)   const;

    /// retrieve a string based on the meta enum table
    const std::string stringFromEnum(const TL::kMeta ienum) const;

    /// get the initial state name based on a dsid
    const std::string initialStateString(const unsigned int dsid) const;
    /// get the generator name based on a dsid
    const std::string generatorString(const unsigned int dsid)    const;
    /// get the sample type name based on a dsid
    const std::string sampleTypeString(const unsigned int dsid)   const;

    /// have logger print out info based on dsid
    void printInfo(const int dsid) const;

  };
}

inline TL::SampleMetaSvc& TL::SampleMetaSvc::get() {
  static SampleMetaSvc inst;
  return inst;
}

inline const std::map<int,std::tuple<TL::kMeta,TL::kMeta,TL::kMeta>>& TL::SampleMetaSvc::table() const {
  return m_table;
}

inline const TL::SampleMetaSvc::TableIter_t TL::SampleMetaSvc::checkTable(const unsigned int dsid) const {
  const TableIter_t itr = m_table.find(dsid);
  if ( itr == m_table.end() ) {
    logger()->critical("can't find DSID! {} not in SampleMetaSvc table!",dsid);
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
  auto itr = m_e2s.find(ienum);
  if ( itr == m_e2s.end() ) {
    logger()->critical("can't find enum entry {}!",ienum);
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

/** @file  SampleMetaSvc.h
 *  @brief TL::SampleMetaSvc class header
 *  @class TL::SampleMetaSvc
 *  @brief C++11 singleton class for handling sample meta data
 *
 *  A utility for connect a DSID with meta data, which includes the
 *  initial state, the generator, and the type (nominal or
 *  systematic).
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
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

namespace TL {

  enum class kCampaign {
    Unknown, Data, MC16a, MC16c, MC16d, MC16e, MC16f
  };

  enum class kInitialState {
    Unknown  , Data     , ttbar   , Wt      ,
    Zjets    , Wjets    , WW      , WZ      ,
    ZZ       , Diboson  , ttbarZ  , ttbarW  ,
    ttbarll  , ttbarphi , tchan   , schan
  };

  enum class kGenerator {
    Unknown           , Data              , PowhegPythia6     , PowhegPythia8     ,
    PowhegHerwig      , PowhegHerwigpp    , Sherpa21          , Sherpa22          ,
    Sherpa221         , MadgraphPythia    , MadgraphPythia8   , aMCatNLOPythia8   ,
    aMCatNLOHerwig    , aMCatNLOHerwigpp  , PowhegPythia8_dil , PowhegPythia6_dil ,
    PowhegHerwig7
  };

  enum class kSampleType {
    Unknown, Data, Nominal, Systematic
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

    std::map<std::string,TL::kSampleType>   m_s2e_ST;
    std::map<TL::kSampleType,std::string>   m_e2s_ST;

    std::map<std::string,TL::kInitialState> m_s2e_IS;
    std::map<TL::kInitialState,std::string> m_e2s_IS;

    std::map<std::string,TL::kGenerator>    m_s2e_G;
    std::map<TL::kGenerator,std::string>    m_e2s_G;

    std::map<std::string,TL::kCampaign>     m_s2e_C;
    std::map<TL::kCampaign,std::string>     m_e2s_C;

    std::map<std::string,TL::kCampaign>     m_rTags;

    void setupMap();

    typedef std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>> SampleTable_t;
    SampleTable_t m_sampleTable;

    const SampleTable_t::const_iterator checkTable(const unsigned int dsid) const;

    template <typename Enumeration>
    auto as_integer(const Enumeration value) const -> typename std::underlying_type<Enumeration>::type {
      return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }

  public:

    /// get the instance of the singleton class
    static SampleMetaSvc& get();

    /// @name Enum identification getters
    /// @{

    /// retrieve a enum value corresponding to the initial state based on a DSID
    TL::kInitialState getInitialState(const unsigned int dsid) const;
    /// retrieve a enum value corresponding to the generator based on a DSID
    TL::kGenerator    getGenerator(const unsigned int dsid)    const;
    /// retrieve a enum value corresponding to the sample type based on a DSID
    TL::kSampleType   getSampleType(const unsigned int dsid)   const;

    /// @}

    /// @name Sample string identification getters
    /// @{

    /// get the initial state name based on a dsid
    const std::string getInitialStateStr(const unsigned int dsid) const;
    /// get the generator name based on a dsid
    const std::string getGeneratorStr(const unsigned int dsid)    const;
    /// get the sample type name based on a dsid
    const std::string getSampleTypeStr(const unsigned int dsid)   const;

    /// retrieve a string based on the InitialState enum
    const std::string as_string(const TL::kInitialState ienum) const;
    /// retrieve a string based on the Generator enum
    const std::string as_string(const TL::kGenerator ienum) const;
    /// retrieve a string based on the SampleType enum
    const std::string as_string(const TL::kSampleType ienum) const;

    /// @}

    /// @name Campaign helper runctions
    /// @{

    /// Given a sample name, get the MC campaign identifier
    TL::kCampaign getCampaign(const std::string& sample_name) const;

    /// retrieve a string based on the Campain enum
    const std::string as_string(const TL::kCampaign ienum) const;

    /// @}

    /// have logger print out info based on dsid
    void printInfo(const int dsid) const;

  };
}

inline TL::SampleMetaSvc& TL::SampleMetaSvc::get() {
  static SampleMetaSvc inst;
  return inst;
}

inline const TL::SampleMetaSvc::SampleTable_t::const_iterator
TL::SampleMetaSvc::checkTable(const unsigned int dsid) const {
  const SampleTable_t::const_iterator itr = m_sampleTable.find(dsid);
  if ( itr == m_sampleTable.end() ) {
    logger()->critical("can't find DSID! {} not in SampleMetaSvc table!",dsid);
  }
  return itr;
}

inline TL::kInitialState TL::SampleMetaSvc::getInitialState(const unsigned int dsid) const {
  auto itr = checkTable(dsid);
  return std::get<0>(itr->second);
}

inline TL::kGenerator TL::SampleMetaSvc::getGenerator(const unsigned int dsid) const {
  auto itr = checkTable(dsid);
  return std::get<1>(itr->second);
}

inline TL::kSampleType TL::SampleMetaSvc::getSampleType(const unsigned int dsid) const {
  auto itr = checkTable(dsid);
  return std::get<2>(itr->second);
}

inline const std::string TL::SampleMetaSvc::as_string(const TL::kInitialState ienum) const {
  auto itr = m_e2s_IS.find(ienum);
  if ( itr == m_e2s_IS.end() ) {
    logger()->critical("can't find enum entry {}!",as_string(ienum));
  }
  return itr->second;
}

inline const std::string TL::SampleMetaSvc::as_string(const TL::kGenerator ienum) const {
  auto itr = m_e2s_G.find(ienum);
  if ( itr == m_e2s_G.end() ) {
    logger()->critical("can't find enum entry {}!",as_string(ienum));
  }
  return itr->second;
}

inline const std::string TL::SampleMetaSvc::as_string(const TL::kSampleType ienum) const {
  auto itr = m_e2s_ST.find(ienum);
  if ( itr == m_e2s_ST.end() ) {
    logger()->critical("can't find enum entry {}!",as_string(ienum));
  }
  return itr->second;
}

inline const std::string TL::SampleMetaSvc::getInitialStateStr(const unsigned int dsid) const {
  return as_string(getInitialState(dsid));
}

inline const std::string TL::SampleMetaSvc::getGeneratorStr(const unsigned int dsid) const {
  return as_string(getGenerator(dsid));
}

inline const std::string TL::SampleMetaSvc::getSampleTypeStr(const unsigned int dsid) const {
  return as_string(getSampleType(dsid));
}

#endif

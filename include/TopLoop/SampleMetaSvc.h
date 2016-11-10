/** @file  SampleMetaSvc.h
 *  @brief TL::SampleMetaSvc class header
 *
 *  A utility for connect a DSID with meta data.
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

  const std::map<std::string,TL::kInitialState> s2eInitialState = {
    { "Data"    , TL::kInitialState::Data_1  } , { "ttbar"  , TL::kInitialState::ttbar  } ,
    { "Wt"      , TL::kInitialState::Wt      } , { "Zjets"  , TL::kInitialState::Zjets  } ,
    { "Wjets"   , TL::kInitialState::Wjets   } , { "WW"     , TL::kInitialState::WW     } ,
    { "WZ"      , TL::kInitialState::WZ      } , { "ZZ"     , TL::kInitialState::ZZ     } ,
    { "Diboson" , TL::kInitialState::Diboson } , { "ttbarW" , TL::kInitialState::ttbarW } ,
    { "ttbarZ"  , TL::kInitialState::ttbarZ  }
  };
  const std::map<std::string,TL::kGenerator> s2eGenerator = {
    { "Data"             , TL::kGenerator::Data_2           } , { "PowhegPythia"    , TL::kGenerator::PowhegPythia    } ,
    { "PowhegPythia8"    , TL::kGenerator::PowhegPythia8    } , { "PowhegHerwig"    , TL::kGenerator::PowhegHerwig    } ,
    { "PowhegHerwigpp"   , TL::kGenerator::PowhegHerwigpp   } , { "Sherpa21"        , TL::kGenerator::Sherpa21        } ,
    { "Sherpa22"         , TL::kGenerator::Sherpa22         } , { "Sherpa221"       , TL::kGenerator::Sherpa221       } ,
    { "MadgraphPythia"   , TL::kGenerator::MadgraphPythia   } , { "MadgraphPythia8" , TL::kGenerator::MadgraphPythia8 } ,
    { "aMCatNLOPythia8"  , TL::kGenerator::aMCatNLOPythia8  } , { "aMCatNLOHerwig"  , TL::kGenerator::aMCatNLOHerwig  } ,
    { "aMCatNLOHerwigpp" , TL::kGenerator::aMCatNLOHerwigpp } , { "Unknown"         , TL::kGenerator::Unknown_2       }
  };
  const std::map<std::string,TL::kSampleType> s2eSampleType = {
    { "Data"       , TL::kSampleType::Data_3     } , { "Nominal" , TL::kSampleType::Nominal   } ,
    { "Systematic" , TL::kSampleType::Systematic } , { "Unknown" , TL::kSampleType::Unknown_3 }
  };
}

namespace TL {
  class SampleMetaSvc : public TObject {
  private:
    std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>> m_table;
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
  if ( istat == TL::kInitialState::Data_1  ) return "Data";
  if ( istat == TL::kInitialState::ttbar   ) return "ttbar";
  if ( istat == TL::kInitialState::Wt      ) return "Wt";
  if ( istat == TL::kInitialState::Zjets   ) return "Zjets";
  if ( istat == TL::kInitialState::Wjets   ) return "Wjets";
  if ( istat == TL::kInitialState::WW      ) return "WW";
  if ( istat == TL::kInitialState::WZ      ) return "WZ";
  if ( istat == TL::kInitialState::ZZ      ) return "ZZ";
  if ( istat == TL::kInitialState::Diboson ) return "Diboson";
  if ( istat == TL::kInitialState::ttbarZ  ) return "ttbarZ";
  if ( istat == TL::kInitialState::ttbarW  ) return "ttbarW";
  TL::Fatal(__PRETTY_FUNCTION__,"Unknown TL::kInitialState");
  return "";
}

inline const std::string TL::SampleMetaSvc::generatorString(const TL::kGenerator igen) const {
  if ( igen == TL::kGenerator::Data_2           ) return "Data";
  if ( igen == TL::kGenerator::PowhegPythia     ) return "PowhegPythia";
  if ( igen == TL::kGenerator::PowhegPythia8    ) return "PowhegPythia8";
  if ( igen == TL::kGenerator::PowhegHerwig     ) return "PowhegHerwig";
  if ( igen == TL::kGenerator::PowhegHerwigpp   ) return "PowhegHerwigpp";
  if ( igen == TL::kGenerator::Sherpa21         ) return "Sherpa21";
  if ( igen == TL::kGenerator::Sherpa22         ) return "Sherpa22";
  if ( igen == TL::kGenerator::Sherpa221        ) return "Sherpa221";
  if ( igen == TL::kGenerator::MadgraphPythia   ) return "MadgraphPythia";
  if ( igen == TL::kGenerator::MadgraphPythia8  ) return "MadgraphPythia8";
  if ( igen == TL::kGenerator::aMCatNLOPythia8  ) return "aMCatNLOPythia8";
  if ( igen == TL::kGenerator::aMCatNLOHerwig   ) return "aMCatNLOHerwig";
  if ( igen == TL::kGenerator::aMCatNLOHerwigpp ) return "aMCatNLOHerwigpp";
  if ( igen == TL::kGenerator::Unknown_2        ) return "Unknown";
  TL::Fatal(__PRETTY_FUNCTION__,"Unknown TL::kGenerator");
  return "";
}

inline const std::string TL::SampleMetaSvc::sampleTypeString(const TL::kSampleType ist) const {
  if ( ist == TL::kSampleType::Data_3     ) return "Data";
  if ( ist == TL::kSampleType::Nominal    ) return "Nominal";
  if ( ist == TL::kSampleType::Systematic ) return "Systematic";
  if ( ist == TL::kSampleType::Unknown_3  ) return "Unknown";
  TL::Fatal(__PRETTY_FUNCTION__,"Unknown TL::kSampleType");
  return "";
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

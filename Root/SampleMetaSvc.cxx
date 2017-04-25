/** @file SampleMetaSvc.cxx
 *  @brief TL::SampleMetaSvc class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

// TopLoop
#include <TopLoop/Core/SampleMetaSvc.h>
#include <TopLoop/json/json.hpp>

// PathResolver
#include <PathResolver/PathResolver.h>

TL::SampleMetaSvc::SampleMetaSvc() {
  setupMaps();
  std::string filepath = PathResolverFindCalibFile("TopLoop/samplemeta.json");
  std::ifstream in(filepath.c_str());
  if ( !in ) {
    TL::Fatal(PFUNC,"cannot fill meta service from file.",filepath,"cannot be found");
  }
  auto j_top = nlohmann::json::parse(in);

  // lambda function to check to see if str is a key in the map, apply
  // the value to applyto. used for each DSID in the loop that
  // follows.
  auto assigner = [](const auto& s2eMap, const auto str, auto& applyto) {
    if ( s2eMap.find(str) == s2eMap.end() ) {
      TL::Fatal(PFUNC,str,"is not setup in our software metadata");
    }
    else {
      applyto = s2eMap.at(str);
    }
  };

  for ( auto const& j_state : j_top) {
    for ( auto const& j_set : j_state ) {
      auto dsidMin = j_set.at("DSID_range").at(0).get<int>();
      auto dsidMax = j_set.at("DSID_range").at(1).get<int>();
      for ( int i = dsidMin; i <= dsidMax; ++i ) {
        TL::kInitialState initstate;
        TL::kGenerator    gen;
        TL::kSampleType   st;
        assigner(m_s2eInitialState,j_set.at("InitialState").get<std::string>(),initstate);
        assigner(m_s2eGenerator,   j_set.at("Generator").get<std::string>(),   gen);
        assigner(m_s2eSampleType,  j_set.at("SampleType").get<std::string>(),  st);
        m_table.emplace(i,std::make_tuple(initstate,gen,st));
      }
    }
  }
}

//___________________________________________________________________
TL::SampleMetaSvc::~SampleMetaSvc() {}

//___________________________________________________________________
// The structure of these maps is directly related to entries in
// samplemeta.txt file. If a new initial state, generator, or type is
// added to that file, this function must be updated.
void TL::SampleMetaSvc::setupMaps() {
  m_s2eInitialState = {
    { "Data"    , TL::kInitialState::Data_1  } ,
    { "ttbar"   , TL::kInitialState::ttbar   } ,
    { "Wt"      , TL::kInitialState::Wt      } ,
    { "Zjets"   , TL::kInitialState::Zjets   } ,
    { "Wjets"   , TL::kInitialState::Wjets   } ,
    { "WW"      , TL::kInitialState::WW      } ,
    { "WZ"      , TL::kInitialState::WZ      } ,
    { "ZZ"      , TL::kInitialState::ZZ      } ,
    { "Diboson" , TL::kInitialState::Diboson } ,
    { "ttbarW"  , TL::kInitialState::ttbarW  } ,
    { "ttbarZ"  , TL::kInitialState::ttbarZ  } ,
    { "ttbarll" , TL::kInitialState::ttbarll }
  };
  m_s2eGenerator = {
    { "Data"             , TL::kGenerator::Data_2            } ,
    { "PowhegPythia6"    , TL::kGenerator::PowhegPythia6     } ,
    { "PowhegPythia8"    , TL::kGenerator::PowhegPythia8     } ,
    { "PowhegPythia8_dil", TL::kGenerator::PowhegPythia8_dil } ,
    { "PowhegHerwig"     , TL::kGenerator::PowhegHerwig      } ,
    { "PowhegHerwigpp"   , TL::kGenerator::PowhegHerwigpp    } ,
    { "Sherpa21"         , TL::kGenerator::Sherpa21          } ,
    { "Sherpa22"         , TL::kGenerator::Sherpa22          } ,
    { "Sherpa221"        , TL::kGenerator::Sherpa221         } ,
    { "MadgraphPythia"   , TL::kGenerator::MadgraphPythia    } ,
    { "MadgraphPythia8"  , TL::kGenerator::MadgraphPythia8   } ,
    { "aMCatNLOPythia8"  , TL::kGenerator::aMCatNLOPythia8   } ,
    { "aMCatNLOHerwig"   , TL::kGenerator::aMCatNLOHerwig    } ,
    { "aMCatNLOHerwigpp" , TL::kGenerator::aMCatNLOHerwigpp  } ,
    { "Unknown"          , TL::kGenerator::Unknown_2         }
  };
  m_s2eSampleType = {
    { "Data"       , TL::kSampleType::Data_3     } ,
    { "Nominal"    , TL::kSampleType::Nominal    } ,
    { "Systematic" , TL::kSampleType::Systematic } ,
    { "Unknown"    , TL::kSampleType::Unknown_3  }
  };
  auto flipMap = [](auto const& templateMap, auto& newMap) {
    for ( auto const& templatePair : templateMap ) {
      newMap.emplace(templatePair.second,templatePair.first);
    }
  };
  flipMap(m_s2eInitialState,m_e2sInitialState);
  flipMap(m_s2eGenerator,   m_e2sGenerator);
  flipMap(m_s2eSampleType,  m_e2sSampleType);
}

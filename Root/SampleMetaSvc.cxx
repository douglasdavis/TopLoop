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
        TL::kMeta initstate;
        TL::kMeta gen;
        TL::kMeta st;
        assigner(m_s2e,j_set.at("InitialState").get<std::string>(),initstate);
        assigner(m_s2e,j_set.at("Generator").get<std::string>()   ,gen);
        assigner(m_s2e,j_set.at("SampleType").get<std::string>()  ,st);
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
  m_s2e = {
    { "Unknown"              , TL::kMeta::Unknown               } ,
      //
    { "Data"                 , TL::kMeta::Data                  } ,
    { "Nominal"              , TL::kMeta::Nominal		} ,
    { "Systematic"           , TL::kMeta::Systematic		} ,
      //
    { "ttbar"                , TL::kMeta::ttbar			} ,
    { "Wt"                   , TL::kMeta::Wt			} ,
    { "Zjets"                , TL::kMeta::Zjets			} ,
    { "Wjets"                , TL::kMeta::Wjets			} ,
    { "WW"                   , TL::kMeta::WW			} ,
    { "WZ"                   , TL::kMeta::WZ			} ,
    { "ZZ"                   , TL::kMeta::ZZ			} ,
    { "Diboson"              , TL::kMeta::Diboson		} ,
    { "ttbarW"               , TL::kMeta::ttbarW		} ,
    { "ttbarZ"               , TL::kMeta::ttbarZ		} ,
    { "ttbarll"              , TL::kMeta::ttbarll		} ,
    { "ttbarphi"             , TL::kMeta::ttbarphi		} ,
      //
    { "PowhegPythia6"        , TL::kMeta::PowhegPythia6         } ,
    { "PowhegPythia6_dil"    , TL::kMeta::PowhegPythia6_dil     } ,
    { "PowhegPythia8"        , TL::kMeta::PowhegPythia8         } ,
    { "PowhegPythia8_dil"    , TL::kMeta::PowhegPythia8_dil     } ,
    { "PowhegHerwig"         , TL::kMeta::PowhegHerwig          } ,
    { "PowhegHerwigpp"       , TL::kMeta::PowhegHerwigpp        } ,
    { "PowhegHerwig7"        , TL::kMeta::PowhegHerwig7         } ,
    { "Sherpa21"             , TL::kMeta::Sherpa21              } ,
    { "Sherpa22"             , TL::kMeta::Sherpa22              } ,
    { "Sherpa221"            , TL::kMeta::Sherpa221             } ,
    { "MadgraphPythia"       , TL::kMeta::MadgraphPythia        } ,
    { "MadgraphPythia8"      , TL::kMeta::MadgraphPythia8       } ,
    { "aMCatNLOPythia8"      , TL::kMeta::aMCatNLOPythia8       } ,
    { "aMCatNLOHerwig"       , TL::kMeta::aMCatNLOHerwig        } ,
    { "aMCatNLOHerwigpp"     , TL::kMeta::aMCatNLOHerwigpp      } ,
  };
  auto flipMap = [](auto const& templateMap, auto& newMap) {
    for ( auto const& templatePair : templateMap ) {
      newMap.emplace(templatePair.second,templatePair.first);
    }
  };
  flipMap(m_s2e, m_e2s);
}

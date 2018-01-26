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

TL::SampleMetaSvc::SampleMetaSvc() : TL::Loggable("TL::SampleMetaSvc") {
  setupMap();
  std::string filepath = PathResolverFindCalibFile("TopLoop/samplemeta.json");
  std::ifstream in(filepath.c_str());
  if ( !in ) {
    logger()->critical("cannot fill meta service from file. {} cannot be found", filepath);
  }
  auto j_top = nlohmann::json::parse(in);

  // lambda function to check to see if str is a key in the map, apply
  // the value to applyto. used for each DSID in the loop that
  // follows.
  auto assigner = [this](const auto& s2eMap, const auto str, auto& applyto) {
    if ( s2eMap.find(str) == s2eMap.end() ) {
      logger()->critical("{} is not setup in our software metadata!");
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
        TL::kGenerator gen;
        TL::kSampleType st;
        assigner(m_s2e_IS,j_set.at("InitialState").get<std::string>(),initstate);
        assigner(m_s2e_G, j_set.at("Generator").get<std::string>()   ,gen);
        assigner(m_s2e_ST,j_set.at("SampleType").get<std::string>()  ,st);
        m_sampleTable.emplace(i,std::make_tuple(initstate,gen,st));
      }
    }
  }
}

//___________________________________________________________________
TL::SampleMetaSvc::~SampleMetaSvc() {}

//___________________________________________________________________
// The structure of these maps is directly related to entries in
// samplemeta.json file. If a new initial state, generator, or type is
// added to that file, this function must be updated.
void TL::SampleMetaSvc::setupMap() {
  m_s2e_IS = {
    { "Unknown"              , TL::kInitialState::Unknown        } ,
    { "Data"                 , TL::kInitialState::Data           } ,
    { "ttbar"                , TL::kInitialState::ttbar          } ,
    { "Wt"                   , TL::kInitialState::Wt             } ,
    { "Zjets"                , TL::kInitialState::Zjets          } ,
    { "Wjets"                , TL::kInitialState::Wjets          } ,
    { "WW"                   , TL::kInitialState::WW             } ,
    { "WZ"                   , TL::kInitialState::WZ             } ,
    { "ZZ"                   , TL::kInitialState::ZZ             } ,
    { "Diboson"              , TL::kInitialState::Diboson        } ,
    { "ttbarW"               , TL::kInitialState::ttbarW         } ,
    { "ttbarZ"               , TL::kInitialState::ttbarZ         } ,
    { "ttbarll"              , TL::kInitialState::ttbarll        } ,
    { "ttbarphi"             , TL::kInitialState::ttbarphi       } ,
    { "tchan"                , TL::kInitialState::tchan          } ,
    { "schan"                , TL::kInitialState::schan          }
  };
  m_s2e_G = {
    { "Unknown"              , TL::kGenerator::Unknown           } ,
    { "Data"                 , TL::kGenerator::Data              } ,
    { "PowhegPythia6"        , TL::kGenerator::PowhegPythia6     } ,
    { "PowhegPythia6_dil"    , TL::kGenerator::PowhegPythia6_dil } ,
    { "PowhegPythia8"        , TL::kGenerator::PowhegPythia8     } ,
    { "PowhegPythia8_dil"    , TL::kGenerator::PowhegPythia8_dil } ,
    { "PowhegHerwig"         , TL::kGenerator::PowhegHerwig      } ,
    { "PowhegHerwigpp"       , TL::kGenerator::PowhegHerwigpp    } ,
    { "PowhegHerwig7"        , TL::kGenerator::PowhegHerwig7     } ,
    { "Sherpa21"             , TL::kGenerator::Sherpa21          } ,
    { "Sherpa22"             , TL::kGenerator::Sherpa22          } ,
    { "Sherpa221"            , TL::kGenerator::Sherpa221         } ,
    { "MadgraphPythia"       , TL::kGenerator::MadgraphPythia    } ,
    { "MadgraphPythia8"      , TL::kGenerator::MadgraphPythia8   } ,
    { "aMCatNLOPythia8"      , TL::kGenerator::aMCatNLOPythia8   } ,
    { "aMCatNLOHerwig"       , TL::kGenerator::aMCatNLOHerwig    } ,
    { "aMCatNLOHerwigpp"     , TL::kGenerator::aMCatNLOHerwigpp  }
  };
  m_s2e_ST = {
    { "Unknown"              , TL::kSampleType::Unknown          } ,
    { "Data"                 , TL::kSampleType::Data             } ,
    { "Nominal"              , TL::kSampleType::Nominal          } ,
    { "Systematic"           , TL::kSampleType::Systematic       }
  };
  m_s2e_C = {
    { "Unknown"              , TL::kCampaign::Unknown            } ,
    { "Data"                 , TL::kCampaign::Data               } ,
    { "MC16a"                , TL::kCampaign::MC16a              } ,
    { "MC16c"                , TL::kCampaign::MC16c              } ,
    { "MC16d"                , TL::kCampaign::MC16d              } ,
    { "MC16e"                , TL::kCampaign::MC16e              } ,
    { "MC16f"                , TL::kCampaign::MC16f              }
  };
  auto flipMap = [](const auto& templateMap, auto& newMap) {
    for ( auto const& templatePair : templateMap ) {
      newMap.emplace(templatePair.second,templatePair.first);
    }
  };
  flipMap(m_s2e_IS, m_e2s_IS);
  flipMap(m_s2e_G,  m_e2s_G);
  flipMap(m_s2e_ST, m_e2s_ST);
  flipMap(m_s2e_C,  m_e2s_C);

  m_rTags = {
    { "r9364"  , TL::kCampaign::MC16a } ,
    { "r9781"  , TL::kCampaign::MC16c } ,
    { "r10201" , TL::kCampaign::MC16d } ,
  };

}

TL::kCampaign TL::SampleMetaSvc::getCampaign(const std::string& sample_name) const {
  if ( sample_name.find("physics_Main") != std::string::npos ) {
    logger()->warn("You asked for the MC campaign related to a data sample!"
                   "Returning TL::kCampaign::Data");
    return TL::kCampaign::Data;
  }
  for ( const auto& entry : m_rTags ) {
    if ( sample_name.find(entry.first) != std::string::npos ) {
      return entry.second;
    }
  }
  logger()->warn("Cannot determine campain from rtag in sample {}", sample_name);
  logger()->warn("Returning  TL::kCampaign::Unknown");
  logger()->debug("Available identifiers (r9364, r9781, r10201)");
  return TL::kCampaign::Unknown;
}

void TL::SampleMetaSvc::printInfo(const int dsid) const {
  logger()->info("Sample DSID:      {}",dsid);
  logger()->info("Sample Process:   {}",getInitialStateStr(dsid));
  logger()->info("Sample Generator: {}",getGeneratorStr(dsid));
  logger()->info("Sample Type:      {}",getSampleTypeStr(dsid));
  return;
}

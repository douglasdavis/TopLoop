/** @file SampleMetaSvc.cxx
 *  @brief TL::SampleMetaSvc class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TopLoop
#include <TopLoop/Core/SampleMetaSvc.h>
#include <TopLoop/json/json.hpp>

// PathResolver
#include <PathResolver/PathResolver.h>

// C++
#include <regex>
#include <vector>

TL::SampleMetaSvc::SampleMetaSvc() : TL::Loggable("TL::SampleMetaSvc") {
  setupMaps();
  std::string filepath = PathResolverFindCalibFile("TopLoop/samplemeta.json");
  std::ifstream in(filepath.c_str());
  if ( in.bad() ) {
    logger()->error("cannot fill meta service from file. {} cannot be found", filepath);
  }
  auto j_top = nlohmann::json::parse(in);

  // lambda function to check to see if str is a key in the map, apply
  // the value to applyto. used for each DSID in the loop that
  // follows.
  auto assigner = [this](const auto& s2eMap, const auto str, auto& applyto) {
    if ( s2eMap.find(str) == s2eMap.end() ) {
      logger()->error("{} is not setup in our software metadata!");
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

  /// campaign luminosities
  std::string camp_filepath = PathResolverFindCalibFile("TopLoop/campaigns.json");
  std::ifstream camp_in(camp_filepath.c_str());
  if ( camp_in.bad() ) {
    logger()->error("cannot fill campaign metadata from file. {} cannot be found", camp_filepath);
  }
  auto j_camp = nlohmann::json::parse(camp_in);
  std::map<TL::kCampaign, float> campaignLumis_23 = {
    { TL::kCampaign::MC15c, j_camp.at("v23").at("MC15c").get<float>() } ,
    { TL::kCampaign::MC16a, j_camp.at("v23").at("MC16a").get<float>() } ,
    { TL::kCampaign::MC16c, j_camp.at("v23").at("MC16c").get<float>() } ,
    { TL::kCampaign::MC16d, j_camp.at("v23").at("MC16d").get<float>() } ,
    { TL::kCampaign::MC16e, j_camp.at("v23").at("MC16e").get<float>() } ,
    { TL::kCampaign::MC16f, j_camp.at("v23").at("MC16f").get<float>() } ,
    { TL::kCampaign::Data,  0.0 }
  };
  std::map<TL::kCampaign, float> campaignLumis_25 = {
    { TL::kCampaign::MC15c, j_camp.at("v25").at("MC15c").get<float>() } ,
    { TL::kCampaign::MC16a, j_camp.at("v25").at("MC16a").get<float>() } ,
    { TL::kCampaign::MC16c, j_camp.at("v25").at("MC16c").get<float>() } ,
    { TL::kCampaign::MC16d, j_camp.at("v25").at("MC16d").get<float>() } ,
    { TL::kCampaign::MC16e, j_camp.at("v25").at("MC16e").get<float>() } ,
    { TL::kCampaign::MC16f, j_camp.at("v25").at("MC16f").get<float>() } ,
    { TL::kCampaign::Data,  0.0 }
  };
  std::map<TL::kCampaign, float> campaignLumis_27 = {
    { TL::kCampaign::MC15c, j_camp.at("v27").at("MC15c").get<float>() } ,
    { TL::kCampaign::MC16a, j_camp.at("v27").at("MC16a").get<float>() } ,
    { TL::kCampaign::MC16c, j_camp.at("v27").at("MC16c").get<float>() } ,
    { TL::kCampaign::MC16d, j_camp.at("v27").at("MC16d").get<float>() } ,
    { TL::kCampaign::MC16e, j_camp.at("v27").at("MC16e").get<float>() } ,
    { TL::kCampaign::MC16f, j_camp.at("v27").at("MC16f").get<float>() } ,
    { TL::kCampaign::Data,  0.0 }
  };
  m_campaignLumis = {{TL::kSgTopNtup::v23, campaignLumis_23},
                     {TL::kSgTopNtup::v25, campaignLumis_25},
                     {TL::kSgTopNtup::v27, campaignLumis_27}};

  logger()->info("| {:>9} | {:>9} | {:>9} | {:>9} | {:>9} | {:>9} | {:>9} |",
                 "SgTopNtup","MC15c", "MC16a", "MC16c", "MC16d", "MC16e", "MC16f");
  for ( const auto& m : m_campaignLumis ) {
    logger()->info("| {:>9} | {:>9} | {:>9} | {:>9} | {:>9} | {:>9} | {:>9} |",
                   as_string(m.first),
                   m.second.at(TL::kCampaign::MC15c),
                   m.second.at(TL::kCampaign::MC16a),
                   m.second.at(TL::kCampaign::MC16c),
                   m.second.at(TL::kCampaign::MC16d),
                   m.second.at(TL::kCampaign::MC16e),
                   m.second.at(TL::kCampaign::MC16f));
  }
}

//___________________________________________________________________
TL::SampleMetaSvc::~SampleMetaSvc() {}

//___________________________________________________________________
// The structure of these maps is directly related to entries in
// samplemeta.json file. If a new initial state, generator, or type is
// added to that file, this function must be updated.
void TL::SampleMetaSvc::setupMaps() {
  m_s2e_IS = {
    { "Unknown"              , TL::kInitialState::Unknown        } ,
    { "Data"                 , TL::kInitialState::Data           } ,
    { "ttbar"                , TL::kInitialState::ttbar          } ,
    { "tW"                   , TL::kInitialState::tW             } ,
    { "tW_DR"                , TL::kInitialState::tW_DR          } ,
    { "tW_DS"                , TL::kInitialState::tW_DS          } ,
    { "Zjets"                , TL::kInitialState::Zjets          } ,
    { "Wjets"                , TL::kInitialState::Wjets          } ,
    { "WW"                   , TL::kInitialState::WW             } ,
    { "WZ"                   , TL::kInitialState::WZ             } ,
    { "ZZ"                   , TL::kInitialState::ZZ             } ,
    { "Diboson"              , TL::kInitialState::Diboson        } ,
    { "ttbarW"               , TL::kInitialState::ttbarW         } ,
    { "ttbarZ"               , TL::kInitialState::ttbarZ         } ,
    { "ttbarll"              , TL::kInitialState::ttbarll        } ,
    { "ttH"                  , TL::kInitialState::ttH            } ,
    { "ttt"                  , TL::kInitialState::ttt            } ,
    { "tttt"                 , TL::kInitialState::tttt           } ,
    { "tchan"                , TL::kInitialState::tchan          } ,
    { "schan"                , TL::kInitialState::schan          }
  };
  m_s2e_G = {
    { "Unknown"              , TL::kGenerator::Unknown             } ,
    { "Data"                 , TL::kGenerator::Data                } ,
    { "PowhegPythia6"        , TL::kGenerator::PowhegPythia6       } ,
    { "PowhegPythia6_dil"    , TL::kGenerator::PowhegPythia6_dil   } ,
    { "PowhegPythia8"        , TL::kGenerator::PowhegPythia8       } ,
    { "PowhegPythia8_dil"    , TL::kGenerator::PowhegPythia8_dil   } ,
    { "PowhegPythia8_had"    , TL::kGenerator::PowhegPythia8_had   } ,
    { "PowhegHerwig"         , TL::kGenerator::PowhegHerwig        } ,
    { "PowhegHerwigpp"       , TL::kGenerator::PowhegHerwigpp      } ,
    { "PowhegHerwig7"        , TL::kGenerator::PowhegHerwig7       } ,
    { "PowhegHerwig7_dil"    , TL::kGenerator::PowhegHerwig7_dil   } ,
    { "Sherpa21"             , TL::kGenerator::Sherpa21            } ,
    { "Sherpa22"             , TL::kGenerator::Sherpa22            } ,
    { "Sherpa221"            , TL::kGenerator::Sherpa221           } ,
    { "Sherpa222"            , TL::kGenerator::Sherpa222           } ,
    { "MadgraphPythia"       , TL::kGenerator::MadgraphPythia      } ,
    { "MadgraphPythia8"      , TL::kGenerator::MadgraphPythia8     } ,
    { "aMCatNLOPythia8"      , TL::kGenerator::aMCatNLOPythia8     } ,
    { "aMCatNLOPythia8_dil"  , TL::kGenerator::aMCatNLOPythia8_dil } ,
    { "aMCatNLOHerwig"       , TL::kGenerator::aMCatNLOHerwig      } ,
    { "aMCatNLOHerwigpp"     , TL::kGenerator::aMCatNLOHerwigpp    }
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
    { "MC16f"                , TL::kCampaign::MC16f              } ,
    { "MC15c"                , TL::kCampaign::MC15c              }
  };
  m_s2e_NT = {
    { "Unknown"              , TL::kSgTopNtup::Unknown           } ,
    { "v23"                  , TL::kSgTopNtup::v23               } ,
    { "v25"                  , TL::kSgTopNtup::v25               } ,
    { "v27"                  , TL::kSgTopNtup::v27               }
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
  flipMap(m_s2e_NT, m_e2s_NT);

  m_rTags = {
    { "r7676"  , TL::kCampaign::MC15c } ,
    { "r9364"  , TL::kCampaign::MC16a } ,
    { "r9781"  , TL::kCampaign::MC16c } ,
    { "r10201" , TL::kCampaign::MC16d } ,
    { "r10724" , TL::kCampaign::MC16e }
  };

}

TL::kCampaign TL::SampleMetaSvc::getCampaign(const std::string& sample_name) const {
  if ( sample_name.find("physics_Main") != std::string::npos ) {
    logger()->debug("You asked for the MC campaign related to a data sample! "
                    "Returning TL::kCampaign::Data");
    return TL::kCampaign::Data;
  }
  for ( const auto& entry : m_rTags ) {
    if ( sample_name.find(entry.first) != std::string::npos ) {
      return entry.second;
    }
  }
  logger()->warn("Cannot determine campain from rtag in sample {}", sample_name);
  logger()->warn("Returning TL::kCampaign::Unknown");
  logger()->debug("Available identifiers (r9364, r9781, r10201)");
  return TL::kCampaign::Unknown;
}

float TL::SampleMetaSvc::getLumi(const TL::kCampaign campaign) const {
  auto itr = m_campaignLumis.at(m_ntupVersion).find(campaign);
  if ( itr == std::end(m_campaignLumis.at(m_ntupVersion)) ) {
    logger()->error("Campaign {} doesn't have a luminosity",getCampaignStr(campaign));
    return 0;
  }
  return itr->second;
}

float TL::SampleMetaSvc::getLumi(const std::vector<TL::kCampaign>& campaigns) const {
  auto accumulator = [this](float val, const auto& icamp) {
    return (val + this->getLumi(icamp));
  };
  return std::accumulate(std::begin(campaigns),std::end(campaigns),0.0f,accumulator);
}

float TL::SampleMetaSvc::getCampaignWeight(const TL::kCampaign campaign,
                                           const std::vector<TL::kCampaign>& campaigns) const {
  return getLumi(campaign)/getLumi(campaigns);
}

float TL::SampleMetaSvc::getCampaignWeight(const std::string& rucioDir,
                                           const std::vector<TL::kCampaign>& campaigns) const {
  return getCampaignWeight(getCampaign(rucioDir),campaigns);
}

bool TL::SampleMetaSvc::isAFII(const std::string& sample_name, bool log_it) const {
  bool isdata = sample_name.find("physics_Main") != std::string::npos;
  if ( isdata ) {
    if ( log_it ) {
      logger()->info("This appears to be data");
    }
    return false;
  }
  std::regex af2regex("(_a[0-9]{3})");
  bool isaf2 = std::regex_search(sample_name,af2regex);
  if ( log_it ) {
    if ( isaf2  ) {
      logger()->info("This appears to be simulation type: AFII");
    }
    else {
      logger()->info("This appears to be simulation type: Full Sim");
    }
  }
  return isaf2;
}

TL::kSgTopNtup TL::SampleMetaSvc::getNtupleVersion(const std::string& sample_name, bool log_it) {
  std::regex v23reg("(v23)");
  std::regex v25reg("(v25)");
  std::regex v27reg("(v27)");
  bool is23 = std::regex_search(sample_name, v23reg);
  bool is25 = std::regex_search(sample_name, v25reg);
  bool is27 = std::regex_search(sample_name, v27reg);
  unsigned int isversionSum = 0;
  for ( auto const isversion : {is23, is25, is27} ) {
    isversionSum += static_cast<unsigned int>(isversion);
  }
  if ( isversionSum > 1 ) {
    logger()->warn("SgTopNtuple version degeneracy detected, set to Unknown.");
    m_ntupVersion = TL::kSgTopNtup::Unknown;
    return TL::kSgTopNtup::Unknown;
  }
  if ( is23 ) {
    if ( log_it ) logger()->info("Sample is v23 single top ntuple");
    m_ntupVersion = TL::kSgTopNtup::v23;
    return TL::kSgTopNtup::v23;
  }
  else if ( is25 ) {
    if ( log_it ) logger()->info("Sample is v25 single top ntuple");
    m_ntupVersion = TL::kSgTopNtup::v25;
    return TL::kSgTopNtup::v25;
  }
  else if ( is27 ) {
    if ( log_it ) logger()->info("Sample is v27 single top ntuple");
    m_ntupVersion = TL::kSgTopNtup::v27;
    return TL::kSgTopNtup::v27;
  }
  else {
    if ( log_it ) logger()->warn("Cannot determine single top ntuple version, set to v25");
    m_ntupVersion = TL::kSgTopNtup::v25;
    return TL::kSgTopNtup::v25;
  }
}

unsigned int TL::SampleMetaSvc::getYear(const unsigned int runNum) const {
  /////////////////////////////////////////////////////////////////////
  // These numbers were grabbed from SUSYTools:
  // athena/PhysicsAnalysis/SUSYPhys/SUSYTools/Root/SUSYObjDef_xAOD.cxx
  /////////////////////////////////////////////////////////////////////
  if      ( runNum < 290000 ) { return 15; }
  else if ( runNum < 320000 ) { return 16; }
  else if ( runNum < 350000 ) { return 17; }
  return 18;
}

bool TL::SampleMetaSvc::tWorTtbarPowPy8(const unsigned int d) const {
  auto initstate = getInitialState(d);
  auto gen       = getGenerator(d);
  std::vector<TL::kInitialState> tops = {
    kInitialState::ttbar,
    kInitialState::tW,
    kInitialState::tW_DR,
    kInitialState::tW_DS
  };
  bool is_top = std::find(std::begin(tops),std::end(tops),initstate) != tops.end();
  bool is_pp8 = (gen == kGenerator::PowhegPythia8 ||
                 gen == kGenerator::PowhegPythia8_dil ||
                 gen == kGenerator::PowhegPythia8_had);
  return (is_top && is_pp8);
}

void TL::SampleMetaSvc::printInfo(const int dsid) const {
  logger()->info("Sample DSID:      {}",dsid);
  logger()->info("Sample Process:   {}",getInitialStateStr(dsid));
  logger()->info("Sample Generator: {}",getGeneratorStr(dsid));
  logger()->info("Sample Type:      {}",getSampleTypeStr(dsid));
}

void TL::SampleMetaSvc::dump() {
  for ( const auto& entry : m_sampleTable ) {
    logger()->info("* {:>7} * {:>9} * {:>20} * {:>10} *",
                   entry.first,
                   as_string(std::get<0>(entry.second)),
                   as_string(std::get<1>(entry.second)),
                   as_string(std::get<2>(entry.second)));
  }
  for ( auto const& camp : m_campaignLumis ) {
    for ( auto const& cl : camp.second ) {
      logger()->info("Campaign  ({}) : {} has associated luminosity: {} /fb",
                     as_string(camp.first),as_string(cl.first),cl.second);
    }
  }
}

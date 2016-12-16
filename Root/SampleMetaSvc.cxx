/** @file SampleMetaSvc.cxx
 *  @brief TL::SampleMetaSvc class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

// TopLoop
#include <TopLoop/Core/SampleMetaSvc.h>

TL::SampleMetaSvc::SampleMetaSvc() {
  std::string ROOTCOREBIN = std::getenv("ROOTCOREBIN");
  if ( ROOTCOREBIN.empty() ) {
    TL::Fatal("ROOTCOREBIN environment variable must be set!");
  }
  setupMaps();
  std::string filepath = ROOTCOREBIN+"/data/TopLoop/samplemeta.txt";
  std::ifstream in(filepath.c_str());
  std::string line;
  if ( !in ) {
    TL::Fatal(__PRETTY_FUNCTION__,"cannot fill meta service from file.",filepath,"cannot be found");
  }

  // lambda function to check to see if str is a key in the map, apply
  // the value to applyto. used for each DSID in the loop that
  // follows.
  auto assigner = [](const auto& s2eMap, const auto& str, auto& applyto) {
    if ( s2eMap.find(str) == s2eMap.end() ) {
      TL::Fatal(__PRETTY_FUNCTION__,str,"is not setup in our software metadata");
    }
    else {
      applyto = s2eMap.at(str);
    }
  };

  for ( ; !in.eof() ; ) {
    if ( !std::getline(in,line) ) break;

    if ( !line.empty() && line.at(0) != '#' ) {
      std::istringstream istr(line);
      int dsidMin, dsidMax;
      std::string process, generator, type;
      istr >> dsidMin >> dsidMax >> process >> generator >> type;

      for ( int i = dsidMin; i <= dsidMax; ++i ) {
        TL::kInitialState initstate;
        TL::kGenerator    gen;
        TL::kSampleType   st;

        assigner(m_s2eInitialState, process,   initstate);
        assigner(m_s2eGenerator,    generator, gen);
        assigner(m_s2eSampleType,   type,      st);

        m_table.emplace(i,std::make_tuple(initstate,gen,st));
      } // loop over DSID range
    } // loop over text in the line
  } // loop over lines in file
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
    { "ttbarZ"  , TL::kInitialState::ttbarZ  }
  };
  m_s2eGenerator = {
    { "Data"             , TL::kGenerator::Data_2           } ,
    { "PowhegPythia"     , TL::kGenerator::PowhegPythia     } ,
    { "PowhegPythia8"    , TL::kGenerator::PowhegPythia8    } ,
    { "PowhegHerwig"     , TL::kGenerator::PowhegHerwig     } ,
    { "PowhegHerwigpp"   , TL::kGenerator::PowhegHerwigpp   } ,
    { "Sherpa21"         , TL::kGenerator::Sherpa21         } ,
    { "Sherpa22"         , TL::kGenerator::Sherpa22         } ,
    { "Sherpa221"        , TL::kGenerator::Sherpa221        } ,
    { "MadgraphPythia"   , TL::kGenerator::MadgraphPythia   } ,
    { "MadgraphPythia8"  , TL::kGenerator::MadgraphPythia8  } ,
    { "aMCatNLOPythia8"  , TL::kGenerator::aMCatNLOPythia8  } ,
    { "aMCatNLOHerwig"   , TL::kGenerator::aMCatNLOHerwig   } ,
    { "aMCatNLOHerwigpp" , TL::kGenerator::aMCatNLOHerwigpp } ,
    { "Unknown"          , TL::kGenerator::Unknown_2        }
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

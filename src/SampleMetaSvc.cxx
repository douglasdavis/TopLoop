/** @file SampleMetaSvc.cxx
 *  @brief TL::SampleMetaSvc class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

// TopLoop
#include <TopLoop/SampleMetaSvc.h>

TL::SampleMetaSvc::SampleMetaSvc() {
  std::string TL_INSTALL = std::getenv("TL_INSTALL");
  if ( TL_INSTALL.empty() ) {
    TL::Fatal("TL_INSTALL environment variable must be set!");
  }
  
  std::string filepath = TL_INSTALL+"/data/samplemeta.txt";
  std::ifstream in(filepath.c_str());
  std::string line;
  if ( !in ) {
    TL::Fatal(__PRETTY_FUNCTION__,"cannot fill meta service from file.",filepath,"cannot be found");
  }

  // lambda function to check to see if str is a key in the map, apply
  // the value to applyto. used for each DSID in the loop that
  // follows. The maps that we use below are defined in the header for
  // this class.
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

        assigner(TL::s2eInitialState, process,   initstate);
        assigner(TL::s2eGenerator,    generator, gen);
        assigner(TL::s2eSampleType,   type,      st);

        m_table.emplace(i,std::make_tuple(initstate,gen,st));
      } // loop over DSID range
    } // loop over text in the line
  } // loop over lines in file
}

TL::SampleMetaSvc::~SampleMetaSvc() {}

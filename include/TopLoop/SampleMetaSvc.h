/** @file  SampleMetaSvc.h
 *  @brief TL::SampleMetaSvc class header
 *
 *  A utility for connect a DSID with meta data.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

// C++
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <tuple>

// TL
#include <TopLoop/Utils.h>

namespace TL {
  enum kInitialState {
    Data_1,
    ttbar,
    Wt,
    Zjets,
    Wjets,
    WW,
    WZ,
    ZZ,
    Diboson,
    ttbarZ,
    ttbarW
  };
  enum kGenerator {
    Data_2,
    PowhegPythia,
    PowhegPythia8,
    PowhegHerwig,
    PowhegHerwigpp,
    Sherpa21,
    Sherpa22,
    Sherpa221,
    MadgraphPythia,
    MadgraphPythia8,
    aMCatNLOPythia8,
    aMCatNLOHerwig,
    aMCatNLOHerwigpp,
    Unknown_2
  };
  enum kSampleType {
    Data_3,
    Nominal,
    Systematic,
    Unknown_3
  };
}

namespace TL {
  class SampleMetaSvc : public TObject {
  private:
    std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>> m_table;
    ClassDef(TL::SampleMetaSvc,1);
  public:
    SampleMetaSvc() {      
      std::string TL_INSTALL = std::getenv("TL_INSTALL");
      if ( TL_INSTALL.empty() ) {
        TL::Fatal("TL_INSTALL environment variable must be set!");
      }
      std::string filepath = TL_INSTALL+"/data/samplemeta.txt";
      std::ifstream in(filepath.c_str());
      std::string line;
      if ( !in ) {
        TL::Fatal("kDSIDMETA cannot be filled because the txt file cannot be found");
      }
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
            if      ( process == "Data"    ) initstate = TL::kInitialState::Data_1;
            else if ( process == "ttbar"   ) initstate = TL::kInitialState::ttbar;
            else if ( process == "Wt"      ) initstate = TL::kInitialState::Wt;
            else if ( process == "Zjets"   ) initstate = TL::kInitialState::Zjets;
            else if ( process == "Wjets"   ) initstate = TL::kInitialState::Wjets;
            else if ( process == "WW"      ) initstate = TL::kInitialState::WW;
            else if ( process == "WZ"      ) initstate = TL::kInitialState::WZ;
            else if ( process == "ZZ"      ) initstate = TL::kInitialState::ZZ;
            else if ( process == "Diboson" ) initstate = TL::kInitialState::Diboson;
            else if ( process == "ttbarZ"  ) initstate = TL::kInitialState::ttbarZ;
            else if ( process == "ttbarW"  ) initstate = TL::kInitialState::ttbarW;
            else TL::Fatal("initial-state in samplemeta.txt not recognized:",process);
            if      ( generator == "Data"             ) gen = TL::kGenerator::Data_2;
            else if ( generator == "PowhegPythia"     ) gen = TL::kGenerator::PowhegPythia;
            else if ( generator == "PowhegPythia8"    ) gen = TL::kGenerator::PowhegPythia8;
            else if ( generator == "PowhegHerwig"     ) gen = TL::kGenerator::PowhegHerwig;
            else if ( generator == "PowhegHerwigpp"   ) gen = TL::kGenerator::PowhegHerwigpp;
            else if ( generator == "Sherpa21"         ) gen = TL::kGenerator::Sherpa21;
            else if ( generator == "Sherpa22"         ) gen = TL::kGenerator::Sherpa22;
            else if ( generator == "Sherpa221"        ) gen = TL::kGenerator::Sherpa221;
            else if ( generator == "MadgraphPythia"   ) gen = TL::kGenerator::MadgraphPythia;
            else if ( generator == "MadgraphPythia8"  ) gen = TL::kGenerator::MadgraphPythia8;
            else if ( generator == "aMCatNLOPythia8"  ) gen = TL::kGenerator::aMCatNLOPythia8;
            else if ( generator == "aMCatNLOHerwig"   ) gen = TL::kGenerator::aMCatNLOHerwig;
            else if ( generator == "aMCatNLOHerwigpp" ) gen = TL::kGenerator::aMCatNLOHerwigpp;
            else if ( generator == "Unknown"          ) gen = TL::kGenerator::Unknown_2;
            else TL::Fatal("generator in samplemeta.txt not recognized:",generator);
            if      ( type == "Data"       ) st = TL::kSampleType::Data_3;
            else if ( type == "Nominal"    ) st = TL::kSampleType::Nominal;
            else if ( type == "Systematic" ) st = TL::kSampleType::Systematic;
            else if ( type == "Unknown"    ) st = TL::kSampleType::Unknown_3;
            else TL::Fatal("sample type in samplemeta.txt not recognized:",type);

            m_table.emplace(i,std::make_tuple(initstate,gen,st));
          }
        }
      }
    }

    virtual ~SampleMetaSvc() {}

    const std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>>& table() const;

    TL::kInitialState  getInitialState(const unsigned int dsid)           const;
    TL::kGenerator     getGenerator(const unsigned int dsid)              const;
    TL::kSampleType    getSampleType(const unsigned int dsid)             const;

    const std::string initStateString(const TL::kInitialState istate)  const;
    const std::string generatorString(const TL::kGenerator igen)       const;
    const std::string sampleTypeString(const TL::kSampleType ist)      const;

    const std::string initStateString(const unsigned int dsid)  const;
    const std::string generatorString(const unsigned int dsid)  const;
    const std::string sampleTypeString(const unsigned int dsid) const;
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

inline const std::string TL::SampleMetaSvc::initStateString(const TL::kInitialState istat)  const {
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

inline const std::string TL::SampleMetaSvc::initStateString(const unsigned int dsid) const {
  return initStateString(getInitialState(dsid));
}

inline const std::string TL::SampleMetaSvc::generatorString(const unsigned int dsid) const {
  return generatorString(getGenerator(dsid));
}

inline const std::string TL::SampleMetaSvc::sampleTypeString(const unsigned int dsid) const {
  return sampleTypeString(getSampleType(dsid));
}

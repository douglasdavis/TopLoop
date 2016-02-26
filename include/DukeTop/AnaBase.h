/** @file  AnaBase.h
 *  @brief DT::AnaBase class header
 *  @class DT::AnaBase
 *  @brief Base class for running a
 *         DukeTop analysis algorithm
 *
 *  This class is a skeleton for the steps needed
 *  to run a DukeTop algorithm. 
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef DT_AnaBase_h
#define DT_AnaBase_h

// DT
#include <DukeTop/Utils.h>

// C++
#include <string>

// ROOT
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

namespace DT {
  class FileManager;
}

namespace DT {

  class AnaBase {

  protected:
    std::string      m_datasetName;
    DT::FileManager* m_fm;

    TTreeReader* m_reader;

    TTreeReaderValue<std::vector<float> >* el_pt;
    TTreeReaderValue<std::vector<float> >* el_phi;
    TTreeReaderValue<std::vector<float> >* el_eta;
    TTreeReaderValue<std::vector<float> >* el_e;

    TTreeReaderValue<std::vector<float> >* mu_pt;
    TTreeReaderValue<std::vector<float> >* mu_phi;
    TTreeReaderValue<std::vector<float> >* mu_eta;
    TTreeReaderValue<std::vector<float> >* mu_e;

    TTreeReaderValue<std::vector<float> >* jet_pt;
    TTreeReaderValue<std::vector<float> >* jet_eta;
    TTreeReaderValue<std::vector<float> >* jet_phi;
    TTreeReaderValue<std::vector<float> >* jet_e;
    TTreeReaderValue<std::vector<float> >* jet_mv1;
    TTreeReaderValue<std::vector<float> >* jet_mvb;

    TTreeReaderValue<std::vector<float> >* ljet_pt;
    TTreeReaderValue<std::vector<float> >* ljet_eta;
    TTreeReaderValue<std::vector<float> >* ljet_phi;
    TTreeReaderValue<std::vector<float> >* ljet_e;
    TTreeReaderValue<std::vector<float> >* ljet_m;
    TTreeReaderValue<std::vector<float> >* ljet_sd12;

    TTreeReaderValue<float>*         met_met;
    TTreeReaderValue<float>*         met_phi;

  public:
    AnaBase();
    virtual ~AnaBase();

    void core_init();
    void init_core_vars();
    
    virtual DT::STATUS init();
    virtual DT::STATUS setupOutput();
    virtual DT::STATUS execute();
    virtual DT::STATUS finish();

    DT::FileManager* fileManager();

    TTreeReader* reader();
    
  };

}

inline DT::FileManager* DT::AnaBase::fileManager() { return m_fm;     }
inline TTreeReader*     DT::AnaBase::reader()      { return m_reader; }

#endif

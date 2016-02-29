/** @file  AnaBase.h
 *  @brief TL::AnaBase class header
 *  @class TL::AnaBase
 *  @brief Base class for running a
 *         TopLoop analysis algorithm
 *
 *  This class is a skeleton for the steps needed
 *  to run a TopLoop algorithm. 
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_AnaBase_h
#define TL_AnaBase_h

// TL
#include <TopLoop/Utils.h>

// C++
#include <string>

// ROOT
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

namespace TL {
  class FileManager;
}

namespace TL {

  class AnaBase {

  protected:
    std::string      m_datasetName;
    TL::FileManager* m_fm;

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

    TTreeReaderValue<float>*         met_met;
    TTreeReaderValue<float>*         met_phi;

  public:
    AnaBase();
    virtual ~AnaBase();

    void core_init();
    void init_core_vars();
    
    virtual TL::STATUS init();
    virtual TL::STATUS setupOutput();
    virtual TL::STATUS execute();
    virtual TL::STATUS finish();

    TL::FileManager* fileManager();

    TTreeReader* reader();
    
  };

}

inline TL::FileManager* TL::AnaBase::fileManager() { return m_fm;     }
inline TTreeReader*     TL::AnaBase::reader()      { return m_reader; }

#endif

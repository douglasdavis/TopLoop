/** @file AnaBase.cxx
 *  @brief TL::AnaBase class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

// TL
#include <TopLoop/AnaBase.h>
#include <TopLoop/FileManager.h>

TL::AnaBase::AnaBase() :
  m_datasetName() {
  core_init();
}

TL::AnaBase::~AnaBase() {}

void TL::AnaBase::core_init() {
  m_fm = new TL::FileManager();
}

void TL::AnaBase::init_core_vars() {
  m_reader = new TTreeReader(fileManager()->rootChain());

  el_pt  = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_pt");
  el_eta = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_eta");
  el_phi = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_pt");
  el_e   = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_e");

  mu_pt  = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_pt");
  mu_eta = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_eta");
  mu_phi = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_pt");
  mu_e   = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_e");

  jet_pt  = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_pt");
  jet_eta = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_eta");
  jet_phi = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_phi");
  jet_e   = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_e");

  jet_mv1 = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_mv1");
  jet_mvb = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_mvb");

  met_met   = new TTreeReaderValue<float>(*m_reader,"met_met");
  met_phi   = new TTreeReaderValue<float>(*m_reader,"met_phi");

  // These variables are currently commented out as they are not part
  // of the default top group ntuple. TODO: set a function to init
  // these variables if called.
  /*
    el_n   = new TTreeReaderValue<UInt_t>(*m_reader,"el_n");
    mu_n   = new TTreeReaderValue<UInt_t>(*m_reader,"mu_n");
    jet_n  = new TTreeReaderValue<UInt_t>(*m_reader,"jet_n");
    met_px    = new TTreeReaderValue<float>(*m_reader,"met_px");
    met_py    = new TTreeReaderValue<float>(*m_reader,"met_py");
    met_sumet = new TTreeReaderValue<float>(*m_reader,"met_sumet");
    Ht = new TTreeReaderValue<float>(*m_reader,"Ht");
  */

}

TL::STATUS TL::AnaBase::init() {
  TL::Warning("init()","This is the base init() class, doesn't do anything!");
  init_core_vars();
  return TL::STATUS::Good;
}

TL::STATUS TL::AnaBase::setupOutput() {
  TL::Warning("setupOutput()","This is the base setupOutput() function, doesn't do anything!");
  return TL::STATUS::Good;
}

TL::STATUS TL::AnaBase::execute() {
  TL::Warning("execute()","This is the base execute() function, doesn't do anything!");
  return TL::STATUS::Good;
}

TL::STATUS TL::AnaBase::finish() {
  TL::Warning("finish()","This is the base finish() function, doesn't do anything!");
  return TL::STATUS::Good;
}

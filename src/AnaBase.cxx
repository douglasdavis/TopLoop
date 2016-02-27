/** @file AnaBase.cxx
 *  @brief DT::AnaBase class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

// DT
#include <DukeTop/AnaBase.h>
#include <DukeTop/FileManager.h>

DT::AnaBase::AnaBase() :
  m_datasetName() {
  core_init();
}

DT::AnaBase::~AnaBase() {}

void DT::AnaBase::core_init() {
  m_fm = new DT::FileManager();
}

void DT::AnaBase::init_core_vars() {
  m_reader = new TTreeReader(fileManager()->rootChain());

  el_n   = new TTreeReaderValue<UInt_t>(*m_reader,"el_n");
  el_pt  = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_pt");
  el_eta = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_eta");
  el_phi = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_pt");
  el_e   = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_e");

  mu_n   = new TTreeReaderValue<UInt_t>(*m_reader,"mu_n");
  mu_pt  = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_pt");
  mu_eta = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_eta");
  mu_phi = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_pt");
  mu_e   = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_e");

  jet_n   = new TTreeReaderValue<UInt_t>(*m_reader,"jet_n");
  jet_pt  = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_pt");
  jet_eta = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_eta");
  jet_phi = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_phi");
  jet_e   = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_e");

  jet_mv1 = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_mv1");
  jet_mvb = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_mvb");

  met_met   = new TTreeReaderValue<float>(*m_reader,"met_met");
  met_phi   = new TTreeReaderValue<float>(*m_reader,"met_phi");
  met_px    = new TTreeReaderValue<float>(*m_reader,"met_px");
  met_py    = new TTreeReaderValue<float>(*m_reader,"met_py");
  met_sumet = new TTreeReaderValue<float>(*m_reader,"met_sumet");

  Ht = new TTreeReaderValue<float>(*m_reader,"Ht");
}

DT::STATUS DT::AnaBase::init() {
  DT::Warning("init()","This is the base init() class, doesn't do anything!");
  init_core_vars();
  return DT::STATUS::Good;
}

DT::STATUS DT::AnaBase::setupOutput() {
  DT::Warning("setupOutput()","This is the base setupOutput() function, doesn't do anything!");
  return DT::STATUS::Good;
}

DT::STATUS DT::AnaBase::execute() {
  DT::Warning("execute()","This is the base execute() function, doesn't do anything!");
  return DT::STATUS::Good;
}

DT::STATUS DT::AnaBase::finish() {
  DT::Warning("finish()","This is the base finish() function, doesn't do anything!");
  return DT::STATUS::Good;
}

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

  el_pt     = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_pt");
  el_eta    = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_eta");
  el_phi    = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_pt");
  el_e      = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_e");
  el_charge = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_charge");

  el_topoetcone20      = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_topoetcone20");
  el_ptvarcone20       = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_ptvarcone20");
  el_d0sig             = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_d0sig");
  el_delta_z0_sintheta = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_delta_z0_sintheta");
  
  mu_pt     = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_pt");
  mu_eta    = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_eta");
  mu_phi    = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_pt");
  mu_e      = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_e");
  mu_charge = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_charge");

  mu_topoetcone20      = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_topoetcone20");
  mu_ptvarcone20       = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_ptvarcone30");
  mu_d0sig             = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_d0sig");
  mu_delta_z0_sintheta = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_delta_z0_sintheta");

  jet_pt  = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_pt");
  jet_eta = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_eta");
  jet_phi = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_phi");
  jet_e   = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_e");

  jet_mv1 = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_mv1");
  jet_mvb = new TTreeReaderValue<std::vector<float> >(*m_reader,"jet_mvb");

  met_met = new TTreeReaderValue<float>(*m_reader,"met_met");
  met_phi = new TTreeReaderValue<float>(*m_reader,"met_phi");

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

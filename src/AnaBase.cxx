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
  m_fm = std::make_shared<TL::FileManager>();
}

void TL::AnaBase::init_core_vars() {
  m_reader = std::make_shared<TTreeReader>(fileManager()->rootChain());

  weight_mc          = std::make_shared<TTRV_float>(*m_reader,"weight_mc");
  weight_pileup      = std::make_shared<TTRV_float>(*m_reader,"weight_pileup");
  weight_leptonSF    = std::make_shared<TTRV_float>(*m_reader,"weight_leptonSF");
  weight_pileup_UP   = std::make_shared<TTRV_float>(*m_reader,"weight_pileup_UP");
  weight_pileup_DOWN = std::make_shared<TTRV_float>(*m_reader,"weight_pileup_DOWN");
  
  eventNumber     = std::make_shared<TTRV_ulongint>(*m_reader,"eventNumber");
  runNumber       = std::make_shared<TTRV_uint>    (*m_reader,"runNumber");
  mcChannelNumber = std::make_shared<TTRV_uint>    (*m_reader,"mcChannelNumber");
  mu              = std::make_shared<TTRV_float>   (*m_reader,"mu");
  
  el_pt     = std::make_shared<TTRV_vec_float>(*m_reader,"el_pt");
  el_eta    = std::make_shared<TTRV_vec_float>(*m_reader,"el_eta");
  el_phi    = std::make_shared<TTRV_vec_float>(*m_reader,"el_pt");
  el_e      = std::make_shared<TTRV_vec_float>(*m_reader,"el_e");
  el_charge = std::make_shared<TTRV_vec_float>(*m_reader,"el_charge");
  el_cl_eta = std::make_shared<TTRV_vec_float>(*m_reader,"el_cl_eta");

  el_topoetcone20      = std::make_shared<TTRV_vec_float>(*m_reader,"el_topoetcone20");
  el_ptvarcone20       = std::make_shared<TTRV_vec_float>(*m_reader,"el_ptvarcone20");
  el_d0sig             = std::make_shared<TTRV_vec_float>(*m_reader,"el_d0sig");
  el_delta_z0_sintheta = std::make_shared<TTRV_vec_float>(*m_reader,"el_delta_z0_sintheta");
  
  mu_pt     = std::make_shared<TTRV_vec_float>(*m_reader,"mu_pt");
  mu_eta    = std::make_shared<TTRV_vec_float>(*m_reader,"mu_eta");
  mu_phi    = std::make_shared<TTRV_vec_float>(*m_reader,"mu_pt");
  mu_e      = std::make_shared<TTRV_vec_float>(*m_reader,"mu_e");
  mu_charge = std::make_shared<TTRV_vec_float>(*m_reader,"mu_charge");

  mu_topoetcone20      = std::make_shared<TTRV_vec_float>(*m_reader,"mu_topoetcone20");
  mu_ptvarcone30       = std::make_shared<TTRV_vec_float>(*m_reader,"mu_ptvarcone30");
  mu_d0sig             = std::make_shared<TTRV_vec_float>(*m_reader,"mu_d0sig");
  mu_delta_z0_sintheta = std::make_shared<TTRV_vec_float>(*m_reader,"mu_delta_z0_sintheta");

  jet_pt  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_pt");
  jet_eta = std::make_shared<TTRV_vec_float>(*m_reader,"jet_eta");
  jet_phi = std::make_shared<TTRV_vec_float>(*m_reader,"jet_phi");
  jet_e   = std::make_shared<TTRV_vec_float>(*m_reader,"jet_e");

  jet_mv1     = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv1");
  jet_mvb     = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mvb");
  jet_mv1c    = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv1c");
  jet_mv2c00  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv2c00");
  jet_mv2c10  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv2c10");
  jet_mv2c20  = std::make_shared<TTRV_vec_float>(*m_reader,"jet_mv2c20");
  jet_ip3dsv1 = std::make_shared<TTRV_vec_float>(*m_reader,"jet_ip3dsv1");
  jet_jvt     = std::make_shared<TTRV_vec_float>(*m_reader,"jet_jvt");

  met_met = std::make_shared<TTRV_float>(*m_reader,"met_met");
  met_phi = std::make_shared<TTRV_float>(*m_reader,"met_phi");
  
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

/** @file AnaBase.cxx
 *  @brief DT::AnaBase class implementation
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

// C++
#include <iostream>

// DT
#include <DukeTop/AnaBase.h>
#include <DukeTop/FileManager.h>
#include <DukeTop/Utils.h>

// ROOT
#include <TLorentzVector.h>

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

  el_pt  = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_pt");
  el_eta = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_eta");
  el_phi = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_pt");
  el_e   = new TTreeReaderValue<std::vector<float> >(*m_reader,"el_e");

  mu_pt  = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_pt");
  mu_eta = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_eta");
  mu_phi = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_pt");
  mu_e   = new TTreeReaderValue<std::vector<float> >(*m_reader,"mu_e");
}

void DT::AnaBase::init() {
  std::cout << "init standard AnaBase" << std::endl;
  init_core_vars();
}

void DT::AnaBase::setupOutput() {
  std::cout << "setupOutput standard AnaBase" << std::endl;
}

void DT::AnaBase::execute() {
  std::cout << "execute standard AnaBase" << std::endl;
}

void DT::AnaBase::finish() {
  std::cout << "finish standard AnaBase" << std::endl;
}

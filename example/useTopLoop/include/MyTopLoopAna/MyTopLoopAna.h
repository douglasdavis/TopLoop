#ifndef MyTopLoopAna_h
#define MyTopLoopAna_h

// TL
#include <TopLoop/AnaBase.h>

class TH1D;
class TFile;

class MyTopLoopAna : public TL::AnaBase {

private:
  int m_eventCounter;
  bool m_singleTopNtuple;
  
  TFile* m_outputFile;
  TH1D*  h_eventMass;
  TH1D*  h_eventHt;

  // The below variables are not included in the default top group
  // ntuple, therefore we include them in our own algorithm
  TTreeReaderValue<UInt_t>* el_n;
  TTreeReaderValue<UInt_t>* mu_n;
  TTreeReaderValue<UInt_t>* jet_n;
  TTreeReaderValue<float>*  met_px;
  TTreeReaderValue<float>*  met_py;
  TTreeReaderValue<float>*  met_sumet;
  TTreeReaderValue<float>*  Ht;

public:
  MyTopLoopAna();
  virtual ~MyTopLoopAna();

  virtual TL::STATUS init();
  virtual TL::STATUS setupOutput();
  virtual TL::STATUS execute();
  virtual TL::STATUS finish();

  void singleTopNtuple();
  
};

inline void MyTopLoopAna::singleTopNtuple() { m_singleTopNtuple = true; }

#endif

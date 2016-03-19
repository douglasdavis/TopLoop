#ifndef MyTopLoopAna_h
#define MyTopLoopAna_h

// ROOT
#include <TTree.h>

// TL
#include <TopLoop/AnaBase.h>
#include <TopLoopEDM/FinalState.h>

class TH1D;
class TFile;

class MyTopLoopAna : public TL::AnaBase {

private:
  int m_eventCounter;
  
  TFile* m_outputFile;
  TH1D*  h_eventMass;
  TH1D*  h_eventHt;
  TTree* m_outTree;

  TL::EDM::FinalState m_finalState;
  
  // The below variables are not included in the default top group
  // ntuple, therefore we include them in our own algorithm
  TTreeReaderValue<float>*  ht;
  TTreeReaderValue<float>*  eT_miss;

public:
  MyTopLoopAna();
  virtual ~MyTopLoopAna();

  virtual TL::STATUS init();
  virtual TL::STATUS setupOutput();
  virtual TL::STATUS execute();
  virtual TL::STATUS finish();

};

#endif

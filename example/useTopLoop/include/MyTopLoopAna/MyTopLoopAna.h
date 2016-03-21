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
  TTree* m_outTree;

  TL::EDM::FinalState m_finalState;
  
public:
  MyTopLoopAna();
  virtual ~MyTopLoopAna();

  virtual TL::STATUS init();
  virtual TL::STATUS setupOutput();
  virtual TL::STATUS execute();
  virtual TL::STATUS finish();

};

#endif

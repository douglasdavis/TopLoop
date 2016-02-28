#ifndef MyTopLoopAna_h
#define MyTopLoopAna_h

// TL
#include <TopLoop/AnaBase.h>

class TH1D;
class TFile;

class MyTopLoopAna : public TL::AnaBase {

private:
  int m_eventCounter;

  TFile* m_outputFile;
  TH1D*  h_eventMass;
  TH1D*  h_eventHt;
  
public:
  MyTopLoopAna();
  virtual ~MyTopLoopAna();

  virtual TL::STATUS init();
  virtual TL::STATUS setupOutput();
  virtual TL::STATUS execute();
  virtual TL::STATUS finish();
  
};

#endif

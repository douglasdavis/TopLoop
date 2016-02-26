#ifndef MyDukeTopAna_h
#define MyDukeTopAna_h

// DT
#include <DukeTop/AnaBase.h>

class TH1D;
class TFile;

class MyDukeTopAna : public DT::AnaBase {

private:
  int m_eventCounter;

  TFile* m_outputFile;
  TH1D*  h_eventMass;
  
public:
  MyDukeTopAna();
  virtual ~MyDukeTopAna();

  virtual DT::STATUS init();
  virtual DT::STATUS setupOutput();
  virtual DT::STATUS execute();
  virtual DT::STATUS finish();
  
};

#endif

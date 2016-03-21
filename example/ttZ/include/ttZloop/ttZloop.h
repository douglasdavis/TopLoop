#ifndef ttZloop_h
#define ttZloop_h

// ROOT
#include <TTree.h>

// TL
#include <TopLoop/AnaBase.h>
#include <TopLoopEDM/FinalState.h>

class TH1D;
class TFile;

class ttZloop : public TL::AnaBase {

private:
  int m_eventCounter;
  std::string m_outfileName;
  TFile* m_outputFile;
  TH1D*  h_eventMass;
  TH1D*  h_eventHt;
  TTree* m_nominalEDMTree;

  TL::EDM::FinalState m_finalState;
  
  // The below variables are not included in the default top group
  // ntuple, therefore we include them in our own algorithm
  TTreeReaderValue<float>*  ht;
  TTreeReaderValue<float>*  eT_miss;

public:
  ttZloop();
  virtual ~ttZloop();

  void setOutfileName(const std::string& outname);
  
  virtual TL::STATUS init();
  virtual TL::STATUS setupOutput();
  virtual TL::STATUS execute();
  virtual TL::STATUS finish();

};

inline void ttZloop::setOutfileName(const std::string& outname) {
  m_outfileName = outname;
}

#endif

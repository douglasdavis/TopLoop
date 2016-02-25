#ifndef MyDukeTopAna_h
#define MyDukeTopAna_h

// DT
#include <DukeTop/AnaBase.h>

class MyDukeTopAna : public DT::AnaBase {

private:
  int m_eventCounter;
  
public:
  MyDukeTopAna();
  virtual ~MyDukeTopAna();

  virtual void init();
  virtual void setupOutput();
  virtual void execute();
  virtual void finish();
  
};

#endif

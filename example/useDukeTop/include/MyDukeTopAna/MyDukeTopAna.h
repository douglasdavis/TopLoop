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

  virtual DT::STATUS init();
  virtual DT::STATUS execute();
  virtual DT::STATUS finish();
  
};

#endif

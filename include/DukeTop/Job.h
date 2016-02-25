/** @file  Job.h
 *  @brief DT::Job class header
 *  @class DT::Job
 *  @brief Job handler.
 *
 *  This class handles the running of the DT::AnaBase algorithm.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef DT_Job_h
#define DT_Job_h

namespace DT {
  class AnaBase;
}

namespace DT {

  class Job {

  protected:
    DT::AnaBase* m_analysis;

  public:
    Job(DT::AnaBase* ab) :
      m_analysis(ab) {}
    virtual ~Job() {}

    void run();

  };

}

#endif

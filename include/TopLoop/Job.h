/** @file  Job.h
 *  @brief TL::Job class header
 *  @class TL::Job
 *  @brief Job handler.
 *
 *  This class handles the running of the TL::AnaBase algorithm.
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

#ifndef TL_Job_h
#define TL_Job_h

namespace TL {
  class AnaBase;
}

namespace TL {

  class Job {

  protected:
    TL::AnaBase* m_analysis;

  public:
    Job() :
      m_analysis(nullptr)
    {}
    Job(TL::AnaBase* ab) :
      m_analysis(ab) {}
    virtual ~Job() {}

    void run();

  };

}

#endif

/** @file  Job.h
 *  @brief TL::Job class header
 *  @class TL::Job
 *  @brief Job handler.
 *
 *  This class handles the running of the TL::Algorithm algorithm.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

#ifndef TL_Job_h
#define TL_Job_h

#include <TopLoop/Core/Utils.h>

namespace TL {
  class Algorithm;
}

namespace TL {

  class Job {

  protected:
    TL::Algorithm* m_analysis;

  public:
    Job() :
      m_analysis(nullptr)
    {}
    Job(TL::Algorithm* ab) :
      m_analysis(ab)
    {}
    virtual ~Job() {}

    TL::StatusCode run();

  };

}

#endif

/** @file  Job.h
 *  @brief TL::Job class header
 *  @class TL::Job
 *  @brief Handles running a TopLoop algorithm.
 *
 *  This class handles the running of the TL::Algorithm algorithm.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
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

    /// default unusable constructor
    Job() : m_analysis(nullptr) {}

    /// constructor which is fed with algorithm pointer
    Job(TL::Algorithm* ab) : m_analysis(ab) {}

    /// detructor
    virtual ~Job() {}

    /// launches the TL::Algorithm and checks the steps.
    TL::StatusCode run();

  };

}

#endif

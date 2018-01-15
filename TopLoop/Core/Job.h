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
#include <TopLoop/Core/Loggable.h>

namespace TL {
  class Algorithm;
  class FileManager;
}

namespace TL {

  class Job : public TL::Loggable {

  protected:
    TL::Algorithm* m_algorithm = nullptr;
    std::unique_ptr<TL::FileManager> m_fm{nullptr};

  public:

    /// default unusable constructor
    Job();

    /// detructor
    virtual ~Job();

    /// function to set the algorithm the job will run
    TL::StatusCode setAlgorithm(TL::Algorithm* alg);

    /// function to set the file manager for the job
    TL::StatusCode setFileManager(std::unique_ptr<TL::FileManager> fm);

    /// launches the TL::Algorithm and checks the steps.
    TL::StatusCode run();

  };

}

#endif

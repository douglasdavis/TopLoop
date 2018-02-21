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
    std::unique_ptr<TL::Algorithm>   m_algorithm{nullptr};
    std::unique_ptr<TL::FileManager> m_fm{nullptr};

  public:

    /// default constructor
    Job();
    /// detructor
    virtual ~Job();

    /// delete copy constructor
    Job(const Job&) = delete;
    /// delete move constructor
    Job(Job&&) = delete;
    /// delete assignemnt operator
    Job& operator=(const Job&) = delete;
    /// delete move assignment operator
    Job& operator=(Job&&) = delete;

    /// function to set the algorithm the job will run
    TL::StatusCode setAlgorithm(std::unique_ptr<TL::Algorithm> alg);

    /// function to set the file manager for the job
    TL::StatusCode setFileManager(std::unique_ptr<TL::FileManager> fm);

    /// launches the TL::Algorithm and checks the steps.
    TL::StatusCode run();

  };

}

#endif

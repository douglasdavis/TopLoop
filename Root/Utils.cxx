/** @file Utils.cxx
 *  @brief TL::Utils functions and TL::StatusCode class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#include <TopLoop/Core/Utils.h>

namespace {

  /// Class used behind the scenes to keep track of the unchecked status codes
  ///
  /// One object of this type is created, just in case there are unchecked
  /// status codes encountered during the job. This single object is finally
  /// deleted at the end of the job, printing some complaints for the user.
  ///
  class UncheckedCounter {

  public:
    /// Default constructor
    UncheckedCounter() : m_uncheckedSuccess( 0 ), m_uncheckedFailure( 0 ) {}
    /// Destructor
    ~UncheckedCounter() {
      std::cerr << "Warning in <TL::StatusCode>:" << std::endl;
      std::cerr << "Warning in <TL::StatusCode>: "
                << "Unchecked status codes encountered during the job"
                << std::endl;
      if( m_uncheckedSuccess ) {
        std::cerr << "Warning in <TL::StatusCode>: "
                  << "Number of unchecked successes: "
                  << m_uncheckedSuccess << std::endl;
      }
      if( m_uncheckedFailure ) {
        std::cerr << "Error in   <TL::StatusCode>: "
                  << "Number of unchecked failures: "
                  << m_uncheckedFailure << std::endl;
      }
      // Let him/her know how to look up the unchecked codes in the easiest
      // way:
      std::cerr << "Warning in <TL::StatusCode>: "
                << "To fail on an unchecked code, call "
                << "TL::StatusCode::enableFailure() at the job's start"
                << std::endl;
      std::cerr << "Warning in <TL::StatusCode>:" << std::endl;
    }

    /// Number of unchecked successful status codes
    int m_uncheckedSuccess;
    /// Number of unchecked failure status codes
    int m_uncheckedFailure;

  }; // class UncheckedCounter

} // private namespace

/// Application-wide setup of whether to fail on unchecked status codes.
///
/// By default applications will not fail on an uncecked status code, they
/// will just keep track of how many status codes were left unchecked by
/// the user.
///
static bool s_failure = false;

TL::StatusCode::StatusCode(unsigned long rstat)
  : m_code(rstat), m_checked(false) {
}

TL::StatusCode::StatusCode(const StatusCode& parent)
  : m_code(parent.m_code), m_checked(false) {

  // Mark the parent's code checked:
  parent.m_checked = true;
}

TL::StatusCode::~StatusCode() {
  if( !m_checked ) {
    // If we are supposed to fail, let's fail right away:
    if ( s_failure ) {
      std::cerr << "Fatal in <TL::StatusCode::~StatusCode> "
                << "Unchecked status code encountered" << std::endl;
      std::abort();
    }
    // Global variable for keeping track of unchecked return codes.
    // It gets deleted only at the end of the process.
    static ::UncheckedCounter s_counter;
    if ( m_code == SUCCESS ) {
      s_counter.m_uncheckedSuccess += 1;
    } else if( m_code == FAILURE ) {
      s_counter.m_uncheckedFailure += 1;
    } else {
      std::cerr << "Fatal in <TL::StatusCode::~StatusCode> "
                << "Unknown status code encountered" << std::endl;
      std::abort();
    }
  }
}

TL::StatusCode& TL::StatusCode::operator= (const StatusCode& rhs) {
  // Check if we need to do anything:
  if( this == &rhs ) {
    return *this;
  }
  // Get the code from the other object:
  m_code = rhs.m_code;
  // Set the checked status flags correctly:
  m_checked = false;
  rhs.m_checked = true;
  // Return this object:
  return *this;
}

TL::StatusCode& TL::StatusCode::operator= (unsigned long code) {
  // Set the code:
  m_code = code;
  // Set the object unchecked:
  m_checked = false;
  // Return this object:
  return *this;
}

bool TL::StatusCode::isSuccess() const {
  setChecked();
  return ( m_code == SUCCESS );
}

bool TL::StatusCode::isFailure() const {
  setChecked();
  return (!isSuccess());
}

TL::StatusCode::operator unsigned long() const {
  setChecked();
  return m_code;
}

void TL::StatusCode::enableFailure() {
  s_failure = true;
  return;
}

void TL::StatusCode::disableFailure() {
  s_failure = false;
  return;
}

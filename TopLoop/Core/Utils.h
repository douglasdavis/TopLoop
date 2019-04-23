/** @file  Utils.h
 *  @brief TL::Utils class header
 *  @namespace TL
 *  @brief the main TopLoop namespace
 *  @namespace TL::EDM
 *  @brief the TopLoop event data model (EDM) namespace
 *
 *  Utility functions for TopLoop
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
 */

#ifndef TL_Utils_h
#define TL_Utils_h

#define TeV 1000000.0
#define GeV 1000.0
#define toGeV 0.001
#define toSqrtGeV 0.0316227766

namespace TL {
class StatusCode {
  /** @class TL::StatusCode
   *  @brief Class for handling return codes
   *
   *  Essentially a copy of StatusCode from ATLAS AsgTools.
   *  You can find the documentation there :)
   */
 public:
  enum { SUCCESS = 1, FAILURE = 0 };
  /// Constructor from an integer status code
  StatusCode(unsigned long rstat = SUCCESS);
  /// Copy constructor
  StatusCode(const StatusCode& parent);
  /// Destructor
  ~StatusCode();

  /// Assignment operator
  StatusCode& operator=(const StatusCode& rhs);
  /// Assignment from an integer code
  StatusCode& operator=(unsigned long code);

  /// Check if the operation was successful
  bool isSuccess() const;
  /// Check if the operation was a failure
  bool isFailure() const;

  /// Automatic conversion operator
  operator unsigned long() const;

  /// Mark the status code as checked, ignoring it thereby
  void setChecked() const { m_checked = true; }
  /// Ignore the status code, marking it as checked
  void ignore() const { setChecked(); }

  /// Enable failure (with a backtrace) on an unchecked status code
  static void enableFailure();
  /// Disable failure (no backtrace) on an unchecked status code
  static void disableFailure();

 private:
  /// Code returned by some function
  unsigned long m_code;
  /// Internal status flag of whether the code was checked by the user
  mutable bool m_checked;
};
}  // namespace TL

/**
 *  @def TL_CHECK
 *  Checks the return code for SUCCESS or FAILURE
 */
#define TL_CHECK(EXP)                                                           \
  {                                                                             \
    const auto sc__ = EXP;                                                      \
    if (sc__.isFailure()) {                                                     \
      if (spdlog::get("TL::StatusCode") == nullptr) {                           \
        spdlog::stdout_color_mt("TL::StatusCode");                              \
      }                                                                         \
      spdlog::get("TL::StatusCode")                                             \
          ->error("TL::StatusCode::FAILURE found in {}!", __PRETTY_FUNCTION__); \
      std::exit(EXIT_FAILURE);                                                  \
    }                                                                           \
  }

#endif

/*! @file  Loggable.h
 *  @brief TL::Loggable class header
 *  @class TL::Loggable
 *  @brief Base class for logging purposes
 *
 *  If you need a class to have logging capabilities from the spdlog
 *  framework, inherit from this base class.
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_Loggable_h
#define TL_Loggable_h

#include <TopLoop/spdlog/spdlog.h>
// avoid clang-format reorder
#include <TopLoop/spdlog/sinks/stdout_color_sinks.h>

namespace TL {
class Loggable {
 protected:
  /// pointer to the spdlog logger object
  std::shared_ptr<spdlog::logger> m_logger{nullptr};

 public:
  /// Only usable constructor, gives name to the logger
  /*!
   *  Should be used with a daughter class like so:
   *
   *  @code{.cpp}
   *
   *  MyClass::MyClass() : TL::Logger("Loggername") {
   *    // ...
   *  }
   *
   *  @endcode
   *
   *  because we require the logger to be named.
   */
  Loggable(const std::string& name) { m_logger = setupLogger(name); }

  /// default constructor deleted
  Loggable() = default;
  /// copy constructor default
  Loggable(const Loggable&) = default;
  /// move constructor default
  Loggable(Loggable&&) = default;

  /// assignment operator default
  Loggable& operator=(const Loggable&) = default;
  /// move assignment operator default
  Loggable& operator=(Loggable&&) = default;

  /// virtual destructor
  virtual ~Loggable() { spdlog::drop(m_logger->name()); }

  static std::shared_ptr<spdlog::logger> setupLogger(const std::string& name) {
    std::string loggername(name);
    if (loggername.length() < 15) {
      while (loggername.length() < 15) {
        loggername.append(" ");
      }
    }
    else {
      while (loggername.length() > 12) {
        loggername.pop_back();
      }
      loggername.append("...");
    }
    return spdlog::stdout_color_st(loggername);
  }

  /// set the level of the logger (see spdlog documentation for levels)
  void setLogLevel(spdlog::level::level_enum lvl) { m_logger->set_level(lvl); }

  /// retrieve a const logger pointer
  virtual const std::shared_ptr<spdlog::logger>& logger() const { return m_logger; }

  /// retrieve the logger pointer
  virtual std::shared_ptr<spdlog::logger>& logger() { return m_logger; }
};
}  // namespace TL

#endif

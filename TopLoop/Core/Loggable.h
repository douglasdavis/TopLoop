/** @file  Loggable.h
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

namespace TL {
  class Loggable {
  protected:

    /// pointer to the spdlog logger object
    std::shared_ptr<spdlog::logger> m_logger{nullptr};
    std::string m_name{};

  public:

    /// default unusable constructor
    Loggable() = default;

    /// only usable constructor, gives name to the logger
    Loggable(const std::string& name) {
      m_logger = spdlog::stdout_color_mt(name);
      m_name = name;
    }

    /// delete copy
    Loggable(const Loggable&) = delete;
    /// default move
    Loggable(Loggable&&) = default;

    /// delete assignment
    Loggable& operator=(const Loggable&) = delete;
    /// default move assignment
    Loggable& operator=(Loggable&&) = default;

    /// virtual destructor
    virtual ~Loggable() {
      spdlog::drop(m_name);
    }

    /// set the level of the logger (see spdlog documentation for levels)
    void setLogLevel(spdlog::level::level_enum lvl) { m_logger->set_level(lvl); }

    /// retrieve a const logger pointer
    const std::shared_ptr<spdlog::logger> logger() const { return m_logger; }

    /// retrieve the logger pointer
    std::shared_ptr<spdlog::logger> logger() { return m_logger; }

  };
}

#endif

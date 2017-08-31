#ifndef TL_Loggable_h
#define TL_Loggable_h

#include <TopLoop/spdlog/spdlog.h>

namespace TL {
  class Loggable {
  protected:
    std::shared_ptr<spdlog::logger> m_logger;

  public:
    Loggable() {}
    Loggable(const std::string& name) {
      m_logger = spdlog::stdout_color_mt(name);
    }
    virtual ~Loggable() {}

    void setLogLevel(spdlog::level::level_enum lvl) { m_logger->set_level(lvl); }

    const std::shared_ptr<spdlog::logger> logger() const { return m_logger; }

    std::shared_ptr<spdlog::logger> logger() { return m_logger; }

  };
}

#endif

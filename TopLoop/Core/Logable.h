#ifndef TL_Logable_h
#define TL_Logable_h

#include <TopLoop/spdlog/spdlog.h>

namespace TL {
  class Logable {
  protected:
    std::shared_ptr<spdlog::logger> m_logger;

  public:
    Logable() {}
    Logable(const std::string& name) {
      m_logger = spdlog::stdout_color_mt(name);
    }
    virtual ~Logable() {}

    void setLogLevel(spdlog::level::level_enum lvl) { m_logger->set_level(lvl); }

    const std::shared_ptr<spdlog::logger> logger() const { return m_logger; }

    std::shared_ptr<spdlog::logger> logger() { return m_logger; }

  };
}

#endif

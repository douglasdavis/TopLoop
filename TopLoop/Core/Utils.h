/** @file  Utils.h
 *  @brief TL::Utils class header
 *
 *  Utility functions for TopLoop
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

#ifndef TL_Utils_h
#define TL_Utils_h

// C++
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cmath>
#include <memory>
#include <string>
#include <sstream>

#define FATAL(PRINTOUT)    \
  ANA_MSG_FATAL(PRINTOUT); \
  std::exit(EXIT_FAILURE);

namespace TL {
  const double TeV   = 1.0e6;
  const double GeV   = 1.0e3;
  const double toGeV = 1.0e-3;

  auto& string_split(const std::string &s, char delim, std::vector<std::string> &elems);
  auto string_split(const std::string &s, char delim);
}

inline auto& TL::string_split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.emplace_back(item);
  }
  return elems;
}

inline auto TL::string_split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  string_split(s, delim, elems);
  return elems;
}

#endif

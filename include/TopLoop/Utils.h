/** @file  Utils.h
 *  @brief TL::Utils class header
 *
 *  Utility functions for TopLoop
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef TL_Utils_h
#define TL_Utils_h

// C++
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>

// boost
#include <boost/range/combine.hpp>

//#define TeV 0.000001
//#define GeV 0.001

namespace TL {
  const double TeV = 1.0e-6;
  const double GeV = 1.0e-3;
  
  enum STATUS {
    Good,
    Fail
  };
}

namespace TL {
  template<class... containers>
  auto zip(containers&... conts) -> decltype(boost::combine(conts...));

  auto string_split(const std::string &s, char delim, std::vector<std::string> &elems)
    ->  std::vector<std::string>& ;
  auto string_split(const std::string &s, char delim)
    -> std::vector<std::string>;

  void Warning(const std::string& fname, const std::string& msg);
  void Info(const std::string& fname, const std::string& msg);
  void Fatal(const std::string& msg);
}

template<class... containers>
inline auto TL::zip(containers&... conts) -> decltype(boost::combine(conts...)) {
  return boost::combine(conts...);
}

inline auto TL::string_split(const std::string &s, char delim, std::vector<std::string> &elems)
  -> std::vector<std::string>& {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.emplace_back(item);
  }
  return elems;
}

inline auto TL::string_split(const std::string &s, char delim)
  -> std::vector<std::string> {
  std::vector<std::string> elems;
  string_split(s, delim, elems);
  return elems;
}

inline void TL::Info(const std::string& fname, const std::string& msg) {
  std::cout << "TopLoop:\tINFO:      "+fname+": "+msg << std::endl;
}

inline void TL::Warning(const std::string& fname, const std::string& msg) {
  std::cout << "TopLoop:\tWARNING:   "+fname+": "+msg << std::endl;
}

inline void TL::Fatal(const std::string& msg) {
  std::cerr << "TopLoop:\tFATAL:     "+msg << std::endl;
  std::exit(EXIT_FAILURE);
}

#endif

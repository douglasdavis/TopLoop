/** @file  Utils.h
 *  @brief DT::Utils class header
 *
 *  Utility functions for DukeTop
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 */

#ifndef DT_Utils_h
#define DT_Utils_h

// C++
#include <iostream>
#include <cstdlib>
#include <sstream>

// boost
#include <boost/range/combine.hpp>

namespace DT {
  template<class... containers>
  auto zip(containers&... conts) -> decltype(boost::combine(conts...));

  auto string_split(const std::string &s, char delim, std::vector<std::string> &elems)
    ->  std::vector<std::string>& ;
  auto string_split(const std::string &s, char delim)
    -> std::vector<std::string>;
 
  void Info(const std::string& fname, const std::string& msg);
  void Fatal(const std::string& msg);
}

template<class... containers>
inline auto DT::zip(containers&... conts) -> decltype(boost::combine(conts...)) {
  return boost::combine(conts...);
}

inline auto DT::string_split(const std::string &s, char delim, std::vector<std::string> &elems)
  -> std::vector<std::string>& {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.emplace_back(item);
  }
  return elems;
}

inline auto DT::string_split(const std::string &s, char delim)
  -> std::vector<std::string> {
  std::vector<std::string> elems;
  string_split(s, delim, elems);
  return elems;
}

inline void DT::Info(const std::string& fname, const std::string& msg) {
  std::cout << "DukeTop:\t\tINFO:\t\t"+fname+": "+msg << std::endl;
}

inline void DT::Fatal(const std::string& msg) {
  std::cerr << "DukeTop:\t\tFATAL:\t\t"+msg << std::endl;
  std::exit(EXIT_FAILURE);
}

#endif

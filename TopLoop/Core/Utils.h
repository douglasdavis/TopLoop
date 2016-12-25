/** @file  Utils.h
 *  @brief TL::Utils class header
 *
 *  Utility functions for TopLoop
 *
 *  @author Douglas Davis < douglas.davis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

#ifndef TL_Utils_h
#define TL_Utils_h

// C++
#include <iostream>
#include <cstdlib>
#include <utility>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <memory>
#include <string>

#define FUNC  __FUNCTION__
#define PFUNC __PRETTY_FUNCTION__

namespace TL {
  // default is MeV so we make the scale factors
  const double TeV = 1.0e6;
  const double GeV = 1.0e3;

  enum STATUS {
    Good,
    Fail,
    Skip
  };

}

namespace TL {

  auto string_split(const std::string &s, char delim, std::vector<std::string> &elems)
    -> std::vector<std::string>&;
  auto string_split(const std::string &s, char delim)
    -> std::vector<std::string>;

  template <typename Arg, typename... Args>
  void TopPrint(std::ostream& out, Arg&& arg, Args&&... args);

  template <typename Arg, typename... Args>
  void Info(Arg&& arg, Args&&... args);

  template <typename Arg, typename... Args>
  void Warning(Arg&& arg, Args&&... args);

  template <typename Arg, typename... Args>
  void Fatal(Arg&& arg, Args&&... args);

  int ProgressPrint(const std::string& func, long cur, long total, int gap);

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

template <typename Arg, typename... Args>
inline void TL::TopPrint(std::ostream& out, Arg&& arg, Args&&... args) {
  out << "TopLoop:  " << std::forward<Arg>(arg);
  using expander = int[];
  (void)expander{0, (void(out << ' ' << std::forward<Args>(args)),0)...};
  out << std::endl;
}

template <typename Arg, typename... Args>
inline void TL::Info(Arg&& arg, Args&&... args) {
  TopPrint(std::cout,"INFO     ",arg,"\t",args...);
}

template <typename Arg, typename... Args>
inline void TL::Warning(Arg&& arg, Args&&... args) {
  TopPrint(std::cout,"WARNING  ",arg,"\t",args...);
}

template <typename Arg, typename... Args>
inline void TL::Fatal(Arg&& arg, Args&&... args) {
  TopPrint(std::cerr,"FATAL    ",arg,"\t",args...);
  std::exit(EXIT_FAILURE);
}

inline int TL::ProgressPrint(const std::string& func,
                              long cur, long total, int range) {
  if ( total > 50 ) {
    auto progress = 100.0*cur/total;
    int gap = total/range;
    if ( cur%gap == 0 ) {
      TL::Info(func,"Events processed:",cur,
	       std::to_string(int(std::round(progress)))+"%");
    }
    return 0;
  }
  else {
    return 0;
  }
}

#endif

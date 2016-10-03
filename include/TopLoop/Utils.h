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
  out << "TopLoop:\t" << std::forward<Arg>(arg);
  using expander = int[];
  (void)expander{0, (void(out << ' ' << std::forward<Args>(args)),0)...};
  out << std::endl;
}

template<typename Arg, typename... Args>
inline void TL::Info(Arg&& arg, Args&&... args) {
  TopPrint(std::cout,"INFO\t",arg,"\t",args...);
}

template<typename Arg, typename... Args>
inline void TL::Warning(Arg&& arg, Args&&... args) {
  TopPrint(std::cout,"WARNING\t",arg,"\t",args...);
}

template<typename Arg, typename... Args>
inline void TL::Fatal(Arg&& arg, Args&&... args) {
  TopPrint(std::cerr,"FATAL\t",arg,"\t",args...);
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

namespace TL {

  static const std::map<unsigned int, std::string> kDSIDTABLE = {
    { 361084 , "wz"    } ,  { 361420 , "zjets" } ,  { 361421 , "zjets" } ,
    { 361422 , "zjets" } ,  { 361423 , "zjets" } ,  { 361424 , "zjets" } ,
    { 361425 , "zjets" } ,  { 361426 , "zjets" } ,  { 361427 , "zjets" } ,
    { 361428 , "zjets" } ,  { 361429 , "zjets" } ,  { 361381 , "zjets" } ,
    { 361380 , "zjets" } ,  { 361383 , "zjets" } ,  { 361382 , "zjets" } ,
    { 361385 , "zjets" } ,  { 361384 , "zjets" } ,  { 361387 , "zjets" } ,
    { 361386 , "zjets" } ,  { 361389 , "zjets" } ,  { 361388 , "zjets" } ,
    { 410015 , "wt"    } ,  { 410016 , "wt"    } ,  { 361439 , "zjets" } ,
    { 361438 , "zjets" } ,  { 361437 , "zjets" } ,  { 361436 , "zjets" } ,
    { 361435 , "zjets" } ,  { 361434 , "zjets" } ,  { 361433 , "zjets" } ,
    { 361432 , "zjets" } ,  { 361431 , "zjets" } ,  { 361430 , "zjets" } ,
    { 361604 , "zz"    } ,  { 361606 , "ww"    } ,  { 361607 , "wz"    } ,
    { 361600 , "ww"    } ,  { 361601 , "wz"    } ,  { 361602 , "wz"    } ,
    { 361603 , "zz"    } ,  { 410003 , "tt"    } ,  { 410002 , "tt"    } ,
    { 410001 , "tt"    } ,  { 410000 , "tt"    } ,  { 361609 , "wz"    } ,
    { 361442 , "zjets" } ,  { 361443 , "zjets" } ,  { 361440 , "zjets" } ,
    { 361441 , "zjets" } ,  { 361610 , "zz"    } ,  { 361108 , "zjets" } ,
    { 361107 , "zjets" } ,  { 361106 , "zjets" } ,  { 361068 , "ww"    } ,
    { 361069 , "wwss"  } ,  { 361372 , "zjets" } ,  { 361373 , "zjets" } ,
    { 361374 , "zjets" } ,  { 361375 , "zjets" } ,  { 361376 , "zjets" } ,
    { 361377 , "zjets" } ,  { 361378 , "zjets" } ,  { 361379 , "zjets" } ,
    { 361063 , "zz"    } ,  { 361064 , "wz"    } ,  { 361065 , "zjets" } ,
    { 361066 , "wz"    } ,  { 361067 , "wz"    } ,  { 361077 , "ww"    } ,
    { 361072 , "zz"    } ,  { 361071 , "wz"    } ,  { 361070 , "wwss"  } ,
    { 361078 , "ww"    } ,  { 361406 , "zjets" } ,  { 361407 , "zjets" } ,
    { 361404 , "zjets" } ,  { 361405 , "zjets" } ,  { 361402 , "zjets" } ,
    { 361403 , "zjets" } ,  { 361400 , "zjets" } ,  { 361401 , "zjets" } ,
    { 361082 , "ww"    } ,  { 361083 , "wz"    } ,  { 361081 , "ww"    } ,
    { 361086 , "zz"    } ,  { 361408 , "zjets" } ,  { 361409 , "zjets" } ,
    { 361415 , "zjets" } ,  { 361414 , "zjets" } ,  { 361417 , "zjets" } ,
    { 361416 , "zjets" } ,  { 361411 , "zjets" } ,  { 361410 , "zjets" } ,
    { 361413 , "zjets" } ,  { 361412 , "zjets" } ,  { 361419 , "zjets" } ,
    { 361418 , "zjets" } ,  { 361392 , "zjets" } ,  { 361393 , "zjets" } ,
    { 361390 , "zjets" } ,  { 361391 , "zjets" } ,  { 361396 , "zjets" } ,
    { 361397 , "zjets" } ,  { 361394 , "zjets" } ,  { 361395 , "zjets" } ,
    { 361398 , "zjets" } ,  { 361399 , "zjets" } ,  { 0      , "data"  }
  };

  static const std::map<unsigned int, std::string> kDSIDTABLENEW = {
    { 410000 , "tt"     } , { 410001 , "tt"     } , { 410002 , "tt"     } ,
    { 410004 , "tt"     } , { 410007 , "tt"     } , { 410008 , "tt"     } ,
    { 410008 , "tt"     } , { 410009 , "tt"     } , { 410021 , "tt"     } ,
    { 410021 , "tt"     } , { 410022 , "tt"     } , { 410022 , "tt"     } ,
    { 410023 , "tt"     } , { 410023 , "tt"     } , { 410024 , "tt"     } ,
    { 410024 , "tt"     } , { 410037 , "tt"     } , { 410038 , "tt"     } ,
    { 410039 , "tt"     } , { 410040 , "tt"     } , { 410041 , "tt"     } ,
    { 410042 , "tt"     } , { 410043 , "tt"     } , { 410044 , "tt"     } ,
    { 410045 , "tt"     } , { 410046 , "tt"     } , { 410051 , "tt"     } ,
    { 410159 , "tt"     } , { 410160 , "tt"     } , { 410161 , "tt"     } ,
    { 410162 , "tt"     } , { 410163 , "tt"     } , { 410186 , "tt"     } ,
    { 410186 , "tt"     } , { 410187 , "tt"     } , { 410187 , "tt"     } ,
    { 410188 , "tt"     } , { 410188 , "tt"     } , { 410189 , "tt"     } ,
    { 410189 , "tt"     } , { 410120 , "tt"     } , { 410121 , "tt"     } ,
    { 410500 , "tt"     } , { 407009 , "tt"     } , { 407010 , "tt"     } ,
    { 407011 , "tt"     } , { 407012 , "tt"     } ,
    { 410011 , "stt"    } , { 410012 , "stt"    } , { 410017 , "stt"    } ,
    { 410018 , "stt"    } , { 410019 , "stt"    } , { 410020 , "stt"    } ,
    { 410047 , "stt"    } , { 410048 , "stt"    } , { 410141 , "stt"    } ,
    { 410013 , "wt"     } , { 410014 , "wt"     } , { 410015 , "wt"     } ,
    { 410016 , "wt"     } , { 410062 , "wt"     } , { 410063 , "wt"     } ,
    { 410064 , "wt"     } , { 410065 , "wt"     } , { 410099 , "wt"     } ,
    { 410100 , "wt"     } , { 410101 , "wt"     } , { 410102 , "wt"     } ,
    { 410103 , "wt"     } , { 410104 , "wt"     } , { 410105 , "wt"     } ,
    { 410106 , "wt"     } , { 410145 , "wt"     } , { 410146 , "wt"     } ,
    { 410147 , "wt"     } , { 410148 , "wt"     } ,
    { 410025 , "sts"    } , { 410026 , "sts"    } , { 410107 , "sts"    } ,
    { 410108 , "sts"    } , { 410109 , "sts"    } , { 410110 , "sts"    } ,
    { 410049 , "tz"     } , { 410050 , "tz"     } , { 410049 , "tz"     } ,
    { 410050 , "tz"     } , { 410151 , "tz"     } , { 410152 , "tz"     } ,
    { 410153 , "tz"     } , { 410154 , "tz"     } , { 410215 , "tz"     } ,
    { 410217 , "tz"     } , { 410066 , "ttv"    } , { 410067 , "ttv"    } ,
    { 410068 , "ttv"    } , { 410069 , "ttv"    } , { 410070 , "ttv"    } ,
    { 410073 , "ttv"    } , { 410074 , "ttv"    } , { 410075 , "ttv"    } ,
    { 410111 , "ttv"    } , { 410112 , "ttv"    } , { 410113 , "ttv"    } ,
    { 410114 , "ttv"    } , { 410115 , "ttv"    } , { 410116 , "ttv"    } ,
    { 410142 , "ttv"    } , { 410143 , "ttv"    } , { 410144 , "ttv"    } ,
    { 410155 , "ttv"    } , { 410156 , "ttv"    } , { 410157 , "ttv"    } ,
    { 410218 , "ttv"    } , { 410219 , "ttv"    } , { 410220 , "ttv"    } ,
    { 410080 , "4top"   } , { 410081 , "ttww"   } ,
    { 410221 , "othtop" } , { 343852 , "othtop" } , { 343853 , "othtop" } ,
    { 343854 , "othtop" } , { 304014 , "othtop" } , { 426072 , "othtop" } ,
    { 426075 , "othtop" } , { 426076 , "othtop" } , { 426077 , "othtop" } ,
    { 426078 , "othtop" } , { 426082 , "othtop" } , { 426085 , "othtop" } ,
    { 426086 , "othtop" } , { 426087 , "othtop" } , { 426088 , "othtop" } ,
    { 361372 , "zjets"  } , { 361373 , "zjets"  } , { 361374 , "zjets"  } ,
    { 361375 , "zjets"  } , { 361376 , "zjets"  } , { 361377 , "zjets"  } ,
    { 361378 , "zjets"  } , { 361379 , "zjets"  } , { 361380 , "zjets"  } ,
    { 361381 , "zjets"  } , { 361382 , "zjets"  } , { 361383 , "zjets"  } ,
    { 361384 , "zjets"  } , { 361385 , "zjets"  } , { 361386 , "zjets"  } ,
    { 361387 , "zjets"  } , { 361388 , "zjets"  } , { 361389 , "zjets"  } ,
    { 361390 , "zjets"  } , { 361391 , "zjets"  } , { 361392 , "zjets"  } ,
    { 361393 , "zjets"  } , { 361394 , "zjets"  } , { 361395 , "zjets"  } ,
    { 361396 , "zjets"  } , { 361397 , "zjets"  } , { 361398 , "zjets"  } ,
    { 361399 , "zjets"  } , { 361400 , "zjets"  } , { 361401 , "zjets"  } ,
    { 361402 , "zjets"  } , { 361403 , "zjets"  } , { 361404 , "zjets"  } ,
    { 361405 , "zjets"  } , { 361406 , "zjets"  } , { 361407 , "zjets"  } ,
    { 361408 , "zjets"  } , { 361409 , "zjets"  } , { 361410 , "zjets"  } ,
    { 361411 , "zjets"  } , { 361412 , "zjets"  } , { 361413 , "zjets"  } ,
    { 361414 , "zjets"  } , { 361415 , "zjets"  } , { 361416 , "zjets"  } ,
    { 361417 , "zjets"  } , { 361418 , "zjets"  } , { 361419 , "zjets"  } ,
    { 361420 , "zjets"  } , { 361421 , "zjets"  } , { 361422 , "zjets"  } ,
    { 361423 , "zjets"  } , { 361424 , "zjets"  } , { 361425 , "zjets"  } ,
    { 361426 , "zjets"  } , { 361427 , "zjets"  } , { 361428 , "zjets"  } ,
    { 361429 , "zjets"  } , { 361430 , "zjets"  } , { 361431 , "zjets"  } ,
    { 361432 , "zjets"  } , { 361433 , "zjets"  } , { 361434 , "zjets"  } ,
    { 361435 , "zjets"  } , { 361436 , "zjets"  } , { 361437 , "zjets"  } ,
    { 361438 , "zjets"  } , { 361439 , "zjets"  } , { 361440 , "zjets"  } ,
    { 361441 , "zjets"  } , { 361442 , "zjets"  } , { 361443 , "zjets"  } ,
    { 361444 , "zjets"  } , { 361445 , "zjets"  } , { 361446 , "zjets"  } ,
    { 361447 , "zjets"  } , { 361448 , "zjets"  } , { 361449 , "zjets"  } ,
    { 361450 , "zjets"  } , { 361451 , "zjets"  } , { 361452 , "zjets"  } ,
    { 361453 , "zjets"  } , { 361454 , "zjets"  } , { 361455 , "zjets"  } ,
    { 361456 , "zjets"  } , { 361457 , "zjets"  } , { 361458 , "zjets"  } ,
    { 361459 , "zjets"  } , { 361460 , "zjets"  } , { 361461 , "zjets"  } ,
    { 361462 , "zjets"  } , { 361463 , "zjets"  } , { 361464 , "zjets"  } ,
    { 361464 , "zjets"  } , { 361465 , "zjets"  } , { 361466 , "zjets"  } ,
    { 361466 , "zjets"  } , { 361467 , "zjets"  } , { 361468 , "zjets"  } ,
    { 361469 , "zjets"  } , { 361470 , "zjets"  } , { 361471 , "zjets"  } ,
    { 361472 , "zjets"  } , { 361473 , "zjets"  } , { 361474 , "zjets"  } ,
    { 361475 , "zjets"  } , { 361476 , "zjets"  } , { 361477 , "zjets"  } ,
    { 361478 , "zjets"  } , { 361479 , "zjets"  } , { 361480 , "zjets"  } ,
    { 361481 , "zjets"  } , { 361482 , "zjets"  } , { 361483 , "zjets"  } ,
    { 361484 , "zjets"  } , { 361485 , "zjets"  } , { 361486 , "zjets"  } ,
    { 361487 , "zjets"  } , { 361488 , "zjets"  } , { 361489 , "zjets"  } ,
    { 361490 , "zjets"  } , { 361491 , "zjets"  } , { 363388 , "zjets"  } ,
    { 363389 , "zjets"  } , { 363390 , "zjets"  } , { 363391 , "zjets"  } ,
    { 363392 , "zjets"  } , { 363393 , "zjets"  } , { 363394 , "zjets"  } ,
    { 363395 , "zjets"  } , { 363396 , "zjets"  } , { 363397 , "zjets"  } ,
    { 363398 , "zjets"  } , { 363399 , "zjets"  } , { 363400 , "zjets"  } ,
    { 363401 , "zjets"  } , { 363402 , "zjets"  } , { 363403 , "zjets"  } ,
    { 363404 , "zjets"  } , { 363405 , "zjets"  } , { 363406 , "zjets"  } ,
    { 363407 , "zjets"  } , { 363408 , "zjets"  } , { 363409 , "zjets"  } ,
    { 363410 , "zjets"  } , { 363411 , "zjets"  } , { 363364 , "zjets"  } ,
    { 363365 , "zjets"  } , { 363366 , "zjets"  } , { 363367 , "zjets"  } ,
    { 363368 , "zjets"  } , { 363369 , "zjets"  } , { 363370 , "zjets"  } ,
    { 363371 , "zjets"  } , { 363372 , "zjets"  } , { 363373 , "zjets"  } ,
    { 363374 , "zjets"  } , { 363375 , "zjets"  } , { 363376 , "zjets"  } ,
    { 363377 , "zjets"  } , { 363378 , "zjets"  } , { 363379 , "zjets"  } ,
    { 363380 , "zjets"  } , { 363381 , "zjets"  } , { 363382 , "zjets"  } ,
    { 363383 , "zjets"  } , { 363384 , "zjets"  } , { 363385 , "zjets"  } ,
    { 363386 , "zjets"  } , { 363387 , "zjets"  } , { 363361 , "zjets"  } ,
    { 363362 , "zjets"  } , { 363363 , "zjets"  } , { 363102 , "zjets"  } ,
    { 363103 , "zjets"  } , { 363104 , "zjets"  } , { 363105 , "zjets"  } ,
    { 363106 , "zjets"  } , { 363107 , "zjets"  } , { 363108 , "zjets"  } ,
    { 363109 , "zjets"  } , { 363110 , "zjets"  } , { 363111 , "zjets"  } ,
    { 363112 , "zjets"  } , { 363113 , "zjets"  } , { 363114 , "zjets"  } ,
    { 363115 , "zjets"  } , { 363116 , "zjets"  } , { 363117 , "zjets"  } ,
    { 363118 , "zjets"  } , { 363119 , "zjets"  } , { 363120 , "zjets"  } ,
    { 363121 , "zjets"  } , { 363122 , "zjets"  } , { 363412 , "zjets"  } ,
    { 363413 , "zjets"  } , { 363414 , "zjets"  } , { 363415 , "zjets"  } ,
    { 363416 , "zjets"  } , { 363417 , "zjets"  } , { 363418 , "zjets"  } ,
    { 363419 , "zjets"  } , { 363420 , "zjets"  } , { 363421 , "zjets"  } ,
    { 363422 , "zjets"  } , { 363423 , "zjets"  } , { 363424 , "zjets"  } ,
    { 363425 , "zjets"  } , { 363426 , "zjets"  } , { 363427 , "zjets"  } ,
    { 363428 , "zjets"  } , { 363429 , "zjets"  } , { 363430 , "zjets"  } ,
    { 363431 , "zjets"  } , { 363432 , "zjets"  } , { 363433 , "zjets"  } ,
    { 363434 , "zjets"  } , { 363435 , "zjets"  } , { 361500 , "zjets"  } ,
    { 361501 , "zjets"  } , { 361502 , "zjets"  } , { 361530 , "zjets"  } ,
    { 361504 , "zjets"  } , { 361505 , "zjets"  } , { 361506 , "zjets"  } ,
    { 361507 , "zjets"  } , { 361508 , "zjets"  } , { 361509 , "zjets"  } ,
    { 361510 , "zjets"  } , { 361511 , "zjets"  } , { 361512 , "zjets"  } ,
    { 361513 , "zjets"  } , { 361514 , "zjets"  } , { 361628 , "zjets"  } ,
    { 361629 , "zjets"  } , { 361630 , "zjets"  } , { 361631 , "zjets"  } ,
    { 361632 , "zjets"  } , { 361633 , "zjets"  } , { 361634 , "zjets"  } ,
    { 361635 , "zjets"  } , { 361636 , "zjets"  } , { 361637 , "zjets"  } ,
    { 361638 , "zjets"  } , { 361639 , "zjets"  } , { 361640 , "zjets"  } ,
    { 361641 , "zjets"  } , { 361642 , "zjets"  } ,
    { 361300 , "wjets"  } , { 361301 , "wjets"  } , { 361302 , "wjets"  } ,
    { 361303 , "wjets"  } , { 361304 , "wjets"  } , { 361305 , "wjets"  } ,
    { 361306 , "wjets"  } , { 361307 , "wjets"  } , { 361308 , "wjets"  } ,
    { 361309 , "wjets"  } , { 361310 , "wjets"  } , { 361311 , "wjets"  } ,
    { 361312 , "wjets"  } , { 361313 , "wjets"  } , { 361314 , "wjets"  } ,
    { 361315 , "wjets"  } , { 361316 , "wjets"  } , { 361317 , "wjets"  } ,
    { 361318 , "wjets"  } , { 361319 , "wjets"  } , { 361319 , "wjets"  } ,
    { 361320 , "wjets"  } , { 361321 , "wjets"  } , { 361322 , "wjets"  } ,
    { 361323 , "wjets"  } , { 361324 , "wjets"  } , { 361325 , "wjets"  } ,
    { 361326 , "wjets"  } , { 361327 , "wjets"  } , { 361328 , "wjets"  } ,
    { 361329 , "wjets"  } , { 361330 , "wjets"  } , { 361331 , "wjets"  } ,
    { 361332 , "wjets"  } , { 361333 , "wjets"  } , { 361334 , "wjets"  } ,
    { 361335 , "wjets"  } , { 361336 , "wjets"  } , { 361337 , "wjets"  } ,
    { 361338 , "wjets"  } , { 361339 , "wjets"  } , { 361340 , "wjets"  } ,
    { 361341 , "wjets"  } , { 361342 , "wjets"  } , { 361343 , "wjets"  } ,
    { 361344 , "wjets"  } , { 361345 , "wjets"  } , { 361346 , "wjets"  } ,
    { 361347 , "wjets"  } , { 361348 , "wjets"  } , { 361349 , "wjets"  } ,
    { 361350 , "wjets"  } , { 361351 , "wjets"  } , { 361352 , "wjets"  } ,
    { 361353 , "wjets"  } , { 361354 , "wjets"  } , { 361355 , "wjets"  } ,
    { 361356 , "wjets"  } , { 361357 , "wjets"  } , { 361358 , "wjets"  } ,
    { 361359 , "wjets"  } , { 361360 , "wjets"  } , { 361361 , "wjets"  } ,
    { 361362 , "wjets"  } , { 361363 , "wjets"  } , { 361364 , "wjets"  } ,
    { 361365 , "wjets"  } , { 361366 , "wjets"  } , { 361367 , "wjets"  } ,
    { 361368 , "wjets"  } , { 361369 , "wjets"  } , { 361370 , "wjets"  } ,
    { 361371 , "wjets"  } , { 363460 , "wjets"  } , { 363461 , "wjets"  } ,
    { 363462 , "wjets"  } , { 363463 , "wjets"  } , { 363464 , "wjets"  } ,
    { 363465 , "wjets"  } , { 363466 , "wjets"  } , { 363467 , "wjets"  } ,
    { 363468 , "wjets"  } , { 363469 , "wjets"  } , { 363470 , "wjets"  } ,
    { 363471 , "wjets"  } , { 363472 , "wjets"  } , { 363473 , "wjets"  } ,
    { 363474 , "wjets"  } , { 363475 , "wjets"  } , { 363476 , "wjets"  } ,
    { 363477 , "wjets"  } , { 363478 , "wjets"  } , { 363479 , "wjets"  } ,
    { 363480 , "wjets"  } , { 363481 , "wjets"  } , { 363482 , "wjets"  } ,
    { 363483 , "wjets"  } , { 363436 , "wjets"  } , { 363437 , "wjets"  } ,
    { 363438 , "wjets"  } , { 363439 , "wjets"  } , { 363440 , "wjets"  } ,
    { 363441 , "wjets"  } , { 363442 , "wjets"  } , { 363443 , "wjets"  } ,
    { 363444 , "wjets"  } , { 363445 , "wjets"  } , { 363446 , "wjets"  } ,
    { 363447 , "wjets"  } , { 363448 , "wjets"  } , { 363449 , "wjets"  } ,
    { 363450 , "wjets"  } , { 363451 , "wjets"  } , { 363452 , "wjets"  } ,
    { 363453 , "wjets"  } , { 363454 , "wjets"  } , { 363455 , "wjets"  } ,
    { 363456 , "wjets"  } , { 363457 , "wjets"  } , { 363458 , "wjets"  } ,
    { 363459 , "wjets"  } , { 363331 , "wjets"  } , { 363332 , "wjets"  } ,
    { 363333 , "wjets"  } , { 363334 , "wjets"  } , { 363335 , "wjets"  } ,
    { 363336 , "wjets"  } , { 363337 , "wjets"  } , { 363338 , "wjets"  } ,
    { 363339 , "wjets"  } , { 363340 , "wjets"  } , { 363341 , "wjets"  } ,
    { 363342 , "wjets"  } , { 363343 , "wjets"  } , { 363344 , "wjets"  } ,
    { 363345 , "wjets"  } , { 363346 , "wjets"  } , { 363347 , "wjets"  } ,
    { 363348 , "wjets"  } , { 363349 , "wjets"  } , { 363350 , "wjets"  } ,
    { 363351 , "wjets"  } , { 363352 , "wjets"  } , { 363353 , "wjets"  } ,
    { 363354 , "wjets"  } , { 361520 , "wjets"  } , { 361521 , "wjets"  } ,
    { 361522 , "wjets"  } , { 361523 , "wjets"  } , { 361524 , "wjets"  } ,
    { 361525 , "wjets"  } , { 361526 , "wjets"  } , { 361527 , "wjets"  } ,
    { 361528 , "wjets"  } , { 361529 , "wjets"  } , { 361530 , "wjets"  } ,
    { 361531 , "wjets"  } , { 361532 , "wjets"  } , { 361533 , "wjets"  } ,
    { 361534 , "wjets"  } ,
    { 361600 , "ww"     } , { 361606 , "ww"     } ,
    { 361601 , "wz"     } , { 361602 , "wz"     } , { 361607 , "wz"     } ,
    { 361608 , "wz"     } , { 361609 , "wz"     } ,
    { 361603 , "zz"     } , { 361604 , "zz"     } , { 361605 , "zz"     } ,
    { 361610 , "zz"     } , { 361611 , "zz"     } ,

    { 0      , "data"   }
  };

}

#endif

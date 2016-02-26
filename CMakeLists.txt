cmake_minimum_required(VERSION 2.8)
project(DukeTop)

# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

option(DEBUGWARN "DEBUGWARN" OFF)
if(DEBUGWARN)
  message("-- adding -Wall (removing -w)")
  set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11 -Wall")
else()
  set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11 -Wall")
endif()

set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/share/modules")

find_package(ROOT REQUIRED)
include_directories("${ROOT_INCLUDE_DIR}")
link_directories("${ROOT_LIBRARY_DIR}")

find_package(Boost REQUIRED)
include_directories("${Boost_INCLUDE_DIRS}")
link_directories("${Boost_LIBRARY_DIR}")

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include")


file(GLOB DukeTop_src
  "src/*.cxx"
  )

file(GLOB DukeTop_inc
  "include/DukeTop/*.h"
  )

list(REMOVE_ITEM DukeTop_inc
  "include/DukeTop/LinkDef.h"
  )

ROOT_GENERATE_DICTIONARY(
  DukeTopDict
  include/DukeTop/AnaBase.h
  include/DukeTop/Job.h
  include/DukeTop/FileManager.h
  include/DukeTop/Utils.h
  LINKDEF include/DukeTop/LinkDef.h
  )

add_library(
  DukeTop SHARED
  ${DukeTop_src}
  DukeTopDict
  )

target_link_libraries(DukeTop ${ROOT_LIBRARIES})

install(DIRECTORY include/DukeTop DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(TARGETS DukeTop DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
install(FILES ${CMAKE_BINARY_DIR}/DukeTopDict_rdict.pcm DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

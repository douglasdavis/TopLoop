#####################################################
## CMake build file for DukeTop library            ##
## Author: Douglas Davis < douglas.davis@cern.ch > ##
##                       < ddavis@phy.duke.edu >   ##
#####################################################

cmake_minimum_required(VERSION 2.8)
project(DukeTop)

## Suppress annoying CMake dev warning
if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
  set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()

## Standard options given to the compiler.
option(DEBUGWARN "DEBUGWARN" OFF)
if(DEBUGWARN)
  message("-- adding -Wall (removing -w)")
  set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11 -Wall")
else()
  set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11 -Wall")
endif()

## Some CMake options to set
set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/share/modules")

## Set up to use ROOT
find_package(ROOT REQUIRED)
include_directories("${ROOT_INCLUDE_DIR}")
link_directories("${ROOT_LIBRARY_DIR}")

## Set up to use Boost
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
include_directories("${Boost_INCLUDE_DIRS}")
link_directories("${Boost_LIBRARY_DIR}")

## Need to include a lot for ROOT to be able to
## to generate the dictionary files
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/include/DukeTop")

## Define a list of all src files
file(GLOB DukeTop_src
  "src/*.cxx"
  )

## Generate a dictionary with all
## headers listed as dependencies
ROOT_GENERATE_DICTIONARY(
  DukeTopDict
  include/DukeTop/AnaBase.h
  include/DukeTop/Utils.h
  include/DukeTop/Job.h
  include/DukeTop/FileManager.h
  LINKDEF include/DukeTop/LinkDef.h
  )

## Make a library which will be
## called libDukeTop.so/.dylib
add_library(
  DukeTop SHARED
  ${DukeTop_src}
  DukeTopDict
  )

## Link the above library with ROOT libraries
target_link_libraries(DukeTop
  ${ROOT_LIBRARIES}
  TreePlayer
  )

## install desired objects to:
## -DCMAKE_INSTALL_PREFIX=[this dir]
install(DIRECTORY include/DukeTop
  DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
  )

install(TARGETS DukeTop
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

install(FILES ${CMAKE_BINARY_DIR}/DukeTopDict_rdict.pcm
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

## grab the CMakeLists.txt file from
## the included example if -DBUILDEX=ON
option(BUILDEX "BUILD_EXAMPLE" OFF)
if (BUILDEX)
  message("-- Will build example")
  add_subdirectory(example/useDukeTop)
else()
  message("-- Will NOT build example")
endif()

#####################################################
## CMake build file for TopLoop library            ##
## Author: Douglas Davis < douglas.davis@cern.ch > ##
##                       < ddavis@phy.duke.edu >   ##
#####################################################

cmake_minimum_required(VERSION 2.8)
project(TopLoop)

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
## For use on lxplus like environments
set(Boost_NO_BOOST_CMAKE ON)
set(Boost_NO_BOOST_CMAKE ON PARENT_SCOPE)

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
include_directories("${CMAKE_SOURCE_DIR}/include/TopLoop")
include_directories("${CMAKE_SOURCE_DIR}/include/TopLoopEDM")

## Define a list of all src files
file(GLOB TopLoop_src
  "src/*.cxx"
  )

## Generate a dictionary with all
## headers listed as dependencies
ROOT_GENERATE_DICTIONARY(
  TopLoopDict
  include/TopLoop/AnaBase.h
  include/TopLoop/Utils.h
  include/TopLoop/Job.h
  include/TopLoop/FileManager.h
  LINKDEF include/TopLoop/LinkDef.h
  )

## Generate a dictionary with all
## headers listed as dependencies
## for the EDM
ROOT_GENERATE_DICTIONARY(
  TopLoopEDMDict
  include/TopLoopEDM/Lepton.h
  include/TopLoopEDM/Jet.h
  include/TopLoopEDM/MET.h
  include/TopLoopEDM/LeptonPair.h
  include/TopLoopEDM/FinalState.h
  LINKDEF include/TopLoopEDM/LinkDef.h
  )

## Make a library which will be
## called libTopLoop.so/.dylib
add_library(
  TopLoop SHARED
  ${TopLoop_src}
  TopLoopDict
  )

add_library(
  TopLoopEDM SHARED
  TopLoopEDMDict
  )

## Link the above libraries with necessary external libraries
target_link_libraries(TopLoop
  ${ROOT_LIBRARIES}
  TreePlayer
  ${Boost_LIBRARIES}
  )

target_link_libraries(TopLoopEDM
  ${ROOT_LIBRARIES}
  )

## install desired objects to:
## -DCMAKE_INSTALL_PREFIX=[this dir]
install(DIRECTORY include/TopLoop
  DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
  )

install(DIRECTORY include/TopLoopEDM
  DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
  )

install(TARGETS TopLoop
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

install(TARGETS TopLoopEDM
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

install(FILES ${CMAKE_BINARY_DIR}/TopLoopDict_rdict.pcm
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

install(FILES ${CMAKE_BINARY_DIR}/TopLoopEDMDict_rdict.pcm
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

## grab the CMakeLists.txt file from
## the included example if -DBUILDEX=ON
option(BUILDEX "BUILD_EXAMPLE" OFF)
if (BUILDEX)
  message("-- Will build example")
  add_subdirectory(example/useTopLoop)
  add_subdirectory(example/ttZ)
else()
  message("-- Will NOT build example")
endif()

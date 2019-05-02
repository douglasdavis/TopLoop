## CMakeLists.txt for TopLoop package to be built within ATLAS
## Analysis Relase 21.2.X series.
## Author: Doug Davis <ddavis@cern.ch>

# Declare the name of this package:
atlas_subdir(TopLoop None)

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE TOPLOOP_GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE TOPLOOP_GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(
  ${CMAKE_SOURCE_DIR}/TopLoop/TopLoop/version.h.in
  ${CMAKE_BINARY_DIR}/generated/TopLoop_version.h
  @ONLY
)

# This package depends on other packages:
atlas_depends_on_subdirs(
  PUBLIC
  Tools/PathResolver
  PhysicsAnalysis/MCTruthClassifier
  PRIVATE
  PhysicsAnalysis/TopPhys/TopPhysUtils/TopDataPreparation
  )

# This package uses ROOT:
find_package(ROOT REQUIRED COMPONENTS Physics Core Tree RIO TreePlayer)
find_package(Boost COMPONENTS filesystem system)

# Build a library that other components can link against:
atlas_add_library(TopLoop
  TopLoop/Core/*.h TopLoop/EDM/*.h TopLoop/nanodm/*.h Root/*.cxx
  PUBLIC_HEADERS TopLoop
  LINK_LIBRARIES
  ${ROOT_LIBRARIES}
  Boost::filesystem
  Boost::system
  PathResolver
  TopDataPreparation
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS})

#atlas_add_dictionary( TopLoopDict
#  TopLoop/TopLoopDict.h TopLoop/selection.xml
#  LINK_LIBRARIES TopLoop )

target_compile_features(TopLoop PUBLIC cxx_std_14)
target_compile_options(TopLoop PRIVATE -Wall -Wextra)

# Install data files from the package:
atlas_install_data(data/*)

# Install python modules
atlas_install_python_modules(python/*)

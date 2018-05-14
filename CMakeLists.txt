## CMakeLists.txt for TopLoop package to be built within ATLAS
## Analysis Relase 21.2.X series.
## Author: Doug Davis <ddavis@cern.ch>

# Declare the name of this package:
atlas_subdir( TopLoop None )

# This package depends on other packages:
atlas_depends_on_subdirs(
  PUBLIC
  Tools/PathResolver
  PhysicsAnalysis/MCTruthClassifier
  )

# This package uses ROOT:
find_package( ROOT REQUIRED COMPONENTS Physics Core Tree RIO TreePlayer )
find_package( Boost COMPONENTS filesystem system )

# Build a library that other components can link against:
atlas_add_library( TopLoop
  TopLoop/Core/*.h TopLoop/EDM/*.h Root/*.cxx
  PUBLIC_HEADERS TopLoop
  LINK_LIBRARIES
  ${ROOT_LIBRARIES}
  Boost::filesystem
  Boost::system
  PathResolver
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} )

atlas_add_dictionary( TopLoopDict
  TopLoop/TopLoopDict.h TopLoop/selection.xml
  LINK_LIBRARIES TopLoop )

target_compile_features( TopLoop PUBLIC cxx_std_14 )

# Install data files from the package:
atlas_install_data( data/* )

# Install python modules
atlas_install_python_modules( python/* )

## CMakeLists.txt for TopLoop package to be built within ATLAS
## Analysis Relase 2.6.X series.
## Author: Doug Davis <ddavis@cern.ch>

# Declare the name of this package:
atlas_subdir( TopLoop None )

# This package depends on other packages:
atlas_depends_on_subdirs( PUBLIC
  PathResolver
  )

# This package uses ROOT:
find_package( ROOT REQUIRED COMPONENTS Physics Core Gpad Tree Hist RIO MathCore Graf TreePlayer )
find_package( Boost COMPONENTS filesystem system )
# Custom definitions needed for this package:
add_definitions( -g )

# Generate a CINT dictionary source file:
atlas_add_root_dictionary( TopLoop _cintDictSource
  ROOT_HEADERS Root/LinkDef.h
  EXTERNAL_PACKAGES ROOT )

# Build a library that other components can link against:
atlas_add_library( TopLoop Root/*.cxx Root/*.h Root/*.icc
  TopLoop/*.h TopLoop/*.icc TopLoop/*/*.h
  TopLoop/*/*.icc ${_cintDictSource} 
  PUBLIC_HEADERS TopLoop
  LINK_LIBRARIES
  ${ROOT_LIBRARIES}
  ${Boost_LIBRARIES}
  PathResolver
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} )

# Install data files from the package:
atlas_install_data( data/* )

# Install python modules
atlas_install_python_modules( python/* )

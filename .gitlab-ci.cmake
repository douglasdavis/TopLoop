#
# Project file for building the analysis framework with its examples and
# tests.
#

# Set the minimum required CMake version:
cmake_minimum_required( VERSION 3.2 FATAL_ERROR )

# Find the project that we depend on:
find_package( AnalysisTop 21.2 REQUIRED )

# Set up CTest:
atlas_ctest_setup()

# Set up the analysis project. Remember that in order to make it easy to
# pick up the code of this project for grid running, the project has to be
# called WorkDir. With later iterations of Panda this restriction may be
# removed.
atlas_project( TopLoopCI 1.0.0
   USE AnalysisTop 21.2 )

# Set up the runtime environment setup script(s):
lcg_generate_env( SH_FILE ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh )
install( FILES ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh
   DESTINATION . )

# Set up CPack:
atlas_cpack_setup()

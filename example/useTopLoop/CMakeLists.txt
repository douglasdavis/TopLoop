include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/TopLoop")
include_directories("${CMAKE_CURRENT_LIST_DIR}")

file(GLOB MyTopLoopAna_src
  "src/*.cxx"
  )

ROOT_GENERATE_DICTIONARY(
  MyTopLoopAnaDict
  include/MyTopLoopAna/MyTopLoopAna.h
  LINKDEF include/MyTopLoopAna/LinkDef.h
  )

add_library(MyTopLoopAna SHARED
  ${MyTopLoopAna_src}
  MyTopLoopAnaDict
  )

target_link_libraries(MyTopLoopAna
  TopLoop
  TopLoopEDM
  ${ROOT_LIBRARIES}
  TreePlayer
  )

add_executable(useMyTopLoopAna ${CMAKE_CURRENT_LIST_DIR}/util/use.cpp)

target_link_libraries(useMyTopLoopAna
  MyTopLoopAna
  TopLoop
  TopLoopEDM
  ${ROOT_LIBRARIES}
  TreePlayer
  ${Boost_LIBRARIES}
  )

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/MyTopLoopAna
  DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
  )

install(TARGETS MyTopLoopAna
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

install(TARGETS useMyTopLoopAna
  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
  )

install(FILES ${CMAKE_BINARY_DIR}/example/useTopLoop/MyTopLoopAnaDict_rdict.pcm
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

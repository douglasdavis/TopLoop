include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/TopLoop")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/TopLoopEDM")
include_directories("${CMAKE_CURRENT_LIST_DIR}")

file(GLOB ttZloop_src
  "src/*.cxx"
  )

ROOT_GENERATE_DICTIONARY(
  ttZloopDict
  include/ttZloop/ttZloop.h
  LINKDEF include/ttZloop/LinkDef.h
  )

add_library(ttZloop SHARED
  ${ttZloop_src}
  ttZloopDict
  )

target_link_libraries(ttZloop
  TopLoop
  TopLoopEDM
  ${ROOT_LIBRARIES}
  TreePlayer
  )

add_executable(usettZloop ${CMAKE_CURRENT_LIST_DIR}/util/ttZuse.cpp)

target_link_libraries(usettZloop
  ttZloop
  TopLoop
  TopLoopEDM
  ${ROOT_LIBRARIES}
  TreePlayer
  ${Boost_LIBRARIES}
  )

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/ttZloop
  DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
  )

install(TARGETS ttZloop
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

install(TARGETS usettZloop
  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
  )

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/python
  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
  USE_SOURCE_PERMISSIONS
  )

install(FILES ${CMAKE_BINARY_DIR}/example/ttZ/ttZloopDict_rdict.pcm
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

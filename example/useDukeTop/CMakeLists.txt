include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/DukeTop")
include_directories("${CMAKE_CURRENT_LIST_DIR}")

file(GLOB MyDukeTopAna_src
  "src/*.cxx"
  )

ROOT_GENERATE_DICTIONARY(
  MyDukeTopAnaDict
  include/MyDukeTopAna/MyDukeTopAna.h
  LINKDEF include/MyDukeTopAna/LinkDef.h
  )

add_library(MyDukeTopAna SHARED
  ${MyDukeTopAna_src}
  MyDukeTopAnaDict
  )

target_link_libraries(MyDukeTopAna
  DukeTop
  ${ROOT_LIBRARIES}
  TreePlayer
  )

add_executable(useMyDukeTopAna ${CMAKE_CURRENT_LIST_DIR}/util/use.cpp)

target_link_libraries(useMyDukeTopAna
  MyDukeTopAna
  DukeTop
  ${ROOT_LIBRARIES}
  TreePlayer
  ${Boost_LIBRARIES}
  )

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/MyDukeTopAna
  DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
  )

install(TARGETS MyDukeTopAna
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

install(TARGETS useMyDukeTopAna
  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
  )

install(FILES ${CMAKE_BINARY_DIR}/example/useDukeTop/MyDukeTopAnaDict_rdict.pcm
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  )

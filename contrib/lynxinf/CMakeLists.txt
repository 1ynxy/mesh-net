cmake_minimum_required(VERSION 3.2)

project(lynxinf DESCRIPTION "Debug & Configuration Library")

FIND_PACKAGE ( Threads )

add_library(lynxinf
  src/debug.cpp
  src/config.cpp
  src/file.cpp
)

target_include_directories(lynxinf PRIVATE lynxinf)
target_include_directories(lynxinf PRIVATE src)

target_link_libraries(lynxinf
  ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties(lynxinf PROPERTIES PUBLIC_HEADER lynxinf/lynxinf.h)

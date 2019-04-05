cmake_minimum_required(VERSION 3.2)

project(lynxengine DESCRIPTION "Basic Component Entity System Engine")

FIND_PACKAGE(Threads)

include_directories(
  contrib/glfw/include
)

add_subdirectory(contrib/glfw)

add_library(lynxengine
  src/core/core.cpp
  src/core/time.cpp
  src/core/input.cpp
  src/core/debug.cpp
  src/core/file.cpp
  src/core/config.cpp
)

target_include_directories(lynxengine PRIVATE lynxengine)
target_include_directories(lynxengine PRIVATE src)

target_link_libraries(lynxengine
  ${CMAKE_THREAD_LIBS_INIT}
  glfw
)

set_target_properties(lynxengine PROPERTIES PUBLIC_HEADER lynxengine/lynxengine.h)

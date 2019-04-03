cmake_minimum_required(VERSION 3.2)

project(lynxengine DESCRIPTION "Basic Component Entity System Engine")

FIND_PACKAGE(Threads)

include_directories(
  contrib/lynxinf/lynxinf
  contrib/glfw/include
)

add_subdirectory(contrib/lynxinf)
add_subdirectory(contrib/glfw)

add_library(lynxengine
  src/core/core.cpp
  src/core/time.cpp
  src/core/input.cpp
)

target_include_directories(lynxengine PRIVATE lynxengine)
target_include_directories(lynxengine PRIVATE src)

target_link_libraries(lynxengine
  ${CMAKE_THREAD_LIBS_INIT}
  lynxinf
  glfw
)

set_target_properties(lynxengine PROPERTIES PUBLIC_HEADER lynxengine/lynxengine.h)

cmake_minimum_required(VERSION 3.2)

project(lynxengine DESCRIPTION "Basic Component Entity System Engine")

include_directories(
  contrib/glad
  contrib/glfw
  contrib/glm
)

add_subdirectory(contrib/glad)
add_subdirectory(contrib/glfw)
add_subdirectory(contrib/glm)

add_library(lynxengine
  lynxengine/lynxengine.h
)

target_include_directories(lynxengine PUBLIC
  lynxengine/
  src/
)

target_link_libraries(lynxengine
  glad
  glfw
  glm
)

set_target_properties(lynxengine PROPERTIES LINKER_LANGUAGE CXX)

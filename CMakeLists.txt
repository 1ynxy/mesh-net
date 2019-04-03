cmake_minimum_required(VERSION 3.2)

project(lynxgame DESCRIPTION "Example Game For Testing LynxEngine")

include_directories(
  contrib/lynxinf/lynxinf
  contrib/lynxengine/lynxengine
  contrib/lynxnet/lynxnet
)

add_subdirectory(contrib/lynxinf)
add_subdirectory(contrib/lynxengine)
add_subdirectory(contrib/lynxnet)

add_executable(lynxgame
  src/main.cpp
)

target_link_libraries(lynxgame
  lynxinf
  lynxengine
  lynxnet
)

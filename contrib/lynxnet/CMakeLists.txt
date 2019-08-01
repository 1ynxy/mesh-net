cmake_minimum_required(VERSION 3.2)

project(lynxnet DESCRIPTION "Net Library For Mesh Networks")

FIND_PACKAGE(Threads)

add_library(lynxnet
  src/peer.cpp
  src/network.cpp
  src/packet.cpp
  src/server.cpp
)

target_include_directories(lynxnet PRIVATE lynxnet)
target_include_directories(lynxnet PRIVATE src)

target_link_libraries(lynxnet
  ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties(lynxnet PROPERTIES PUBLIC_HEADER lynxnet/lynxnet.h)

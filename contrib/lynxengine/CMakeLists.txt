cmake_minimum_required(VERSION 3.2)

project(lynxengine DESCRIPTION "Basic Component Entity System Engine")

add_subdirectory(contrib/glad)

find_package(Threads REQUIRED)
find_package(glm REQUIRED)

add_library(lynxengine
  src/resource/asset/req/lodepng.cpp

  src/data/colour.cpp

  src/core/core.cpp
  src/core/time.cpp
  src/core/input.cpp
  src/core/debug.cpp
  src/core/file.cpp
  src/core/config.cpp
  src/core/display.cpp

  src/resource/resource.cpp
  src/resource/asset.cpp
  src/resource/asset/sprite.cpp
  src/resource/asset/shader.cpp
  src/resource/asset/mesh.cpp
  src/resource/asset/rendertexture.cpp

  src/render/render.cpp
  src/render/compositor.cpp

  src/ces/scene.cpp
  src/ces/entity.cpp
  src/ces/component.cpp
  src/ces/component/transform.cpp
  src/ces/component/camera.cpp
)

target_include_directories(lynxengine PRIVATE lynxengine)
target_include_directories(lynxengine PRIVATE src)

target_link_libraries(lynxengine
  ${CMAKE_THREAD_LIBS_INIT}
  -ldl
  glad
  glfw
  glm
)

set_target_properties(lynxengine PROPERTIES PUBLIC_HEADER lynxengine/lynxengine.h)

cmake_minimum_required(VERSION 3.1)
project(Game)
set(Game_VERSION_MAJOR 0)
set(Game_VERSION_MINOR 1)
set(EXECUTABLE_NAME "game")

set(SOURCES
  Main.cpp
  World.cpp
  Shape.cpp
)

include_directories (
  "${PROJECT_SOURCE_DIR}"
  "/usr/include/Box2D"
  "/usr/include/SFML"
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_link_libraries(
  ${EXECUTABLE_NAME}
  /usr/include/Box2D/Release/libliquidfun.a
  sfml-graphics
  sfml-system
  sfml-window
)

target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_constexpr)


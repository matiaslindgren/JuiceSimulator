cmake_minimum_required(VERSION 3.1)
project(Game)
set(Game_VERSION_MAJOR 0)
set(Game_VERSION_MINOR 1)
set(EXECUTABLE_NAME "game.exe")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
  Main.cpp
  World.cpp
  Polygon.cpp
  TextBox.cpp
  Grid.cpp
  DebugDraw.cpp
  Adapter.cpp
  WorldCallbacks.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

include_directories (
  "${PROJECT_SOURCE_DIR}"
  "/usr/include/Box2D"
  "/usr/include/SFML"
  "/usr/local/include/tclap"
)

target_link_libraries(
  ${EXECUTABLE_NAME}
  /usr/include/Box2D/Release/libliquidfun.a
  sfml-graphics
  sfml-system
  sfml-window
)

target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_constexpr)


# TESTS

include(CTest)

add_test(RunWithoutRendering ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_NAME} --no-rendering)



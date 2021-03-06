cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(Game)
set(Game_VERSION_MAJOR 0)
set(Game_VERSION_MINOR 1)
set(EXECUTABLE_NAME "game.exe")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
  Adapter.cpp
  GameItem.cpp
  DebugDraw.cpp
  DebugLines.cpp
  EventHandler.cpp
  DrawableParticleSystem.cpp
  Items.cpp
  Main.cpp
  TextBox.cpp
  World.cpp
  WorldCallbacks.cpp
)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

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


# TESTS

include(CTest)

add_test(RunWithoutRendering ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_NAME} --no-rendering)



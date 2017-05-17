cmake_minimum_required(VERSION 3.1)
project(Game)
set(Game_VERSION_MAJOR 0)
set(Game_VERSION_MINOR 1)
set(EXECUTABLE_NAME "game.exe")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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


cmake_policy(SET CMP0048 NEW)
cmake_minimum_required( VERSION 3.18 )
project( Moinsen VERSION 0.1.0 LANGUAGES CXX )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/" )
add_library(libchess SHARED IMPORTED)
set_target_properties(libchess PROPERTIES
IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libchess/build/shared/libchess.so"
INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/")
find_package (Threads)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -Werror")
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -march=native -O3" )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g" )
set(
  SRC_FILES
  src/eval.cpp
  src/info.cpp
  src/search.cpp
  src/eval.hpp
  src/info.hpp
  src/search.hpp
  src/uci.hpp
)
add_executable(
  ${PROJECT_NAME}
  src/moinsen.cpp
  src/benchmark.cpp
  ${SRC_FILES}
)
target_link_libraries( ${PROJECT_NAME} libchess Threads::Threads)


add_executable(
  test-${PROJECT_NAME}
  tests/main.cpp
  tests/search.cpp
  tests/movetime.cpp
  tests/position.cpp
  ${SRC_FILES}
)
target_link_libraries( test-${PROJECT_NAME} PRIVATE libchess Threads::Threads)

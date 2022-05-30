cmake_policy(SET CMP0048 NEW)
cmake_minimum_required( VERSION 3.18 )
project( Moinsen VERSION 0.1.0 LANGUAGES CXX )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/" )
add_library(libchess SHARED IMPORTED)
set_target_properties(libchess PROPERTIES
IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libchess/build/shared/libchess.so"
INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -Werror")
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -march=native -O3" )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g" )
set(
  SRC_FILES
  src/uci.hpp
  src/search.cpp
  src/search.hpp
  src/eval.cpp
  src/eval.hpp
)
add_executable(
  ${PROJECT_NAME}
  src/moinsen.cpp
  ${SRC_FILES}
)
target_link_libraries( ${PROJECT_NAME} libchess)


add_executable(
  test-${PROJECT_NAME}
  tests/main.cpp
  tests/search.cpp
  tests/movetime.cpp
  ${SRC_FILES}
)
target_link_libraries( test-${PROJECT_NAME} PRIVATE libchess )

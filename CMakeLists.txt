cmake_policy(SET CMP0048 NEW)
cmake_minimum_required( VERSION 3.18 )
project( Moinsen VERSION 0.1.0 LANGUAGES CXX )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/" )
add_library(libchess STATIC IMPORTED)
set_target_properties(libchess PROPERTIES
IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libchess/build/static/libchess.a"
INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/")
find_package (Threads)

include(CMakeBaseFiles.txt)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -Werror")
add_executable(
  ${PROJECT_NAME}
  src/moinsen.cpp
  src/benchmark.cpp
  ${SRC_FILES}
)
target_link_libraries( ${PROJECT_NAME} libchess Threads::Threads)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_START_STATIC ON)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_END_STATIC ON)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++ -static)


add_executable(
  test-${PROJECT_NAME}
  tests/main.cpp
  tests/search.cpp
  tests/movetime.cpp
  tests/position.cpp
  ${SRC_FILES}
)
target_link_libraries( test-${PROJECT_NAME} PRIVATE libchess Threads::Threads)

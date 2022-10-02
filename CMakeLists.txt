cmake_policy(SET CMP0048 NEW)
cmake_minimum_required( VERSION 3.18 )
project( Moinsen VERSION 0.1.0 LANGUAGES CXX )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/" )
add_library(libchess STATIC IMPORTED)



if (MSVC)
  set_target_properties(libchess PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libchess/build/static/Release/chess.lib"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/")
else()
  set_target_properties(libchess PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libchess/build/static/libchess.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/")
endif()

find_package (Threads)

include(CMakeBaseFiles.txt)

if (UNIX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -Werror")
endif()

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
  tests/koth.cpp
  tests/main.cpp
  tests/movetime.cpp
  tests/phasedScore.cpp
  tests/position.cpp
  tests/search.cpp
  tests/threecheck.cpp
  ${SRC_FILES}
)
target_link_libraries( test-${PROJECT_NAME} PRIVATE libchess Threads::Threads)

add_executable(
  tune-${PROJECT_NAME}
  src/tune.cpp
  ${SRC_FILES}
)
target_link_libraries( tune-${PROJECT_NAME} PRIVATE libchess Threads::Threads "-ltbb")

include(CTest)

add_test(NAME Tests 
    COMMAND $<TARGET_FILE:test-${PROJECT_NAME}>
    WORKING_DIRECTORY ${BUILD_RPATH}
)

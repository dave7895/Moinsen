cmake_minimum_required(VERSION 3.20)

project(inference-Moinsen CXX)
find_package(functions_framework_cpp REQUIRED)
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/" )
add_library(libchess STATIC IMPORTED)
set_target_properties(libchess PROPERTIES
IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libchess/build/static/libchess.a"
INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libchess/src/")
find_package (Threads)
include(CMakeBaseFiles.txt)
add_executable(inferEx infer.cpp ${SRC_FILES})
set_target_properties(inferEx PROPERTIES LINK_SEARCH_START_STATIC ON)
set_target_properties(inferEx PROPERTIES LINK_SEARCH_END_STATIC ON)
target_compile_features(inferEx PRIVATE cxx_std_20)
target_link_options(inferEx PRIVATE -static-libgcc -static-libstdc++ -static)
target_link_libraries(inferEx PRIVATE functions-framework-cpp::framework libchess Threads::Threads)

include(GNUInstallDirs)
install(TARGETS inferEx RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
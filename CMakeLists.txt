cmake_minimum_required(VERSION 3.0)
project(GBEmu)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Use last C++ standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14" )

# Organice the files. Binaries and library.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_JAVA_TARGET_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set directories
set(GLOBAL_INC_FILES ${CMAKE_SOURCE_DIR}/include)
set(GLOBAL_SRC_FILES ${CMAKE_SOURCE_DIR}/src)

# Add directories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(gui)
add_subdirectory(libs/blargg)
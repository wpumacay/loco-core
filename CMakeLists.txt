cmake_minimum_required(VERSION 3.15..3.19)

# -------------------------------------
# Get loco_cmake to help us configure our CMake based project
include(FetchContent)
FetchContent_Declare(
  loco_cmake
  GIT_REPOSITORY https://github.com/wpumacay/loco_cmake.git
  GIT_TAG dev
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(loco_cmake)
include(${loco_cmake_SOURCE_DIR}/Index.cmake)
# -------------------------------------

project(
  Loco
  VERSION 0.0.1
  DESCRIPTION "A locomotion framework for DeepRL research"
  HOMEPAGE_URL "https://github.com/wpumacay/loco-core"
  LANGUAGES C CXX)

option(LOCO_BUILD_LOGS "Build with logs enabled" ON)
option(LOCO_BUILD_PROFILING "Build with profiling tools enabled" ON)
option(LOCO_BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
option(LOCO_BUILD_EXAMPLES "Build C/C++ examples" ON)
option(LOCO_BUILD_TESTS "Build C/C++ tests" ON)
option(LOCO_BUILD_DOCS "Build documentation" ON)

loco_initialize_project()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(Dependencies)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/loco/)

# cmake-format: off
# -------------------------------------
loco_create_target(loco STATIC
  SOURCES
    ${SOURCE_DIR}/core/common.cpp
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  TARGET_DEPENDENCIES
    math::math utils::utils tinyxml2::tinyxml2
  WARNING_AS_ERRORS
    FALSE)

# cmake-format: on

if(LOCO_BUILD_LOGS)
  target_compile_definitions(loco PUBLIC -DLOCO_LOGS_ENABLED)
endif()

if(LOCO_BUILD_PROFILING)
  target_compile_definitions(loco PUBLIC -DLOCO_PROFILING_ENABLED)
endif()

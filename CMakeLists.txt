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

option(LOCO_BUILD_BACKEND_MUJOCO "Build with support for MuJoCo" ON)
option(LOCO_BUILD_BACKEND_BULLET "Build with support for Bullet" ON)
option(LOCO_BUILD_BACKEND_DART "Build with support for Dart" ON)

loco_initialize_project()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(Dependencies)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/loco/)

# cmake-format: off
# -------------------------------------
loco_create_target(LocoCoreCpp STATIC
  SOURCES
    ${SOURCE_DIR}/core/common.cpp
    ${SOURCE_DIR}/core/scenario_t.cpp
    ${SOURCE_DIR}/core/simulation_t.cpp
    # Bullet backend implementations
    ${SOURCE_DIR}/backends/bullet/common_bullet.cpp
    ${SOURCE_DIR}/backends/bullet/simulation_impl_bullet.cpp
    # MuJoCo backend implementations
    ${SOURCE_DIR}/backends/mujoco/common_mujoco.cpp
    ${SOURCE_DIR}/backends/mujoco/simulation_impl_mujoco.cpp
    # DART backend implementation
    ${SOURCE_DIR}/backends/dart/common_dart.cpp
    ${SOURCE_DIR}/backends/dart/simulation_impl_dart.cpp
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  TARGET_DEPENDENCIES
    math::math
    utils::utils
    tinyxml2::tinyxml2
    mujoco::mujoco
    bullet::bullet
    dart::dart
  WARNINGS_AS_ERRORS
    FALSE)
# cmake-format: on

include(Definitions)

add_library(loco::core ALIAS LocoCoreCpp)

if(LOCO_BUILD_EXAMPLES)
  add_subdirectory(examples/cpp)
endif()

cmake_minimum_required(VERSION 3.15..3.19)

# -------------------------------------
# Get loco_cmake to help us configure our CMake based project
include(FetchContent)
FetchContent_Declare(
  loco_cmake
  GIT_REPOSITORY https://github.com/wpumacay/loco_cmake.git
  GIT_TAG e210ab141a1a85dd1330d436530319ba6bf8c42c # Version v0.1.2
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(loco_cmake)
include(${loco_cmake_SOURCE_DIR}/Index.cmake)
# -------------------------------------

project(
  Loco
  VERSION 0.1.2
  DESCRIPTION "A locomotion framework for DeepRL research"
  HOMEPAGE_URL "https://github.com/wpumacay/loco-core"
  LANGUAGES C CXX)

option(LOCO_BUILD_LOGS "Build with logs enabled" ON)
option(LOCO_BUILD_PROFILING "Build with profiling tools enabled" ON)
option(LOCO_BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
option(LOCO_BUILD_EXAMPLES "Build C/C++ examples" ON)
option(LOCO_BUILD_TESTS "Build C/C++ tests" ON)
option(LOCO_BUILD_DOCS "Build documentation" ON)

option(LOCO_BUILD_BACKEND_MUJOCO "Build with support for MuJoCo" OFF)
option(LOCO_BUILD_BACKEND_BULLET "Build with support for Bullet" OFF)
option(LOCO_BUILD_BACKEND_DART "Build with support for Dart" OFF)

option(LOCO_BUILD_VISUALIZER_OPENGL
       "Build with support for our OpenGL visualizer" OFF)
option(LOCO_BUILD_VISUALIZER_MESHCAT
       "Build with support for external visualizer based on MeshcatCpp" OFF)

loco_initialize_project()

# -------------------------------------
# Allow the usage of our helper macros and functions
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# -------------------------------------
# Bring our dependencies accordingly
include(LocoDependencies)

# cmake-format: off
# -------------------------------------
loco_create_target(LocoCoreCpp STATIC
  SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/loco/core/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/loco/core/single_body/single_body_t.cpp
    ## ${CMAKE_CURRENT_SOURCE_DIR}/source/loco/core/simulation_t.cpp
    ## ${CMAKE_CURRENT_SOURCE_DIR}/source/loco/core/visualizer_t.cpp
    ## ${CMAKE_CURRENT_SOURCE_DIR}/source/loco/core/scenario_t.cpp
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  TARGET_DEPENDENCIES
    math::math
    utils::utils
    tinyxml2::tinyxml2
  WARNINGS_AS_ERRORS
    FALSE)
# cmake-format: on

# -------------------------------------
# Quick workaround to enable RPATH for our target
set_target_properties(
  LocoCoreCpp
  PROPERTIES INSTALL_RPATH $ORIGIN
             INSTALL_RPATH_USE_LINK_PATH OFF
             BUILD_WITH_INSTALL_RPATH ON
             MACOSX_RPATH ON)

include(LocoBackendsSetup)
include(LocoVisualizersSetup)
include(LocoDefinitions)

# -------------------------------------
# Expose an alias for the library (CMake namespace convention)
add_library(loco::core ALIAS LocoCoreCpp)

# -------------------------------------
# Add C++ examples to the build process
if(LOCO_BUILD_EXAMPLES)
  add_subdirectory(examples/cpp)
endif()

# -------------------------------------
# Add C++ tests to the build process
if(LOCO_BUILD_TESTS)
  add_subdirectory(tests/cpp)
endif()

# -------------------------------------
# Add Python bindings to the build process
if(LOCO_BUILD_PYTHON_BINDINGS)
  add_subdirectory(python/loco/bindings)
endif()

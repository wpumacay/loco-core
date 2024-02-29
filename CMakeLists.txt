cmake_minimum_required(VERSION 3.15..3.19)

# -------------------------------------
# Get loco_cmake to help us configure our CMake based project
include(FetchContent)
FetchContent_Declare(
  loco_cmake
  GIT_REPOSITORY https://github.com/wpumacay/loco_cmake.git
  GIT_TAG 0270626101bec01dbf9ae7f1b5febfb82092bf0c # Version v0.1.5
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(loco_cmake)
include(${loco_cmake_SOURCE_DIR}/Index.cmake)
# -------------------------------------

project(
  Loco
  VERSION 0.1.3
  DESCRIPTION "A locomotion framework for DeepRL research"
  HOMEPAGE_URL "https://github.com/wpumacay/loco-core"
  LANGUAGES C CXX)

option(LOCO_BUILD_LOGS "Build with logs enabled" ON)
option(LOCO_BUILD_PROFILING "Build with profiling tools enabled" ON)
option(LOCO_BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
option(LOCO_BUILD_EXAMPLES "Build C/C++ examples" ON)
option(LOCO_BUILD_TESTS "Build C/C++ tests" ON)
option(LOCO_BUILD_DOCS "Build documentation" OFF)

option(LOCO_BUILD_BACKEND_MUJOCO "Build with support for MuJoCo" OFF)
option(LOCO_BUILD_BACKEND_BULLET "Build with support for Bullet" OFF)
option(LOCO_BUILD_BACKEND_DART "Build with support for Dart" OFF)

option(LOCO_BUILD_VISUALIZER_OPENGL
       "Build with support for our OpenGL visualizer" OFF)
option(LOCO_BUILD_VISUALIZER_MESHCAT
       "Build with support for external visualizer based on MeshcatCpp" ON)

# cmake-format: off
set(LOCO_BUILD_CXX_STANDARD 17 CACHE STRING "The C++ standard to be used")
set_property(CACHE LOCO_BUILD_CXX_STANDARD PROPERTY STRINGS 11 14 17 20)
# cmake-format: on

loco_initialize_project(CXX_STANDARD ${LOCO_BUILD_CXX_STANDARD})

# -------------------------------------
# Allow the usage of our helper macros and functions
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# -------------------------------------
# Bring our dependencies accordingly
include(LocoDependencies)

# cmake-format: off
# -------------------------------------
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
loco_create_target(LocoCoreCpp SHARED
  SOURCES
    ${SOURCE_DIR}/loco/core/common.cpp
    ${SOURCE_DIR}/loco/core/visualizer/drawable_t.cpp
    ${SOURCE_DIR}/loco/core/visualizer/drawable_primitives.cpp
    ${SOURCE_DIR}/loco/core/single_body/single_body_collider_t.cpp
    ${SOURCE_DIR}/loco/core/single_body/single_body_t.cpp
    ${SOURCE_DIR}/loco/core/scenario_t.cpp
    ${SOURCE_DIR}/loco/core/visualizer/visualizer_t.cpp
    ## ${CMAKE_CURRENT_SOURCE_DIR}/source/loco/core/simulation_t.cpp
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  TARGET_DEPENDENCIES
    math::math
    utils::utils
    tinyxml2::tinyxml2
  CXX_STANDARD
    ${LOCO_BUILD_CXX_STANDARD}
  WARNINGS_AS_ERRORS
    FALSE)
# cmake-format: on

# -------------------------------------
# # Quick workaround to enable RPATH for our target set_target_properties(
# LocoCoreCpp PROPERTIES INSTALL_RPATH $ORIGIN INSTALL_RPATH_USE_LINK_PATH OFF
# BUILD_WITH_INSTALL_RPATH ON MACOSX_RPATH ON)

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

include_guard()

# ~~~
# CMake configuration for third-party dependencies.
#
# Dependencies:
# * json
# * tinyxml2
# * pybind11
# * catch2
# * utils
# * math
#
# - Based on the superbuild script by jeffamstutz for ospray
#   https://github.com/jeffamstutz/superbuild_ospray/blob/main/macros.cmake
# - Based on MuJoCo's helper find_or_fetch content macro
#   https://github.com/deepmind/mujoco/blob/main/cmake/FindOrFetch.cmake
# ~~~

# -------------------------------------
option(FIND_OR_FETCH_USE_SYSTEM_PACKAGE
       "Whether or not to give priority to system-wide package search" OFF)

# cmake-format: off

# ------------------------------------------------------------------------------
# MuJoCo is one of the supported physics backends for simulation
# ------------------------------------------------------------------------------
# Make sure we avoid including tests (might conflict due to gtest)
set(MUJOCO_BUILD_TESTS OFF CACHE BOOL "Don't build MuJoCo unittests")

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME mujoco
  LIBRARY_NAME mujoco
  GIT_REPO https://github.com/deepmind/mujoco.git
  GIT_TAG 2.3.0
  TARGETS mujoco::mujoco
  BUILD_ARGS
    -DMUJOCO_BUILD_EXAMPLES=OFF
    -DMUJOCO_BUILD_SIMULATE=OFF
    -DMUJOCO_BUILD_TESTS=OFF
    -DMUJOCO_TEST_PYTHON_UTIL=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Bullet is one of the supported physics backends for simulation
# ------------------------------------------------------------------------------
# Make sure we avoid including tests (might conflict due to gtest)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Don't build bullet unittests")

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME Bullet
  LIBRARY_NAME bullet
  GIT_REPO https://github.com/bulletphysics/bullet3.git
  GIT_TAG 3.24
  TARGETS LinearMath BulletCollision BulletDynamics
  BUILD_ARGS
    -DBUILD_BULLET2_DEMOS=OFF
    -DBUILD_BULLET3=OFF
    -DBUILD_PYBULLET=OFF
    -DBUILD_EXTRAS=OFF
    -DBUILD_CLSOCKET=OFF
    -DBUILD_CPU_DEMOS=OFF
    -DBUILD_EGL=OFF
    -DBUILD_ENET=OFF
    -DBUILD_OPENGL3_DEMOS=OFF
    -DBUILD_UNIT_TESTS=OFF
  EXCLUDE_FROM_ALL)

# Group the required bullet libraries into a single target to ease its usage
add_library(bullet INTERFACE)
target_link_libraries(bullet INTERFACE LinearMath)
target_link_libraries(bullet INTERFACE BulletCollision)
target_link_libraries(bullet INTERFACE BulletDynamics)
target_include_directories(bullet INTERFACE ${bullet_SOURCE_DIR}/src)
add_library(bullet::bullet ALIAS bullet)

# ------------------------------------------------------------------------------
# DART is one of the supported physics backends for simulation
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME dart
  LIBRARY_NAME dart
  GIT_REPO https://github.com/dartsim/dart.git
  GIT_TAG v6.12.2
  TARGETS dart dart-collision-bullet
  BUILD_ARGS
    -DDART_BUILD_GUI_OSG=OFF
    -DDART_BUILD_EXTRAS=OFF
    -DDART_BUILD_DARTPY=OFF
    -DDART_CODECOV=OFF
  PATCH_COMMAND
    "${GIT_EXECUTABLE}"
    "apply"
    "-q"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/dart-no-uninstall-target.patch"
    "||"
    "${CMAKE_COMMAND}"
    "-E"
    "true"
  EXCLUDE_FROM_ALL)

# Group the required DART libraries into a single target to ease its usage
add_library(dart_libs INTERFACE)
target_link_libraries(dart_libs INTERFACE dart)
target_link_libraries(dart_libs INTERFACE dart-collision-bullet)
target_include_directories(dart_libs INTERFACE ${dart_SOURCE_DIR}/src)
add_library(dart::dart ALIAS dart_libs)

# ------------------------------------------------------------------------------
# Use leethomason's xml library (parse urdf resource files)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME tinyxml2
  LIBRARY_NAME tinyxml2
  GIT_REPO https://github.com/leethomason/tinyxml2
  GIT_TAG 9.0.0
  TARGETS tinyxml2::tinyxml2
  BUILD_ARGS
    -Dtinyxml2_BUILD_TESTING=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Spdlog is used for the logging functionality (internally uses the fmt lib)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME spdlog
  LIBRARY_NAME spdlog
  GIT_REPO https://github.com/gabime/spdlog.git
  GIT_TAG v1.9.2
  TARGETS spdlog::spdlog
  BUILD_ARGS
    -DSPDLOG_BUILD_SHARED=OFF
    -DSPDLOG_BUILD_EXAMPLE=OFF
    -DSPDLOG_BUILD_EXAMPLE_HO=OFF
    -DSPDLOG_BUILD_TESTS=OFF
    -DSPDLOG_BUILD_TESTS_HO=OFF
    -DSPDLOG_BUILD_BENCH=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API
# ------------------------------------------------------------------------------
# Hint to Pybind11 that we want to use the newer FindPython module from CMake
set(PYBIND11_FINDPYTHON ON CACHE BOOL "Use newer FindPython (CMake 3.15+)")

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME pybind11
  LIBRARY_NAME pybind11
  GIT_REPO https://github.com/pybind/pybind11.git
  GIT_TAG v2.10.0
  TARGETS pybind11::headers
  BUILD_ARGS
    -DPYBIND11_TEST=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Catch2 is used for making unit-tests in C++ land. It's API is simple yet quite
# powerfull (e.g. we can make use of template-parametrized tests-cases)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME Catch2
  LIBRARY_NAME catch2
  GIT_REPO https://github.com/catchorg/Catch2.git
  GIT_TAG v2.x
  TARGETS Catch2::Catch2
  BUILD_ARGS
    -DCATCH_INSTALL_DOCS=OFF
    -DCATCH_INSTALL_EXTRAS=OFF
    -DCATCH_DEVELOPMENT_BUILD=OFF
  EXCLUDE_FROM_ALL)

# Add custom scripts for test-case registration to the module path
if (catch2_POPULATED)
  list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib")
endif()

# ------------------------------------------------------------------------------
# LocoUtils exposes some utilities that we'll use (like logs, profiling, etc.)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME utils
  GIT_REPO https://github.com/wpumacay/utils.git
  GIT_TAG dev
  TARGETS utils::utils
  BUILD_ARGS
    -DLOCOUTILS_BUILD_PYTHON_BINDINGS=ON
    -DLOCOUTILS_BUILD_EXAMPLES=OFF
    -DLOCOUTILS_BUILD_DOCS=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# LocoMath is used as math library (defines vectors, matrices, and operations
# that could be used on these types). The API is similar to Eigen's
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME math
  GIT_REPO https://github.com/wpumacay/math.git
  GIT_TAG dev
  TARGETS math::math math::math_py_helpers
  BUILD_ARGS
    -DLOCOMATH_BUILD_PYTHON_BINDINGS=ON
    -DLOCOMATH_BUILD_EXAMPLES=OFF
    -DLOCOMATH_BUILD_TESTS=OFF
    -DLOCOMATH_BUILD_DOCS=OFF
  EXCLUDE_FROM_ALL)

# cmake-format: on

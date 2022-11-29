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

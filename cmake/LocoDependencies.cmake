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
set(LOCO_DEP_VERSION_mujoco
    95a07e85ccaf31a7daabfb2f34f376e75534881d # Release 2.3.2
    CACHE STRING "Version of MuJoCo to be fetched (used as physics backend)")

set(LOCO_DEP_VERSION_bullet
    2c204c49e56ed15ec5fcfa71d199ab6d6570b3f5 # Release 3.25
    CACHE STRING "Version of Bullet to be fetched (used as physics backend)")

set(LOCO_DEP_VERSION_dart
    c5b8f0abfb2545754ebc872e4aad78020da88a62 # Release 6.13
    CACHE STRING "Version of DART to be fetched (used as physics backend)")

set(LOCO_DEP_VERSION_catch2
    182c910b4b63ff587a3440e08f84f70497e49a81 # Release 2.13.10
    CACHE STRING "Version of Catch2 to be fetched (used for unittests)")

set(LOCO_DEP_VERSION_tinyxml2
    1dee28e51f9175a31955b9791c74c430fe13dc82 # Release 9.0.0
    CACHE STRING
          "Version of TinyXML2 to be fetched (used to handle urdf and mjcf)")

set(LOCO_DEP_VERSION_renderer
    06e74a25ca30ef3b234507cef71a8eea77822b32 # Version v0.3.2
    CACHE STRING "Version of the internal renderer to be fetched")

set(LOCO_DEP_VERSION_utils
    58623f20534cbe4cc3f20cdeac46a27ca4f21790 # Version v0.2.2
    CACHE STRING "Version of internal utilities repo to be fetched")

set(LOCO_DEP_VERSION_math
    7f863e0dbf9f0e9ba0d35b38ab912baf7431053f # Version v0.5.1
    CACHE STRING "Version of internal math repo to be fetched")

set(LOCO_DEP_VERSION_pybind11
    5b0a6fc2017fcc176545afe3e09c9f9885283242 # Release v2.10.4
    CACHE STRING "Version of PyBind11 to be fetched (used for python bindings)")

mark_as_advanced(LOCO_DEP_VERSION_mujoco)
mark_as_advanced(LOCO_DEP_VERSION_bullet)
mark_as_advanced(LOCO_DEP_VERSION_dart)
mark_as_advanced(LOCO_DEP_VERSION_catch2)
mark_as_advanced(LOCO_DEP_VERSION_tinyxml2)
mark_as_advanced(LOCO_DEP_VERSION_renderer)
mark_as_advanced(LOCO_DEP_VERSION_utils)
mark_as_advanced(LOCO_DEP_VERSION_math)
mark_as_advanced(LOCO_DEP_VERSION_pybind11)

option(FIND_OR_FETCH_USE_SYSTEM_PACKAGE
       "Whether or not to give priority to system-wide package search" OFF)

# cmake-format: off

# ------------------------------------------------------------------------------
# MuJoCo is one of the supported physics backends for simulation
# ------------------------------------------------------------------------------
if(LOCO_BUILD_BACKEND_MUJOCO)
  # Make sure we avoid including tests (might conflict due to gtest)
  set(MUJOCO_BUILD_TESTS OFF CACHE BOOL "Don't build MuJoCo unittests")

  loco_find_or_fetch_dependency(
    USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
    PACKAGE_NAME mujoco
    LIBRARY_NAME mujoco
    GIT_REPO https://github.com/deepmind/mujoco.git
    GIT_TAG ${LOCO_DEP_VERSION_mujoco}
    GIT_PROGRESS FALSE
    TARGETS mujoco::mujoco
    BUILD_ARGS
      -DMUJOCO_BUILD_EXAMPLES=OFF
      -DMUJOCO_BUILD_SIMULATE=OFF
      -DMUJOCO_BUILD_TESTS=OFF
      -DMUJOCO_TEST_PYTHON_UTIL=OFF
    EXCLUDE_FROM_ALL)

  # If using system package, make sure the imported target is found
  if(USE_SYSTEM_PACKAGE)
    if(NOT TARGET mujoco::mujoco)
      message(WARNING "Couldn't find imported target mujoco::mujoco")
    endif()
  endif()
endif()

# ------------------------------------------------------------------------------
# Bullet is one of the supported physics backends for simulation
# ------------------------------------------------------------------------------
if(LOCO_BUILD_BACKEND_BULLET)
  # Make sure we avoid including tests (might conflict due to gtest)
  set(BUILD_UNIT_TESTS OFF CACHE BOOL "Don't build bullet unittests")

  loco_find_or_fetch_dependency(
    USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
    PACKAGE_NAME Bullet
    LIBRARY_NAME bullet
    GIT_REPO https://github.com/bulletphysics/bullet3.git
    GIT_TAG ${LOCO_DEP_VERSION_bullet}
    GIT_PROGRESS FALSE
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
  if (FIND_OR_FETCH_USE_SYSTEM_PACKAGE)
    # Embed the exported variables from the system package
    target_include_directories(bullet INTERFACE ${BULLET_INCLUDE_DIRS})
    target_link_libraries(bullet INTERFACE ${BULLET_LIBRARIES})
  else()
    # Embed information created by Bullet's CMake workflow
    target_link_libraries(bullet INTERFACE LinearMath)
    target_link_libraries(bullet INTERFACE BulletCollision)
    target_link_libraries(bullet INTERFACE BulletDynamics)
    target_include_directories(bullet INTERFACE ${bullet_SOURCE_DIR}/src)
  endif()
  add_library(bullet::bullet ALIAS bullet)
endif()

# ------------------------------------------------------------------------------
# DART is one of the supported physics backends for simulation
# ------------------------------------------------------------------------------
if(LOCO_BUILD_BACKEND_DART)
  if(NOT FIND_OR_FETCH_USE_SYSTEM_PACKAGE)
    loco_find_or_fetch_dependency(
      USE_SYSTEM_PACKAGE FALSE
      PACKAGE_NAME DART
      LIBRARY_NAME ${LOCO_DEP_VERSION_dart}
      GIT_REPO https://github.com/dartsim/dart.git
      GIT_TAG v6.12.2
      GIT_PROGRESS FALSE
      TARGETS dart dart-collision-bullet dart-collision-ode
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

    add_library(dart_libs INTERFACE)
    target_link_libraries(dart_libs INTERFACE dart)
    target_link_libraries(dart_libs INTERFACE dart-collision-bullet)
    target_link_libraries(dart_libs INTERFACE dart-collision-ode)
    target_include_directories(dart_libs INTERFACE ${dart_SOURCE_DIR}/src)
    add_library(dart::dart ALIAS dart_libs)
  else()
    find_package(DART REQUIRED COMPONENTS collision-bullet collision-ode CONFIG)
    find_package(Eigen3 REQUIRED)

    add_library(dart_libs INTERFACE)
    target_link_libraries(dart_libs INTERFACE dart)
    target_link_libraries(dart_libs INTERFACE dart-collision-bullet)
    target_link_libraries(dart_libs INTERFACE dart-collision-ode)
    target_link_libraries(dart_libs INTERFACE Eigen3::Eigen)
    add_library(dart::dart ALIAS dart_libs)
  endif()
endif()

# ------------------------------------------------------------------------------
# Use leethomason's xml library (parse urdf resource files)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME tinyxml2
  LIBRARY_NAME tinyxml2
  GIT_REPO https://github.com/leethomason/tinyxml2
  GIT_TAG ${LOCO_DEP_VERSION_tinyxml2}
  GIT_PROGRESS FALSE
  TARGETS tinyxml2::tinyxml2
  BUILD_ARGS
    -Dtinyxml2_BUILD_TESTING=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME pybind11
  LIBRARY_NAME pybind11
  GIT_REPO https://github.com/pybind/pybind11.git
  GIT_TAG ${LOCO_DEP_VERSION_pybind11}
  GIT_PROGRESS FALSE
  TARGETS pybind11::headers
  BUILD_ARGS
    -DPYBIND11_TEST=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Catch2 is used for making unit-tests in C++ land. It's API is simple yet quite
# powerfull (e.g. we can make use of template-parametrized tests-cases)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME Catch2
  LIBRARY_NAME catch2
  GIT_REPO https://github.com/catchorg/Catch2.git
  GIT_TAG ${LOCO_DEP_VERSION_catch2}
  GIT_PROGRESS FALSE
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
# 'Renderer' is a minimal rendering engine that we'll use for visualization
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME renderer
  GIT_REPO https://github.com/wpumacay/renderer.git
  GIT_TAG ${LOCO_DEP_VERSION_renderer}
  GIT_PROGRESS FALSE
  TARGETS renderer::renderer
  BUILD_ARGS
    -DRENDERER_BUILD_PYTHON_BINDINGS=ON
    -DRENDERER_BUILD_EXAMPLES=OFF
    -DRENDERER_BUILD_TESTS=OFF
    -DRENDERER_BUILD_DOCS=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# 'Utils' exposes some utilities that we'll use (like logs, profiling, etc.)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME utils
  GIT_REPO https://github.com/wpumacay/utils.git
  GIT_TAG ${LOCO_DEP_VERSION_utils}
  GIT_PROGRESS FALSE
  TARGETS utils::utils
  BUILD_ARGS
    -DUTILS_BUILD_PYTHON_BINDINGS=ON
    -DUTILS_BUILD_EXAMPLES=OFF
    -DUTILS_BUILD_DOCS=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# 'Math' is used as math library (defines vectors, matrices, and operations that
# could be used on these types). The API is similar to the one used by Eigen.
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME math
  GIT_REPO https://github.com/wpumacay/math.git
  GIT_TAG ${LOCO_DEP_VERSION_math}
  GIT_PROGRESS FALSE
  TARGETS math::math math::math_py_helpers
  BUILD_ARGS
    -DMATH_BUILD_PYTHON_BINDINGS=ON
    -DMATH_BUILD_EXAMPLES=OFF
    -DMATH_BUILD_TESTS=OFF
    -DMATH_BUILD_DOCS=OFF
  EXCLUDE_FROM_ALL)

# cmake-format: on

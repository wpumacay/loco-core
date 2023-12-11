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
    8f9c690c8593df7531597aa1a3589febb731ff0a # Release 3.0.0
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
    574d4bd9ac349e066f5367d3c21bcd0e612fe1a5 # Version v0.3.9
    CACHE STRING "Version of the internal renderer to be fetched")

set(LOCO_DEP_VERSION_utils
    687d4dea4b55afd13405d00b7aef6993e056b36d # Version v0.2.9
    CACHE STRING "Version of internal utilities repo to be fetched")

set(LOCO_DEP_VERSION_math
    a31f55fb57983286ad8e30c8915b3461d9ce8557 # Version v0.6.7
    CACHE STRING "Version of internal math repo to be fetched")

set(LOCO_DEP_VERSION_pybind11
    8a099e44b3d5f85b20f05828d919d2332a8de841 # Release v2.11.1
    CACHE STRING "Version of PyBind11 to be fetched (used for python bindings)")

set(LOCO_DEP_VERSION_meshcatcpp
    a84be7add7f344d61e615bee7f26e6a7d5444f2a
    CACHE STRING "Version of MeshcatCpp to be fetched (for meshcat visualizer")

mark_as_advanced(LOCO_DEP_VERSION_mujoco)
mark_as_advanced(LOCO_DEP_VERSION_bullet)
mark_as_advanced(LOCO_DEP_VERSION_dart)
mark_as_advanced(LOCO_DEP_VERSION_catch2)
mark_as_advanced(LOCO_DEP_VERSION_tinyxml2)
mark_as_advanced(LOCO_DEP_VERSION_renderer)
mark_as_advanced(LOCO_DEP_VERSION_utils)
mark_as_advanced(LOCO_DEP_VERSION_math)
mark_as_advanced(LOCO_DEP_VERSION_pybind11)
mark_as_advanced(LOCO_DEP_VERSION_meshcatcpp)

option(FIND_OR_FETCH_USE_SYSTEM_PACKAGE
       "Whether or not to give priority to system-wide package search" OFF)

# cmake-format: off

# ------------------------------------------------------------------------------
# MuJoCo is one of the supported physics backends for simulation
# ------------------------------------------------------------------------------
if(LOCO_BUILD_BACKEND_MUJOCO)
  set(MUJOCO_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(MUJOCO_BUILD_SIMULATE OFF CACHE BOOL "" FORCE)
  set(MUJOCO_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(MUJOCO_TEST_PYTHON_UTIL OFF CACHE BOOL "" FORCE)

  loco_find_or_fetch_dependency(
    USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
    PACKAGE_NAME mujoco
    LIBRARY_NAME mujoco
    GIT_REPO https://github.com/deepmind/mujoco.git
    GIT_TAG ${LOCO_DEP_VERSION_mujoco}
    GIT_PROGRESS FALSE
    GIT_SHALLOW TRUE
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
  set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Don't build bullet unittests")
  set(BUILD_BULLET3 OFF CACHE BOOL "Don't build bullet unittests")
  set(BUILD_PYBULLET OFF CACHE BOOL "Don't build bullet unittests")
  set(BUILD_EXTRAS OFF CACHE BOOL "Don't build bullet unittests")
  set(BUILD_CLSOCKET OFF CACHE BOOL "Don't build bullet unittests")
  set(BUILD_CPU_DEMOS OFF CACHE BOOL "Don't build bullet unittests")
  set(BUILD_EGL OFF CACHE BOOL "Don't build bullet unittests")
  set(BUILD_ENET OFF CACHE BOOL "Don't build bullet unittests")
  set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "Don't build bullet unittests")
  set(BUILD_UNIT_TESTS OFF CACHE BOOL "Don't build bullet unittests")

  loco_find_or_fetch_dependency(
    USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
    PACKAGE_NAME Bullet
    LIBRARY_NAME bullet
    GIT_REPO https://github.com/bulletphysics/bullet3.git
    GIT_TAG ${LOCO_DEP_VERSION_bullet}
    GIT_PROGRESS FALSE
    GIT_SHALLOW TRUE
    TARGETS LinearMath BulletCollision BulletDynamics
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
    set(DART_BUILD_GUI_OSG OFF CACHE BOOL "" FORCE)
    set(DART_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
    set(DART_BUILD_DARTPY OFF CACHE BOOL "" FORCE)
    set(DART_CODECOV OFF CACHE BOOL "" FORCE)

    loco_find_or_fetch_dependency(
      USE_SYSTEM_PACKAGE FALSE
      PACKAGE_NAME DART
      LIBRARY_NAME ${LOCO_DEP_VERSION_dart}
      GIT_REPO https://github.com/dartsim/dart.git
      GIT_TAG v6.12.2
      GIT_PROGRESS FALSE
      GIT_SHALLOW TRUE
      TARGETS dart dart-collision-bullet dart-collision-ode
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
set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME tinyxml2
  LIBRARY_NAME tinyxml2
  GIT_REPO https://github.com/leethomason/tinyxml2
  GIT_TAG ${LOCO_DEP_VERSION_tinyxml2}
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE
  TARGETS tinyxml2::tinyxml2
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API
# ------------------------------------------------------------------------------
set(PYBIND11_TEST OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME pybind11
  LIBRARY_NAME pybind11
  GIT_REPO https://github.com/pybind/pybind11.git
  GIT_TAG ${LOCO_DEP_VERSION_pybind11}
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE
  TARGETS pybind11::headers
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Catch2 is used for making unit-tests in C++ land. It's API is simple yet quite
# powerfull (e.g. we can make use of template-parametrized tests-cases)
# ------------------------------------------------------------------------------
set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "" FORCE)
set(CATCH_DEVELOPMENT_BUILD OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME Catch2
  LIBRARY_NAME catch2
  GIT_REPO https://github.com/catchorg/Catch2.git
  GIT_TAG ${LOCO_DEP_VERSION_catch2}
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE
  TARGETS Catch2::Catch2
  EXCLUDE_FROM_ALL)

# Add custom scripts for test-case registration to the module path
if (catch2_POPULATED)
  list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib")
endif()

# ------------------------------------------------------------------------------
# MeshCatCpp is a self contained C++ interface of the MeshCat visualizer. It
# allows us to integrate MeshCat as one more visualizer one minto Loco
# ------------------------------------------------------------------------------

if (LOCO_BUILD_VISUALIZER_MESHCAT)
  set(MESHCAT_CPP_BUILT_EXAMPLES OFF CACHE BOOL "" FORCE)

  loco_find_or_fetch_dependency(
    USE_SYSTEM_PACKAGE FALSE
    PACKAGE_NAME MeshcatCpp
    LIBRARY_NAME meshcatcpp
    GIT_REPO https://github.com/ami-iit/meshcat-cpp.git
    GIT_TAG ${LOCO_DEP_VERSION_meshcatcpp}
    GIT_PROGRESS FALSE
    GIT_SHALLOW FALSE
    TARGETS MeshcatCpp::MeshcatCpp
    EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------
# 'Renderer' is a minimal rendering engine that we'll use for visualization
# ------------------------------------------------------------------------------
if (LOCO_BUILD_VISUALIZER_OPENGL)
  set(RENDERER_BUILD_PYTHON_BINDINGS OFF CACHE BOOL "" FORCE)
  set(RENDERER_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(RENDERER_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(RENDERER_BUILD_DOCS OFF CACHE BOOL "" FORCE)

  loco_find_or_fetch_dependency(
    USE_SYSTEM_PACKAGE FALSE
    LIBRARY_NAME renderer
    GIT_REPO https://github.com/wpumacay/renderer.git
    GIT_TAG ${LOCO_DEP_VERSION_renderer}
    GIT_PROGRESS FALSE
    GIT_SHALLOW FALSE
    TARGETS renderer::renderer
    EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------
# 'Utils' exposes some utilities that we'll use (like logs, profiling, etc.)
# ------------------------------------------------------------------------------
set(UTILS_BUILD_PYTHON_BINDINGS OFF CACHE BOOL "" FORCE)
set(UTILS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(UTILS_BUILD_DOCS OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME utils
  GIT_REPO https://github.com/wpumacay/utils.git
  GIT_TAG ${LOCO_DEP_VERSION_utils}
  GIT_PROGRESS FALSE
  GIT_SHALLOW FALSE
  TARGETS utils::utils
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# 'Math3d' is used as math library (defines vectors, matrices, and operations
# that could be used on these types). The API is similar to Eigen
# ------------------------------------------------------------------------------
set(MATH_BUILD_PYTHON_BINDINGS OFF CACHE BOOL "" FORCE)
set(MATH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MATH_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MATH_BUILD_DOCS OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME math
  GIT_REPO https://github.com/wpumacay/math.git
  GIT_TAG ${LOCO_DEP_VERSION_math}
  GIT_PROGRESS FALSE
  GIT_SHALLOW FALSE
  TARGETS math::math
  EXCLUDE_FROM_ALL)

# cmake-format: on

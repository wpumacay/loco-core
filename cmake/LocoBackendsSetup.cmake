include_guard()

if(NOT TARGET LocoCoreCpp)
  return()
endif()

# -------------------------------------
# Add sources according to the platforms that are enabled by the user
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/loco)

if(LOCO_BUILD_BACKEND_MUJOCO)
  target_sources(
    LocoCoreCpp
    PRIVATE ${SOURCE_DIR}/backends/mujoco/common_mujoco.cpp
            ${SOURCE_DIR}/backends/mujoco/simulation_impl_mujoco.cpp)
  target_link_libraries(LocoCoreCpp PUBLIC mujoco::mujoco)
endif()

if(LOCO_BUILD_BACKEND_BULLET)
  target_sources(
    LocoCoreCpp
    PRIVATE ${SOURCE_DIR}/backends/bullet/common_bullet.cpp
            ${SOURCE_DIR}/backends/bullet/simulation_impl_bullet.cpp)
  target_link_libraries(LocoCoreCpp PUBLIC bullet::bullet)
endif()

if(LOCO_BUILD_BACKEND_DART)
  target_sources(
    LocoCoreCpp PRIVATE ${SOURCE_DIR}/backends/dart/common_dart.cpp
                        ${SOURCE_DIR}/backends/dart/simulation_impl_dart.cpp)
  target_link_libraries(LocoCoreCpp PUBLIC dart::dart)
endif()

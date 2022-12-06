#pragma once

#include <mujoco/mujoco.h>

#include <loco/core/common.hpp>

namespace loco {
namespace mujoco {

/// Deleter for mjModel (when using unique_ptr)
struct MjcModelDeleter {
    auto operator()(mjModel* ptr) const -> void;
};

/// Deleter for mjData (when using unique_ptr)
struct MjcDataDeleter {
    auto operator()(mjData* ptr) const -> void;
};

/// Deleter for mjvScene (when using unique_ptr)
struct MjvSceneDeleter {
    auto operator()(mjvScene* ptr) const -> void;
};

/// Converts the given MuJoCo vec3 pointer into our vec3_t type
auto vec3_from_mjc(const mjtNum* p_vec) -> Vec3;

/// Converts the given MuJoCo quat pointer into our quat_t type
auto quat_from_mjc(const mjtNum* p_quat) -> Quat;

}  // namespace mujoco
}  // namespace loco

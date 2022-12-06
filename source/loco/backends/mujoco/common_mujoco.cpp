#include <loco/backends/mujoco/common_mujoco.hpp>

namespace loco {
namespace mujoco {

auto MjcModelDeleter::operator()(mjModel* ptr) const -> void {
    if (ptr != nullptr) {
        mj_deleteModel(ptr);
    }
}

auto MjcDataDeleter::operator()(mjData* ptr) const -> void {
    if (ptr != nullptr) {
        mj_deleteData(ptr);
    }
}

auto MjvSceneDeleter::operator()(mjvScene* ptr) const -> void {
    if (ptr != nullptr) {
        mjv_freeScene(ptr);
    }
}

auto vec3_from_mjc(const mjtNum* p_vec) -> Vec3 {
    return Vec3(ToScalar(p_vec[0]), ToScalar(p_vec[1]), ToScalar(p_vec[2]));
}

auto quat_from_mjc(const mjtNum* p_quat) -> Quat {
    return Quat(ToScalar(p_quat[0]), ToScalar(p_quat[1]), ToScalar(p_quat[2]),
                ToScalar(p_quat[3]));
}

}  // namespace mujoco
}  // namespace loco

#include <loco/backends/scenario_impl_mujoco.hpp>

namespace loco {
namespace mujoco {

// Resource deleters -----------------------------------------------------------

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

// MuJoCo implementation of the scenario API -----------------------------------

}  // namespace mujoco
}  // namespace loco

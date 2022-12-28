#include <loco/backends/mujoco/simulation_impl_mujoco.hpp>

namespace loco {
namespace mujoco {

SimulationImplMujoco::SimulationImplMujoco(core::Scenario::ptr scenario)
    : SimulationImpl(std::move(scenario)) {
    // Implement any required initial setup for the bullet backend
}

auto SimulationImplMujoco::Init() -> void {
    ///
}

auto SimulationImplMujoco::Reset() -> void {
    ///
}

auto SimulationImplMujoco::Step(Scalar step) -> void {
    if (m_Model == nullptr || m_Data == nullptr) {
        return;
    }

    mjtNum sim_start = m_Data->time;
    while (m_Data->time - sim_start < static_cast<mjtNum>(step)) {
        // Take a step in the simulation
        mj_step(m_Model.get(), m_Data.get());
    }
}

auto SimulationImplMujoco::SetTimeStep(Scalar fixed_step) -> void {
    if (m_Model == nullptr) {
        return;
    }

    m_Model->opt.timestep = static_cast<mjtNum>(fixed_step);
}

auto SimulationImplMujoco::SetGravity(const Vec3& gravity) -> void {
    if (m_Model == nullptr) {
        return;
    }

    m_Model->opt.gravity[0] = static_cast<mjtNum>(gravity.x());
    m_Model->opt.gravity[1] = static_cast<mjtNum>(gravity.y());
    m_Model->opt.gravity[2] = static_cast<mjtNum>(gravity.z());
}

auto SimulationImplMujoco::mujoco_model() -> mjModel& {
    if (m_Model == nullptr) {
        throw std::runtime_error(
            "SimulationImplMujoco::mujoco_model >>> Must initialize the "
            "backend first before accesing the internal mjModel struct");
    }
    return *m_Model;
}

auto SimulationImplMujoco::mujoco_model() const -> const mjModel& {
    if (m_Model == nullptr) {
        throw std::runtime_error(
            "SimulationImplMujoco::mujoco_model >>> Must initialize the "
            "backend first before accesing the internal mjModel struct");
    }
    return *m_Model;
}

auto SimulationImplMujoco::mujoco_data() -> mjData& {
    if (m_Data == nullptr) {
        throw std::runtime_error(
            "SimulationImplMujoco::mujoco_data >>> Must initialize the backend "
            "first before accessing the internal mjData struct");
    }
    return *m_Data;
}

auto SimulationImplMujoco::mujoco_data() const -> const mjData& {
    if (m_Data == nullptr) {
        throw std::runtime_error(
            "SimulationImplMujoco::mujoco_data >>> Must initialize the backend "
            "first before accessing the internal mjData struct");
    }
    return *m_Data;
}

auto SimulationImplMujoco::mujoco_scene() -> mjvScene& {
    if (m_Scene == nullptr) {
        throw std::runtime_error(
            "SimulationImplMujoco::mujoco_scene >>> Must initialize the "
            "backend first before accessing the internal mjvScene struct");
    }
    return *m_Scene;
}

auto SimulationImplMujoco::mujoco_scene() const -> const mjvScene& {
    if (m_Scene == nullptr) {
        throw std::runtime_error(
            "SimulationImplMujoco::mujoco_scene >>> Must initialize the "
            "backend first before accessing the internal mjvScene struct");
    }
    return *m_Scene;
}

}  // namespace mujoco
}  // namespace loco

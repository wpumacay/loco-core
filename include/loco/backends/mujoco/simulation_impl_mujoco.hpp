#pragma once

#include <memory>

#include <loco/backends/mujoco/common_mujoco.hpp>
#include <loco/core/impl/simulation_impl.hpp>

namespace loco {
namespace mujoco {

/// Implements the simulation API for the MuJoCo backend
class SimulationImplMujoco : public core::SimulationImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SimulationImplMujoco)

    DEFINE_SMART_POINTERS(SimulationImplMujoco)

 public:
    /// Creates an adapter to interact with the internal MuJoCo simulation
    explicit SimulationImplMujoco(core::Scenario::ptr scenario);

    ~SimulationImplMujoco() override = default;

    auto Init() -> void override;

    auto Reset() -> void override;

    auto Step(Scalar step) -> void override;

    auto SetTimeStep(Scalar fixed_step) -> void override;

    auto SetGravity(const Vec3& gravity) -> void override;

    /// Returns a mutable reference to the internal MuJoCo simulation model
    auto mujoco_model() -> mjModel* { return m_Model.get(); }

    /// Returns an unmutable reference to the internal MuJoCo simulation model
    auto mujoco_model() const -> const mjModel* { return m_Model.get(); }

    /// Returns a mutable reference to the internal MuJoCo simulation data
    auto mujoco_data() -> mjData* { return m_Data.get(); }

    /// Returns an unmutable reference to the internal MuJoCo simulation data
    auto mujoco_data() const -> const mjData* { return m_Data.get(); }

    /// Returns a mutable reference to the internal MuJoCo simulation data
    auto mujoco_scene() -> mjvScene* { return m_Scene.get(); }

    /// Returns an unmutable reference to the internal MuJoCo simulation data
    auto mujoco_scene() const -> const mjvScene* { return m_Scene.get(); }

 protected:
    /// Model struct containing the simulation structure
    std::unique_ptr<mjModel, MjcModelDeleter> m_Model = nullptr;
    /// Data struct containing simulation data
    std::unique_ptr<mjData, MjcDataDeleter> m_Data = nullptr;
    /// Scene struct containing visualization data
    std::unique_ptr<mjvScene, MjvSceneDeleter> m_Scene = nullptr;
};

}  // namespace mujoco
}  // namespace loco

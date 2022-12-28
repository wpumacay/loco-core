#pragma once

#include <memory>

#include <loco/backends/bullet/common_bullet.hpp>
#include <loco/core/impl/simulation_impl.hpp>

namespace loco {
namespace bullet {

/// Implements the simulation API for the Bullet backend
class SimulationImplBullet : public core::SimulationImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SimulationImplBullet)

    DEFINE_SMART_POINTERS(SimulationImplBullet)

 public:
    /// Creates an adapter to interact with the internal Bullet simulation
    explicit SimulationImplBullet(core::Scenario::ptr scenario);

    ~SimulationImplBullet() override = default;

    auto Init() -> void override;

    auto Reset() -> void override;

    auto Step(Scalar step) -> void override;

    auto SetTimeStep(Scalar step) -> void override;

    auto SetGravity(const Vec3& gravity) -> void override;

    /// Returns a mutable reference to the internal bullet world
    auto bullet_world() -> btDynamicsWorld&;

    /// Returns an unmutable reference to the internal bullet world
    auto bullet_world() const -> const btDynamicsWorld&;

 protected:
    /// Bullet's dynamics world used to simulate our scenario
    std::unique_ptr<btDynamicsWorld> m_World = nullptr;
    /// Bullet's constraint solver used for the simulation
    std::unique_ptr<btConstraintSolver> m_ConstraintSolver = nullptr;
    /// Bullet's collision dispatcher used for collision detection
    std::unique_ptr<btCollisionDispatcher> m_CollisionDispatcher = nullptr;
    /// Bullet's collision configuration used for collision detection
    std::unique_ptr<btCollisionConfiguration> m_CollisionConfig = nullptr;
    /// Bullet's broadphase interface used for collision detection
    std::unique_ptr<btBroadphaseInterface> m_Broadphase = nullptr;

    /// Fixed timestep taken on the simulation
    btScalar m_FixedTimeStep = static_cast<btScalar>(1e-3);
    /// Max. number of fixed simulation steps possible
    size_t m_MaxSubSteps = 20;
};

}  // namespace bullet
}  // namespace loco

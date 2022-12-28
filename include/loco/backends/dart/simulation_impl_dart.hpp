#pragma once

#include <memory>

#include <loco/backends/dart/common_dart.hpp>
#include <loco/core/impl/simulation_impl.hpp>

namespace loco {
namespace dart {

/// Implements the simulation API for the Bullet backend
class SimulationImplDart : public core::SimulationImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SimulationImplDart)

    DEFINE_SMART_POINTERS(SimulationImplDart)

 public:
    /// Creates an adapter to interact with the internal Bullet simulation
    explicit SimulationImplDart(core::Scenario::ptr scenario);

    ~SimulationImplDart() override = default;

    auto Init() -> void override;

    auto Reset() -> void override;

    auto Step(Scalar step) -> void override;

    auto SetTimeStep(Scalar step) -> void override;

    auto SetGravity(const Vec3& gravity) -> void override;

    /// Returns a mutable reference to the internal dart world
    auto dart_world() -> ::dart::simulation::World& { return *m_World; }

    /// Returns an unmutable reference to the internal dart world
    auto dart_world() const -> const ::dart::simulation::World& {
        return *m_World;
    }

 protected:
    ::dart::simulation::WorldPtr m_World;
};

}  // namespace dart
}  // namespace loco

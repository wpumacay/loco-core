#pragma once

#include <loco/backends/dart/common_dart.hpp>
#include <loco/core/impl/simulation_impl.hpp>

namespace loco {
namespace dart {

/// Implements the simulation API for the DART backend
class SimulationImplDart : public core::SimulationImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SimulationImplDart)

    DEFINE_SMART_POINTERS(SimulationImplDart)

 public:
    /// Creates an adapter to interact with the internal Dart simulation
    explicit SimulationImplDart(core::Scenario::ptr scenario);

    ~SimulationImplDart() override = default;

    auto Init() -> void override;

    auto Reset() -> void override;

    auto Step(Scalar step) -> void override;

    auto SetTimeStep(Scalar fixed_step) -> void override;

    auto SetGravity(const Vec3& gravity) -> void override;

 protected:
    /// DART's world used to simulate our scenario
    ::dart::simulation::WorldPtr m_World = nullptr;
};

}  // namespace dart
}  // namespace loco

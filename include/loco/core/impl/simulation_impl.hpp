#pragma once

#include <memory>
#include <utility>

#include <loco/core/common.hpp>
#include <loco/core/scenario_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace loco {
namespace core {

/// Represents an adapter that links to a specific simulation backend
class SimulationImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SimulationImpl)

    DEFINE_SMART_POINTERS(SimulationImpl)

 public:
    explicit SimulationImpl(Scenario::ptr scenario)
        : m_Scenario(std::move(scenario)) {}

    /// Clean/Release allocated resources
    virtual ~SimulationImpl() = default;

    /// Initializes the backend for us to simulate the given scenario
    virtual auto Init() -> void = 0;

    /// Resets the simulation to its initial configuration
    virtual auto Reset() -> void = 0;

    /// Advances the simulation by the given amount of time
    virtual auto Step(Scalar step) -> void = 0;

    /// Sets the internal timestep (amount of time each sim. step takes)
    virtual auto SetTimeStep(Scalar step) -> void = 0;

    /// Sets the internal gravity
    virtual auto SetGravity(const Vec3& gravity) -> void = 0;

 protected:
    /// The scenario to be simulated
    Scenario::ptr m_Scenario;
};

/// Represents a dummy adapter for a scenario (no simulation happens)
class SimulationImplNone : public SimulationImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SimulationImplNone)

    DEFINE_SMART_POINTERS(SimulationImplNone);

 public:
    explicit SimulationImplNone(Scenario::ptr scenario)
        : SimulationImpl(std::move(scenario)) {}

    ~SimulationImplNone() override = default;

    auto Init() -> void override {}

    auto Reset() -> void override {}

    auto Step(Scalar step) -> void override {}

    auto SetTimeStep(Scalar step) -> void override {}

    auto SetGravity(const Vec3& gravity) -> void override {}
};

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

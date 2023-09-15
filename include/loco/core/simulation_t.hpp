#pragma once

#include <memory>
#include <utility>

#include "./common.hpp"
#include "./scenario_t.hpp"
#include "./impl/simulation_impl.hpp"

namespace loco {
namespace core {

/// Core simulation object, used to interact with the simulation itself
class Simulation {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Simulation)

    DEFINE_SMART_POINTERS(Simulation)

 public:
    /// Creates a simulation for the given scenario
    explicit Simulation(Scenario::ptr scenario, eBackendType backend_type)
        : m_Scenario(std::move(scenario)), m_BackendType(backend_type) {}

    /// Releases/Frees all allocated resources of this simulation
    ~Simulation() = default;

    /// Initializes the internal backend and sets the simulation ready
    auto Init() -> void;

    /// Resets the scenario to its initial configuration
    auto Reset() -> void;

    /// Advances the simulation by the given amount of time
    auto Step(Scalar step) -> void;

    /// Sets the timestep of the simulation
    auto SetTimeStep(Scalar step) -> void;

    /// Sets the gravity for this simulation
    auto SetGravity(const Vec3& gravity) -> void;

    /// Returns the current value of the simulation's timestep
    auto timestep() const -> Scalar { return m_TimeStep; }

    /// Returns the current value of the simulation's gravity vector
    auto gravity() const -> Vec3 { return m_Gravity; }

    /// Returns the type of backend used internally for the simulation
    auto backend_type() const -> eBackendType { return m_BackendType; }

    /// Returns a mutable reference to the internal pimpl
    auto impl() -> SimulationImpl&;

    /// Returns an unmutable reference to the internal pimpl
    auto impl() const -> const SimulationImpl&;

 protected:
    /// Scenario to be simulated
    Scenario::ptr m_Scenario = nullptr;

    /// The type of backend being used for this physics simulation
    eBackendType m_BackendType = eBackendType::NONE;

    /// The value of gravity on this simulation
    Vec3 m_Gravity = {ToScalar(0.0), ToScalar(0.0), ToScalar(-9.81)};

    /// The time step by which the simulation advances on each simulation step
    Scalar m_TimeStep = ToScalar(0.001);

    /// Backend-specific interface to the physics engine
    SimulationImpl::uptr m_BackendImpl = nullptr;
};

}  // namespace core
}  // namespace loco

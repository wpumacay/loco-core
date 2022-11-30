#pragma once

#include <loco/core/common.hpp>
#include <loco/core/impl/scenario_impl.hpp>

namespace loco {
namespace core {

class Scenario {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Scenario)

    DEFINE_SMART_POINTERS(Scenario)

 public:
    /// Creates a scenario with a default dummy runtime and backend
    Scenario() = default;

    /// Releases/Frees all allocated resources of this scenario
    ~Scenario() = default;

    /// Initializes the internal backend and sets the simulation ready
    /// \param[in] backend_type The backend to use for the simulation
    auto Initialize(const eBackendType& backend_type) -> void;

    /// Sets the timestep of the simulation
    /// \param[in] step The time step by which the simulation advances
    auto SetTimeStep(Scalar step) -> void;

    /// Sets the gravity for this simulation
    /// \param[in] gravity The value of gravity for this scenario
    auto SetGravity(const Vec3& gravity) -> void;

 protected:
    /// The value of gravity in this senario
    Vec3 m_Gravity = {static_cast<Scalar>(0.0), static_cast<Scalar>(0.0),
                      static_cast<Scalar>(-9.81)};

    /// The time step by which the simulation advances on each simulation step
    Scalar m_TimeStep = static_cast<Scalar>(0.001);

    /// The type of backend being used for the physics simulaiton
    eBackendType m_BackendType = eBackendType::NONE;

    /// Backend specific implementation of this scenario
    ScenarioImpl::uptr m_Impl = nullptr;
};

}  // namespace core
}  // namespace loco

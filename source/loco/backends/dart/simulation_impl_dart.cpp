// Disable warnings generated by the Bullet codebase
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#pragma clang diagnostic ignored "-Wcast-align"
#endif

#include <loco/backends/dart/simulation_impl_dart.hpp>

#include <dart/collision/ode/OdeCollisionDetector.hpp>
//// #include <dart/collision/bullet/BulletCollisionDetector.hpp>

namespace loco {
namespace dart {

SimulationImplDart::SimulationImplDart(core::Scenario::ptr scenario)
    : SimulationImpl(std::move(scenario)) {
    // Implement any required initial setup for the bullet backend
}

auto SimulationImplDart::Init() -> void {
    m_World = ::dart::simulation::World::create();
    m_World->getConstraintSolver()->setCollisionDetector(
        ::dart::collision::OdeCollisionDetector::create());
}

auto SimulationImplDart::Reset() -> void {
    // Reset the internal adapters for bodies, articulated systems, etc.
}

auto SimulationImplDart::Step(Scalar step) -> void {
    if (m_World != nullptr) {
        auto time_start = m_World->getTime();
        while (m_World->getTime() - time_start < step) {
            m_World->step();
        }
    }
}

auto SimulationImplDart::SetTimeStep(Scalar step) -> void {
    if (m_World != nullptr) {
        m_World->setTimeStep(step);
    }
}

auto SimulationImplDart::SetGravity(const Vec3& gravity) -> void {
    if (m_World != nullptr) {
        m_World->setGravity(loco::dart::vec3_to_eigen(gravity));
    }
}

}  // namespace dart
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

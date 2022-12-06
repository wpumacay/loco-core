#include <loco/core/simulation_t.hpp>

// Disable warnings generated by the Bullet codebase
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#pragma clang diagnostic ignored "-Wcast-align"
#endif

#include <loco/backends/bullet/simulation_impl_bullet.hpp>

namespace loco {
namespace core {

auto Simulation::Init() -> void {
    switch (m_BackendType) {
        case eBackendType::NONE:
            m_BackendImpl = std::make_unique<SimulationImplNone>(m_Scenario);
            break;
        case eBackendType::MUJOCO:
            // TODO(wilbert): use here SimulationImplMujoco
            break;
        case eBackendType::BULLET:
            m_BackendImpl =
                std::make_unique<bullet::SimulationImplBullet>(m_Scenario);
            break;
        case eBackendType::DART:
            // TODO(wilbert): use here SimulationImplDart
            break;
    }

    if (m_BackendImpl != nullptr) {
        m_BackendImpl->Init();
    }

    // TODO(wilbert): initialize all objects in this scenario
}

auto Simulation::SetTimeStep(Scalar step) -> void {
    m_TimeStep = step;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetTimeStep(step);
    }
}

auto Simulation::SetGravity(const Vec3& gravity) -> void {
    m_Gravity = gravity;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetGravity(gravity);
    }
}

auto Simulation::Reset() -> void {
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->Reset();
    }
}

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

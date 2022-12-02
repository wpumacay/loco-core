#include <loco/core/simulation_t.hpp>

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
    }

    // TODO(wilbert): initialize all objects in this scenario
}

auto Simulation::SetTimeStep(Scalar step) -> void {
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetTimeStep(step);
    }
}

auto Simulation::SetGravity(const Vec3& gravity) -> void {
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetGravity(gravity);
    }
}

}  // namespace core
}  // namespace loco

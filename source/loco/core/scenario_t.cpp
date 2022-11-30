#include <loco/core/scenario_t.hpp>

namespace loco {
namespace core {

auto Scenario::Initialize(const eBackendType& backend_type) -> void {
    m_BackendType = backend_type;
    switch (backend_type) {
        case eBackendType::NONE:
            m_Impl = std::make_unique<ScenarioImplNone>();
            break;
        case eBackendType::MUJOCO:
            // TODO(wilbert): use here ScenarioImplMujoco
            break;
    }

    // TODO(wilbert): initialize all objects in this scenario
}

auto Scenario::SetTimeStep(Scalar step) -> void {
    if (m_Impl != nullptr) {
        m_Impl->SetTimeStep(step);
    }
}

auto Scenario::SetGravity(const Vec3& gravity) -> void {
    if (m_Impl != nullptr) {
        m_Impl->SetGravity(gravity);
    }
}

}  // namespace core
}  // namespace loco

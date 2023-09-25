#include <stdexcept>

#include <utils/logging.hpp>

#include <loco/core/single_body/single_body_t.hpp>

namespace loco {
namespace core {

auto SingleBody::Initialize(const eBackendType& backend_type) -> void {
    m_BackendType = backend_type;

    // Do some configuration before --------------------------------------------
    switch (m_BackendType) {
        case ::loco::eBackendType::NONE: {
            m_BackendImpl = std::make_unique<SingleBodyImplNone>();
            break;
        }
        case ::loco::eBackendType::MUJOCO:
        case ::loco::eBackendType::BULLET:
        case ::loco::eBackendType::DART:
        default: {
            m_BackendImpl = std::make_unique<SingleBodyImplNone>();
            break;
        }
    }

    // Reset the configuration of the body (zero|default configuration) --------
    Reset();

    // Do some configuration after ---------------------------------------------
    // ...
}

auto SingleBody::Reset() -> void {
    // TODO(wilbert): expose Vec3::ZERO, Vec3::X, Vec3::Y, and Vec3::Z
    totalTorque = Vec3(ToScalar(0.0), ToScalar(0.0), ToScalar(0.0));
    totalForceCOM = Vec3(ToScalar(0.0), ToScalar(0.0), ToScalar(0.0));

    m_Pose = pose0;
    m_LinearVel = linearVel0;
    m_AngularVel = angularVel0;

    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetPose(m_Pose);
        m_BackendImpl->SetLinearVelocity(m_LinearVel);
        m_BackendImpl->SetAngularVelocity(m_AngularVel);
    }
}

auto SingleBody::SetPose(const Pose& pose) -> void {
    m_Pose = pose;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetPose(pose);
    }
}

auto SingleBody::SetPosition(const Vec3& pos) -> void {
    m_Pose.position = pos;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetPose(m_Pose);
    }
}

auto SingleBody::SetOrientation(const Quat& quat) -> void {
    m_Pose.orientation = quat;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetPose(m_Pose);
    }
}

auto SingleBody::SetLinearVelocity(const Vec3& linear_vel) -> void {
    m_LinearVel = linear_vel;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetLinearVelocity(linear_vel);
    }
}

auto SingleBody::SetAngularVelocity(const Vec3& angular_vel) -> void {
    m_AngularVel = angular_vel;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetAngularVelocity(angular_vel);
    }
}

auto SingleBody::impl() -> ISingleBodyImpl& {
    if (m_BackendImpl == nullptr) {
        throw std::runtime_error(
            "SingleBody::impl >>> Must initialize this body before using its "
            "adapter. Make sure you've called 'Init' first.");
    }
    return *m_BackendImpl;
}

auto SingleBody::impl() const -> const ISingleBodyImpl& {
    if (m_BackendImpl == nullptr) {
        throw std::runtime_error(
            "SingleBody::impl >>> Must initialize this body before using its "
            "adapter. Make sure you've called 'Init' first.");
    }
    return *m_BackendImpl;
}

}  // namespace core
}  // namespace loco

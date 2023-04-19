#include <stdexcept>

#include <loco/core/body_t.hpp>

#include <utils/logging.hpp>

namespace loco {
namespace core {

auto Body::Initialize(const eBackendType& backend_type) -> void {
    m_BackendType = backend_type;

    // Do some configuration before
    // ...

    Reset();

    // Do some configuration after
    // ...
}

auto Body::Reset() -> void {
    // TODO(wilbert): expose Vec3::ZERO, Vec3::X, Vec3::Y, and Vec3::Z
    totalTorque = Vec3(ToScalar(0.0), ToScalar(0.0), ToScalar(0.0));
    totalForceCOM = Vec3(ToScalar(0.0), ToScalar(0.0), ToScalar(0.0));

    switch (m_BackendType) {
        case ::loco::eBackendType::NONE: {
            m_BackendImpl = std::make_unique<BodyImplNone>();
            break;
        }
        case ::loco::eBackendType::MUJOCO:
        case ::loco::eBackendType::BULLET:
        case ::loco::eBackendType::DART:
        default: {
            m_BackendImpl = std::make_unique<BodyImplNone>();
            break;
        }
    }
}

auto Body::SetPose(const Pose& pose) -> void {
    m_Pose = pose;
    if (m_BackendImpl == nullptr) {
        LOG_CORE_WARN(
            "Body::SetPose >>> tried setting a pose in world-space for the "
            "rigid body without a valid adapter.");
        return;
    }
    m_BackendImpl->SetPose(pose);
}

auto Body::SetLinearVelocity(const Vec3& linear_vel) -> void {
    m_LinearVel = linear_vel;
    if (m_BackendImpl == nullptr) {
        LOG_CORE_WARN(
            "Body::SetLinearVelocity >>> tried setting the linear velocity of "
            "the rigid body without a valid adapter");
        return;
    }
    m_BackendImpl->SetLinearVelocity(linear_vel);
}

auto Body::SetAngularVelocity(const Vec3& angular_vel) -> void {
    m_AngularVel = angular_vel;
    if (m_BackendImpl == nullptr) {
        LOG_CORE_WARN(
            "Body::SetAngularVelocity >>> tried setting the angular velocity "
            "of the rigid body without a valid adapter");
        return;
    }
    m_BackendImpl->SetAngularVelocity(angular_vel);
}

auto Body::impl() -> IBodyImpl& {
    if (m_BackendImpl == nullptr) {
        throw std::runtime_error(
            "Body::impl >>> Must initialize this body before using its "
            "adapter. Make sure you've called 'Init' first.");
    }
    return *m_BackendImpl;
}

auto Body::impl() const -> const IBodyImpl& {
    if (m_BackendImpl == nullptr) {
        throw std::runtime_error(
            "Body::impl >>> Must initialize this body before using its "
            "adapter. Make sure you've called 'Init' first.");
    }
    return *m_BackendImpl;
}

}  // namespace core
}  // namespace loco

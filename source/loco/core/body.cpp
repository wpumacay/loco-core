#include <loco/core/body.hpp>

namespace loco {
namespace core {

auto Body::SimBefore() -> void {
    // Apply all control forces and torques before taking a simulation step
    if (m_Impl != nullptr) {
        m_Impl->SetForceCOM(m_TotalForce);
        m_Impl->SetTorque(m_TotalTorque);
    }
}

auto Body::SimAfter() -> void {
    // Get all information required after a simulation step was taken
    if (m_Impl != nullptr) {
        m_Position = m_Impl->GetPosition();
        m_Orientation = m_Impl->GetOrientation();
        m_LinearVel = m_Impl->GetLinearVelocity();
        m_AngularVel = m_Impl->GetAngularVelocity();
    }
    // Clear both force and torque accummulators
    m_TotalForce = {0, 0, 0};
    m_TotalTorque = {0, 0, 0};
}

auto Body::SimReset() -> void {
    m_Position = m_Position0;
    m_Orientation = m_Orientation0;
    m_LinearVel = m_LinearVel0;
    m_AngularVel = m_AngularVel0;

    m_TotalForce = {0, 0, 0};
    m_TotalTorque = {0, 0, 0};

    if (m_Impl != nullptr) {
        m_Impl->SetPosition(m_Position0);
        m_Impl->SetOrientation(m_Orientation);
        m_Impl->SetLinearVelocity(m_LinearVel0);
        m_Impl->SetAngularVelocity(m_AngularVel0);
    }
}

auto Body::SetPosition(const Vec3& position) -> void {
    m_Position = position;
    if (m_Impl != nullptr) {
        m_Impl->SetPosition(position);
    }
}

auto Body::SetOrientation(const Quat& orientation) -> void {
    m_Orientation = orientation;
    if (m_Impl != nullptr) {
        m_Impl->SetOrientation(orientation);
    }
}

auto Body::SetLinearVelocity(const Vec3& linear_vel) -> void {
    m_LinearVel = linear_vel;
    if (m_Impl != nullptr) {
        m_Impl->SetLinearVelocity(linear_vel);
    }
}

auto Body::SetAngularVelocity(const Vec3& angular_vel) -> void {
    m_AngularVel = angular_vel;
    if (m_Impl != nullptr) {
        m_Impl->SetAngularVelocity(angular_vel);
    }
}

auto Body::SetInitialPosition(const Vec3& position) -> void {
    m_Position0 = position;
}

auto Body::SetInitialOrientation(const Quat& orientation) -> void {
    m_Orientation0 = orientation;
}

auto Body::SetInitialLinearVelocity(const Vec3& linear_vel0) -> void {
    m_LinearVel0 = linear_vel0;
}

auto Body::SetInitialAngularVelocity(const Vec3& angular_vel0) -> void {
    m_AngularVel0 = angular_vel0;
}

auto Body::AddForceCOM(const Vec3& force) -> void {
    // TODO(wilbert): implement += operator in loco::math
    m_TotalForce = m_TotalForce + force;
}

auto Body::AddTorque(const Vec3& torque) -> void {
    // TODO(wilbert): implement += operator in loco::math
    m_TotalTorque = m_TotalTorque + torque;
}

}  // namespace core
}  // namespace loco

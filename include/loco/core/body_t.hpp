#pragma once

#include <loco/core/common.hpp>
#include <loco/core/body_impl.hpp>

namespace loco {
namespace core {

class Body {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Body)

    DEFINE_SMART_POINTERS(Body)

 public:
    Body() = default;

    ~Body() = default;

    auto SimBefore() -> void;

    auto SimAfter() -> void;

    auto SimReset() -> void;

    auto SetPosition(const Vec3& position) -> void;

    auto SetOrientation(const Quat& orientation) -> void;

    auto SetLinearVelocity(const Vec3& linear_vel) -> void;

    auto SetAngularVelocity(const Vec3& angular_vel) -> void;

    auto SetInitialPosition(const Vec3& position0) -> void;

    auto SetInitialOrientation(const Quat& orientation0) -> void;

    auto SetInitialLinearVelocity(const Vec3& linear_vel0) -> void;

    auto SetInitialAngularVelocity(const Vec3& angular_vel0) -> void;

    auto AddForceCOM(const Vec3& force) -> void;

    auto AddTorque(const Vec3& torque) -> void;

 protected:
    /// Position of this body w.r.t. the world frame
    Vec3 m_Position;
    /// Orientation of this body w.r.t. the world frame
    Quat m_Orientation;
    /// Initial position of this body w.r.t. the world frame
    Vec3 m_Position0;
    /// Initial orientation of this body w.r.t. the world frame
    Quat m_Orientation0;

    /// Linear velocity of this body w.r.t. the world frame
    Vec3 m_LinearVel;
    /// Angular velocity of this body w.r.t. the world frame
    Vec3 m_AngularVel;
    /// Initial value of the linear velocity of this body
    Vec3 m_LinearVel0;
    /// Initial value of the angular velocity of this body
    Vec3 m_AngularVel0;

    /// Total force to be applied at the COM of this body
    Vec3 m_TotalForce;
    /// Total torque to be applied to this body
    Vec3 m_TotalTorque;

    /// Backend-specific implementation
    BodyImpl::uptr m_Impl;
};

}  // namespace core
}  // namespace loco

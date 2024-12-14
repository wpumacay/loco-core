#pragma once

#include <string>

#include <loco/core/common.hpp>

namespace loco {
namespace core {

/// Interface for body adapters to link to internal physics backend
class ISingleBodyImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(ISingleBodyImpl)

    DEFINE_SMART_POINTERS(ISingleBodyImpl)

 public:
    ISingleBodyImpl() = default;

    /// Releases/Frees all allocated resources for this adapter
    virtual ~ISingleBodyImpl() = default;

    /// Sets the pose of the associated body in world space
    /// \param[in] pose The desired pose of the rigid body in world space
    virtual auto SetPose(Pose pose) -> void = 0;

    /// Sets the linear velocity of the associated body
    /// \param[in] linear_vel The desired linear velocity of the body
    virtual auto SetLinearVelocity(Vec3 linear_vel) -> void = 0;

    /// Sets the angular velocity of the associated body
    /// \param[in] angular_vel The desired angular velocity of the body
    virtual auto SetAngularVelocity(Vec3 angular_vel) -> void = 0;

    /// Sets the total force at the COM of the body
    /// \param[in] force The desired total force to be applied
    virtual auto SetForceCOM(Vec3 force) -> void = 0;

    /// Sets the total torque applied to the body
    /// \param[in] torque The desired total torque to be applied
    virtual auto SetTorque(Vec3 torque) -> void = 0;

    /// Returns the type of backend being used internally for simulation
    LOCO_NODISCARD auto type() const -> eBackendType { return m_BackendType; }

    /// Returns a string representation of this body adapter
    LOCO_NODISCARD virtual auto ToString() const -> std::string = 0;

 protected:
    /// The internall type of backend used for simulation
    ::loco::eBackendType m_BackendType = ::loco::eBackendType::NONE;
};

/// Represents a dummy adapter that connects to no backend
class SingleBodyImplNone : public ISingleBodyImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SingleBodyImplNone)

    DEFINE_SMART_POINTERS(SingleBodyImplNone)

 public:
    SingleBodyImplNone() = default;

    ~SingleBodyImplNone() override = default;

    auto SetPose(Pose pose) -> void override;

    auto SetLinearVelocity(Vec3 linear_vel) -> void override;

    auto SetAngularVelocity(Vec3 angular_vel) -> void override;

    auto SetForceCOM(Vec3 force) -> void override;

    auto SetTorque(Vec3 torque) -> void override;

    LOCO_NODISCARD auto ToString() const -> std::string override;
};

}  // namespace core
}  // namespace loco

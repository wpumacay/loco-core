#pragma once

#include <memory>
#include <utility>

#include "../common.hpp"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace loco {
namespace core {

/// \brief Interface for body adapters to link to internal physics backend
///
/// This class provides an interface and defines the API that is exposed to the
/// user to interact with the internal backend-specific implementation of a
/// rigid-body
class IBodyImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(IBodyImpl)

    DEFINE_SMART_POINTERS(IBodyImpl)

 public:
    /// Releases/Frees all allocated resources for this adapter
    virtual ~IBodyImpl() = default;

    /// \brief Sets the pose of the associated rigid-body
    ///
    /// Updates the pose in world space by using the backend-specific resources
    /// that links to the associated body. The backend could be using either
    /// maximal or minimal coordinates; either way, the implementation of the
    /// adapter should be in charge of using the backend's API to set the pose
    /// correctly
    ///
    /// \param[in] pose The desired pose of the rigid body in world space
    virtual auto SetPose(const Pose& pose) -> void = 0;

    /// \brief Updates the internal backend with the linear velocity of the
    /// associated body
    ///
    /// \param[in] linear_vel The desired linear velocity of the associated
    /// rigid-body
    virtual auto SetLinearVelocity(const Vec3& linear_vel) -> void = 0;

    /// \brief Updates the internal backend with the angular velocity of the
    /// associated body
    ///
    /// \param[in] angular_vel The desired angular velocity of the associated
    /// rigid body
    virtual auto SetAngularVelocity(const Vec3& angular_vel) -> void = 0;

    /// \brief Sets the given total force at the center of mass for the
    /// associated body
    ///
    /// \param[in] force The desired total force to be applied to the associated
    /// rigid body at its center of mass
    virtual auto SetForceCOM(const Vec3& force) -> void = 0;

    /// \brief Sets the given total torque for the associated body
    ///
    /// \param[in] torque The desired total torque to be applied to the
    /// associated rigid body
    virtual auto SetTorque(const Vec3& torque) -> void = 0;

    /// \brief Returns the type of backend being used internally for simulation
    auto type() const -> eBackendType { return m_BackendType; }

 protected:
    /// The internall type of backend used for simulation
    ::loco::eBackendType m_BackendType = ::loco::eBackendType::NONE;
};

/// \brief Represents a dummy adapter that connects to no backend
class BodyImplNone : public IBodyImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(BodyImplNone)

    DEFINE_SMART_POINTERS(BodyImplNone)

 public:
    // Documentation inherited
    ~BodyImplNone() override = default;

    // Documentation inherited
    auto SetPose(const Pose& pose) -> void override {}

    // Documentation inherited
    auto SetLinearVelocity(const Vec3& linear_vel) -> void override {}

    // Documentation inherited
    auto SetAngularVelocity(const Vec3& angular_vel) -> void override {}

    // Documentation inherited
    auto SetForceCOM(const Vec3& force) -> void override {}

    // Documentation inherited
    auto SetTorque(const Vec3& torque) -> void override {}
};

}  // namespace core
}  // namespace loco

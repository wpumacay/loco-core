#pragma once

#include <string>
#include <utility>

#include <loco/core/common.hpp>
#include <loco/core/single_body/impl/single_body_impl.hpp>

namespace loco {
namespace core {

class LOCO_API SingleBody {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SingleBody)

    DEFINE_SMART_POINTERS(SingleBody)

 public:
    /// Creates a single body using the given configuration
    /// \param[in] data Body data to be used to create and configure this body
    explicit SingleBody(::loco::BodyData data, const Pose& p_pose)
        : m_Data(std::move(data)), m_Pose(p_pose) {}

    /// Creates a single body using the given configuration
    /// \param[in] data Body data to be used to create and configure this body
    /// \param[in] p_position The position of this body in world space
    /// \param[in] p_orientation The orientation of this body in world space
    explicit SingleBody(::loco::BodyData data, const Vec3& p_position,
                        const Quat& p_orientation = Quat(1.0, 0.0, 0.0, 0.0))
        : m_Data(std::move(data)), m_Pose(Pose(p_position, p_orientation)) {}

    /// Deletes all allocated resources
    ~SingleBody() = default;

    /// Initializes this body's internal resources
    /// \param[in] backend_type The internal backend to be used for this body
    auto Initialize(eBackendType backend_type) -> void;

    /// Sets the adapter used for simulating this body
    /// \param[in] adapter The adapter to be used by this body
    auto SetAdapter(ISingleBodyImpl::uptr adapter) -> void;

    /// Resets the body to its default/zero configuration
    auto Reset() -> void;

    /// Sets the pose of this rigid body in world space
    /// \param[in] pose The desired pose in world space
    auto SetPose(Pose pose) -> void;

    /// Sets the position of this rigid body in world space
    /// \param[in] pos The desired position in world space
    auto SetPosition(Vec3 pos) -> void;

    /// Sets the orientation of this rigid body in world space
    /// \param[in] quat The desired orientation in world space
    auto SetOrientation(Quat quat) -> void;

    /// Sets the linear velocity of this rigid body to the given value
    /// \param[in] linear_vel The desired linear velocity of this body
    auto SetLinearVelocity(Vec3 linear_vel) -> void;

    /// Sets the angular velocity of this rigid body to the given value
    /// \param[in] angular_vel The angular velocity of this body
    auto SetAngularVelocity(Vec3 angular_vel) -> void;

    /// Returns the current pose of this rigid body in world space
    LOCO_NODISCARD auto pose() const -> Pose { return m_Pose; }

    /// Returns the current position of this rigid body in world space
    LOCO_NODISCARD auto position() const -> Vec3 { return m_Pose.position; }

    /// Returns the current orientation of this rigid body in world space
    LOCO_NODISCARD auto orientation() const -> Quat {
        return m_Pose.orientation;
    }

    /// Returns the linear velocity of this rigid body
    LOCO_NODISCARD auto linear_vel() const -> Vec3 { return m_LinearVel; }

    /// Returns the angular velocity of this rigid body
    LOCO_NODISCARD auto angular_vel() const -> Vec3 { return m_AngularVel; }

    /// Returns the type of backend associated with this body
    LOCO_NODISCARD auto backend_type() const -> eBackendType {
        return m_BackendType;
    }

    /// Returns a mutable reference to the interface to the backend
    LOCO_NODISCARD auto impl() -> ISingleBodyImpl&;

    /// Returns an unmutable reference to the interface to the backend
    LOCO_NODISCARD auto impl() const -> const ISingleBodyImpl&;

    /// Returns the string representation of the body
    LOCO_NODISCARD auto ToString() const -> std::string;

 public:
    // ---------------------------------------------------------------------- //
    // These are made public as we don't actually need to control everything
    // with getters and setters, and don't require a direct call to the backend
    // ---------------------------------------------------------------------- //

    /// The initial pose of this rigid body in world space
    Pose pose0;
    /// The initial linear velocity of this rigid body
    Vec3 linearVel0;
    /// The initial angular velocity of this rigid body
    Vec3 angularVel0;

    /// The total force to be applied at the COM of this rigid body
    Vec3 totalForceCOM;
    /// The total torque to be applied at this rigid body
    Vec3 totalTorque;

 protected:
    /// The configuration data for this body
    ::loco::BodyData m_Data;

    /// The current pose of this rigid body in world space
    Pose m_Pose;

    /// The current linear velocity of this rigid body
    Vec3 m_LinearVel;

    /// The current angular velocity of this rigid body
    Vec3 m_AngularVel;

    /// The backend type used for simulating this body
    eBackendType m_BackendType{eBackendType::NONE};

    /// The adapter used to interact with the internal physics backend
    ISingleBodyImpl::uptr m_BackendImpl{nullptr};
};

// Factory functions for primtive shapes

}  // namespace core
}  // namespace loco

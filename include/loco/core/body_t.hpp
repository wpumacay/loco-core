#pragma once

#include <string>
#include <memory>
#include <utility>

#include "./common.hpp"
#include "./impl/body_impl.hpp"

namespace loco {
namespace core {

class Body {
    NO_COPY_NO_MOVE_NO_ASSIGN(Body)

    DEFINE_SMART_POINTERS(Body)

 public:
    /// \brief Creates a body using the given configuration
    ///
    /// \param[in] data Body data to be used to create and configure this body
    explicit Body(::loco::BodyData data) : m_Data(std::move(data)) {}

    /// \brief Deletes all allocated resources
    ~Body() = default;

    /// \brief Initializes this body's internal resources
    ///
    /// \param[in] backend_type The internal backend to be used for this body
    auto Initialize(const eBackendType& backend_type) -> void;

    /// \brief Resets the body to its default/zero configuration
    auto Reset() -> void;

    /// \brief Sets the pose of this rigid body in world space
    ///
    /// \param[in] pose The desired pose in world space
    auto SetPose(const Pose& pose) -> void;

    /// \brief Sets the linear velocity of this rigid body to the given value
    ///
    /// \param[in] linear_vel The desired linear velocity of this body
    auto SetLinearVelocity(const Vec3& linear_vel) -> void;

    /// \brief Sets the angular velocity of this rigid body to the given value
    ///
    /// \param[in] angular_vel The angular velocity of this body
    auto SetAngularVelocity(const Vec3& angular_vel) -> void;

    /// \brief Returns the current pose of this rigid body
    auto pose() const -> Pose { return m_Pose; }

    /// \brief Returns the linear velocity of this rigid body
    auto linear_vel() const -> Vec3 { return m_LinearVel; }

    /// \brief Returns the angular velocity of this rigid body
    auto angular_vel() const -> Vec3 { return m_AngularVel; }

    /// \brief Returns a mutable reference to the interface to the backend
    auto impl() -> IBodyImpl&;

    /// \brief Returns an unmutable reference to the interface to the backend
    auto impl() const -> const IBodyImpl&;

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
    eBackendType m_BackendType = eBackendType::NONE;
    /// The adapter used to interact with the internal physics backend
    IBodyImpl::uptr m_BackendImpl = nullptr;
};

}  // namespace core
}  // namespace loco

#pragma once

#include <loco/core/common.hpp>

namespace loco {
namespace core {

class BodyImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(BodyImpl)

    DEFINE_SMART_POINTERS(BodyImpl)

 public:
    BodyImpl();

    virtual ~BodyImpl();

    virtual auto SetPosition(const Vec3& position) -> void = 0;

    virtual auto SetOrientation(const Quat& orientation) -> void = 0;

    virtual auto SetLinearVelocity(const Vec3& linear_vel) -> void = 0;

    virtual auto SetAngularVelocity(const Vec3& angular_vel) -> void = 0;

    virtual auto SetForceCOM(const Vec3& force) -> void = 0;

    virtual auto SetTorque(const Vec3& torque) -> void = 0;

    virtual auto GetPosition() -> Vec3 = 0;

    virtual auto GetOrientation() -> Quat = 0;

    virtual auto GetLinearVelocity() -> Vec3 = 0;

    virtual auto GetAngularVelocity() -> Vec3 = 0;
};

}  // namespace core
}  // namespace loco

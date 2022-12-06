#pragma once

#include <btBulletDynamicsCommon.h>

#include <loco/core/common.hpp>

namespace loco {
namespace bullet {

/// Converts the given vec3 into Bullet's btVector3
auto vec3_to_bt(const Vec3& vec) -> btVector3;

/// Converts the given mat3 into Bullet's btMatrix3x3
auto mat3_to_bt(const Mat3& mat) -> btMatrix3x3;

/// Converts the given quat into Bullet's btQuaternion
auto quat_to_bt(const Quat& quat) -> btQuaternion;

/// Converts the given mat4 into Bullet's btTransform
auto mat4_to_bt(const Mat4& mat) -> btTransform;

}  // namespace bullet
}  // namespace loco

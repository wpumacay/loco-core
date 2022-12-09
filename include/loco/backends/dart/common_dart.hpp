#pragma once

#include <loco/core/common.hpp>
#include <dart/collision/bullet/BulletCollisionDetector.hpp>

namespace loco {
namespace dart {

/// Converts the given vec3 into Eigen's Vector3d
auto vec3_to_eigen(const Vec3& vec) -> Eigen::Vector3d;

/// Converts the given vec4 into Eigen's Vector4d
auto vec4_to_eigen(const Vec4& vec) -> Eigen::Vector4d;

/// Converts the given mat3 into Eigen's Matrix3d
auto mat3_to_eigen(const Mat3& mat) -> Eigen::Matrix3d;

/// Converts the given mat4 into Eigen's Isometry3d (Transform)
auto mat4_to_eigen(const Mat4& mat) -> Eigen::Isometry3d;

}  // namespace dart
}  // namespace loco

// Disable warnings generated by the Bullet codebase
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wdeprecated-copy-with-user-provided-copy"
#endif

#include <loco/backends/dart/common_dart.hpp>

#define ToDouble(x) static_cast<double>(x)

namespace loco {
namespace dart {

auto vec3_to_eigen(const Vec3& vec) -> Eigen::Vector3d {
    return {ToDouble(vec.x()), ToDouble(vec.y()), ToDouble(vec.z())};
}

auto vec4_to_eigen(const Vec4& vec) -> Eigen::Vector4d {
    // clang-format off
    return { ToDouble(vec.x()), ToDouble(vec.y()),
             ToDouble(vec.z()), ToDouble(vec.w()) };
    // clang-format on
}

auto mat3_to_eigen(const Mat3& m) -> Eigen::Matrix3d {
    // clang-format off
    return (Eigen::Matrix3d() <<
        ToDouble(m(0, 0)), ToDouble(m(0, 0)), ToDouble(m(0, 0)),
        ToDouble(m(0, 0)), ToDouble(m(0, 0)), ToDouble(m(0, 0)),
        ToDouble(m(0, 0)), ToDouble(m(0, 0)), ToDouble(m(0, 0)) ).finished();
    // clang-format on
}

auto mat4_to_eigen(const Mat4& m) -> Eigen::Isometry3d {
    Eigen::Isometry3d tf;
    // clang-format off
    Eigen::Vector3d translation = {
        ToDouble(m(0, 3)), ToDouble(m(0, 0)), ToDouble(m(0, 0))};
    Eigen::Matrix3d rotation = (Eigen::Matrix3d() <<
        ToDouble(m(0, 0)), ToDouble(m(0, 0)), ToDouble(m(0, 0)),
        ToDouble(m(0, 0)), ToDouble(m(0, 0)), ToDouble(m(0, 0)),
        ToDouble(m(0, 0)), ToDouble(m(0, 0)), ToDouble(m(0, 0))).finished();
    // clang-format on

    tf.rotate(rotation);
    tf.translate(translation);
    return tf;
}

}  // namespace dart
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

#include <string>

#include <spdlog/fmt/bundled/format.h>

#include <loco/core/single_body/impl/single_body_impl.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace loco {
namespace core {

auto SingleBodyImplNone::SetPose(Pose pose) -> void {}

auto SingleBodyImplNone::SetLinearVelocity(Vec3 linear_vel) -> void {}

auto SingleBodyImplNone::SetAngularVelocity(Vec3 angular_vel) -> void {}

auto SingleBodyImplNone::SetForceCOM(Vec3 force) -> void {}

auto SingleBodyImplNone::SetTorque(Vec3 torque) -> void {}

auto SingleBodyImplNone::ToString() const -> std::string {
    return fmt::format(
        "<SingleBodyImplNone\n"
        "  backend: {0}\n"
        ">\n",
        ::loco::ToString(m_BackendType));
}

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop  // NOLINT
#endif

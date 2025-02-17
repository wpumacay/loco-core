#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <string>

#include <spdlog/fmt/bundled/format.h>

#include <loco/core/impl/simulation_impl.hpp>

namespace loco {
namespace core {

auto SimulationImplNone::Init() -> void {}

auto SimulationImplNone::Reset() -> void {}

auto SimulationImplNone::Step(Scalar step) -> void {}

auto SimulationImplNone::SetTimeStep(Scalar step) -> void {}

auto SimulationImplNone::SetGravity(Vec3 gravity) -> void {}

auto SimulationImplNone::ToString() const -> std::string {
    return fmt::format(
        "<SimulationImplNone\n"
        ">\n");
}

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop  // NOLINT
#endif

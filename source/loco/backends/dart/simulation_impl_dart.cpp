#include <loco/backends/dart/simulation_impl_dart.hpp>

namespace loco {
namespace dart {

SimulationImplDart::SimulationImplDart(core::Scenario::ptr scenario)
    : SimulationImpl(std::move(scenario)) {
    ///
}

auto SimulationImplDart::Init() -> void {
    ///
}

auto SimulationImplDart::Reset() -> void {
    ///
}

auto SimulationImplDart::Step(Scalar step) -> void {
    ///
}

auto SimulationImplDart::SetTimeStep(Scalar fixed_step) -> void {
    ///
}

auto SimulationImplDart::SetGravity(const Vec3& gravity) -> void {
    ///
}

}  // namespace dart
}  // namespace loco

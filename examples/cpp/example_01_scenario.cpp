#include <loco/core/scenario_t.hpp>

auto main() -> int {
    // Create a simple scenario and try out its API
    loco::core::Scenario scenario;
    scenario.Initialize(loco::eBackendType::NONE);

    scenario.SetTimeStep(ToScalar(0.01));
    scenario.SetGravity({ToScalar(0.0), ToScalar(0.0), ToScalar(-1.62)});

    return 0;
}

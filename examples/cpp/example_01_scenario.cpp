#include <loco/core/scenario_t.hpp>
#include <loco/core/simulation_t.hpp>

auto main() -> int {
    auto scenario = std::make_shared<loco::core::Scenario>();
    auto sim = std::make_shared<loco::core::Simulation>(
        scenario, loco::eBackendType::NONE);

    sim->Init();

    sim->SetTimeStep(ToScalar(0.01));
    sim->SetGravity({ToScalar(0.0), ToScalar(0.0), ToScalar(-1.62)});

    return 0;
}

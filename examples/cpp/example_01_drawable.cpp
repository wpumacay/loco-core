#include <loco/core/common.hpp>
#include <loco/core/visualizer/drawable_t.hpp>
#include <loco/core/visualizer/visualizer_t.hpp>

auto main() -> int {
    auto scenario = ::loco::core::Scenario::Create();
    auto visualizer = ::loco::core::Visualizer::Create(scenario);
    visualizer->Init(::loco::eVisualizerType::VIS_MESHCAT);

    while (true) {
        visualizer->Update();
    }

    return 0;
}

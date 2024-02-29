#include <loco/core/common.hpp>
#include <loco/core/visualizer/drawable_t.hpp>
#include <loco/core/visualizer/drawable_primitives.hpp>
#include <loco/core/visualizer/visualizer_t.hpp>

auto main() -> int {
    // Create a simple scene
    auto scenario = ::loco::core::Scenario::Create();
    auto box = ::loco::core::viz::Box::Create("box", Vec3(0.0F, 0.0F, 1.0F),
                                              Vec3(0.1F, 0.1F, 0.1F));

    auto plane = ::loco::core::viz::Plane::Create(
        "plane", Vec3(0.0F, 0.0F, 0.0F), Vec2(1.0F, 1.0F));

    scenario->AddDrawable(box);
    scenario->AddDrawable(plane);

    // Initialize the visualizer
    auto visualizer = ::loco::core::Visualizer::Create(scenario);
    visualizer->Init(::loco::eVisualizerType::VIS_MESHCAT);

    while (true) {
        visualizer->Update();
    }

    return 0;
}

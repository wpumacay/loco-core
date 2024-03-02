#include <loco/core/common.hpp>
#include <loco/core/visualizer/drawable_t.hpp>
#include <loco/core/visualizer/drawable_primitives.hpp>
#include <loco/core/visualizer/visualizer_t.hpp>

auto main() -> int {
    auto scenario = ::loco::core::Scenario::Create();
    auto box = ::loco::core::viz::Box::Create("box", Vec3(0.0F, 0.0F, 1.0F),
                                              Vec3(0.1F, 0.1F, 0.1F));
    auto sphere = ::loco::core::viz::Sphere::Create(
        "sphere", Vec3(0.0F, 1.0F, 1.0F), 0.1F);

    auto cylinder = ::loco::core::viz::Cylinder::Create(
        "cylinder", Vec3(0.0F, -1.0F, 1.0F), 0.1F, 0.4F);

    auto ellipsoid = ::loco::core::viz::Ellipsoid::Create(
        "ellipsoid", Vec3(1.0F, 0.0F, 1.0F), Vec3(0.1F, 0.2F, 0.3F));

    auto plane = ::loco::core::viz::Plane::Create(
        "plane", Vec3(0.0F, 0.0F, 0.0F), Vec2(1.0F, 1.0F));

    scenario->AddDrawable(box);
    scenario->AddDrawable(sphere);
    scenario->AddDrawable(cylinder);
    scenario->AddDrawable(ellipsoid);
    scenario->AddDrawable(plane);

    // Initialize the visualizer
    auto visualizer = ::loco::core::Visualizer::Create(scenario);
    visualizer->Init(::loco::eVisualizerType::VIS_MESHCAT);

    while (true) {
        visualizer->Update();
    }

    return 0;
}

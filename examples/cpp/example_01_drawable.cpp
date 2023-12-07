#include <loco/core/common.hpp>
#include <loco/core/visualizer/drawable_t.hpp>

auto main() -> int {
    // Create a sphere ---------------------------
    ::loco::DrawableData sphere_data;
    sphere_data.type = ::loco::eShapeType::SPHERE;
    sphere_data.size = {1.0, 1.0, 1.0};

    auto sphere_viz =
        ::loco::core::Drawable::Create("sphere_viz", Pose(), sphere_data);

    return 0;
}

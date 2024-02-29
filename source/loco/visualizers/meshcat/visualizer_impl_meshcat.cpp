#include <loco/visualizers/meshcat/visualizer_impl_meshcat.hpp>

#include <loco/visualizers/meshcat/drawable_impl_meshcat.hpp>

namespace loco {
namespace meshcat {

VisualizerImplMeshcat::VisualizerImplMeshcat(
    ::loco::core::Scenario::ptr scenario)
    : ::loco::core::VisualizerImpl(std::move(scenario)) {}

auto VisualizerImplMeshcat::Init() -> void {
    m_MeshcatInstance = std::make_unique<MeshcatCpp::Meshcat>();

    // Collect all free drawables
    auto num_drawables = m_Scenario->num_drawables();
    for (size_t i = 0; i < num_drawables; ++i) {
        auto drawable = m_Scenario->GetDrawableByIndex(i);
        auto drawable_adapter = std::make_unique<DrawableImplMeshcat>(
            drawable->name(), drawable->data(), m_MeshcatInstance);
        drawable->SetAdapter(std::move(drawable_adapter));
        drawable->SetPose(drawable->pose());
    }
}

auto VisualizerImplMeshcat::Reset() -> void {}

auto VisualizerImplMeshcat::Update() -> void {}

}  // namespace meshcat
}  // namespace loco

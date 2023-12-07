#include <loco/visualizers/meshcat/visualizer_impl_meshcat.hpp>

namespace loco {
namespace meshcat {

VisualizerImplMeshcat::VisualizerImplMeshcat(
    ::loco::core::Scenario::ptr scenario)
    : ::loco::core::VisualizerImpl(std::move(scenario)) {}

auto VisualizerImplMeshcat::Init() -> void {
    m_MeshcatInstance = std::make_unique<MeshcatCpp::Meshcat>();
}

auto VisualizerImplMeshcat::Reset() -> void {}

auto VisualizerImplMeshcat::Update() -> void {}

}  // namespace meshcat
}  // namespace loco

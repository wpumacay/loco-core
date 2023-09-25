#include <loco/core/visualizer/visualizer_t.hpp>

#include <stdexcept>

namespace loco {
namespace core {

auto Visualizer::Init() -> void {
    switch (m_VisualizerType) {
        case eVisualizerType::NONE:
            m_VisualizerImpl = std::make_unique<VisualizerImplNone>(m_Scenario);
            break;
        case eVisualizerType::VIS_GL:
            break;
        case eVisualizerType::VIS_MESHCAT:
            break;
    }

    // TODO(wilbert): could create drawables for associated object here (colletc
    // from associated objects like single bodies, etc.), instead of doing it
    // after the call to initialize the visualizer. I'm in doubt of which way
    // makes more sense and results in a clearer interface

    if (m_VisualizerImpl != nullptr) {
        m_VisualizerImpl->Init();
    }

    // TODO(wilbert): initialize all drawables in this scenario
    // 1. Free drawables in the scenario (don't belong to other objects)
    // 2. Drawables associated to single bodies
    // 3. Drawables associated to the elements of compound bodies, recursively
    // 4. Drawables associated to the elements of kinematic trees, recursively
}

auto Visualizer::Reset() -> void {
    if (m_VisualizerImpl != nullptr) {
        m_VisualizerImpl->Reset();
    }
}

auto Visualizer::Update() -> void {
    if (m_VisualizerImpl != nullptr) {
        m_VisualizerImpl->Update();
    }
}

auto Visualizer::impl() -> VisualizerImpl& {
    if (m_VisualizerImpl == nullptr) {
        throw std::runtime_error(
            "Visualizer >>> Must initialize the visualizer first before "
            "accessing its internal impl");
    }
    return *m_VisualizerImpl;
}

auto Visualizer::impl() const -> const VisualizerImpl& {
    if (m_VisualizerImpl == nullptr) {
        throw std::runtime_error(
            "Visualizer >>> Must initialize the visualizer first before "
            "accessing its internal impl");
    }
    return *m_VisualizerImpl;
}

}  // namespace core
}  // namespace loco

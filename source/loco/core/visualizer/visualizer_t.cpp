#include <stdexcept>

#include <loco/core/visualizer/visualizer_t.hpp>

#if defined(LOCO_VISUALIZER_MESHCAT_ENABLED)
#include <loco/visualizers/meshcat/visualizer_impl_meshcat.hpp>
#endif

namespace loco {
namespace core {

auto Visualizer::Init(eVisualizerType type) -> void {
    m_VisualizerType = type;
    switch (m_VisualizerType) {
        case eVisualizerType::NONE: {
            m_VisualizerImpl = std::make_unique<VisualizerImplNone>(m_Scenario);
            break;
        }

#if defined(LOCO_VISUALIZE_OPENGL_ENABLED)
        case eVisualizerType::VIS_GL: {
            break;
        }
#endif

#if defined(LOCO_VISUALIZER_MESHCAT_ENABLED)
        case eVisualizerType::VIS_MESHCAT: {
            m_VisualizerImpl =
                std::make_unique<::loco::meshcat::VisualizerImplMeshcat>(
                    m_Scenario);
            break;
        }
#endif
        default: {
            LOCO_CORE_ERROR(
                "Visualizer::Init >>> requested visualizer type {0} is "
                "not supported",
                ::loco::ToString(m_VisualizerType));
            break;
        }
    }

    // TODO(wilbert): could create drawables for associated object here (collect
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

auto Visualizer::AddDrawable(Drawable::ptr drawable) -> void {
    if (m_Scenario != nullptr) {
        m_Scenario->AddDrawable(std::move(drawable));
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

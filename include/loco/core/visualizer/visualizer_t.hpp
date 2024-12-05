#pragma once

#include <utility>

#include <loco/core/common.hpp>
#include <loco/core/scenario_t.hpp>
#include <loco/core/visualizer/impl/visualizer_impl.hpp>

namespace loco {
namespace core {

/// Core visualizer object, used to visualize a given scenario
class LOCO_API Visualizer {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Visualizer)

    DEFINE_SMART_POINTERS(Visualizer)

 public:
    /// \brief Creates a visualizer and populates it with the given scenario
    explicit Visualizer(Scenario::ptr scenario)
        : m_Scenario(std::move(scenario)) {}

    /// Releases/Frees all allocated resources of this visualizer
    ~Visualizer() = default;

    /// Initializes the internal visualizer interface
    auto Init(eVisualizerType type) -> void;

    /// Resets the visualizer to its initial configuration
    auto Reset() -> void;

    /// Updates the state of the visualizer, syncing to the scenario state
    auto Update() -> void;

    /// Adds the given drawable to the visualizer
    /// \param[in] drawable The drawable to be added to this visualizer
    auto AddDrawable(Drawable::ptr drawable) -> void;

    /// Returns the type of visualizer backend used internally
    LOCO_NODISCARD auto visualizer_type() const -> eVisualizerType {
        return m_VisualizerType;
    }

    /// Returns a mutable reference to the internal backend-specific impl
    LOCO_NODISCARD auto impl() -> VisualizerImpl&;

    /// Returns an unmutable reference to the internal backend-specific impl
    LOCO_NODISCARD auto impl() const -> const VisualizerImpl&;

 protected:
    /// Scenario to be simulated
    Scenario::ptr m_Scenario = nullptr;

    /// The type of backend being used for this physics simulation
    eVisualizerType m_VisualizerType = eVisualizerType::NONE;

    /// Backend-specific interface to the visualizer backend
    VisualizerImpl::uptr m_VisualizerImpl = nullptr;
};

}  // namespace core
}  // namespace loco

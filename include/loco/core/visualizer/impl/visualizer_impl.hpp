#pragma once

#include <memory>
#include <utility>

#include <loco/core/common.hpp>
#include <loco/core/scenario_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace loco {
namespace core {

/// Represents an adapter that links to a specific visualizer backend
class VisualizerImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(VisualizerImpl)

    DEFINE_SMART_POINTERS(VisualizerImpl)

 public:
    explicit VisualizerImpl(Scenario::ptr scenario)
        : m_Scenario(std::move(scenario)) {}

    /// Clean/Release allocated resources
    virtual ~VisualizerImpl() = default;

    /// Initializes the visualizer backend
    virtual auto Init() -> void = 0;

    /// Resets the visualizer to its initial configuration
    virtual auto Reset() -> void = 0;

    /// Updates the state of the visualizer backend with the scenario
    virtual auto Update() -> void = 0;

 protected:
    /// The scenario to be visualized
    Scenario::ptr m_Scenario;
};

/// Represents a dummy adapter for a visualizer (no visualization happens)
class VisualizerImplNone : public VisualizerImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(VisualizerImplNone)

    DEFINE_SMART_POINTERS(VisualizerImplNone)

 public:
    explicit VisualizerImplNone(Scenario::ptr scenario)
        : VisualizerImpl(std::move(scenario)) {}

    ~VisualizerImplNone() override = default;

    auto Init() -> void override {}

    auto Reset() -> void override {}

    auto Update() -> void override {}
};

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

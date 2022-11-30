#pragma once

#include <loco/core/common.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace loco {
namespace core {

/// Represents an adapter that connects a Scenario to a simulation backend
class ScenarioImpl {
    // cppcheck-suppress unknownMacro
    DEFAULT_COPY_AND_MOVE_AND_ASSIGN(ScenarioImpl)

    DEFINE_SMART_POINTERS(ScenarioImpl)

 public:
    ScenarioImpl() = default;

    /// Clean/Release allocated resources
    virtual ~ScenarioImpl() = default;

    /// Sets the internal timestep (amount of time each sim. step takes)
    virtual auto SetTimeStep(Scalar step) -> void = 0;

    /// Sets the internal gravity
    virtual auto SetGravity(const Vec3& gravity) -> void = 0;
};

/// Represents a dummy adapter for a scenario (no simulation happens)
class ScenarioImplNone : public ScenarioImpl {
    DEFINE_SMART_POINTERS(ScenarioImplNone);

 public:
    ScenarioImplNone() = default;

    // NOLINTNEXTLINE
    auto SetTimeStep(Scalar step) -> void {}

    // NOLINTNEXTLINE
    auto SetGravity(const Vec3& gravity) -> void {}
};

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

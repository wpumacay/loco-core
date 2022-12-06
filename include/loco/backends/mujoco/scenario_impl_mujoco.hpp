#pragma once

#include <mujoco/mujoco.h>

#include <loco/core/impl/scenario_impl.hpp>

namespace loco {
namespace mujoco {

/// Deleter for mjModel (when using unique_ptr)
struct MjcModelDeleter {
    auto operator()(mjModel* ptr) const -> void;
};

/// Deleter for mjData (when using unique_ptr)
struct MjcDataDeleter {
    auto operator()(mjData* ptr) const -> void;
};

/// Deleter for mjvScene (when using unique_ptr)
struct MjvSceneDeleter {
    auto operator()(mjvScene* ptr) const -> void;
};

class ScenarioImplMujoco : public core::ScenarioImpl {
 private:
};

}  // namespace mujoco
}  // namespace loco

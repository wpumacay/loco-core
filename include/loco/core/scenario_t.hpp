#pragma once

#include <loco/core/common.hpp>

namespace loco {
namespace core {

class Scenario {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Scenario)

    DEFINE_SMART_POINTERS(Scenario)

 public:
    /// Creates a scenario with a default dummy runtime and backend
    Scenario() = default;

    /// Releases/Frees all allocated resources of this scenario
    ~Scenario() = default;

 protected:
    // TODO(wilbert): add instance variables for bodies, articulated systems
};

}  // namespace core
}  // namespace loco

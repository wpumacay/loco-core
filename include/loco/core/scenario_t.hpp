#pragma once

#include <vector>
#include <string>

#include "./common.hpp"
#include "./body_t.hpp"

namespace loco {
namespace core {

class Scenario {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Scenario)

    DEFINE_SMART_POINTERS(Scenario)

 public:
    /// Max number of bodies allowed in a single scene (used for array capacity)
    static constexpr size_t MAX_BODIES = 128;

    /// Creates a scenario with a default dummy runtime and backend
    Scenario();

    /// Releases/Frees all allocated resources of this scenario
    ~Scenario();

    /// Returns the current number of bodies in this scenario
    auto num_bodies() const -> size_t { return this->bodies.size(); }

    /// Returns whether this scenario is empty or not
    auto empty() const -> bool { return this->bodies.empty(); }

    /// Returns a string representation of this scenario
    auto toString() const -> std::string;

 public:
    /// The list of bodies currently hold by this scenario
    std::vector<Body::ptr> bodies;
};

}  // namespace core
}  // namespace loco

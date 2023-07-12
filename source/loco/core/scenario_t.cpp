#include <stdexcept>

#include <loco/core/scenario_t.hpp>

#include <spdlog/fmt/bundled/format.h>

namespace loco {
namespace core {

constexpr size_t Scenario::MAX_BODIES;

Scenario::Scenario() { this->bodies.reserve(Scenario::MAX_BODIES); }

Scenario::~Scenario() { this->bodies.clear(); }

auto Scenario::toString() const -> std::string {
    return fmt::format("Scenario(num_bodies={}, max_bodies={})", num_bodies(),
                       Scenario::MAX_BODIES);
}

}  // namespace core
}  // namespace loco

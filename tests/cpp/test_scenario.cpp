#include <catch2/catch.hpp>
#include <loco/core/scenario_t.hpp>

// NOLINTNEXTLINE
TEST_CASE("Scenario type", "[Scenario]") {
    SECTION("Default constructor") {
        ::loco::core::Scenario test_scenario;
        REQUIRE(test_scenario.num_bodies() == 0);
        REQUIRE(test_scenario.empty());
        REQUIRE_FALSE(test_scenario.toString().empty());

        REQUIRE(test_scenario.bodies.empty());
        REQUIRE(test_scenario.bodies.capacity() ==
                ::loco::core::Scenario::MAX_BODIES);
    }
}

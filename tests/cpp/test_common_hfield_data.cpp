#include <catch2/catch.hpp>
#include <loco/core/common.hpp>

#include <cmath>
#include <cstring>

// NOLINTNEXTLINE
TEST_CASE("HfieldData constructors", "[HfieldData]") {
    SECTION("Hfield default constructor") {
        ::loco::HeightfieldData hfield_data;
        REQUIRE(hfield_data.n_width_samples == 0);
        REQUIRE(hfield_data.n_depth_samples == 0);
        REQUIRE(hfield_data.heights == nullptr);
    }

    SECTION("Hfield copy constructor") {
        ::loco::HeightfieldData hfield_data_1;
        constexpr size_t GRID_WIDTH = 10;
        constexpr size_t GRID_DEPTH = 10;
        hfield_data_1.n_width_samples = GRID_WIDTH;
        hfield_data_1.n_depth_samples = GRID_DEPTH;
        hfield_data_1.heights =
            std::make_unique<Scalar[]>(GRID_WIDTH * GRID_DEPTH);  // NOLINT
        for (size_t i = 0; i < GRID_DEPTH; ++i) {
            for (size_t j = 0; j < GRID_WIDTH; ++j) {
                hfield_data_1.heights[j + GRID_WIDTH * i] = ToScalar(i + j);
            }
        }
        // Make sure copy constructor works as expected
        ::loco::HeightfieldData hfield_data_2(hfield_data_1);
        REQUIRE(hfield_data_2.n_width_samples == GRID_WIDTH);
        REQUIRE(hfield_data_2.n_depth_samples == GRID_DEPTH);
        REQUIRE(hfield_data_2.heights != nullptr);
        REQUIRE(hfield_data_2.heights.get() != hfield_data_1.heights.get());
        REQUIRE(memcmp(hfield_data_2.heights.get(), hfield_data_1.heights.get(),
                       sizeof(Scalar) * GRID_WIDTH * GRID_DEPTH) == 0);
        // The base from which we copy should still be valid (it's only copied)
        REQUIRE(hfield_data_1.n_width_samples == GRID_WIDTH);
        REQUIRE(hfield_data_1.n_depth_samples == GRID_DEPTH);
        REQUIRE(hfield_data_1.heights != nullptr);
    }

    SECTION("Hfield copy assignment operator") {
        ::loco::HeightfieldData hfield_data_1;
        constexpr size_t GRID_WIDTH = 2;
        constexpr size_t GRID_DEPTH = 2;
        hfield_data_1.n_width_samples = GRID_WIDTH;
        hfield_data_1.n_depth_samples = GRID_DEPTH;
        hfield_data_1.heights =
            std::make_unique<Scalar[]>(GRID_WIDTH * GRID_DEPTH);  // NOLINT
        hfield_data_1.heights[0] = ToScalar(0.1);
        hfield_data_1.heights[1] = ToScalar(0.2);
        hfield_data_1.heights[2] = ToScalar(0.3);
        hfield_data_1.heights[3] = ToScalar(0.4);

        // Make sure the copy assignment operator works as expected
        ::loco::HeightfieldData hfield_data_2;
        hfield_data_2 = hfield_data_1;
        REQUIRE(hfield_data_2.n_width_samples == GRID_WIDTH);
        REQUIRE(hfield_data_2.n_depth_samples == GRID_DEPTH);
        REQUIRE(hfield_data_2.heights != nullptr);
        REQUIRE(hfield_data_2.heights.get() != hfield_data_1.heights.get());
        REQUIRE(hfield_data_2.heights[0] == ToScalar(0.1));
        REQUIRE(hfield_data_2.heights[1] == ToScalar(0.2));
        REQUIRE(hfield_data_2.heights[2] == ToScalar(0.3));
        REQUIRE(hfield_data_2.heights[3] == ToScalar(0.4));

        // The base from which we copy should still be valid (it's only copied)
        REQUIRE(hfield_data_1.n_width_samples == GRID_WIDTH);
        REQUIRE(hfield_data_1.n_depth_samples == GRID_DEPTH);
        REQUIRE(hfield_data_1.heights != nullptr);
        // We should be able to modify the base without modifying the copy
        constexpr size_t NEW_GRID_WIDTH = 3;
        constexpr size_t NEW_GRID_DEPTH = 3;
        constexpr size_t NEW_GRID_NSAMPLES = NEW_GRID_WIDTH * NEW_GRID_DEPTH;
        hfield_data_1.n_width_samples = NEW_GRID_WIDTH;
        hfield_data_1.n_depth_samples = NEW_GRID_DEPTH;
        hfield_data_1.heights =
            std::make_unique<Scalar[]>(NEW_GRID_NSAMPLES);  // NOLINT
        hfield_data_1.heights[0] = ToScalar(0.5);
        hfield_data_1.heights[1] = ToScalar(0.6);
        hfield_data_1.heights[2] = ToScalar(0.7);
        hfield_data_1.heights[3] = ToScalar(0.8);
        hfield_data_1.heights[4] = ToScalar(0.9);
        hfield_data_1.heights[5] = ToScalar(1.0);
        hfield_data_1.heights[6] = ToScalar(1.1);
        hfield_data_1.heights[7] = ToScalar(1.2);
        hfield_data_1.heights[8] = ToScalar(1.3);
        // Checking our base changes as we expected
        REQUIRE(hfield_data_1.n_width_samples == NEW_GRID_WIDTH);
        REQUIRE(hfield_data_1.n_depth_samples == NEW_GRID_DEPTH);
        REQUIRE(hfield_data_1.heights[0] == ToScalar(0.5));
        REQUIRE(hfield_data_1.heights[1] == ToScalar(0.6));
        REQUIRE(hfield_data_1.heights[2] == ToScalar(0.7));
        REQUIRE(hfield_data_1.heights[3] == ToScalar(0.8));
        REQUIRE(hfield_data_1.heights[4] == ToScalar(0.9));
        REQUIRE(hfield_data_1.heights[5] == ToScalar(1.0));
        REQUIRE(hfield_data_1.heights[6] == ToScalar(1.1));
        REQUIRE(hfield_data_1.heights[7] == ToScalar(1.2));
        REQUIRE(hfield_data_1.heights[8] == ToScalar(1.3));
        // Checking the copy keep unchanged even if base was modified
        REQUIRE(hfield_data_2.n_width_samples == GRID_WIDTH);
        REQUIRE(hfield_data_2.n_depth_samples == GRID_DEPTH);
        REQUIRE(hfield_data_2.heights[0] == ToScalar(0.1));
        REQUIRE(hfield_data_2.heights[1] == ToScalar(0.2));
        REQUIRE(hfield_data_2.heights[2] == ToScalar(0.3));
        REQUIRE(hfield_data_2.heights[3] == ToScalar(0.4));
    }
}

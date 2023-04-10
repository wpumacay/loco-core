#include <catch2/catch.hpp>
#include <loco/core/common.hpp>

// NOLINTNEXTLINE
TEST_CASE("MeshData constructors", "[MeshData]") {
    SECTION("Default constructor") {
        ::loco::MeshData mesh_data;
        REQUIRE(mesh_data.n_vertices == 0);
        REQUIRE(mesh_data.n_faces == 0);
        REQUIRE(mesh_data.filepath.empty());
        REQUIRE(mesh_data.vertices == nullptr);
        REQUIRE(mesh_data.faces == nullptr);
    }
}

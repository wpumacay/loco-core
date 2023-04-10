#include <catch2/catch.hpp>
#include <loco/core/common.hpp>

#include <cmath>
#include <cstring>

// NOLINTNEXTLINE
TEST_CASE("MeshData constructors", "[MeshData]") {
    SECTION("MeshData default constructor") {
        ::loco::MeshData mesh_data;
        REQUIRE(mesh_data.n_vertices == 0);
        REQUIRE(mesh_data.n_faces == 0);
        REQUIRE(mesh_data.filepath.empty());
        REQUIRE(mesh_data.vertices == nullptr);
        REQUIRE(mesh_data.faces == nullptr);
    }

    SECTION("MeshData copy constructor") {
        ::loco::MeshData mesh_data_1;
        // Test using a simple quad
        //
        // p3 (-1, 1, 0) *-------* p2(1, 1, 0)
        //               |       |
        //               |       |
        // p0(-1, -1, 0) *-------* p1(1, -1, 0)
        constexpr size_t NUM_VERTICES = 4;
        constexpr size_t NUM_FACES = 2;
        constexpr const char* FILEPATH = "/home/someone/monkey.stl";
        mesh_data_1.filepath = FILEPATH;
        mesh_data_1.n_vertices = NUM_VERTICES;
        mesh_data_1.vertices =
            std::make_unique<Scalar[]>(3 * NUM_VERTICES);  // NOLINT
        // vertex p0 = (-1.0, -1.0, 0.0)
        mesh_data_1.vertices[0] = ToScalar(-1.0);
        mesh_data_1.vertices[1] = ToScalar(-1.0);
        mesh_data_1.vertices[2] = ToScalar(0.0);
        // vertex p1 = (1.0, -1.0, 0.0)
        mesh_data_1.vertices[3] = ToScalar(1.0);
        mesh_data_1.vertices[4] = ToScalar(-1.0);
        mesh_data_1.vertices[5] = ToScalar(0.0);
        // vertex p2 = (1.0, 1.0, 0.0)
        mesh_data_1.vertices[6] = ToScalar(1.0);
        mesh_data_1.vertices[7] = ToScalar(1.0);
        mesh_data_1.vertices[8] = ToScalar(0.0);
        // vertex p3 = (1.0, 1.0, 0.0)
        mesh_data_1.vertices[9] = ToScalar(-1.0);
        mesh_data_1.vertices[10] = ToScalar(1.0);
        mesh_data_1.vertices[11] = ToScalar(0.0);
        mesh_data_1.n_faces = NUM_FACES;
        mesh_data_1.faces =
            std::make_unique<uint32_t[]>(3 * NUM_FACES);  // NOLINT
        // triangle p0-p1-p2
        mesh_data_1.faces[0] = 0;
        mesh_data_1.faces[1] = 1;
        mesh_data_1.faces[2] = 2;
        // triangle p0-p2-p3
        mesh_data_1.faces[3] = 0;
        mesh_data_1.faces[4] = 2;
        mesh_data_1.faces[5] = 3;

        // Make sure the copy constructor works as expected
        ::loco::MeshData mesh_data_2(mesh_data_1);
        REQUIRE(mesh_data_2.filepath == FILEPATH);
        REQUIRE(mesh_data_2.n_vertices == NUM_VERTICES);
        REQUIRE(mesh_data_2.n_faces == NUM_FACES);
        REQUIRE(mesh_data_2.vertices != nullptr);
        REQUIRE(mesh_data_2.faces != nullptr);
        // vertex p0 = (-1.0, -1.0, 0.0)
        REQUIRE(mesh_data_2.vertices[0] == ToScalar(-1.0));
        REQUIRE(mesh_data_2.vertices[1] == ToScalar(-1.0));
        REQUIRE(mesh_data_2.vertices[2] == ToScalar(0.0));
        // vertex p1 = (1.0, -1.0, 0.0)
        REQUIRE(mesh_data_2.vertices[3] == ToScalar(1.0));
        REQUIRE(mesh_data_2.vertices[4] == ToScalar(-1.0));
        REQUIRE(mesh_data_2.vertices[5] == ToScalar(0.0));
        // vertex p2 = (1.0, 1.0, 0.0)
        REQUIRE(mesh_data_2.vertices[6] == ToScalar(1.0));
        REQUIRE(mesh_data_2.vertices[7] == ToScalar(1.0));
        REQUIRE(mesh_data_2.vertices[8] == ToScalar(0.0));
        // vertex p3 = (1.0, 1.0, 0.0)
        REQUIRE(mesh_data_2.vertices[9] == ToScalar(-1.0));
        REQUIRE(mesh_data_2.vertices[10] == ToScalar(1.0));
        REQUIRE(mesh_data_2.vertices[11] == ToScalar(0.0));
        // triangle p0-p1-p2
        REQUIRE(mesh_data_2.faces[0] == 0);
        REQUIRE(mesh_data_2.faces[1] == 1);
        REQUIRE(mesh_data_2.faces[2] == 2);
        // triangle p0-p2-p3
        REQUIRE(mesh_data_2.faces[3] == 0);
        REQUIRE(mesh_data_2.faces[4] == 2);
        REQUIRE(mesh_data_2.faces[5] == 3);
    }
}

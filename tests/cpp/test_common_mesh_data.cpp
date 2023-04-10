#include <catch2/catch.hpp>
#include <loco/core/common.hpp>

#include <cmath>
#include <cstring>

constexpr size_t NUM_VERTICES = 4;
constexpr size_t NUM_FACES = 2;
constexpr const char* FILEPATH = "/home/someone/monkey.stl";

// NOLINTNEXTLINE
auto create_test_mesh(::loco::MeshData& mesh_data) -> void {
    // Test using a simple quad
    //
    // p3 (-1, 1, 0) *-------* p2(1, 1, 0)
    //               |       |
    //               |       |
    // p0(-1, -1, 0) *-------* p1(1, -1, 0)
    mesh_data.filepath = FILEPATH;
    mesh_data.n_vertices = NUM_VERTICES;
    mesh_data.vertices =
        std::make_unique<Scalar[]>(3 * NUM_VERTICES);  // NOLINT
    // vertex p0 = (-1.0, -1.0, 0.0)
    mesh_data.vertices[0] = ToScalar(-1.0);
    mesh_data.vertices[1] = ToScalar(-1.0);
    mesh_data.vertices[2] = ToScalar(0.0);
    // vertex p1 = (1.0, -1.0, 0.0)
    mesh_data.vertices[3] = ToScalar(1.0);
    mesh_data.vertices[4] = ToScalar(-1.0);
    mesh_data.vertices[5] = ToScalar(0.0);
    // vertex p2 = (1.0, 1.0, 0.0)
    mesh_data.vertices[6] = ToScalar(1.0);
    mesh_data.vertices[7] = ToScalar(1.0);
    mesh_data.vertices[8] = ToScalar(0.0);
    // vertex p3 = (1.0, 1.0, 0.0)
    mesh_data.vertices[9] = ToScalar(-1.0);
    mesh_data.vertices[10] = ToScalar(1.0);
    mesh_data.vertices[11] = ToScalar(0.0);
    mesh_data.n_faces = NUM_FACES;
    mesh_data.faces = std::make_unique<uint32_t[]>(3 * NUM_FACES);  // NOLINT
    // triangle p0-p1-p2
    mesh_data.faces[0] = 0;
    mesh_data.faces[1] = 1;
    mesh_data.faces[2] = 2;
    // triangle p0-p2-p3
    mesh_data.faces[3] = 0;
    mesh_data.faces[4] = 2;
    mesh_data.faces[5] = 3;
}

// NOLINTNEXTLINE
auto validate_test_mesh(const ::loco::MeshData& mesh_data) -> void {
    REQUIRE(mesh_data.filepath == FILEPATH);
    REQUIRE(mesh_data.n_vertices == NUM_VERTICES);
    REQUIRE(mesh_data.n_faces == NUM_FACES);
    REQUIRE(mesh_data.vertices != nullptr);
    REQUIRE(mesh_data.faces != nullptr);
    // vertex p0 = (-1.0, -1.0, 0.0)
    REQUIRE(mesh_data.vertices[0] == ToScalar(-1.0));
    REQUIRE(mesh_data.vertices[1] == ToScalar(-1.0));
    REQUIRE(mesh_data.vertices[2] == ToScalar(0.0));
    // vertex p1 = (1.0, -1.0, 0.0)
    REQUIRE(mesh_data.vertices[3] == ToScalar(1.0));
    REQUIRE(mesh_data.vertices[4] == ToScalar(-1.0));
    REQUIRE(mesh_data.vertices[5] == ToScalar(0.0));
    // vertex p2 = (1.0, 1.0, 0.0)
    REQUIRE(mesh_data.vertices[6] == ToScalar(1.0));
    REQUIRE(mesh_data.vertices[7] == ToScalar(1.0));
    REQUIRE(mesh_data.vertices[8] == ToScalar(0.0));
    // vertex p3 = (1.0, 1.0, 0.0)
    REQUIRE(mesh_data.vertices[9] == ToScalar(-1.0));
    REQUIRE(mesh_data.vertices[10] == ToScalar(1.0));
    REQUIRE(mesh_data.vertices[11] == ToScalar(0.0));
    // triangle p0-p1-p2
    REQUIRE(mesh_data.faces[0] == 0);
    REQUIRE(mesh_data.faces[1] == 1);
    REQUIRE(mesh_data.faces[2] == 2);
    // triangle p0-p2-p3
    REQUIRE(mesh_data.faces[3] == 0);
    REQUIRE(mesh_data.faces[4] == 2);
    REQUIRE(mesh_data.faces[5] == 3);
}

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
        create_test_mesh(mesh_data_1);

        // Make sure the copy constructor works as expected
        ::loco::MeshData mesh_data_2(mesh_data_1);
        validate_test_mesh(mesh_data_2);

        // Also, the original shouldn't have changed
        REQUIRE(mesh_data_1.filepath == FILEPATH);
        REQUIRE(mesh_data_1.n_vertices == NUM_VERTICES);
        REQUIRE(mesh_data_1.vertices != nullptr);
        REQUIRE(mesh_data_1.vertices.get() != mesh_data_2.vertices.get());
        REQUIRE(memcmp(mesh_data_1.vertices.get(), mesh_data_2.vertices.get(),
                       sizeof(Scalar) * NUM_VERTICES * 3) == 0);
        REQUIRE(mesh_data_1.n_faces == NUM_FACES);
        REQUIRE(mesh_data_1.faces != nullptr);
        REQUIRE(mesh_data_1.faces.get() != mesh_data_2.faces.get());
        REQUIRE(memcmp(mesh_data_1.faces.get(), mesh_data_2.faces.get(),
                       sizeof(uint32_t) * NUM_FACES * 3) == 0);
    }

    // This test should be quite similar to the previous one; the only thing
    // that will change is the way we create the copy from the original
    SECTION("MeshData copy assignment operator") {
        ::loco::MeshData mesh_data_1;
        create_test_mesh(mesh_data_1);

        // Make sure the copy assignment operator works as expected
        ::loco::MeshData mesh_data_2;
        mesh_data_2 = mesh_data_1;
        validate_test_mesh(mesh_data_2);

        // Also, the original shouldn't have changed
        REQUIRE(mesh_data_1.filepath == FILEPATH);
        REQUIRE(mesh_data_1.n_vertices == NUM_VERTICES);
        REQUIRE(mesh_data_1.vertices != nullptr);
        REQUIRE(mesh_data_1.vertices.get() != mesh_data_2.vertices.get());
        REQUIRE(memcmp(mesh_data_1.vertices.get(), mesh_data_2.vertices.get(),
                       sizeof(Scalar) * NUM_VERTICES * 3) == 0);
        REQUIRE(mesh_data_1.n_faces == NUM_FACES);
        REQUIRE(mesh_data_1.faces != nullptr);
        REQUIRE(mesh_data_1.faces.get() != mesh_data_2.faces.get());
        REQUIRE(memcmp(mesh_data_1.faces.get(), mesh_data_2.faces.get(),
                       sizeof(uint32_t) * NUM_FACES * 3) == 0);
    }

    SECTION("MeshData move constructor") {
        ::loco::MeshData mesh_data_1;
        create_test_mesh(mesh_data_1);

        // Make sure the move constructor works as expected
        ::loco::MeshData mesh_data_2(std::move(mesh_data_1));
        validate_test_mesh(mesh_data_2);

        // The original should be invalidated
        REQUIRE(mesh_data_1.filepath.empty());
        REQUIRE(mesh_data_1.n_vertices == 0);
        REQUIRE(mesh_data_1.vertices == nullptr);
        REQUIRE(mesh_data_1.n_faces == 0);
        REQUIRE(mesh_data_1.faces == nullptr);
    }

    SECTION("MeshData move assignment operator") {
        ::loco::MeshData mesh_data_1;
        create_test_mesh(mesh_data_1);

        // Make sure the move assignment operator works as expected
        ::loco::MeshData mesh_data_2;
        mesh_data_2 = std::move(mesh_data_1);
        validate_test_mesh(mesh_data_2);

        // The original should be invalidated
        REQUIRE(mesh_data_1.filepath.empty());
        REQUIRE(mesh_data_1.n_vertices == 0);
        REQUIRE(mesh_data_1.vertices == nullptr);
        REQUIRE(mesh_data_1.n_faces == 0);
        REQUIRE(mesh_data_1.faces == nullptr);
    }
}

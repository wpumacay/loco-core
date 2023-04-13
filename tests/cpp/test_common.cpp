#include <catch2/catch.hpp>
#include <loco/core/common.hpp>

#include <cmath>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#endif

// NOLINTNEXTLINE
TEST_CASE("ShapeData type", "[ShapeData]") {
    SECTION("Default constructor") {
        ::loco::ShapeData shape_data;
        REQUIRE(shape_data.type == ::loco::eShapeType::SPHERE);
        REQUIRE(shape_data.size == Vec3(0.0, 0.0, 0.0));
        REQUIRE(shape_data.mesh_data.filepath.empty());
        REQUIRE(shape_data.mesh_data.n_vertices == 0);
        REQUIRE(shape_data.mesh_data.vertices == nullptr);
        REQUIRE(shape_data.mesh_data.n_faces == 0);
        REQUIRE(shape_data.mesh_data.faces == nullptr);
        REQUIRE(shape_data.hfield_data.n_width_samples == 0);
        REQUIRE(shape_data.hfield_data.n_depth_samples == 0);
        REQUIRE(shape_data.hfield_data.heights == nullptr);
        REQUIRE(shape_data.local_tf.position == Vec3(0.0, 0.0, 0.0));
        REQUIRE(shape_data.local_tf.orientation == Quat(1.0, 0.0, 0.0, 0.0));
    }
}

TEST_CASE("ColliderData type", "[ColliderData]") {
    SECTION("Default constructor") {
        ::loco::ColliderData collider_data;
        REQUIRE(collider_data.collision_group == 1);
        REQUIRE(collider_data.collision_mask == 1);
        const Vec3 DEFAULT_FRICTION = {1.0, 0.005, 0.0001};
        REQUIRE(collider_data.friction == DEFAULT_FRICTION);
        REQUIRE(collider_data.children.empty());
    }
}

TEST_CASE("DrawableData type", "[DrawableData]") {
    SECTION("Default constructor") {
        ::loco::DrawableData drawable_data;
        const Vec3 DEFAULT_COLOR = {0.7, 0.5, 0.3};
        REQUIRE(drawable_data.color == DEFAULT_COLOR);
        REQUIRE(drawable_data.texture.empty());
        REQUIRE(drawable_data.children.empty());
    }
}

TEST_CASE("InertialData type", "[InertialData]") {
    SECTION("Default constructor") {
        ::loco::InertialData inertial_data;
        REQUIRE(std::abs(inertial_data.mass - 1.0) < 1e-5);
        REQUIRE(inertial_data.inertia == Mat3::Zeros());
        REQUIRE(inertial_data.local_tf.position == Vec3(0.0, 0.0, 0.0));
        REQUIRE(inertial_data.local_tf.orientation == Quat(1.0, 0.0, 0.0, 0.0));
    }
}

TEST_CASE("BodyData type", "[BodyData]") {
    SECTION("Default constructor") {
        ::loco::BodyData body_data;
        REQUIRE(body_data.dyntype == ::loco::eDynamicsType::DYNAMIC);
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

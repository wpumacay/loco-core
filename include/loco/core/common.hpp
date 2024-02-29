#pragma once

#include <cstdlib>

#include <string>
#include <vector>
#include <memory>
#include <utility>

#include <math/vec2_t.hpp>
#include <math/vec3_t.hpp>
#include <math/vec4_t.hpp>
#include <math/mat3_t.hpp>
#include <math/mat4_t.hpp>
#include <math/quat_t.hpp>
#include <math/euler_t.hpp>
#include <math/pose3d_t.hpp>

#include <utils/logging.hpp>

using Scalar = float;
using Vec2 = ::math::Vector2<Scalar>;
using Vec3 = ::math::Vector3<Scalar>;
using Vec4 = ::math::Vector4<Scalar>;
using Quat = ::math::Quaternion<Scalar>;
using Mat3 = ::math::Matrix3<Scalar>;
using Mat4 = ::math::Matrix4<Scalar>;
using Euler = ::math::Euler<Scalar>;
using Pose = ::math::Pose3d<Scalar>;

// NOLINTNEXTLINE
#define ToScalar(x) static_cast<Scalar>(x)

namespace loco {

/// Maximum number of generalized coordinates possible (3-xyz + 4-quat)
constexpr size_t MAX_NUM_QPOS = 7;
/// Maximum number of degrees of freedom possible (3-xyz + 3-rpy)
constexpr size_t MAX_NUM_QVEL = 6;
/// Number of generalized coordinates for prismatic|slide joints : 1qpos, 1qvel
constexpr size_t NUM_QPOS_JOINT_PRISMATIC = 1;
/// Number of generalized coordinates for revolute|hinge joints : 1pos, 1qvel
constexpr size_t NUM_QPOS_JOINT_REVOLUTE = 1;
/// Number of generalized coordinates for spherical|ball joints : 4qpos, 3qvel
constexpr size_t NUM_QPOS_JOINT_SPHERICAL = 4;
/// Number of generalized coordinates for free|none joints : 7qpos, 6qvel
constexpr size_t NUM_QPOS_JOINT_FREE = 7;
/// Default density (density of water 1000 kg/m3 ) used for mass calculations
constexpr Scalar DEFAULT_DENSITY = static_cast<Scalar>(1000.0);
/// The suffix used for generating names of colliders out of the body name
constexpr const char* SUFFIX_COLLIDER = "_col";
/// The suffix used for generating names of drawables out of the body name
constexpr const char* SUFFIX_DRAWABLE = "_viz";

/// The unit vector in the x-direction
const Vec3 UNIT_VEC_X = {ToScalar(1.0), ToScalar(0.0), ToScalar(0.0)};
/// The unit vector in the y-direction
const Vec3 INIT_VEC_Y = {ToScalar(0.0), ToScalar(1.0), ToScalar(0.0)};
/// The unit vector in the z-direction
const Vec3 INIT_VEC_Z = {ToScalar(0.0), ToScalar(0.0), ToScalar(1.0)};

/// The default color of the drawables
const Vec3 DEFAULT_COLOR = {ToScalar(0.7), ToScalar(0.5), ToScalar(0.3)};

/// Represents all available physics engines that can be used as backends
enum class eBackendType {
    /// Represents a dummy backend (mostly for visualization purposes)
    NONE,
    /// Represents the usage of MuJoCo as physics backend
    MUJOCO,
    /// Represents the usage of Bullet as physics backend
    BULLET,
    /// Represents the usage of DART as physics backend
    DART,
};

/// Returns the string representation of the given backend enumerator
auto ToString(const eBackendType& backend_type) -> std::string;

/// Represents all available visualizers integrated with this framework
enum class eVisualizerType {
    /// Represents a dummy visualizer (for debugging purposes)
    NONE,
    /// Represents the GL Visualizer, implemented with our own renderer
    VIS_GL,
    /// Represents the Meshcat Visualizer, integrated with meshcat (external)
    VIS_MESHCAT,
};

/// Returns the string representation of the given visualizer enumerator
auto ToString(const eVisualizerType& visualizer_type) -> std::string;

/// Represents all available shapes
enum class eShapeType {
    /// Represents a plane, defined by its width and depth
    PLANE,
    /// Represents a box, defined by its width, height and depth
    BOX,
    /// Represents a sphere, defined by its radius
    SPHERE,
    /// Represents a cylinder, defined by its height and radius
    CYLINDER,
    /// Represents a capsule, defined by its height and radius
    CAPSULE,
    /// Represents an ellipsoid, defined by its radii
    ELLIPSOID,
    /// Represents a convex mesh, defined by its vertices
    CONVEX_MESH,
    /// Represents a triangular mesh, defined by its vertices
    TRIANGULAR_MESH,
    /// Represents a heghtfield, defined by its elevation data over an area
    HEIGHTFIELD,
    /// Represents a compound shape, defined by its children
    COMPOUND,
};

/// Returns the string representation of the given shape type
auto ToString(const eShapeType& shape_type) -> std::string;

/// Represents all available dynamics options
enum class eDynamicsType {
    /// Defines an object that is dynamic, i.e. physics applies to this object
    DYNAMIC,
    /// Defines an object that is static, i.e. physics updates don't apply to it
    STATIC,
};

/// Returns the string representation of the given dynamics option
auto ToString(const eDynamicsType& dyn_type) -> std::string;

/// Represents user-defined mesh data (for convex and triangular shapes)
struct MeshData {
    /// Absolute path to the mesh resource (if creating mesh from file)
    std::string filepath;
    /// User vertex-data of the mesh resource (if creating programmatically)
    std::unique_ptr<Scalar[]> vertices = nullptr;  // NOLINT
    /// Number of elements in the vertices buffer
    size_t n_vertices = 0;
    /// User index-data of the mesh resource (if creating programmatically)
    std::unique_ptr<uint32_t[]> faces = nullptr;  // NOLINT
    /// Number of faces composing this mesh
    size_t n_faces = 0;

    // RAII (make sure that we can also export bindings correctly) -------

    /// Creates a default MeshData object with default initialized values
    MeshData() = default;

    /// Releases all allocated resources of this object
    ~MeshData() = default;

    /// Copy constructor for MeshData object. Copies fields if possible, and
    /// makes deep copies of fields that have ownership (like unique_ptr)
    MeshData(const MeshData& other)
        : filepath(other.filepath),
          n_vertices(other.n_vertices),
          n_faces(other.n_faces) {
        // Handle the deep-copy of the vertices buffer --------
        auto vert_num_scalars = 3 * n_vertices;
        // NOLINTNEXTLINE
        vertices = std::unique_ptr<Scalar[]>(new Scalar[vert_num_scalars]);
        memcpy(vertices.get(), other.vertices.get(),
               sizeof(Scalar) * vert_num_scalars);
        // ----------------------------------------------------

        // Handle the deep-copy of the faces buffer -----------
        auto faces_num_scalars = 3 * n_faces;
        // NOLINTNEXTLINE
        faces = std::unique_ptr<uint32_t[]>(new uint32_t[faces_num_scalars]);
        memcpy(faces.get(), other.faces.get(),
               sizeof(Scalar) * faces_num_scalars);
        // ----------------------------------------------------
    }

    /// Copy assignment operator for MeshData object. Similarly to the copy
    /// constructor, make a full copy of all attributes
    auto operator=(const MeshData& other) -> MeshData& {
        filepath = other.filepath;
        n_vertices = other.n_vertices;
        n_faces = other.n_faces;
        // Handle the deep-copy of the vertices buffer --------
        auto vert_num_scalars = 3 * n_vertices;
        // NOLINTNEXTLINE
        vertices = std::unique_ptr<Scalar[]>(new Scalar[vert_num_scalars]);
        memcpy(vertices.get(), other.vertices.get(),
               sizeof(Scalar) * vert_num_scalars);
        // ----------------------------------------------------

        // Handle the deep-copy of the faces buffer -----------
        auto faces_num_scalars = 3 * n_faces;
        // NOLINTNEXTLINE
        faces = std::unique_ptr<uint32_t[]>(new uint32_t[faces_num_scalars]);
        memcpy(faces.get(), other.faces.get(),
               sizeof(Scalar) * faces_num_scalars);
        return *this;
        // ----------------------------------------------------
    }

    /// Move constructor, transfers ownership of other object's data
    MeshData(MeshData&& other) noexcept
        : filepath(std::move(other.filepath)),
          vertices(std::move(other.vertices)),
          n_vertices(other.n_vertices),
          faces(std::move(other.faces)),
          n_faces(other.n_faces) {
        other.filepath = "";
        other.n_vertices = 0;
        other.n_faces = 0;
        other.vertices = nullptr;
        other.faces = nullptr;
    }

    /// Move assignment operator, transfers ownership of other object's data
    auto operator=(MeshData&& other) noexcept -> MeshData& {
        // Transfer ownership
        filepath = std::move(other.filepath);
        n_vertices = other.n_vertices;
        n_faces = other.n_faces;
        vertices = std::move(other.vertices);
        faces = std::move(other.faces);

        // Set to a default state the given r-value reference
        other.filepath = "";
        other.n_vertices = 0;
        other.n_faces = 0;
        other.vertices = nullptr;
        other.faces = nullptr;

        return *this;
    }

    // -------------------------------------------------------------------
};

/// Represents user-defined heightfield data (for heightfield shapes)
struct HeightfieldData {
    /// Number of samples of the hfield's area in the x-dimension
    size_t n_width_samples = 0;
    /// Number of samples of the hfield's area in the y-dimension
    size_t n_depth_samples = 0;
    /// Elevation data stored in row-major order, and normalized to range [0-1]
    std::unique_ptr<Scalar[]> heights = nullptr;  // NOLINT

    // RAII (make sure that we can also export bindings correctly) -------

    /// Creates a default HeighfieldData object with default initialized values
    HeightfieldData() = default;

    /// Releases all allocated resources of this object
    ~HeightfieldData() = default;

    /// Copy constructor for HeightfieldData object. Copies fields if possible,
    /// and makes deep copies of fields that have ownership (like unique_ptr)
    HeightfieldData(const HeightfieldData& other)
        : n_width_samples(other.n_width_samples),
          n_depth_samples(other.n_depth_samples) {
        // Handle the deep-copy of the heights buffer ---------
        auto grid_num_scalars = n_width_samples * n_depth_samples;
        // NOLINTNEXTLINE
        heights = std::unique_ptr<Scalar[]>(new Scalar[grid_num_scalars]);
        memcpy(heights.get(), other.heights.get(),
               sizeof(Scalar) * grid_num_scalars);
        // ----------------------------------------------------
    }

    /// Copy assignment operator for HeightfieldData object. Similarly to the
    /// copy constructor, make a full copy of all attributes
    auto operator=(const HeightfieldData& other) -> HeightfieldData& {
        n_width_samples = other.n_width_samples;
        n_depth_samples = other.n_depth_samples;
        // Handle the deep-copy of the heights buffer ---------
        auto grid_num_scalars = n_width_samples * n_depth_samples;
        // NOLINTNEXTLINE
        heights = std::unique_ptr<Scalar[]>(new Scalar[grid_num_scalars]);
        memcpy(heights.get(), other.heights.get(),
               sizeof(Scalar) * grid_num_scalars);
        // ----------------------------------------------------
        return *this;
    }

    /// Move constructor, transfers ownership of other object's data
    HeightfieldData(HeightfieldData&& other) noexcept
        : n_width_samples(other.n_width_samples),
          n_depth_samples(other.n_depth_samples),  // NOLINT
          heights(std::move(other.heights)) {
        other.n_width_samples = 0;
        other.n_depth_samples = 0;
        other.heights = nullptr;
    }

    /// Move assignment operator, transfers ownership of other object's data
    auto operator=(HeightfieldData&& other) noexcept -> HeightfieldData& {
        n_width_samples = other.n_width_samples;
        n_depth_samples = other.n_depth_samples;
        heights = std::move(other.heights);

        // Set to a default state the given r-value reference
        other.n_width_samples = 0;
        other.n_depth_samples = 0;
        other.heights = nullptr;

        return *this;
    }

    // -------------------------------------------------------------------
};

/// Represents the data that fully describes a shape
struct ShapeData {
    /// Type of this shape
    eShapeType type = eShapeType::SPHERE;
    /// Size of the shape
    Vec3 size;
    /// Mesh data required for convex and triangular mesh shapes
    MeshData mesh_data;
    /// Heightfield data required for heightfield shapes
    HeightfieldData hfield_data;
    /// Local transform w.r.t. parent shape (otherwise not used if no parent)
    Pose local_tf;
};

/// Represents the data that defines a collider
struct ColliderData : ShapeData {
    /// The group-id used for filtering collision pair checks
    int32_t collision_group = 1;
    /// The mask-id used for filtering collision pair checks
    int32_t collision_mask = 1;
    /// The friction coefficients packed as a vec3 (sliding, rolling, etc.)
    Vec3 friction = {ToScalar(1.0), ToScalar(0.005), ToScalar(0.0001)};
    /// Children colliders (in case of being a commpound collider)
    std::vector<ColliderData> children;
};

/// Represents the data that defines a drawable
struct DrawableData : ShapeData {
    /// The color of this drawable
    Vec3 color = {ToScalar(0.7), ToScalar(0.5), ToScalar(0.3)};
    /// The id of the texture of this drawable
    std::string texture;
    /// Children drawable (in case of being a commpound drawable)
    std::vector<DrawableData> children;
};

/// Represents the inertial properties of a body
struct InertialData {
    /// The mass of the related body
    Scalar mass = ToScalar(1.0);
    /// The inertia matrix of the related body
    Mat3 inertia;
    /// The relative transform of the body's COM reference frame
    Pose local_tf;
};

/// Represents the properties of a rigid body
struct BodyData {
    /// Dynamics type used for this rigid body
    eDynamicsType dyntype = eDynamicsType::DYNAMIC;
    /// Inertial properties of this rigid body
    InertialData inertia;
    /// Collider data of this rigid body
    ColliderData collider;
    /// Drawable data of this rigid body
    DrawableData drawable;
};

}  // namespace loco

#ifdef LOCO_LOGS_ENABLED
// NOLINTNEXTLINE
#define LOCO_CORE_TRACE(...) LOG_CORE_TRACE(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_CORE_INFO(...) LOG_CORE_INFO(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_CORE_WARN(...) LOG_CORE_WARN(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_CORE_ERROR(...) LOG_CORE_ERROR(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_CORE_CRITICAL(...) LOG_CORE_CRITICAL(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_CORE_ASSERT(x, ...) LOG_CORE_ASSERT((x), __VA_ARGS__)

// NOLINTNEXTLINE
#define LOCO_TRACE(...) LOG_TRACE(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_INFO(...) LOG_INFO(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_WARN(...) LOG_WARN(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_ERROR(...) LOG_ERROR(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_CRITICAL(...) LOG_CRITICAL(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOCO_ASSERT(x, ...) LOG_ASSERT((x), __VA_ARGS__)
#else
// NOLINTNEXTLINE
#define LOCO_CORE_TRACE(...) ((void)0)
// NOLINTNEXTLINE
#define LOCO_CORE_INFO(...) ((void)0)
// NOLINTNEXTLINE
#define LOCO_CORE_WARN(...) ((void)0)
// NOLINTNEXTLINE
#define LOCO_CORE_ERROR(...) ((void)0)
// NOLINTNEXTLINE
#define LOCO_CORE_CRITICAL(...) assert(false)
// NOLINTNEXTLINE
#define LOCO_CORE_ASSERT(x, ...) assert((x))

// NOLINTNEXTLINE
#define LOCO_TRACE(...) ((void)0)
// NOLINTNEXTLINE
#define LOCO_INFO(...) ((void)0)
// NOLINTNEXTLINE
#define LOCO_WARN(...) ((void)0)
// NOLINTNEXTLINE
#define LOCO_ERROR(...) ((void)0)
// NOLINTNEXTLINE
#define LOCO_CRITICAL(...) assert(false)
// NOLINTNEXTLINE
#define LOCO_ASSERT(x, ...) assert((x))
#endif

#ifdef LOCO_PROFILING_ENABLED
// NOLINTNEXTLINE
#define LOCO_PROFILE_SCOPE(n) PROFILE_SCOPE(n)
// NOLINTNEXTLINE
#define LOCO_PROFILE_SCOPE_IN_SESSION(n, s) PROFILE_SCOPE_IN_SESSION(n, s)
// NOLINTNEXTLINE
#define LOCO_PROFILE_FUNCTION() PROFILE_FUNCTION()
// NOLINTNEXTLINE
#define LOCO_PROFILE_FUNCTION_IN_SESSION(s) PROFILE_FUNCTION_IN_SESSION(s)
#else
// NOLINTNEXTLINE
#define LOCO_PROFILE_SCOPE(n) ((void)0)
// NOLINTNEXTLINE
#define LOCO_PROFILE_SCOPE_IN_SESSION(n, s) ((void)0)
// NOLINTNEXTLINE
#define LOCO_PROFILE_FUNCTION() ((void)0)
// NOLINTNEXTLINE
#define LOCO_PROFILE_FUNCTION_IN_SESSION(s) ((void)0)
#endif

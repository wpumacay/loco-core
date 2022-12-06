#pragma once

#include <cstdlib>

#include <string>
#include <vector>

#include <math/vec2_t.hpp>
#include <math/vec3_t.hpp>
#include <math/vec4_t.hpp>
#include <math/quat_t.hpp>
#include <math/mat4_t.hpp>

#include <utils/logging.hpp>

using Scalar = float;
using Vec2 = math::Vector2<Scalar>;
using Vec3 = math::Vector3<Scalar>;
using Vec4 = math::Vector4<Scalar>;
using Quat = math::Quaternion<Scalar>;
using Mat3 = math::Matrix3<Scalar>;
using Mat4 = math::Matrix4<Scalar>;

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
    const Scalar* vertices = nullptr;
    /// Number of elements in the vertices buffer
    size_t n_vertices = 0;
    /// User index-data of the mesh resource (if creating programmatically)
    const Scalar* faces = nullptr;
    /// Number of elements in the faces buffer
    size_t n_indices = 0;
};

/// Represents user-defined heightfield data (for heightfield shapes)
struct HeightfieldData {
    /// Number of samples of the hfield's area in the x-dimension
    size_t n_width_samples = 0;
    /// Number of samples of the hfield's area in the y-dimension
    size_t n_depth_samples = 0;
    /// Elevation data stored in row-major order, and normalized to range [0-1]
    const Scalar* heights = nullptr;
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
    /// List of children shapes (in case of compound shapes)
    std::vector<ShapeData> children;
    /// Local transform w.r.t. parent shape (otherwise not used if no parent)
    Mat4 local_tf;
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

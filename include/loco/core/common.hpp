#pragma once

// clang-format off
#include <cstdlib>

#include <loco/math/vec2_t_impl.hpp>
#include <loco/math/vec3_t_impl.hpp>
#include <loco/math/vec4_t_impl.hpp>
#include <loco/math/mat4_t_impl.hpp>

#include <loco/utils/logging.hpp>
// clang-format on

using Vec2 = loco::math::Vector2<float>;
using Vec3 = loco::math::Vector3<float>;
using Vec4 = loco::math::Vector4<float>;

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
constexpr double DEFAULT_DENSITY = 1000.0;

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

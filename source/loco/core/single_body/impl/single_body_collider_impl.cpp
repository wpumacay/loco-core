#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#include <loco/core/single_body/impl/single_body_collider_impl.hpp>

namespace loco {
namespace core {

auto SingleBodyColliderImplNone::ChangeSize(Vec3 size) -> void {
    // Do nothing (no backend)
}

auto SingleBodyColliderImplNone::ChangeVertexData(
    size_t num_vertices, const Scalar* ptr_vertices, size_t num_faces,
    const uint32_t* ptr_faces) -> void {
    // Do nothing (no backend)
}

auto SingleBodyColliderImplNone::ChangeElevationData(
    size_t n_width_samples, size_t n_depth_samples,
    const Scalar* ptr_heights) -> void {
    // Do nothing (no backend)
}

auto SingleBodyColliderImplNone::ChangeCollisionGroup(int32_t col_group)
    -> void {
    // Do nothing (no backend)
}

auto SingleBodyColliderImplNone::ChangeCollisionMask(int32_t col_mask) -> void {
    // Do nothing (no backend)
}

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

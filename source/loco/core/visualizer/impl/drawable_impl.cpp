#include <string>

#include <loco/core/visualizer/impl/drawable_impl.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace loco {
namespace core {

auto DrawableImplNone::SetPose(const Pose& pose) -> void {}

auto DrawableImplNone::SetColor(const Vec3& color) -> void {}

auto DrawableImplNone::SetTexture(const std::string& tex_filepath) -> void {}

auto DrawableImplNone::ChangeSize(const Vec3& size) -> void {}

auto DrawableImplNone::ChangeVertexData(size_t num_vertices,
                                        const Scalar* ptr_vertices,
                                        size_t num_faces,
                                        const uint32_t* ptr_faces) -> void {}

auto DrawableImplNone::ChangeElevationData(size_t n_width_samples,
                                           size_t n_depth_samples,
                                           const Scalar* ptr_heights) -> void {}

auto DrawableImplNone::SetVisible(bool visible) -> void {}

auto DrawableImplNone::SetWireframe(bool wireframe) -> void {}

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop  // NOLINT
#endif

#include <loco/visualizers/meshcat/drawable_impl_meshcat.hpp>

namespace loco {
namespace meshcat {

DrawableImplMeshcat::DrawableImplMeshcat(
    std::string name, ::loco::DrawableData data,
    std::shared_ptr<MeshcatCpp::Meshcat> handle)
    : m_Data(std::move(data)),
      m_Name(std::move(name)),
      m_Handle(std::move(handle)) {
    ::loco::meshcat::CreateShape(*m_Handle, m_Name, m_Data);
}

auto DrawableImplMeshcat::SetPose(const Pose& pose) -> void {}

auto DrawableImplMeshcat::SetColor(const Vec3& color) -> void {}

auto DrawableImplMeshcat::SetTexture(const std::string& tex_filepath) -> void {}

auto DrawableImplMeshcat::ChangeSize(const Vec3& size) -> void {}

auto DrawableImplMeshcat::ChangeVertexData(size_t num_vertices,
                                           const Scalar* ptr_vertices,
                                           size_t num_faces,
                                           const uint32_t* ptr_faces) -> void {}

auto DrawableImplMeshcat::ChangeElevationData(size_t n_width_samples,
                                              size_t n_depth_samples,
                                              const Scalar* ptr_heights)
    -> void {}

auto DrawableImplMeshcat::SetVisible(bool visible) -> void {}

auto DrawableImplMeshcat::SetWireframe(bool wireframe) -> void {}

}  // namespace meshcat
}  // namespace loco

#include <loco/visualizers/meshcat/drawable_impl_meshcat.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

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

auto DrawableImplMeshcat::SetPose(const Pose& pose) -> void {
    Mat4 tf(pose.position, pose.orientation);
    std::array<double, 16> tf_array = {
        static_cast<double>(tf(0, 0)), static_cast<double>(tf(1, 0)),
        static_cast<double>(tf(2, 0)), static_cast<double>(tf(3, 0)),
        static_cast<double>(tf(0, 1)), static_cast<double>(tf(1, 1)),
        static_cast<double>(tf(2, 1)), static_cast<double>(tf(3, 1)),
        static_cast<double>(tf(0, 2)), static_cast<double>(tf(1, 2)),
        static_cast<double>(tf(2, 2)), static_cast<double>(tf(3, 2)),
        static_cast<double>(tf(0, 3)), static_cast<double>(tf(1, 3)),
        static_cast<double>(tf(2, 3)), static_cast<double>(tf(3, 3))};

    auto mat_view = ::loco::meshcat::ConvertToMatrixView(tf_array);
    m_Handle->set_transform("/loco/" + m_Name, mat_view);
}

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

auto DrawableImplMeshcat::SetVisible(bool visible) -> void {
    m_Handle->set_property("/loco/" + m_Name, "visible", visible);
}

auto DrawableImplMeshcat::SetWireframe(bool wireframe) -> void {}

}  // namespace meshcat
}  // namespace loco

#include <cstring>
#include <stdexcept>

#include <utils/logging.hpp>

#include <loco/core/visualizer/drawable_t.hpp>

namespace loco {
namespace core {

auto Drawable::Initialize(eVisualizerType vis_type) -> void {
    m_VisualizerType = vis_type;

    // Do some configuration ---------------------------------------------------
    switch (m_VisualizerType) {
        case ::loco::eVisualizerType::NONE: {
            m_BackendImpl = std::make_unique<DrawableImplNone>();
            break;
        }

        case ::loco::eVisualizerType::VIS_GL: {
            /* TODO(wilbert): Instantiate the vis-gl adapter */
            break;
        }

        case ::loco::eVisualizerType::VIS_MESHCAT: {
            /* TODO(wilbert): Instantiate the vis-meshcat adapter */
            break;
        }

        default: {
            m_BackendImpl = std::make_unique<DrawableImplNone>();
            break;
        }
    }
}

auto Drawable::SetVisible(bool visible) -> void {
    m_Visible = visible;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetVisible(m_Visible);
    }
}

auto Drawable::SetWireframe(bool wireframe) -> void {
    m_Wireframe = wireframe;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetWireframe(m_Wireframe);
    }
}

auto Drawable::SetPosition(const Vec3& pos) -> void {
    m_Pose.position = pos;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetPose(m_Pose);
    }
}

auto Drawable::SetOrientation(const Quat& quat) -> void {
    m_Pose.orientation = quat;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetPose(m_Pose);
    }
}

auto Drawable::SetPose(const Pose& pose) -> void {
    m_Pose = pose;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetPose(m_Pose);
    }
}

auto Drawable::SetColor(const Vec3& color) -> void {
    m_Data.color = color;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->SetColor(m_Data.color);
    }
}

auto Drawable::SetTexture(const std::string& tex_filepath) -> void {
    m_Data.texture = tex_filepath;
    if (m_BackendImpl != nullptr) {
        // Hints the backend to handle the loading and setup of the given tex.
        m_BackendImpl->SetTexture(m_Data.texture);
    }
}

auto Drawable::ChangeSize(const Vec3& size) -> void {
    m_Data.size = size;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

auto Drawable::ChangeVertexData(size_t num_vertices, const Scalar* ptr_vertices,
                                size_t num_faces, const uint32_t* ptr_faces)
    -> void {
    // Store the data into our own copy just to keep track ---------------------
    const auto VERT_NUM_ELEMENTS = 3 * num_vertices;
    const auto FACES_NUM_ELEMENTS = 3 * num_faces;
    m_Data.mesh_data.n_vertices = num_vertices;
    m_Data.mesh_data.n_faces = num_faces;
    // NOLINTNEXTLINE
    m_Data.mesh_data.vertices = std::make_unique<Scalar[]>(VERT_NUM_ELEMENTS);
    memcpy(m_Data.mesh_data.vertices.get(), ptr_vertices,
           sizeof(Scalar) * VERT_NUM_ELEMENTS);
    // NOLINTNEXTLINE
    m_Data.mesh_data.faces = std::make_unique<uint32_t[]>(FACES_NUM_ELEMENTS);
    memcpy(m_Data.mesh_data.faces.get(), ptr_faces,
           sizeof(uint32_t) * FACES_NUM_ELEMENTS);
    // -------------------------------------------------------------------------
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeVertexData(num_vertices, ptr_vertices, num_faces,
                                        ptr_faces);
    }
}

auto Drawable::ChangeElevationData(size_t n_width_samples,
                                   size_t n_depth_samples,
                                   const Scalar* ptr_heights) -> void {
    // Store the data into our own copy just to keep track ---------------------
    const auto GRID_NUM_SCALARS = n_width_samples * n_depth_samples;
    m_Data.hfield_data.n_width_samples = n_width_samples;
    m_Data.hfield_data.n_depth_samples = n_depth_samples;
    // NOLINTNEXTLINE
    m_Data.hfield_data.heights = std::make_unique<Scalar[]>(GRID_NUM_SCALARS);
    memcpy(m_Data.hfield_data.heights.get(), ptr_heights,
           sizeof(Scalar) * GRID_NUM_SCALARS);
    // -------------------------------------------------------------------------
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeElevationData(n_width_samples, n_depth_samples,
                                           ptr_heights);
    }
}

}  // namespace core
}  // namespace loco

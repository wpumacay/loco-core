#include <stdexcept>
#include <cstring>

#include <utils/logging.hpp>

#include <loco/core/single_body/single_body_collider_t.hpp>

namespace loco {
namespace core {

auto SingleBodyCollider::ChangeSize(const Vec3& size) -> void {
    m_Data.size = size;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(size);
    }
}

auto SingleBodyCollider::ChangeVertexData(size_t num_vertices,
                                          const Scalar* ptr_vertices,
                                          size_t num_faces,
                                          const uint32_t* ptr_faces) -> void {
    // Resize the buffer to its new storage size if required
    if (m_Data.mesh_data.n_vertices != num_vertices) {
        m_Data.mesh_data.n_vertices = num_vertices;
        m_Data.mesh_data.vertices =
            std::make_unique<Scalar[]>(3 * num_vertices);  // NOLINT
    }
    if (m_Data.mesh_data.n_faces != num_faces) {
        m_Data.mesh_data.n_faces = num_faces;
        m_Data.mesh_data.faces =
            std::make_unique<uint32_t[]>(3 * num_faces);  // NOLINT
    }
    memcpy(m_Data.mesh_data.vertices.get(), ptr_vertices,
           sizeof(Scalar) * 3 * num_vertices);
    memcpy(m_Data.mesh_data.faces.get(), ptr_faces,
           sizeof(uint32_t) * 3 * num_faces);

    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeVertexData(num_vertices, ptr_vertices, num_faces,
                                        ptr_faces);
    }
}

auto SingleBodyCollider::ChangeElevationData(size_t n_width_samples,
                                             size_t n_depth_samples,
                                             const Scalar* ptr_heights)
    -> void {
    // Resize the buffer to its new storage size if required
    const auto N_GRID_SAMPLES = n_width_samples * n_depth_samples;
    if (m_Data.hfield_data.n_width_samples != n_width_samples ||
        m_Data.hfield_data.n_depth_samples != n_depth_samples) {
        m_Data.hfield_data.n_width_samples = n_width_samples;
        m_Data.hfield_data.n_depth_samples = n_depth_samples;
        // NOLINTNEXTLINE
        m_Data.hfield_data.heights = std::make_unique<Scalar[]>(N_GRID_SAMPLES);
    }
    memcpy(m_Data.hfield_data.heights.get(), ptr_heights,
           sizeof(Scalar) * N_GRID_SAMPLES);

    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeElevationData(n_width_samples, n_depth_samples,
                                           ptr_heights);
    }
}

auto SingleBodyCollider::ChangeCollisionGroup(int32_t col_group) -> void {
    m_Data.collision_group = col_group;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeCollisionGroup(col_group);
    }
}

auto SingleBodyCollider::ChangeCollisionMask(int32_t col_mask) -> void {
    m_Data.collision_mask = col_mask;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeCollisionMask(col_mask);
    }
}

}  // namespace core
}  // namespace loco

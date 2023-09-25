#pragma once

#include <utility>

#include <loco/core/common.hpp>
#include <loco/core/single_body/impl/single_body_collider_impl.hpp>

namespace loco {
namespace core {

class SingleBodyCollider {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SingleBodyCollider)

    DEFINE_SMART_POINTERS(SingleBodyCollider)

 public:
    /// \brief Creates a single body collider using the given configuration
    ///
    /// \param[in] data Collider data to be used to create this collider
    explicit SingleBodyCollider(::loco::ColliderData data)
        : m_Data(std::move(data)) {}

    /// \brief Releases all allocated resources for this collider
    ~SingleBodyCollider() = default;

    /// \brief Changes the size of the collider
    ///
    /// \param[in] size The new size of the collider
    auto ChangeSize(const Vec3& size) -> void;

    /// \brief Changes the vertex data of the mesh collider (if applicable)
    ///
    /// \param[in] num_vertices The number of vertices the given mesh has
    /// \param[in] ptr_vertices The buffer where the vertex data is stored
    /// \param[in] num_faces The number of faces the given mesh has
    /// \param[in] ptr_faces The buffer where the faces data is stored
    auto ChangeVertexData(size_t num_vertices, const Scalar* ptr_vertices,
                          size_t num_faces, const uint32_t* ptr_faces) -> void;

    /// \brief Changes the elevation data of the hfield collider (if applicable)
    ///
    /// \param[in] n_width_samples The number of samples in the first dimension
    /// \param[in] n_depth_samples The number of samples in the second dimension
    /// \param[in] ptr_heights The buffer where the new elevation data is stored
    auto ChangeElevationData(size_t n_width_samples, size_t n_depth_samples,
                             const Scalar* ptr_heights) -> void;

    /// \brief Changes the collision group of this collider
    ///
    /// \param[in] col_group The collision group given by the user
    auto ChangeCollisionGroup(int32_t col_group) -> void;

    /// \brief Changes the collision mask of this collider
    ///
    /// \param[in] col_mask The collision mask given by the user
    auto ChangeCollisionMask(int32_t col_mask) -> void;

    /// \brief Returns the shape of the collider
    auto shape() const -> eShapeType { return m_Data.type; }

    /// \brief Returns the size of the collider
    auto size() const -> Vec3 { return m_Data.size; }

    /// \brief Returns the collision group of the collider
    auto collision_group() const -> int32_t { return m_Data.collision_group; }

    /// \brief Returns the collision mask of the collider
    auto collision_mask() const -> int32_t { return m_Data.collision_mask; }

    /// \brief Returns an mutable reference to the config data of the collider
    auto data() const -> const ColliderData& { return m_Data; }

 protected:
    /// The configuration data for this collider
    ::loco::ColliderData m_Data;

    /// The backend type used for simulating this single body collider
    eBackendType m_BackendType = eBackendType::NONE;
    /// The adapter used to interact with the internal physics backend
    ISingleBodyColliderImpl::uptr m_BackendImpl = nullptr;
};

}  // namespace core
}  // namespace loco

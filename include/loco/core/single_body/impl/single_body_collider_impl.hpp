#pragma once

#include <memory>
#include <utility>

#include <loco/core/common.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace loco {
namespace core {

/// \brief Interface for collider adapters to link to internal phys. backend
class ISingleBodyColliderImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(ISingleBodyColliderImpl)

    DEFINE_SMART_POINTERS(ISingleBodyColliderImpl)

 public:
    ISingleBodyColliderImpl() = default;

    /// Releases/Frees all allocated resources for this adapter
    virtual ~ISingleBodyColliderImpl() = default;

    /// \brief Updates the size of the collider in simulation
    ///
    /// \param[in] size The new size of the collider in simulation
    virtual auto ChangeSize(const Vec3& size) -> void = 0;

    /// \brief Updates the vertex data of the mesh collider in simulation
    ///
    /// \param[in] num_vertices The number of vertices the given mesh has
    /// \param[in] ptr_vertices The buffer where the vertex data is stored
    /// \param[in] num_faces The number of faces the given mesh has
    /// \param[in] ptr_faces The buffer where the faces data is stored
    virtual auto ChangeVertexData(size_t num_vertices,
                                  const Scalar* ptr_vertices, size_t num_faces,
                                  const uint32_t* ptr_faces) -> void = 0;

    /// \brief Updates the elevation data of the hfield collider in simulation
    ///
    /// \param[in] n_width_samples The number of samples in the first dimension
    /// \param[in] n_depth_samples The number of samples in the second dimension
    /// \param[in] ptr_heights The buffer where the new elevation data is stored
    virtual auto ChangeElevationData(size_t n_width_samples,
                                     size_t n_depth_samples,
                                     const Scalar* ptr_heights) -> void = 0;

    /// \brief Updates the collision group of this collider in simulation
    ///
    /// \param[in] col_group The collision group given by the user
    virtual auto ChangeCollisionGroup(int32_t col_group) -> void = 0;

    /// \brief Updates the collision mask of this collider in simulation
    ///
    /// \param[in] col_mask The collision mask given by the user
    virtual auto ChangeCollisionMask(int32_t collision_mask) -> void = 0;
};

/// \brief Represents a dummy adapter that links to no backend
class SingleBodyColliderImplNone : public ISingleBodyColliderImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(SingleBodyColliderImplNone)

    DEFINE_SMART_POINTERS(SingleBodyColliderImplNone)

 public:
    SingleBodyColliderImplNone() = default;

    // Documentation inherited
    ~SingleBodyColliderImplNone() override = default;

    // Documentation inherited
    auto ChangeSize(const Vec3& size) -> void override {}

    // Documentation inherited
    auto ChangeVertexData(size_t num_vertices, const Scalar* ptr_vertices,
                          size_t num_faces, const uint32_t* ptr_faces)
        -> void override {}

    // Documentation inherited
    auto ChangeElevationData(size_t n_width_samples, size_t n_depth_samples,
                             const Scalar* ptr_heights) -> void override {}

    // Documentation inherited
    auto ChangeCollisionGroup(int32_t col_group) -> void override {}

    // Documentation inherited
    auto ChangeCollisionMask(int32_t collision_mask) -> void override {}
};

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

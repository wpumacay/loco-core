#pragma once

#include <string>

#include <loco/core/common.hpp>

namespace loco {
namespace core {

/// Interface for drawable adapters to link to internal vis. backend
class IDrawableImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(IDrawableImpl)

    DEFINE_SMART_POINTERS(IDrawableImpl)

 public:
    IDrawableImpl() = default;

    /// Releaes all allocated resources for this adapter
    virtual ~IDrawableImpl() = default;

    /// Sets the pose in world-space of the associated drawable
    /// \param[in] pose The desired pose in the world frame
    virtual auto SetPose(const Pose& pose) -> void = 0;

    /// Sets the color of the associated drawable
    /// \param[in] color The desired color of the associated drawable
    virtual auto SetColor(const Vec3& color) -> void = 0;

    /// Sets a texture with given id to the associated drawable
    /// \param[in] tex_filepath The path to the texture for this drawable
    virtual auto SetTexture(const std::string& tex_filepath) -> void = 0;

    /// Updates the size of the associated drawable
    /// \param[in] size The desired new size of the drawable
    virtual auto ChangeSize(const Vec3& size) -> void = 0;

    /// Updates the vertex data of the associated mesh drawable
    /// \param[in] num_vertices The number of vertices the given mesh has
    /// \param[in] ptr_vertices The buffer where the vertex data is stored
    /// \param[in] num_faces The number of faces the given mesh has
    /// \param[in] ptr_faces The buffer where the faces data is stored
    virtual auto ChangeVertexData(size_t num_vertices,
                                  const Scalar* ptr_vertices, size_t num_faces,
                                  const uint32_t* ptr_faces) -> void = 0;

    /// Updates the elevation data of the associated heightfield drawable
    /// \param[in] n_width_samples The number of samples in the first dimension
    /// \param[in] n_depth_samples The number of samples in the second dimension
    /// \param[in] ptr_heights The buffer where the new elevation data is stored
    virtual auto ChangeElevationData(size_t n_width_samples,
                                     size_t n_depth_samples,
                                     const Scalar* ptr_heights) -> void = 0;

    /// Updates the visibility of the associated drawable
    /// \param[in] visible Whether or not the drawable is visible
    virtual auto SetVisible(bool visible) -> void = 0;

    /// Updates the wireframe mode of the associated drawable
    /// \param[in] wireframe Whether or not the drawable is shown as a wireframe
    virtual auto SetWireframe(bool wireframe) -> void = 0;
};

/// Represents a dummy adapter that links to no vis. backend
class DrawableImplNone : public IDrawableImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(DrawableImplNone)

    DEFINE_SMART_POINTERS(DrawableImplNone)

 public:
    DrawableImplNone() = default;

    ~DrawableImplNone() override = default;

    auto SetPose(const Pose& pose) -> void override;

    auto SetColor(const Vec3& color) -> void override;

    auto SetTexture(const std::string& tex_filepath) -> void override;

    auto ChangeSize(const Vec3& size) -> void override;

    auto ChangeVertexData(size_t num_vertices, const Scalar* ptr_vertices,
                          size_t num_faces,
                          const uint32_t* ptr_faces) -> void override;

    auto ChangeElevationData(size_t n_width_samples, size_t n_depth_samples,
                             const Scalar* ptr_heights) -> void override;

    auto SetVisible(bool visible) -> void override;

    auto SetWireframe(bool wireframe) -> void override;
};

}  // namespace core
}  // namespace loco

#pragma once

#include <memory>
#include <string>
#include <utility>

#include <loco/core/common.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace loco {
namespace core {

/// \brief Interface for drawable adapters to link to internal vis. backend
class IDrawableImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(IDrawableImpl)

    DEFINE_SMART_POINTERS(IDrawableImpl)

 public:
    IDrawableImpl() = default;

    /// \brief Releaes all allocated resources for this adapter
    virtual ~IDrawableImpl() = default;

    /// \brief Sets the pose in world-space of the associated drawable
    ///
    /// \param[in] pose The desired pose in the world frame
    virtual auto SetPose(const Pose& pose) -> void = 0;

    /// \brief Sets the color of the associated drawable
    ///
    /// This method allows the user to change the color of the associated
    /// drawble, by requesting a color change to the internal vis. backend.
    /// The material used for the drawable might vary between vis. backend
    /// implementations. However, the color should set most fields associated
    /// with the colorness of the body (might have ambient, diffuse, specular
    /// components if using a Phong-like material, or other components if using
    /// PBR). For a more detailed update of the material properties, an
    /// implementation for interacting with the internal material system is
    /// required.
    ///
    /// \param[in] color The desired color of the associated drawable
    virtual auto SetColor(const Vec3& color) -> void = 0;

    /// \brief Sets a texture with given id to the associated drawable
    ///
    /// Similar to SetColor, this method should try to do the best approximation
    /// possible depending on the internal material properties and material
    /// system. For example, in the case of a Phong-like material, the texture
    /// we would set is the albedo map. For a more basic type of material, the
    /// implementation should look to set a texture that gives similar results
    /// to setting an albedo map or diffuse map.
    ///
    /// \param[in] tex_filepath The path to the texture to be loaded for the
    /// drawable
    virtual auto SetTexture(const std::string& tex_filepath) -> void = 0;

    /// \brief Updates the size of the associated drawable
    ///
    /// \param[in] size The desired new size of the drawable, set according to
    /// the shape type of the associated drawable
    virtual auto ChangeSize(const Vec3& size) -> void = 0;

    /// \brief Updates the vertex data of the associated mesh drawable
    ///
    /// \param[in] num_vertices The number of vertices the given mesh has
    /// \param[in] ptr_vertices The buffer where the vertex data is stored
    /// \param[in] num_faces The number of faces the given mesh has
    /// \param[in] ptr_faces The buffer where the faces data is stored
    virtual auto ChangeVertexData(size_t num_vertices,
                                  const Scalar* ptr_vertices, size_t num_faces,
                                  const uint32_t* ptr_faces) -> void = 0;

    /// \brief Updates the elevation data of the associated heightfield drawable
    ///
    /// \param[in] n_width_samples The number of samples in the first dimension
    /// \param[in] n_depth_samples The number of samples in the second dimension
    /// \param[in] ptr_heights The buffer where the new elevation data is stored
    virtual auto ChangeElevationData(size_t n_width_samples,
                                     size_t n_depth_samples,
                                     const Scalar* ptr_heights) -> void = 0;

    /// \brief Updates the visibility of the associated drawable
    ///
    /// \param[in] visible Whether or not the associated drawable should be
    /// visible
    virtual auto SetVisible(bool visible) -> void = 0;

    /// \brief Updates the wireframe mode of the associated drawable
    ///
    /// \param[in] wireframe Whether or not the associated drawable should be
    /// rendered as a wireframe
    virtual auto SetWireframe(bool wireframe) -> void = 0;
};

/// \brief Represents a dummy adapter that links to no vis. backend
class DrawableImplNone : public IDrawableImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(DrawableImplNone)

    DEFINE_SMART_POINTERS(DrawableImplNone)

 public:
    DrawableImplNone() = default;

    // Documentation inherited
    ~DrawableImplNone() override = default;

    // Documentation inherited
    auto SetPose(const Pose& pose) -> void override {}

    // Documentation inherited
    auto SetColor(const Vec3& color) -> void override {}

    // Documentation inherited
    auto SetTexture(const std::string& tex_filepath) -> void override {}

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
    auto SetVisible(bool visible) -> void override {}

    // Documentation inherited
    auto SetWireframe(bool wireframe) -> void override {}
};

}  // namespace core
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop  // NOLINT
#endif

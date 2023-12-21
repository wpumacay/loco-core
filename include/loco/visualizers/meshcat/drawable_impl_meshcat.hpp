#pragma once

#include <memory>
#include <string>

#include <loco/core/visualizer/impl/drawable_impl.hpp>

#include <loco/visualizers/meshcat/common_meshcat.hpp>

namespace loco {
namespace meshcat {

class DrawableImplMeshcat : public ::loco::core::IDrawableImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(DrawableImplMeshcat)

    DEFINE_SMART_POINTERS(DrawableImplMeshcat)

 public:
    /// \brief Creates a drawable adapter with the given configuration
    ///
    /// \param[in] name The name of the associated drawable
    /// \param[in] data The visual data associated with the drawable
    /// \param[in] handle The handle to the MeshcatCpp interface
    explicit DrawableImplMeshcat(std::string name, ::loco::DrawableData data,
                                 std::shared_ptr<MeshcatCpp::Meshcat> handle);

    ~DrawableImplMeshcat() override = default;

    /// \brief Sets the internal pose in world space to the given value
    ///
    /// \param[in] pose The desired pose in world space
    auto SetPose(const Pose& pose) -> void override;

    /// \brief Sets the internal color attributes to the given vaslue
    ///
    /// \param[in] color The desired color of the associated drawable
    auto SetColor(const Vec3& color) -> void override;

    /// \brief Sets a texture with given id/path to the associated drawable
    ///
    /// \param[in] tex_filepath The path to the texture to be loaded for the
    /// drawable
    auto SetTexture(const std::string& tex_filepath) -> void override;

    /// \brief Updates the size of the associated drawable
    ///
    /// \param[in] size The desired new sise of the drawable, set according to
    /// the shape type of the drawable
    auto ChangeSize(const Vec3& size) -> void override;

    /// \brief Updates the vertex data of the associated mesh drawable
    ///
    /// \param[in] num_vertices The number of vertices the given mesh has
    /// \param[in] ptr_vertices The buffer where the vertex data is stored
    /// \param[in] num_faces The number of faces the given mesh has
    /// \param[in] ptr_faces The buffer where the faces data is stored
    auto ChangeVertexData(size_t num_vertices, const Scalar* ptr_vertices,
                          size_t num_faces, const uint32_t* ptr_faces)
        -> void override;

    /// \brief Updates the elevation data of the associated heightfield drawable
    ///
    /// \param[in] n_width_samples The number of samples in the first dimension
    /// \param[in] n_depth_samples The number of samples in the second dimension
    /// \param[in] ptr_heights The buffer where the new elevation data is stored
    auto ChangeElevationData(size_t n_width_samples, size_t n_depth_samples,
                             const Scalar* ptr_heights) -> void override;

    /// \brief Updates the visibility of the associated drawable
    ///
    /// \param[in] visible Whether or not the associated drawable should be
    /// visible
    auto SetVisible(bool visible) -> void override;

    /// \brief Updates the wireframe mode of the associated drawable
    ///
    /// \param[in] wireframe Whether or not the associated drawable should be
    /// rendered as a wireframe
    auto SetWireframe(bool wireframe) -> void override;

 private:
    /// A copy of the data for the linked drawable
    ::loco::DrawableData m_Data;

    /// The associated name of the drawable we're adapting
    std::string m_Name;

    /// The handle to the MeshcatCpp interface
    std::shared_ptr<MeshcatCpp::Meshcat> m_Handle = nullptr;
};

}  // namespace meshcat
}  // namespace loco

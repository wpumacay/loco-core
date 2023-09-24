#pragma once

#include <string>
#include <utility>

#include <loco/core/common.hpp>
#include <loco/core/visualizer/impl/drawable_impl.hpp>

namespace loco {
namespace core {

class Drawable {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Drawable)

    DEFINE_SMART_POINTERS(Drawable)

 public:
    /// \brief Creates a drawable using the given user configuration
    explicit Drawable(::loco::DrawableData data, const Pose& p_pose)
        : m_Data(std::move(data)), m_Pose(p_pose) {}

    /// \brief Creates a drawable with given config at given world position
    explicit Drawable(::loco::DrawableData data, const Vec3& p_position)
        : m_Data(std::move(data)) {
        m_Pose.position = p_position;
    }

    /// \brief Releases all allocated resources for this drawable
    ~Drawable() = default;

    /// \brief Initializes the drawable with the given visualizer type
    auto Initialize(eVisualizerType vis_type) -> void;

    /// \brief Sets whether or not the drawable should be visible
    ///
    /// \param[in] visible Whether or not the drawable should be visible
    auto SetVisible(bool visible) -> void;

    /// \brief Sets whether or not the drawable should be rendered as wireframe
    ///
    /// \param[in] wireframe Whether or not to render the drawable as wireframe
    auto SetWireframe(bool wireframe) -> void;

    /// \brief Sets the position of this drawable in world space
    ///
    /// \param[in] pos The desired position of the body in world space
    auto SetPosition(const Vec3& pos) -> void;

    /// \brief Sets the orientation of this drawable in world space
    ///
    /// \param[in] quat The desired orientation of the body in world space
    auto SetOrientation(const Quat& quat) -> void;

    /// \brief Sets the pose of this drawable in workd space
    ///
    /// \param[in] pose The desired pose of the body in world space
    auto SetPose(const Pose& pose) -> void;

    /// \brief Sets the color of this drawable
    ///
    /// \param[in] color The desired color of this drawable
    auto SetColor(const Vec3& color) -> void;

    /// \brief Sets the texture applied to this drawable
    ///
    /// \param[in] tex_filepath The path to the texture that we want to use
    auto SetTexture(const std::string& tex_filepath) -> void;

    /// \brief Updates the size of this drawable according to its shape type
    ///
    /// \param[in] size The desired size for the shape of this drawable
    auto ChangeSize(const Vec3& size) -> void;

    /// \brief Updates the vertex data of the associated mesh shape
    ///
    /// \param[in] num_vertices The number of vertices the given mesh has
    /// \param[in] ptr_vertices The buffer where the vertex data is stored
    /// \param[in] num_faces The number of faces the given mesh has
    /// \param[in] ptr_faces The buffer where the faces data is stored
    auto ChangeVertexData(size_t num_vertices, const Scalar* ptr_vertices,
                          size_t num_faces, const uint32_t* ptr_faces) -> void;

    /// \brief Updates the elevation data of the associated heightfield shape
    ///
    /// \param[in] n_width_samples The number of samples in the first dimension
    /// \param[in] n_depth_samples The number of samples in the second dimension
    /// \param[in] ptr_heights The buffer where the new elevation data is stored
    auto ChangeElevationData(size_t n_width_samples, size_t n_depth_samples,
                             const Scalar* ptr_heights) -> void;

 protected:
    /// The configuration data for this drawable
    ::loco::DrawableData m_Data;

    /// The current pose of this drawable in world space
    Pose m_Pose;
    /// Flag to store whether or not this drawable is visible
    bool m_Visible = true;
    /// Flag to store whether or not this drawable is rendered as a wireframe
    bool m_Wireframe = false;

    /// The backend type used for the visualizer this drawable belongs to
    eVisualizerType m_VisualizerType = eVisualizerType::NONE;
    /// The adapter used to interact with the internal visualizer backend
    IDrawableImpl::uptr m_BackendImpl = nullptr;
};

}  // namespace core
}  // namespace loco

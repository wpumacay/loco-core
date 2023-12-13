#pragma once

// TODO(wilbert): The current implementation is intended for a moderate number
// of drawables in the screen, as it creates a separate insteance for each that
// does its own allocation in the heap. This can be mitigated by using custom
// allocators, and by relaxing the constraint on both copy and move.

#include <string>

#include <loco/core/visualizer/drawable_t.hpp>

namespace loco {
namespace core {
namespace viz {

/// \brief Class that represents a drawable with a box shape
///
/// This box class is a helper class that is exposed to make building of box
/// drawables easier. It exposes an API for a box shape, as well as the default
/// API from the Drawable base class.
class Box : public Drawable {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Box)

    DEFINE_SMART_POINTERS(Box)

 public:
    /// \brief Creates a box given its name, pose and size
    ///
    /// \param[in] p_name The name of the box drawable
    /// \param[in] p_pose The pose of the box drawable in world space
    /// \param[in] p_extents The size of the box in each dimension (XYZ)
    explicit Box(std::string p_name, Pose p_pose, Vec3 p_extents);

    /// \brief Creates a box given its name, position and size
    ///
    /// \param[in] p_name The name of the box drawable
    /// \param[in] p_position The position of the box drawable in world space
    /// \param[in] p_extents The size of the box in each dimension (XYZ)
    explicit Box(std::string p_name, Vec3 p_position, Vec3 p_extents);

    /// \brief Releases all allocated resources for this box drawable
    ~Box() = default;

    /// \brief Sets the box size to the given extents by the user
    ///
    /// \param[in] extents The size of the box in each dimension (XYZ)
    auto SetExtents(Vec3 extents) -> void;

    /// \brief Sets the box width to the given value
    ///
    /// \param[in] width The width of the box (x-dimension)
    auto SetWidth(Scalar width) -> void;

    /// \brief Sets the box depth to the given value
    ///
    /// \param[in] depth The depth of the box (y-dimension)
    auto SetDepth(Scalar depth) -> void;

    /// \brief Sets the box height to the given value
    ///
    /// \param[in] height The height of the box (z-dimension)
    auto SetHeight(Scalar height) -> void;

    /// \brief Returns the size of the box
    auto extents() const -> const Vec3& { return m_Extents; }

    /// \brief Returns the width of the box
    auto width() const -> Scalar { return m_Extents.x(); }

    /// \brief Returns the depth of the box
    auto depth() const -> Scalar { return m_Extents.y(); }

    /// \brief Returns the height of the box
    auto height() const -> Scalar { return m_Extents.z(); }

 protected:
    /// \brief The size of the box in each dimension (XYZ)
    Vec3 m_Extents = {ToScalar(1.0), ToScalar(1.0), ToScalar(1.0)};
};

/// \brief Class that represents a drawable with a plane shape
///
/// This plane class is a helper class that is exposed to make building of plane
/// drawables easier. It exposes an API for a plane shape, as well as the
/// default API from the Drawable base class.
class Plane : public Drawable {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Plane)

    DEFINE_SMART_POINTERS(Plane)

 public:
    /// \brief Creates a plane given its name, pose and size
    ///
    /// \param[in] p_name The name of the plane drawable
    /// \param[in] p_pose The pose of the plane drawable in world space
    /// \param[in] p_extents The size of the plane in each dimension (XY)
    explicit Plane(std::string p_name, Pose p_pose, Vec2 p_extents);

    /// \brief Creates a plane given its name, position and size
    ///
    /// \param[in] p_name The name of the plane drawable
    /// \param[in] p_position The position of the plane drawable in world space
    /// \param[in] p_extents The size of the plane in each dimension (XY)
    explicit Plane(std::string p_name, Vec3 p_position, Vec2 p_extents);

    /// \brief Releases all allocated resources for this plane drawable
    ~Plane() = default;

    /// \brief Sets the plane size to the given extents by the user
    ///
    /// \param[in] extents The size of the plane in each dimension (XY)
    auto SetExtents(Vec2 extents) -> void;

    /// \brief Sets the plane width to the given value
    ///
    /// \param[in] width The width of the plane (x-dimension)
    auto SetWidth(Scalar width) -> void;

    /// \brief Sets the plane height to the given value
    ///
    /// \param[in] height The depth of the plane (y-dimension)
    auto SetDepth(Scalar depth) -> void;

    /// \brief Returns the size of the plane
    auto extents() const -> const Vec2& { return m_Extents; }

    /// \brief Returns the width of the plane
    auto width() const -> Scalar { return m_Extents.x(); }

    /// \brief Returns the height of the plane
    auto depth() const -> Scalar { return m_Extents.y(); }

 protected:
    /// \brief The size of the plane in each dimension (XY)
    Vec2 m_Extents = {ToScalar(1.0), ToScalar(1.0)};
};

class Sphere : public Drawable {};

class Cylinder : public Drawable {};

class Capsule : public Drawable {};

class Ellipsoid : public Drawable {};

class ConvexMesh : public Drawable {};

class Heightfield : public Drawable {};

}  // namespace viz
}  // namespace core
}  // namespace loco

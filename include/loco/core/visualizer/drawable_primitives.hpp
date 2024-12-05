#pragma once

// TODO(wilbert): The current implementation is intended for a moderate number
// of drawables in the screen, as it creates a separate instance for each that
// does its own allocation in the heap. This can be mitigated by using custom
// allocators, and by relaxing the constraint on both copy and move.

#include <string>

#include <loco/core/visualizer/drawable_t.hpp>
#include "utils/common.hpp"

namespace loco {
namespace core {
namespace viz {

/// \brief Class that represents a drawable with a box shape
///
/// This box class is a helper class that is exposed to make building of box
/// drawables easier. It exposes an API for a box shape, as well as the default
/// API from the Drawable base class.
class LOCO_API Box : public Drawable {
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
    LOCO_NODISCARD auto extents() const -> const Vec3& { return m_Extents; }

    /// \brief Returns the width of the box
    LOCO_NODISCARD auto width() const -> Scalar { return m_Extents.x(); }

    /// \brief Returns the depth of the box
    LOCO_NODISCARD auto depth() const -> Scalar { return m_Extents.y(); }

    /// \brief Returns the height of the box
    LOCO_NODISCARD auto height() const -> Scalar { return m_Extents.z(); }

 protected:
    /// \brief The size of the box in each dimension (XYZ)
    Vec3 m_Extents = {ToScalar(1.0), ToScalar(1.0), ToScalar(1.0)};
};

/// \brief Class that represents a drawable with a plane shape
///
/// This plane class is a helper class that is exposed to make building of plane
/// drawables easier. It exposes an API for a plane shape, as well as the
/// default API from the Drawable base class.
class LOCO_API Plane : public Drawable {
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
    LOCO_NODISCARD auto extents() const -> const Vec2& { return m_Extents; }

    /// \brief Returns the width of the plane
    LOCO_NODISCARD auto width() const -> Scalar { return m_Extents.x(); }

    /// \brief Returns the height of the plane
    LOCO_NODISCARD auto depth() const -> Scalar { return m_Extents.y(); }

 protected:
    /// \brief The size of the plane in each dimension (XY)
    Vec2 m_Extents = {ToScalar(1.0), ToScalar(1.0)};
};

/// \brief Class that represents a drawable with a sphere shape
///
/// This sphere class is a helper class that is exposed to make building of
/// sphere drawables easier. It exposes an API for a sphere shape, as well as
/// the default API from the Drawable base class.
class LOCO_API Sphere : public Drawable {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Sphere)

    DEFINE_SMART_POINTERS(Sphere)

 public:
    /// \brief Creates a sphere given its name, pose and radius
    explicit Sphere(std::string p_name, Pose p_pose, Scalar p_radius);

    /// \brief Creates a sphere given its name, radius and position
    explicit Sphere(std::string p_name, Vec3 p_position, Scalar p_radius);

    /// \biref Releases all allocated resources for this sphere drawable
    ~Sphere() = default;

    /// \brief Sets the radius of the sphere
    auto SetRadius(Scalar p_radius) -> void;

    /// \brief Returns the radius of the sphere
    LOCO_NODISCARD auto radius() const -> Scalar { return m_Radius; }

 protected:
    /// \brief The radius of the sphere
    Scalar m_Radius;
};

/// \brief Class that represents a drawable with a cylinder shape
class LOCO_API Cylinder : public Drawable {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Cylinder)

    DEFINE_SMART_POINTERS(Cylinder)

 public:
    /// \brief Creates a cylinder given its name, pose, radius and height
    explicit Cylinder(std::string p_name, Pose p_pose, Scalar p_radius,
                      Scalar p_height);

    /// \brief Creates a cylinder given its name, position, radius and  height
    explicit Cylinder(std::string p_name, Vec3 p_position, Scalar p_radius,
                      Scalar p_height);

    /// \biref Releases all allocated resources for this cylinder drawable
    ~Cylinder() = default;

    /// \brief Sets the radius of the cylinder
    auto SetRadius(Scalar p_radius) -> void;

    /// \brief Sets the height of the cylinder
    auto SetHeight(Scalar p_height) -> void;

    /// \brief Returns the radius of the cylinder
    LOCO_NODISCARD auto radius() const -> Scalar { return m_Radius; }

    /// \brief Returns the height of the cylinder
    LOCO_NODISCARD auto height() const -> Scalar { return m_Height; }

 protected:
    /// \brief The radius of the cylinder
    Scalar m_Radius;

    /// \brief The height of the cylinder
    Scalar m_Height;
};

class LOCO_API Ellipsoid : public Drawable {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Ellipsoid)

    DEFINE_SMART_POINTERS(Ellipsoid)

 public:
    /// \brief Creates an ellipsoid given its name, pose and radii
    explicit Ellipsoid(std::string p_name, Pose p_pose, Vec3 p_radii);

    /// \brief Creates an ellipsoid given its name, position and radii
    explicit Ellipsoid(std::string p_name, Vec3 p_position, Vec3 p_radii);

    /// \brief Releases all allocated resources for this ellipsoid drawable
    ~Ellipsoid() = default;

    /// \brief Sets the radii of the ellipsoid
    auto SetRadii(Vec3 p_radii) -> void;

    /// \brief Sets the x-radius of the ellipsoid
    auto SetRadiusX(Scalar radius_x) -> void;

    /// \brief Sets the x-radius of the ellipsoid
    auto SetRadiusY(Scalar radius_y) -> void;

    /// \brief Sets the x-radius of the ellipsoid
    auto SetRadiusZ(Scalar radius_z) -> void;

    /// \brief Returns the radii of the ellipsoid
    LOCO_NODISCARD auto radii() const -> Vec3 { return m_Radii; }

 protected:
    /// \brief The radii of the ellipsoid
    Vec3 m_Radii;
};

class LOCO_API Capsule : public Drawable {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Capsule)

    DEFINE_SMART_POINTERS(Capsule)

 public:
    /// \brief Creates a capsule given its name, pose, radius and height
    explicit Capsule(std::string name, Pose pose, Scalar radius, Scalar height);

    /// \brief Creates a capsule given its name, position, radius and height
    explicit Capsule(std::string name, Vec3 xyz, Scalar radius, Scalar height);

    /// \brief Releases all allocated resources for this capsule drawable
    ~Capsule() = default;

    /// \brief Sets the radius of the capsule
    auto SetRadius(Scalar radius) -> void;

    /// \brief Sets the height of the capsule
    auto SetHeight(Scalar height) -> void;

    /// \brief Returns the radius of the capsule
    LOCO_NODISCARD auto radius() const -> Scalar { return m_Radius; }

    /// \brief Returns the height of the capsule
    LOCO_NODISCARD auto height() const -> Scalar { return m_Height; }

 protected:
    /// \brief The radius of the caps of the capsule
    Scalar m_Radius;

    /// \brief The height of the capsule (length of the cylindrical part)
    Scalar m_Height;
};

class LOCO_API ConvexMesh : public Drawable {};

class LOCO_API Heightfield : public Drawable {};

}  // namespace viz
}  // namespace core
}  // namespace loco

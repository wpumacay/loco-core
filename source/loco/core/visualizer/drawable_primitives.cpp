#include <loco/core/visualizer/drawable_primitives.hpp>
#include <string>
#include "loco/core/common.hpp"

namespace loco {
namespace core {
namespace viz {

// -------------------------------------------------------------------------- //
//                               Box primitive                                //
// -------------------------------------------------------------------------- //

Box::Box(std::string p_name, Pose p_pose, Vec3 p_extents)
    : Drawable(std::move(p_name), p_pose), m_Extents(p_extents) {
    m_Data.type = eShapeType::BOX;
    m_Data.size = m_Extents;
    m_Data.color = ::loco::DEFAULT_COLOR;
}

Box::Box(std::string p_name, Vec3 p_position, Vec3 p_extents)
    : Drawable(std::move(p_name), p_position), m_Extents(p_extents) {
    m_Data.type = eShapeType::BOX;
    m_Data.size = m_Extents;
    m_Data.color = ::loco::DEFAULT_COLOR;
}

auto Box::SetExtents(Vec3 extents) -> void {
    m_Extents = extents;
    m_Data.size = m_Extents;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(extents);
    }
}

auto Box::SetWidth(Scalar width) -> void {
    m_Extents.x() = width;
    m_Data.size = m_Extents;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

auto Box::SetDepth(Scalar depth) -> void {
    m_Extents.y() = depth;
    m_Data.size = m_Extents;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

auto Box::SetHeight(Scalar height) -> void {
    m_Extents.z() = height;
    m_Data.size = m_Extents;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

// -------------------------------------------------------------------------- //
//                              Plane primitive                               //
// -------------------------------------------------------------------------- //

Plane::Plane(std::string p_name, Pose p_pose, Vec2 p_extents)
    : Drawable(std::move(p_name), p_pose), m_Extents(p_extents) {
    m_Data.type = eShapeType::PLANE;
    m_Data.size = {m_Extents.x(), m_Extents.y(), ToScalar(0.0)};
    m_Data.color = ::loco::DEFAULT_COLOR;
}

Plane::Plane(std::string p_name, Vec3 p_position, Vec2 p_extents)
    : Drawable(std::move(p_name), p_position), m_Extents(p_extents) {
    m_Data.type = eShapeType::PLANE;
    m_Data.size = {m_Extents.x(), m_Extents.y(), ToScalar(0.0)};
    m_Data.color = ::loco::DEFAULT_COLOR;
}

auto Plane::SetExtents(Vec2 extents) -> void {
    m_Extents = extents;
    m_Data.size.x() = m_Extents.x();
    m_Data.size.y() = m_Extents.y();
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

auto Plane::SetWidth(Scalar width) -> void {
    m_Extents.x() = width;
    m_Data.size.x() = width;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

auto Plane::SetDepth(Scalar depth) -> void {
    m_Extents.y() = depth;
    m_Data.size.y() = depth;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

// -------------------------------------------------------------------------- //
//                             Sphere primitive                               //
// -------------------------------------------------------------------------- //

Sphere::Sphere(std::string p_name, Pose p_pose, Scalar p_radius)
    : Drawable(std::move(p_name), p_pose), m_Radius(p_radius) {
    m_Data.type = eShapeType::SPHERE;
    m_Data.size = {m_Radius, m_Radius, m_Radius};
    m_Data.color = ::loco::DEFAULT_COLOR;
}

Sphere::Sphere(std::string p_name, Vec3 p_position, Scalar p_radius)
    : Drawable(std::move(p_name), p_position), m_Radius(p_radius) {
    m_Data.type = eShapeType::SPHERE;
    m_Data.size = {m_Radius, m_Radius, m_Radius};
    m_Data.color = ::loco::DEFAULT_COLOR;
}

auto Sphere::SetRadius(Scalar radius) -> void {
    m_Radius = radius;
    m_Data.size = {m_Radius, m_Radius, m_Radius};
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

// -------------------------------------------------------------------------- //
//                             Cylinder primitive                             //
// -------------------------------------------------------------------------- //

Cylinder::Cylinder(std::string p_name, Pose p_pose, Scalar p_radius,
                   Scalar p_height)
    : Drawable(std::move(p_name), p_pose),
      m_Radius(p_radius),
      m_Height(p_height) {
    m_Data.type = eShapeType::CYLINDER;
    m_Data.size = {m_Radius, m_Radius, m_Height};
    m_Data.color = ::loco::DEFAULT_COLOR;
}

Cylinder::Cylinder(std::string p_name, Vec3 p_position, Scalar p_radius,
                   Scalar p_height)
    : Drawable(std::move(p_name), p_position),
      m_Radius(p_radius),
      m_Height(p_height) {
    m_Data.type = eShapeType::CYLINDER;
    m_Data.size = {m_Radius, m_Radius, m_Height};
    m_Data.color = ::loco::DEFAULT_COLOR;
}

auto Cylinder::SetRadius(Scalar radius) -> void {
    m_Radius = radius;
    m_Data.size = {m_Radius, m_Radius, m_Height};
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

auto Cylinder::SetHeight(Scalar height) -> void {
    m_Height = height;
    m_Data.size = {m_Radius, m_Radius, m_Height};
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

// -------------------------------------------------------------------------- //
//                             Ellipsoid primitive                            //
// -------------------------------------------------------------------------- //

Ellipsoid::Ellipsoid(std::string p_name, Pose p_pose, Vec3 p_radii)
    : Drawable(std::move(p_name), p_pose), m_Radii(p_radii) {
    m_Data.type = eShapeType::ELLIPSOID;
    m_Data.size = m_Radii;
    m_Data.color = ::loco::DEFAULT_COLOR;
}

Ellipsoid::Ellipsoid(std::string p_name, Vec3 p_position, Vec3 p_radii)
    : Drawable(std::move(p_name), p_position), m_Radii(p_radii) {
    m_Data.type = eShapeType::ELLIPSOID;
    m_Data.size = m_Radii;
    m_Data.color = ::loco::DEFAULT_COLOR;
}

auto Ellipsoid::SetRadii(Vec3 radii) -> void {
    m_Radii = radii;
    m_Data.size = m_Radii;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

auto Ellipsoid::SetRadiusX(Scalar radius_x) -> void {
    m_Radii.x() = radius_x;
    m_Data.size = m_Radii;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

auto Ellipsoid::SetRadiusY(Scalar radius_y) -> void {
    m_Radii.y() = radius_y;
    m_Data.size = m_Radii;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

auto Ellipsoid::SetRadiusZ(Scalar radius_z) -> void {
    m_Radii.z() = radius_z;
    m_Data.size = m_Radii;
    if (m_BackendImpl != nullptr) {
        m_BackendImpl->ChangeSize(m_Data.size);
    }
}

}  // namespace viz
}  // namespace core
}  // namespace loco

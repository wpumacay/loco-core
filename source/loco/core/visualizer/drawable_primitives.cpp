#include <loco/core/visualizer/drawable_primitives.hpp>

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

}  // namespace viz
}  // namespace core
}  // namespace loco

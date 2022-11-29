#include <loco/core/common.hpp>

namespace loco {

auto ToString(const eShapeType& shape_type) -> std::string {
    switch (shape_type) {
        case eShapeType::PLANE:
            return "plane";
        case eShapeType::BOX:
            return "box";
        case eShapeType::SPHERE:
            return "sphere";
        case eShapeType::CYLINDER:
            return "cylinder";
        case eShapeType::CAPSULE:
            return "capsule";
        case eShapeType::ELLIPSOID:
            return "ellipsoid";
        case eShapeType::CONVEX_MESH:
            return "convex_mesh";
        case eShapeType::TRIANGULAR_MESH:
            return "triangular_mesh";
        case eShapeType::HEIGHTFIELD:
            return "heightfield";
        case eShapeType::COMPOUND:
            return "compound";
    }
}

auto ToString(const eDynamicsType& dyn_type) -> std::string {
    switch (dyn_type) {
        case eDynamicsType::DYNAMIC:
            return "dynamic";
        case eDynamicsType::STATIC:
            return "static";
    }
}

}  // namespace loco

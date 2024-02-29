#include <loco/visualizers/meshcat/common_meshcat.hpp>

namespace loco {
namespace meshcat {

auto CreateShape(MeshcatCpp::Meshcat& handle, const std::string& name,
                 const ::loco::DrawableData& data) -> void {
    auto material = MeshcatCpp::Material::get_default_material();
    material.set_color(static_cast<uint8_t>(data.color.x() * 255),
                       static_cast<uint8_t>(data.color.y() * 255),
                       static_cast<uint8_t>(data.color.z() * 255));
    switch (data.type) {
        case ::loco::eShapeType::BOX: {
            handle.set_object(
                "/loco/" + name,
                MeshcatCpp::Box(static_cast<double>(data.size.x()),
                                static_cast<double>(data.size.y()),
                                static_cast<double>(data.size.z())),
                material);
            break;
        }

        case ::loco::eShapeType::PLANE: {
            handle.set_object(
                "/loco/" + name,
                MeshcatCpp::Box(static_cast<double>(data.size.x()),
                                static_cast<double>(data.size.y()), 0.01),
                material);
            break;
        }

        default: {
            LOCO_CORE_ERROR(
                "CreateShape >> Unsupported shape type for drawable: {}",
                ::loco::ToString(data.type));
            throw std::runtime_error("Unsupported shape type for drawable");
        }
    }
}

auto ConvertToMatrixView(std::array<double, 16>& tf_array)
    -> MeshcatCpp::MatrixView<double> {
    constexpr MeshcatCpp::MatrixView<double>::index_type ROWS = 4;
    constexpr MeshcatCpp::MatrixView<double>::index_type COLS = 4;
    constexpr auto ORDER = MeshcatCpp::MatrixStorageOrdering::ColumnMajor;

    return MeshcatCpp::make_matrix_view(tf_array.data(), ROWS, COLS, ORDER);
}

}  // namespace meshcat
}  // namespace loco

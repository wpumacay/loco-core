#pragma once

#include <array>
#include <stdexcept>
#include <string>

#include <MeshcatCpp/Material.h>
#include <MeshcatCpp/MatrixView.h>
#include <MeshcatCpp/Meshcat.h>
#include <MeshcatCpp/Shape.h>

#include <loco/core/common.hpp>

namespace loco {
namespace meshcat {

/// \brief Sends a request to meshcat to create the given shape
///
/// \param[in] handle The handle to the MeshcatCpp interface
/// \param[in] name The name of the associated drawable
/// \param[in] data The visual data associated with the drawable
auto CreateShape(MeshcatCpp::Meshcat& handle, const std::string& name,
                 const ::loco::DrawableData& data) -> void;

/// \brief Converts the given pose to a matrix view
///
/// \param[in] tf_array The 4x4 transform given a a list of doubles
/// \return The matrix view of the given transform
auto ConvertToMatrixView(std::array<double, 16>& tf_array)
    -> MeshcatCpp::MatrixView<double>;

}  // namespace meshcat
}  // namespace loco

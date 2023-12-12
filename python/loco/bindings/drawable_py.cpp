#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <conversions_py.hpp>

#include <utils/logging.hpp>

#include <loco/core/common.hpp>
#include <loco/core/visualizer/drawable_t.hpp>

namespace py = pybind11;

namespace loco {

// NOLINTNEXTLINE
auto bindings_drawable(py::module& m) -> void {
    {
        using Class = ::loco::core::Drawable;
        constexpr auto ClassName = "Drawable";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<std::string, Pose, ::loco::DrawableData>())
            .def_property("visible", &Class::visible, &Class::SetVisible)
            .def_property("wireframe", &Class::wireframe, &Class::SetWireframe)
            .def_property("position", &Class::position, &Class::SetPosition)
            .def_property("orientation", &Class::orientation,
                          &Class::SetOrientation)
            .def_property("pose", &Class::pose, &Class::SetPose)
            .def_property("color", &Class::color, &Class::SetColor)
            .def_property("texture", &Class::texture, &Class::SetTexture)
            .def_property("size", &Class::size, &Class::ChangeSize)
            .def("ChangeVertexData",
                 [](Class& self, const py::array_t<Scalar>& np_vertices,
                    const py::array_t<Scalar>& np_faces) -> void {
                     auto info_vertices = np_vertices.request();
                     auto info_faces = np_faces.request();

                     LOG_CORE_INFO("np_vertices.size = {}", info_vertices.size);
                     LOG_CORE_INFO("np_faces.size = {}", info_faces.size);
                 })
            .def(
                "ChangeElevationData",
                [](Class& self, const py::array_t<Scalar>& np_heights) -> void {
                    auto info_heights = np_heights.request();

                    LOG_CORE_INFO("np_heights.size = {}", info_heights.size);
                    LOG_CORE_INFO("np_heights.ndim = {}", info_heights.ndim);
                })
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }
}

}  // namespace loco

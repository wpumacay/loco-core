#include <pybind11/pybind11.h>

#include <conversions_py.hpp>

#include <loco/core/simulation_t.hpp>

namespace py = pybind11;

namespace loco {

// NOLINTNEXTLINE
auto bindings_simulation(py::module& m) -> void {
    {
        using Class = ::loco::core::Simulation;
        constexpr auto ClassName = "Simulation";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<::loco::core::Scenario::ptr, ::loco::eBackendType>())
            .def("Init", &Class::Init)
            .def("Reset", &Class::Reset)
            .def("Step", &Class::Step)
            .def("SetTimeStep", &Class::SetTimeStep)
            .def("SetGravity",
                 [](Class& self, const py::array_t<Scalar>& np_gravity) {
                     self.SetGravity(
                         ::math::nparray_to_vec3<Scalar>(np_gravity));
                 })
            .def_property_readonly("timestep", &Class::timestep)
            .def_property_readonly("gravity", &Class::gravity)
            .def_property_readonly("backend_type", &Class::backend_type)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<Simulation\n"
                           "  timestep={}\n"
                           "  gravity={}\n"
                           "  backend_type={}\n"
                           ">")
                    .format(self.timestep(), self.gravity().toString(),
                            ::loco::ToString(self.backend_type()));
            });
    }
}

}  // namespace loco

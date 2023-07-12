#include <pybind11/pybind11.h>

#include <conversions_py.hpp>

#include <loco/core/scenario_t.hpp>

namespace py = pybind11;

namespace loco {

// NOLINTNEXTLINE
auto bindings_scenario(py::module& m) -> void {
    {
        using Class = ::loco::core::Scenario;
        constexpr auto ClassName = "Scenario";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<>())
            .def_property_readonly("num_bodies", &Class::num_bodies)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str("Scenario(num_bodies={}, max_bodies={})")
                    .format(self.num_bodies(), Class::MAX_BODIES);
            });
    }
}

}  // namespace loco

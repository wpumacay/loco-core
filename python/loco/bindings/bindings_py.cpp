#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include <utils/logging.hpp>

namespace py = pybind11;

namespace loco {
extern auto bindings_common(py::module& m) -> void;  // NOLINT
extern auto bindings_body(py::module& m) -> void;    // NOLINT

//// extern auto bindings_scenario(py::module& m) -> void;    // NOLINT
//// extern auto bindings_simulation(py::module& m) -> void;  // NOLINT
//// extern auto bindings_collider(py::module& m) -> void;  // NOLINT
//// extern auto bindings_drawable(py::module& m) -> void;  // NOLINT
}  // namespace loco

// NOLINTNEXTLINE
PYBIND11_MODULE(loco_bindings, m) {
    try {
        py::module::import("math3d");
    } catch (py::error_already_set& e) {
        e.restore();
        LOG_CORE_ERROR(
            "Didn't find required module math3d. Won't be able to access "
            "fields that are math3d types, it will likely crash :(");
    }

    ::loco::bindings_common(m);
    ::loco::bindings_body(m);
    // ::loco::bindings_scenario(m);
    // ::loco::bindings_simulation(m);
    // ::loco::bindings_collider(m);
    // ::loco::bindings_drawable(m);
}

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace loco {
extern auto bindings_common(py::module& m) -> void;    // NOLINT
extern auto bindings_collider(py::module& m) -> void;  // NOLINT
extern auto bindings_drawable(py::module& m) -> void;  // NOLINT
extern auto bindings_body(py::module& m) -> void;      // NOLINT
}  // namespace loco

// NOLINTNEXTLINE
PYBIND11_MODULE(loco, m) {
    ::loco::bindings_common(m);
    // ::loco::bindings_collider(m);
    // ::loco::bindings_drawable(m);
}
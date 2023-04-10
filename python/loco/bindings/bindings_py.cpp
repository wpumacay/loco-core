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
    // Import bindings for our math types (avoids issues with undeclared funcs.)
    // TODO(wilbert): Currently we have to install math3d separately, even
    // though we build the math3d bindings as part of our build steps for loco.
    // Try to "autoinstall" the dependencies' bindings as required
    py::module::import("math3d");

    ::loco::bindings_common(m);
    // ::loco::bindings_collider(m);
    // ::loco::bindings_drawable(m);
}

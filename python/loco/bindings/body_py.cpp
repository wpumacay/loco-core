#include <pybind11/pybind11.h>

#include <conversions_py.hpp>

#include <loco/core/body_t.hpp>

namespace py = pybind11;

namespace loco {

// NOLINTNEXTLINE
auto bindings_body(py::module& m) -> void {
    {
        using Class = ::loco::core::Body;
        constexpr auto ClassName = "Body";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<::loco::BodyData>())
            .def_property(
                "pose", [](const Class& self) -> Pose { return self.pose(); },
                [](Class& self, const Pose& pose) -> void {
                    self.SetPose(pose);
                })
            .def_property(
                "position",
                [](const Class& self) -> py::array_t<Scalar> {
                    return ::math::vec3_to_nparray<Scalar>(self.position());
                },
                [](Class& self,
                   const py::array_t<Scalar>& np_position) -> void {
                    self.SetPosition(
                        ::math::nparray_to_vec3<Scalar>(np_position));
                })
            .def_property(
                "orientation",
                [](const Class& self) -> py::array_t<Scalar> {
                    auto np_quat = py::array_t<Scalar>(Quat::BUFFER_COUNT);
                    auto quat = self.orientation();
                    memcpy(np_quat.request().ptr, quat.data(),
                           Quat::BUFFER_SIZE);
                    return np_quat;
                },
                [](Class& self,
                   const py::array_t<Scalar>& np_orientation) -> void {
                    auto info = np_orientation.request();
                    if (info.size != Quat::BUFFER_COUNT) {
                        throw std::runtime_error(
                            "Body::orientation >>> received incompatible array "
                            "size, expected 4 elements for a quaternion");
                    }
                    Quat quat;
                    memcpy(quat.data(), info.ptr, Quat::BUFFER_SIZE);
                })
            .def_property(
                "linear_vel",
                [](const Class& self) -> py::array_t<Scalar> {
                    return ::math::vec3_to_nparray<Scalar>(self.linear_vel());
                },
                [](Class& self,
                   const py::array_t<Scalar>& np_linear_vel) -> void {
                    self.SetLinearVelocity(
                        ::math::nparray_to_vec3<Scalar>(np_linear_vel));
                })
            .def_property(
                "angular_vel",
                [](const Class& self) -> py::array_t<Scalar> {
                    return ::math::vec3_to_nparray<Scalar>(self.angular_vel());
                },
                [](Class& self,
                   const py::array_t<Scalar>& np_angular_vel) -> void {
                    self.SetAngularVelocity(
                        ::math::nparray_to_vec3<Scalar>(np_angular_vel));
                })
            .def_readwrite("pose0", &Class::pose0)
            .def_readwrite("linearVel0", &Class::linearVel0)
            .def_readwrite("angularVel0", &Class::angularVel0)
            .def_readwrite("totalForceCOM", &Class::totalForceCOM)
            .def_readwrite("totalTorque", &Class::totalTorque)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str("Body(pos={}, quat={}, v={}, w={})")
                    .format(self.position().toString(),
                            self.orientation().toString(),
                            self.linear_vel().toString(),
                            self.angular_vel().toString());
            });
    }
}

}  // namespace loco

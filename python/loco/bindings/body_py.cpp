#include <pybind11/pybind11.h>

#include <conversions_py.hpp>

#include <loco/core/single_body/single_body_t.hpp>

namespace py = pybind11;

namespace loco {

// NOLINTNEXTLINE
auto bindings_body(py::module& m) -> void {
    {
        using Class = ::loco::core::SingleBody;
        constexpr auto ClassName = "SingleBody";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init([](::loco::BodyData data, const Pose& p_pose) {
                return std::make_shared<Class>(data, p_pose);
            }))
            .def(py::init([](::loco::BodyData data, const Vec3& p_position,
                             const Quat& p_orientation) {
                return std::make_shared<Class>(data, p_position, p_orientation);
            }))
            .def(py::init([](::loco::BodyData data,
                             const py::array_t<Scalar>& np_position,
                             const py::array_t<Scalar>& np_orientation) {
                return std::make_shared<Class>(
                    data, ::math::nparray_to_vec3<Scalar>(np_position),
                    ::math::nparray_to_quat<Scalar>(np_orientation));
            }))
            .def(py::init([](::loco::BodyData data,
                             const py::array_t<Scalar>& np_position) {
                return std::make_shared<Class>(
                    data, ::math::nparray_to_vec3<Scalar>(np_position));
            }))
            .def("SetPose", &Class::SetPose)
            .def("SetPose",
                 [](Class& self, const py::array_t<Scalar>& np_pose) {
                     self.SetPose(
                         Pose(::math::nparray_to_mat4<Scalar>(np_pose)));
                 })
            .def("pose", &Class::pose)
            .def("SetPosition", &Class::SetPosition)
            .def("SetPosition",
                 [](Class& self, const py::array_t<Scalar>& np_position) {
                     self.SetPosition(
                         ::math::nparray_to_vec3<Scalar>(np_position));
                 })
            .def("position", &Class::position)
            .def("SetOrientation", &Class::SetOrientation)
            .def("SetOrientation",
                 [](Class& self, const py::array_t<Scalar>& np_quat) {
                     self.SetOrientation(
                         ::math::nparray_to_quat<Scalar>(np_quat));
                 })
            .def("SetLinearVelocity", &Class::SetLinearVelocity)
            .def("SetLinearVelocity",
                 [](Class& self, const py::array_t<Scalar>& np_linearvel) {
                     self.SetLinearVelocity(
                         ::math::nparray_to_vec3<Scalar>(np_linearvel));
                 })
            .def("linear_vel", &Class::linear_vel)
            .def("SetAngularVelocity", &Class::SetAngularVelocity)
            .def("SetAngularVelocity",
                 [](Class& self, const py::array_t<Scalar>& np_angularvel) {
                     self.SetAngularVelocity(
                         ::math::nparray_to_vec3<Scalar>(np_angularvel));
                 })
            .def("angular_vel", &Class::angular_vel)
            .def_readwrite("pose0", &Class::pose0)
            .def_readwrite("linearVel0", &Class::linearVel0)
            .def_readwrite("angularVel0", &Class::angularVel0)
            .def_readwrite("totalForceCOM", &Class::totalForceCOM)
            .def_readwrite("totalTorque", &Class::totalTorque)
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }
}

}  // namespace loco

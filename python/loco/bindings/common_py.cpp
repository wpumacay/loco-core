#include <vector>
#include <cstring>
#include <stdexcept>
#include <algorithm>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <conversions_py.hpp>

#include <loco/core/common.hpp>

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<::loco::ColliderData>);
PYBIND11_MAKE_OPAQUE(std::vector<::loco::DrawableData>);

namespace loco {

// NOLINTNEXTLINE
auto bindings_common(py::module& m) -> void {
    m.attr("MAX_NUM_QPOS") = ::loco::MAX_NUM_QPOS;
    m.attr("MAX_NUM_QVEL") = ::loco::MAX_NUM_QVEL;
    m.attr("NUM_QPOS_JOINT_PRISMATIC") = ::loco::NUM_QPOS_JOINT_PRISMATIC;
    m.attr("NUM_QPOS_JOINT_REVOLUTE") = ::loco::NUM_QPOS_JOINT_REVOLUTE;
    m.attr("NUM_QPOS_JOINT_SPHERICAL") = ::loco::NUM_QPOS_JOINT_SPHERICAL;
    m.attr("NUM_QPOS_JOINT_FREE") = ::loco::NUM_QPOS_JOINT_FREE;
    m.attr("DEFAULT_DENSITY") = ::loco::DEFAULT_DENSITY;

    {
        using Enum = ::loco::eBackendType;
        constexpr auto EnumName = "BackendType";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("NONE", Enum::NONE)
            .value("MUJOCO", Enum::MUJOCO)
            .value("BULLET", Enum::BULLET)
            .value("DART", Enum::DART);
    }

    {
        using Enum = ::loco::eVisualizerType;
        constexpr auto EnumName = "VisualizerType";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("NONE", Enum::NONE)
            .value("VIS_GL", Enum::VIS_GL)
            .value("VIS_MESHCAT", Enum::VIS_MESHCAT);
    }

    {
        using Enum = ::loco::eShapeType;
        constexpr auto EnumName = "ShapeType";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("PLANE", Enum::PLANE)
            .value("BOX", Enum::BOX)
            .value("SPHERE", Enum::SPHERE)
            .value("CYLINDER", Enum::CYLINDER)
            .value("ELLIPSOID", Enum::ELLIPSOID)
            .value("CAPSULE", Enum::CAPSULE)
            .value("CONVEX_MESH", Enum::CONVEX_MESH)
            .value("TRIANGULAR_MESH", Enum::TRIANGULAR_MESH)
            .value("HEIGHTFIELD", Enum::HEIGHTFIELD)
            .value("COMPOUND", Enum::COMPOUND);
    }

    {
        using Enum = ::loco::eDynamicsType;
        constexpr auto EnumName = "DynamicsType";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("DYNAMIC", Enum::DYNAMIC)
            .value("STATIC", Enum::STATIC);
    }

    {
        using Class = ::loco::MeshData;
        constexpr auto ClassName = "MeshData";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def_readwrite("filepath", &Class::filepath)
            .def_readwrite("n_vertices", &Class::n_vertices)
            .def_readwrite("n_faces", &Class::n_faces)
            .def_property(
                "vertices",
                [](Class& self) -> py::array_t<Scalar> {
                    // Make sure we have valid dimensions (if not, return empty)
                    if (self.n_vertices < 1) {
                        return py::array_t<Scalar>();  // NOLINT
                    }

                    // Notice that each vertex is expected to have 3 scalar
                    // entries in the array, so the total number of scalar is
                    // actually (3 * n_vertices)
                    const auto NUM_SCALARS = 3 * self.n_vertices;

                    // For a non-initialized buffer just return an array of
                    // zeros with the given number of vertices
                    if (self.vertices == nullptr) {
                        // NOLINTNEXTLINE
                        self.vertices = std::make_unique<Scalar[]>(NUM_SCALARS);
                        std::fill(self.vertices.get(),
                                  self.vertices.get() + NUM_SCALARS, 0.0);
                    }

                    // Return a view of the vertices as an np.array[float]
                    return py::array_t<Scalar>(
                        static_cast<ssize_t>(NUM_SCALARS), self.vertices.get(),
                        py::cast(self));
                },
                [](Class& self, const py::array_t<Scalar>& array_np) -> void {
                    // We received here a numpy array from the user, which comes
                    // with a given size. We updated the internal "vertices"
                    // pointer by copying all the data, and also update the
                    // "n_vertices" property to the number of vertices obtained.
                    // Notice that for a vertex we consider three scalar entries
                    // in the buffer (not just one per vertex)
                    auto info = array_np.request();
                    auto n_scalars = static_cast<size_t>(info.size);
                    if (n_scalars % 3 != 0) {
                        LOCO_CORE_WARN(
                            "MeshData::vertices >>> the given numpy array "
                            "contains {0} scalars, which is not multiple of 3. "
                            "We will drop some scalars to make it work though",
                            n_scalars);
                    }
                    self.n_vertices = n_scalars / 3;
                    auto num_scalars = self.n_vertices * 3;
                    if (self.vertices == nullptr) {
                        // NOLINTNEXTLINE
                        self.vertices = std::make_unique<Scalar[]>(num_scalars);
                    }
                    memcpy(self.vertices.get(), info.ptr,
                           sizeof(Scalar) * num_scalars);
                })
            .def_property(
                "faces",
                [](Class& self) -> py::array_t<uint32_t> {
                    // Make sure we have valid dimensions (if not, return empty)
                    if (self.n_faces < 1) {
                        return py::array_t<uint32_t>();  // NOLINT
                    }

                    // Similar to the vertices, notice that for each face we
                    // have 3 indices in the faces/indices buffer
                    const auto NUM_INDICES = 3 * self.n_faces;

                    // For a non-initialized buffer just return an array of
                    // zeros with the given number of indices
                    if (self.faces == nullptr) {
                        // NOLINTNEXTLINE
                        self.faces = std::make_unique<uint32_t[]>(NUM_INDICES);
                        std::fill(self.faces.get(),
                                  self.faces.get() + NUM_INDICES, 0);
                    }

                    // Return a view of the faces as an np.array[int]
                    return py::array_t<uint32_t>(
                        static_cast<ssize_t>(NUM_INDICES), self.faces.get(),
                        py::cast(self));
                },
                [](Class& self, const py::array_t<uint32_t>& array_np) -> void {
                    auto info = array_np.request();
                    auto n_indices = static_cast<size_t>(info.size);
                    if (n_indices % 3 != 0) {
                        LOG_CORE_WARN(
                            "MeshData::faces >>> the given numpy array "
                            "contains {0} indices, which is not multiple of 3. "
                            "We will drop some indices to make it work though",
                            n_indices);
                    }
                    self.n_faces = n_indices / 3;
                    n_indices = self.n_faces * 3;
                    if (self.faces == nullptr) {
                        // NOLINTNEXTLINE
                        self.faces = std::make_unique<uint32_t[]>(n_indices);
                    }
                    memcpy(self.faces.get(), info.ptr,
                           sizeof(uint32_t) * n_indices);
                })
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<MeshData\n"
                           "  filepath: \"{}\"\n"
                           "  num_vertices: {}\n"
                           "  num_faces: {}\n"
                           ">")
                    .format(self.filepath, self.n_vertices, self.n_faces);
            });
    }

    {
        using Class = ::loco::HeightfieldData;
        constexpr auto ClassName = "HeightfieldData";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def_readwrite("n_width_samples", &Class::n_width_samples)
            .def_readwrite("n_depth_samples", &Class::n_depth_samples)
            .def_property(
                "heights",
                [](Class& self) -> py::array_t<Scalar> {
                    // Make sure we have valid dimensions
                    if (self.n_width_samples < 1 || self.n_depth_samples < 1) {
                        return py::array_t<Scalar>();  // NOLINT
                    }
                    // Initialize the buffer to zeros if not initialized yet
                    if (self.heights == nullptr) {
                        // NOLINTNEXTLINE
                        self.heights = std::make_unique<Scalar[]>(
                            self.n_width_samples * self.n_depth_samples);
                        std::fill(self.heights.get(),
                                  self.heights.get() + (self.n_width_samples *
                                                        self.n_depth_samples),
                                  0.0);
                    }

                    return py::array(
                        py::buffer_info(
                            self.heights.get(), sizeof(Scalar),
                            py::format_descriptor<Scalar>::format(), 2,
                            {self.n_depth_samples, self.n_width_samples},
                            {sizeof(Scalar) * self.n_width_samples,
                             sizeof(Scalar)}),
                        py::cast(self));
                },
                [](Class& self, const py::array_t<Scalar>& array_np) -> void {
                    auto info = array_np.request();
                    if (info.ndim != 2) {
                        throw std::runtime_error(
                            "HeightfieldData::heights >>> tried using with "
                            "incompatible dimensions. Expects a np.ndarray of "
                            "2 dimensions (m, n).");
                    }
                    auto old_width_samples = self.n_width_samples;
                    auto old_depth_samples = self.n_depth_samples;
                    self.n_width_samples = static_cast<size_t>(info.shape[1]);
                    self.n_depth_samples = static_cast<size_t>(info.shape[0]);
                    auto num_samples =
                        self.n_width_samples * self.n_depth_samples;
                    // Create some new storage if we either have new dimensions,
                    // or we have no storage allocated yet
                    if (old_width_samples != self.n_width_samples ||
                        old_depth_samples != self.n_depth_samples ||
                        self.heights == nullptr) {
                        // NOLINTNEXTLINE
                        self.heights = std::make_unique<Scalar[]>(num_samples);
                    }

                    memcpy(self.heights.get(), info.ptr,
                           sizeof(Scalar) * num_samples);
                })
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<HeightfieldData\n"
                           "  num_width_samples: {}\n"
                           "  num_depth_samples: {}\n"
                           ">")
                    .format(self.n_width_samples, self.n_depth_samples);
            });
    }

    {
        using Class = ::loco::ShapeData;
        constexpr auto ClassName = "ShapeData";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def_readwrite("type", &Class::type)
            // TODO(wilbert): Add custom casters for vec3 <-> py::array_t<float>
            .def_readwrite("size", &Class::size)
            .def_readwrite("mesh_data", &Class::mesh_data)
            .def_readwrite("hfield_data", &Class::hfield_data)
            .def_readwrite("local_tf", &Class::local_tf)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<ShapeData\n"
                           "  type: {}\n"
                           "  size: {}\n"
                           "  local_pos: {}\n"
                           "  local_quat: {}\n"
                           ">")
                    .format(::loco::ToString(self.type), self.size.toString(),
                            self.local_tf.position.toString(),
                            self.local_tf.orientation.toString());
            });
    }

    {
        // Bind stl container std::vector<::loco::ColliderData>
        py::bind_vector<std::vector<::loco::ColliderData>>(
            m, "VectorColliderData");

        using Class = ::loco::ColliderData;
        using BaseClass = ::loco::ShapeData;
        constexpr auto ClassName = "ColliderData";  // NOLINT
        py::class_<Class, BaseClass>(m, ClassName)
            .def(py::init<>())
            .def_readwrite("collision_group", &Class::collision_group)
            .def_readwrite("collision_mask", &Class::collision_mask)
            .def_readwrite("friction", &Class::friction)
            .def_readwrite("children", &Class::children)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<ColliderData\n"
                           "  type: {}\n"
                           "  size: {}\n"
                           "  collision_group: {}\n"
                           "  collision_mask: {}\n"
                           "  friction: {}\n"
                           "  num_children: {}\n"
                           "  local_pos: {}\n"
                           "  local_quat: {}\n"
                           ">")
                    .format(::loco::ToString(self.type), self.size.toString(),
                            self.collision_group, self.collision_mask,
                            self.friction.toString(), self.children.size(),
                            self.local_tf.position.toString(),
                            self.local_tf.orientation.toString());
            });
    }

    {
        // Bind stl container std::vector<::loco::DrawableData>
        py::bind_vector<std::vector<::loco::DrawableData>>(
            m, "VectorDrawableData");

        using Class = ::loco::DrawableData;
        using BaseClass = ::loco::ShapeData;
        constexpr auto ClassName = "DrawableData";  // NOLINT
        py::class_<Class, BaseClass>(m, ClassName)
            .def(py::init<>())
            .def_readwrite("color", &Class::color)
            .def_readwrite("texture", &Class::texture)
            .def_readwrite("children", &Class::children)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<DrawableData\n"
                           "  type: {}\n"
                           "  size: {}\n"
                           "  color: {}\n"
                           "  texture: \"{}\"\n"
                           "  num_children: {}\n"
                           "  local_pos: {}\n"
                           "  local_quat: {}\n"
                           ">")
                    .format(::loco::ToString(self.type), self.size.toString(),
                            self.color.toString(), self.texture,
                            self.children.size(),
                            self.local_tf.position.toString(),
                            self.local_tf.orientation.toString());
            });
    }

    {
        using Class = ::loco::InertialData;
        constexpr auto ClassName = "InertialData";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def_readwrite("mass", &Class::mass)
            .def_readwrite("inertia", &Class::inertia)
            .def_readwrite("local_tf", &Class::local_tf)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<InertialData\n"
                           "  mass: {}\n"
                           "  inertia: \n{}\n"
                           "  local_pos: {}\n"
                           "  local_quat: {}\n"
                           ">")
                    .format(self.mass, self.inertia.toString(),
                            self.local_tf.position.toString(),
                            self.local_tf.orientation.toString());
            });
    }

    {
        using Class = ::loco::BodyData;
        constexpr auto ClassName = "BodyData";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def_readwrite("dyntype", &Class::dyntype)
            .def_readwrite("inertia", &Class::inertia)
            .def_readwrite("collider", &Class::collider)
            .def_readwrite("drawable", &Class::drawable)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<BodyData\n"
                           "  dyntype: {}\n"
                           ">")
                    .format(::loco::ToString(self.dyntype));
            });
    }
}

}  // namespace loco

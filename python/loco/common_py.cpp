#include <cstring>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <conversions_py.hpp>

#include <loco/core/common.hpp>

namespace py = pybind11;

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
            .value("CYLINDER", Enum::SPHERE)
            .value("ELLIPSOID", Enum::ELLIPSOID)
            .value("CAPSULE", Enum::CAPSULE)
            .value("ELLIPSOID", Enum::ELLIPSOID)
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
            .def_readwrite("n_indices", &Class::n_faces)
            .def_property(
                "vertices",
                [](const Class& self) -> py::array_t<Scalar> {
                    // We're assumming that the user configured the vertices
                    // previous to making this read step, so we just return a
                    // buffer of the size given by this "n_vertices" property,
                    // and the data comes from the "vertices" pointer. Notice
                    // that each vertex is expected to have 3 scalar entries in
                    // the array, so the total number of scalar is actually
                    // (3 * n_vertices)
                    const auto NUM_SCALARS =
                        static_cast<ssize_t>(3 * self.n_vertices);
                    return py::array_t<Scalar>(NUM_SCALARS, self.vertices);
                },
                [](Class& self, const py::array_t<Scalar>& array_np) -> void {
                    // We received here a numpy array from the user, which comes
                    // with a given size. We updated the internal "vertices"
                    // pointer by copying all the data, and also update the
                    // "n_vertices" property to the number of vertices obtained.
                    // Notice that for a vertex we consider three scalar entries
                    // in the buffer (not just one per vertex)
                    auto array_buffer_info = array_np.request();
                    auto n_scalars =
                        static_cast<size_t>(array_buffer_info.size);
                    if (n_scalars % 3 != 0) {
                        LOCO_CORE_WARN(
                            "MeshData::vertices >>> the given numpy array "
                            "contains {0} scalars, which is not multiple of 3. "
                            "We will drop some scalars to make it work though",
                            n_scalars);
                    }
                    self.n_vertices = n_scalars / 3;
                    auto n_total_scalars = self.n_vertices * 3;
                    memcpy(self.vertices, array_buffer_info.ptr,
                           sizeof(Scalar) * n_total_scalars);
                })
            .def_property(
                "indices",
                [](const Class& self) -> py::array_t<uint32_t> {
                    // Similar to the vertices, notice that for each face we
                    // have 3 indices in the faces/indices buffer, so we're
                    // making the correction for the number of elements we have
                    // to return in the numpy array. Also similarly to the
                    // previous case, we're assumming that the user has
                    // previously configured the faces buffer by writing to it,
                    // so we have a valid number of faces in the internal buffer
                    const auto NUM_INDICES =
                        static_cast<ssize_t>(3 * self.n_faces);
                    return py::array_t<uint32_t>(NUM_INDICES, self.faces);
                },
                [](Class& self, const py::array_t<uint32_t>& array_np) -> void {
                    auto array_buffer_info = array_np.request();
                    auto n_indices =
                        static_cast<size_t>(array_buffer_info.size);
                    if (n_indices % 3 != 0) {
                        LOG_CORE_WARN(
                            "MeshData::faces >>> the given numpy array "
                            "contains {0} indices, which is not multiple of 3. "
                            "We will drop some indices to make it work though",
                            n_indices);
                    }
                    self.n_faces = n_indices / 3;
                    auto n_total_indices = self.n_faces * 3;
                    memcpy(self.faces, array_buffer_info.ptr,
                           sizeof(uint32_t) * n_total_indices);
                });
    }
}

}  // namespace loco

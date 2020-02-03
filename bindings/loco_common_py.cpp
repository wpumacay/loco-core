
#include <loco_common_py.h>

namespace py = pybind11;

namespace loco
{
    void bindings_common( py::module& m )
    {
        m.attr( "MAX_NUM_QPOS" ) = loco::MAX_NUM_QPOS;
        m.attr( "MAX_NUM_QVEL" ) = loco::MAX_NUM_QVEL;
        m.attr( "NUM_QPOS_JOINT_PRISMATIC" ) = loco::NUM_QPOS_JOINT_PRISMATIC;
        m.attr( "NUM_QPOS_JOINT_REVOLUTE" ) = loco::NUM_QPOS_JOINT_REVOLUTE;
        m.attr( "NUM_QPOS_JOINT_SPHERICAL" ) = loco::NUM_QPOS_JOINT_SPHERICAL;
        m.attr( "NUM_QPOS_JOINT_FREE" ) = loco::NUM_QPOS_JOINT_FREE;
        m.attr( "DEFAULT_DENSITY" ) = loco::DEFAULT_DENSITY;
    }
}
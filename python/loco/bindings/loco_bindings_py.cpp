
#include <loco_common_py.h>

namespace py = pybind11;

namespace loco
{
    extern void bindings_common( py::module& m );
    extern void bindings_utils( py::module& m );
    extern void bindings_object( py::module& m );
    extern void bindings_data( py::module& m );
    extern void bindings_primitives( py::module& m );
    extern void bindings_scenario( py::module& m );
    extern void bindings_visualizer( py::module& m );
    extern void bindings_simulation( py::module& m );
    extern void bindings_runtime( py::module& m );
}

PYBIND11_MODULE( loco_sim, m )
{
    loco::bindings_common( m );
    loco::bindings_utils( m );
    loco::bindings_object( m );
    loco::bindings_data( m );
    loco::bindings_primitives( m );
    loco::bindings_scenario( m );
    loco::bindings_visualizer( m );
    loco::bindings_simulation( m );
    loco::bindings_runtime( m );

    // Execute some required initialization steps
    loco::TLogger::Init();
}
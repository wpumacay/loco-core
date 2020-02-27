
#include <loco_common_py.h>

namespace py = pybind11;

namespace loco
{
    extern void bindings_common( py::module& m );
    extern void bindings_components_data( py::module& m );
    extern void bindings_components_elements( py::module& m );
    extern void bindings_primitives( py::module& m );
    extern void bindings_runtime_and_components( py::module& m );
}

PYBIND11_MODULE( loco_sim, m )
{
    loco::bindings_common( m );
    loco::bindings_components_data( m );
    loco::bindings_components_elements( m );
    loco::bindings_primitives( m );
    loco::bindings_runtime_and_components( m );

    // Execute some required initialization steps
    loco::TLogger::Init();
}
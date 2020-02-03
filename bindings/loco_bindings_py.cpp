
#include <loco_common_py.h>

namespace py = pybind11;

namespace loco
{
    extern void bindings_common( py::module& m );
    extern void bindings_components( py::module& m );
}

PYBIND11_MODULE( loco, m )
{
    py::module m_sim = m.def_submodule( "sim", "Simulation module of 'Loco'" );

    loco::bindings_common( m_sim );
    loco::bindings_components( m_sim );

    m.attr( "__version__" ) = "0.0.1";
}
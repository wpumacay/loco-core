
#include <loco_common_py.h>

namespace py = pybind11;

namespace loco
{
    extern void bindings_common( py::module& m );
    extern void bindings_object( py::module& m );
    extern void bindings_data( py::module& m );
    namespace primitives {
        extern void bindings_primitives( py::module& m );
    }
    namespace kintree {
        //// extern void bindings_kinematic_trees( py::module& m );
    }
    namespace terrain {
        //// extern void bindings_terrain_generators( py::module& m );
    }
    extern void bindings_scenario( py::module& m );
    extern void bindings_visualizer( py::module& m );
    extern void bindings_simulation( py::module& m );
    extern void bindings_runtime( py::module& m );
}

PYBIND11_MODULE( loco_sim, mod_core )
{
    auto mod_primitives = mod_core.def_submodule( "primitives", "Module containing Single-Body Primitives functionality" );
    //// auto mod_kintrees = mod_core.def_submodule( "kintree", "Module containing Kinematic-trees functionality" );
    //// auto mod_terrain = mod_core.def_submodule( "terrain", "Module containting Terrain-Generation functionality" );

    loco::bindings_common( mod_core );
    loco::bindings_object( mod_core );
    loco::bindings_data( mod_core );
    loco::primitives::bindings_primitives( mod_primitives );
    //// loco::kintree::bindings_kinematic_trees( mod_kintrees );
    //// loco::terrain::bindings_terrain_generators( mod_terrain );
    loco::bindings_scenario( mod_core );
    loco::bindings_visualizer( mod_core );
    loco::bindings_simulation( mod_core );
    loco::bindings_runtime( mod_core );

    // Execute some required initialization steps
    tinyutils::Logger::Init();
    tinyutils::Clock::Init();
    tinyutils::Profiler::Init();
    tinyutils::PerlinNoise::Init();
}

#include <loco_common_py.h>
#include <loco_runtime.h>

namespace py = pybind11;

namespace loco
{
    void bindings_runtime( py::module& m )
    {
        // Bindings for TRuntime
        {
            m.attr( "RENDERING_NONE" ) = loco::config::rendering::NONE;
            m.attr( "RENDERING_GLVIZ_GLFW" ) = loco::config::rendering::GLVIZ_GLFW;
            m.attr( "RENDERING_GLVIZ_EGL" ) = loco::config::rendering::GLVIZ_EGL;

            m.attr( "PHYSICS_NONE" ) = loco::config::physics::NONE;
            m.attr( "PHYSICS_MUJOCO" ) = loco::config::physics::MUJOCO;
            m.attr( "PHYSICS_BULLET" ) = loco::config::physics::BULLET;
            m.attr( "PHYSICS_RAISIM" ) = loco::config::physics::RAISIM;
            m.attr( "PHYSICS_DART" ) = loco::config::physics::DART;

            py::class_<TRuntime>( m, "Runtime" )
                .def( py::init<const std::string&, const std::string&>(),
                      py::arg( "physics_backend" ), py::arg( "rendering_backend" ) )
                .def( "CreateSimulation",
                      &TRuntime::CreateSimulation,
                      py::arg( "scenario" ), py::return_value_policy::reference )
                .def( "CreateVisualizer",
                      &TRuntime::CreateVisualizer,
                      py::arg( "scenario" ),
                      py::arg( "windowWidth" ) = 1024,
                      py::arg( "windowHeight" ) = 768,
                      py::arg( "windowResizable" ) = true,
                      py::arg( "renderOffscreen" ) = false, py::return_value_policy::reference )
                .def( "DestroySimulation", &TRuntime::DestroySimulation )
                .def( "DestroyVisualizer", &TRuntime::DestroyVisualizer )
                .def( "__repr__", []( const TRuntime* self )
                    {
                        auto _strrep = std::string( "Runtime(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "dlpath-sim  : " + self->dlpathSim() + "\n";
                        _strrep += "dlpath-viz  : " + self->dlpathViz() + "\n";

                        auto _simulation = self->GetCurrentSimulation();
                        _strrep += "simulation  :\n";
                        _strrep += "\texists        : " + std::string( _simulation ? "yes" : "no" ) + "\n";
                        if ( _simulation )
                        {
                            _strrep += "\tcpp-address   : " + tinyutils::PointerToHexAddress( _simulation ) + "\n";
                            _strrep += "\tbackend-id    : " + _simulation->backendId() + "\n";
                        }

                        auto _visualizer = self->GetCurrentVisualizer();
                        _strrep += "visualizer  :\n";
                        _strrep += "\texists        : " + std::string( _visualizer ? "yes" : "no" ) + "\n";
                        if ( _visualizer )
                        {
                            _strrep += "\tcpp-address   : " + tinyutils::PointerToHexAddress( _visualizer ) + "\n";
                            _strrep += "\tbackend-id    : " + _visualizer->backendId() + "\n";
                        }
                        _strrep += ")";
                        return _strrep;
                    } );
        }
    }
}
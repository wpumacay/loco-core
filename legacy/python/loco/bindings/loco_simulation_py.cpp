
#include <loco_common_py.h>
#include <loco_simulation.h>

namespace py = pybind11;

namespace loco
{
    void bindings_simulation( py::module& m )
    {
        // Bindings for TISimulation interface and related null-impl
        {
            py::class_<TISimulation>( m, "ISimulation" )
                .def( "Initialize", &TISimulation::Initialize )
                .def( "Step", &TISimulation::Step )
                .def( "Reset", &TISimulation::Reset )
                .def( "Pause", &TISimulation::Pause )
                .def( "Resume", &TISimulation::Resume )
                // @issue: Avoid returning references to objects that hold std::"container"<unique_ptr>, as
                // it tries to run the binding machinery onto these containers as well (even if private and
                // not being exposed). The user should use instead use the created scenario reference
                // .def_property_readonly( "scenario", &TISimulation::scenario )
                .def_property_readonly( "running", &TISimulation::running )
                .def_property_readonly( "backendId", &TISimulation::backendId )
                .def( "__repr__", []( const TISimulation* self )
                    {
                        auto _strrep = std::string( "Simulation(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "backend-id  : " + self->backendId() + "\n";
                        _strrep += "running     : " + std::string( self->running() ? "true" : "false" ) + "\n";
                        _strrep += "cpp-scenario: " + tinyutils::PointerToHexAddress( self->scenario() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_<TNullSimulation, TISimulation>( m, "NullSimulation" )
                .def( py::init<TScenario*>(), py::arg( "scenario" ) ); // no ownership nor nurse-patient required
        }
    }
}
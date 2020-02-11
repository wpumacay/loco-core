
#include <loco_common_py.h>
#include <loco_scenario.h>
//// #include <loco_visualizer.h>
//// #include <loco_simulation.h>
//// #include <loco_runtime.h>

namespace py = pybind11;

namespace loco
{
    void bindings_runtime_and_components( py::module& m )
    {
        // Bindings for TScenario class
        {
            py::class_<TScenario>( m, "Scenario" )
                .def( py::init<>() )
                .def( "Initialize", &TScenario::Initialize )
                .def( "PreStep", &TScenario::PreStep )
                .def( "PostStep", &TScenario::PostStep )
                .def( "Reset", &TScenario::Reset )
                .def( "AddSingleBody", &TScenario::AddSingleBody, py::arg( "body" ),
                      py::return_value_policy::reference,
                      py::keep_alive<1, 2>() )
                .def( "GetSingleBodyByName", []( TScenario* self, const std::string& name ) -> TSingleBody*
                    {
                        return self->GetSingleBodyByName( name );
                    },
                    py::arg( "name" ), py::return_value_policy::reference )
                .def( "GetNumSingleBodies", &TScenario::GetNumSingleBodies )
                .def( "HasSingleBodyNamed", &TScenario::HasSingleBodyNamed, py::arg( "name" ) )
                .def( "GetSingleBodiesList", []( TScenario* self ) -> std::vector<TSingleBody*>
                    {
                        return self->GetSingleBodiesList();
                    },
                    py::return_value_policy::reference )
                .def( "__repr__", []( const TScenario* self )
                    {
                        auto _strrep = std::string( "Scenario(\n" );
                        _strrep += "cpp-address         : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "num-single-bodies   : " + std::to_string( self->GetNumSingleBodies() ) + "\n";
                        _strrep += "single-bodies       :\n";
                        auto singleBodiesList = self->GetSingleBodiesList();
                        for ( auto singleBody : singleBodiesList )
                            _strrep += "\t(name=" + singleBody->name() + ", pos=" + loco::ToString( singleBody->pos() ) + ")\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }
    }
}
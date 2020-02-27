
#include <loco_common_py.h>
#include <loco_scenario.h>
#include <loco_simulation.h>
#include <visualizer/loco_visualizer.h>
#include <loco_runtime.h>

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

        // Bindings for TISimulation interface and related null-impl
        {
            py::class_<TISimulation>( m, "ISimulation" )
                .def( "Initialize", &TISimulation::Initialize )
                .def( "Step", &TISimulation::Step )
                .def( "Reset", &TISimulation::Reset )
                .def( "Pause", &TISimulation::Pause )
                .def( "Resume", &TISimulation::Resume )
                // @issue: Avoid returning references to objects that hold std::"container"<unique_ptr>, as
                // it tries to run the binding machinery onto this containers as well (even if private and
                // not being exposed). The user should use instead use the created scenario reference
                // .def_property_readonly( "scenario", &TISimulation::scenario )
                .def_property_readonly( "running", &TISimulation::running )
                .def_property_readonly( "backendId", &TISimulation::backendId )
                .def( "__repr__", []( const TISimulation* self )
                    {
                        auto _strrep = std::string( "Simulation(\n" );
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "backend-id  : " + self->backendId() + "\n";
                        _strrep += "running     : " + std::string( self->running() ? "true" : "false" ) + "\n";
                        _strrep += "cpp-scenario: " + loco::PointerToHexAddress( self->scenario() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_<TNullSimulation, TISimulation>( m, "NullSimulation" )
                .def( py::init<TScenario*>(), py::arg( "scenario" ) ); // no ownership nor nurse-patient required
        }

        // Bindings for TIVisualizer and its related components
        {
            py::enum_<loco::eVizCameraType>( m, "VizCameraType", py::arithmetic() )
                .value( "ORBIT", loco::eVizCameraType::ORBIT )
                .value( "FPS", loco::eVizCameraType::FPS )
                .value( "FIXED", loco::eVizCameraType::FIXED );

            py::enum_<loco::eVizLightType>( m, "VizLightType", py::arithmetic() )
                .value( "DIRECTIONAL", loco::eVizLightType::DIRECTIONAL )
                .value( "POINT", loco::eVizLightType::POINT )
                .value( "SPOT", loco::eVizLightType::SPOT );

            py::class_<TVizCamera>( m, "VizCamera" )
                .def( py::init( []( const std::string& name, 
                                    const eVizCameraType& type,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_target )
                        {
                            return std::make_unique<TVizCamera>( name,
                                                                 type,
                                                                 tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                                 tinymath::nparray_to_vector<TScalar, 3>( arr_target ) );
                        } ), 
                      py::arg( "name" ), py::arg( "type" ), py::arg( "position" ), py::arg( "target" ) )
                .def( "Update", &TVizCamera::Update )
                .def( "Reset", &TVizCamera::Reset )
                .def_property( "position",
                    []( const TVizCamera* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->position() );
                        },
                    []( TVizCamera* self, const py::array_t<TScalar>& arr_position )
                        {
                            self->SetPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_position ) );
                        } )
                .def_property( "target",
                    []( const TVizCamera* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->target() );
                        },
                    []( TVizCamera* self, const py::array_t<TScalar>& arr_target )
                        {
                            self->SetTarget( tinymath::nparray_to_vector<TScalar, 3>( arr_target ) );
                        } )
                .def_property_readonly( "name", &TVizCamera::name )
                .def_property_readonly( "type", &TVizCamera::type )
                .def( "__repr__", []( const TVizCamera* self )
                    {
                        auto _strrep = std::string( "VizCamera(\n" );
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "type        : " + loco::ToString( self->type() ) + "\n";
                        _strrep += "position    : " + loco::ToString( self->position() ) + "\n";
                        _strrep += "target      : " + loco::ToString( self->target() ) + "\n";
                        return _strrep;
                    } );

            py::class_<TVizLight>( m, "VizLight" )
                .def( py::init( []( const std::string& name,
                                    const eVizLightType& type,
                                    const py::array_t<TScalar>& arr_ambient,
                                    const py::array_t<TScalar>& arr_diffuse,
                                    const py::array_t<TScalar>& arr_specular )
                        {
                            return std::make_unique<TVizLight>( name,
                                                                type,
                                                                tinymath::nparray_to_vector<TScalar, 3>( arr_ambient ),
                                                                tinymath::nparray_to_vector<TScalar, 3>( arr_diffuse ),
                                                                tinymath::nparray_to_vector<TScalar, 3>( arr_specular ) );
                        } ),
                      py::arg( "name" ), py::arg( "type" ), py::arg( "ambient" ), py::arg( "diffuse" ), py::arg( "specular" ) )
                .def( "Update", &TVizLight::Update )
                .def( "Reset", &TVizLight::Reset )
                .def_property( "position",
                    []( const TVizLight* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->position() );
                        },
                    []( TVizLight* self, const py::array_t<TScalar>& arr_position )
                        {
                            self->SetPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_position ) );
                        } )
                .def_property( "direction",
                    []( const TVizLight* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->direction() );
                        },
                    []( TVizLight* self, const py::array_t<TScalar>& arr_direction )
                        {
                            self->SetDirection( tinymath::nparray_to_vector<TScalar, 3>( arr_direction ) );
                        } )
                .def_property( "ambient",
                    []( const TVizLight* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->ambient() );
                        },
                    []( TVizLight* self, const py::array_t<TScalar>& arr_ambient )
                        {
                            self->ChangeAmbientColor( tinymath::nparray_to_vector<TScalar, 3>( arr_ambient ) );
                        } )
                .def_property( "diffuse",
                    []( const TVizLight* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->diffuse() );
                        },
                    []( TVizLight* self, const py::array_t<TScalar>& arr_diffuse )
                        {
                            self->ChangeDiffuseColor( tinymath::nparray_to_vector<TScalar, 3>( arr_diffuse ) );
                        } )
                .def_property( "specular",
                    []( const TVizLight* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->specular() );
                        },
                    []( TVizLight* self, const py::array_t<TScalar>& arr_specular )
                        {
                            self->ChangeSpecularColor( tinymath::nparray_to_vector<TScalar, 3>( arr_specular ) );
                        } )
                .def_property( "intensity", &TVizLight::intensity, &TVizLight::ChangeIntensity )
                .def_property( "castShadows", &TVizLight::castShadows, &TVizLight::SetCastShadows )
                .def_property( "innerCutoff", &TVizLight::innerCutoff, &TVizLight::ChangeInnerCutoff )
                .def_property( "outerCutoff", &TVizLight::outerCutoff, &TVizLight::ChangeOuterCutoff )
                .def_property_readonly( "name", &TVizLight::name )
                .def_property_readonly( "type", &TVizLight::type )
                .def( "__repr__", []( const TVizLight* self )
                    {
                        auto _strrep = std::string( "VizLight(\n" );
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "type        : " + loco::ToString( self->type() ) + "\n";
                        _strrep += "position    : " + loco::ToString( self->position() ) + "\n";
                        _strrep += "direction   : " + loco::ToString( self->direction() ) + "\n";
                        _strrep += "ambient     : " + loco::ToString( self->ambient() ) + "\n";
                        _strrep += "diffuse     : " + loco::ToString( self->diffuse() ) + "\n";
                        _strrep += "specular    : " + loco::ToString( self->specular() ) + "\n";
                        _strrep += "intensity   : " + std::to_string( self->intensity() ) + "\n";
                        return _strrep;
                    } );

            py::class_<TIVisualizer>( m, "" )
                .def( "Initialize", &TIVisualizer::Initialize )
                .def( "Update", &TIVisualizer::Update )
                .def( "Reset", &TIVisualizer::Reset )
                .def( "CreateCamera",
                      []( TIVisualizer* self,
                          const std::string& name,
                          const eVizCameraType& type,
                          const py::array_t<TScalar>& arr_position,
                          const py::array_t<TScalar>& arr_target ) -> TVizCamera*
                        {
                            return self->CreateCamera( name,
                                                       type,
                                                       tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                       tinymath::nparray_to_vector<TScalar, 3>( arr_target ) );
                        },
                      py::arg( "name" ), py::arg( "type" ), py::arg( "position" ), py::arg( "target" ),
                      py::return_value_policy::reference )
                .def( "CreateLight",
                      []( TIVisualizer* self,
                          const std::string& name,
                          const eVizLightType& type,
                          const py::array_t<TScalar>& arr_ambient,
                          const py::array_t<TScalar>& arr_diffuse,
                          const py::array_t<TScalar>& arr_specular ) -> TVizLight*
                        {
                            return self->CreateLight( name,
                                                      type,
                                                      tinymath::nparray_to_vector<TScalar, 3>( arr_ambient ),
                                                      tinymath::nparray_to_vector<TScalar, 3>( arr_diffuse ),
                                                      tinymath::nparray_to_vector<TScalar, 3>( arr_specular ) );
                        },
                      py::arg( "name" ), py::arg( "type" ), py::arg( "ambient" ), py::arg( "diffuse" ), py::arg( "specular" ),
                      py::return_value_policy::reference )
                .def( "GetCurrentCamera",
                      []( TIVisualizer* self ) -> TVizCamera*
                        {
                            return self->GetCurrentCamera();
                        },
                      py::return_value_policy::reference )
                .def( "GetCurrentLight",
                      []( TIVisualizer* self ) -> TVizLight*
                        {
                            return self->GetCurrentLight();
                        },
                      py::return_value_policy::reference )
                .def( "ChangeCurrentCamera", &TIVisualizer::ChangeCurrentCamera, py::arg( "name" ) )
                .def( "ChangeCurrentLight", &TIVisualizer::ChangeCurrentLight, py::arg( "name" ) )
                .def( "GetCameraByName",
                      []( TIVisualizer* self, const std::string& name ) -> TVizCamera*
                        {
                            return self->GetCameraByName( name );
                        },
                      py::arg( "name" ), py::return_value_policy::reference )
                .def( "GetLightByName",
                      []( TIVisualizer* self, const std::string& name ) -> TVizLight*
                        {
                            return self->GetLightByName( name );
                        },
                      py::arg( "name" ), py::return_value_policy::reference )
                .def( "HasCameraNamed", &TIVisualizer::HasCameraNamed, py::arg( "name" ) )
                .def( "HasLightNamed", &TIVisualizer::HasLightNamed, py::arg( "name" ) )
                .def( "IsActive", &TIVisualizer::IsActive )
                .def( "IsKeyDown", &TIVisualizer::IsKeyDown, py::arg( "key" ) )
                .def( "CheckSingleKeyPress", &TIVisualizer::CheckSingleKeyPress, py::arg( "key" ) )
                .def( "IsMouseDown", &TIVisualizer::IsMouseDown, py::arg( "button" ) )
                .def( "GetCursorPosition", []( const TIVisualizer* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 2>( self->GetCursorPosition() );
                    } )
                .def( "DrawLine",
                      []( TIVisualizer* self,
                          const py::array_t<TScalar>& arr_start,
                          const py::array_t<TScalar>& arr_end,
                          const py::array_t<TScalar>& arr_color )
                        {
                            self->DrawLine( tinymath::nparray_to_vector<TScalar, 3>( arr_start ),
                                            tinymath::nparray_to_vector<TScalar, 3>( arr_end ),
                                            tinymath::nparray_to_vector<TScalar, 3>( arr_color ) );
                        },
                      py::arg( "start" ), py::arg( "end" ), py::arg( "color" ) )
                .def( "DrawAABB",
                      []( TIVisualizer* self,
                          const py::array_t<TScalar>& arr_aabbMin,
                          const py::array_t<TScalar>& arr_aabbMax,
                          const py::array_t<TScalar>& arr_aabbTransform,
                          const py::array_t<TScalar>& arr_color )
                        {
                            self->DrawAABB( tinymath::nparray_to_vector<TScalar, 3>( arr_aabbMin ),
                                            tinymath::nparray_to_vector<TScalar, 3>( arr_aabbMax ),
                                            tinymath::nparray_to_matrix<TScalar, 4>( arr_aabbTransform ),
                                            tinymath::nparray_to_vector<TScalar, 3>( arr_color ) );
                        },
                      py::arg( "aabbMin" ), py::arg( "aabbMax" ), py::arg( "transform" ), py::arg( "color" ) )
                .def( "DrawSolidCylinderX",
                      []( TIVisualizer* self,
                          float radius,float height,
                          const py::array_t<TScalar>& arr_transform,
                          const py::array_t<TScalar>& arr_rgba )
                        {
                            self->DrawSolidCylinderX( radius, height,
                                                      tinymath::nparray_to_matrix<TScalar, 4>( arr_transform ),
                                                      tinymath::nparray_to_vector<TScalar, 4>( arr_rgba ) );
                        },
                      py::arg( "radius" ), py::arg( "height" ), py::arg( "transform" ), py::arg( "rgba" ) )
                .def( "DrawSolidCylinderY",
                      []( TIVisualizer* self,
                          float radius,float height,
                          const py::array_t<TScalar>& arr_transform,
                          const py::array_t<TScalar>& arr_rgba )
                        {
                            self->DrawSolidCylinderY( radius, height,
                                                      tinymath::nparray_to_matrix<TScalar, 4>( arr_transform ),
                                                      tinymath::nparray_to_vector<TScalar, 4>( arr_rgba ) );
                        },
                      py::arg( "radius" ), py::arg( "height" ), py::arg( "transform" ), py::arg( "rgba" ) )
                .def( "DrawSolidCylinderZ",
                      []( TIVisualizer* self,
                          float radius,float height,
                          const py::array_t<TScalar>& arr_transform,
                          const py::array_t<TScalar>& arr_rgba )
                        {
                            self->DrawSolidCylinderZ( radius, height,
                                                      tinymath::nparray_to_matrix<TScalar, 4>( arr_transform ),
                                                      tinymath::nparray_to_vector<TScalar, 4>( arr_rgba ) );
                        },
                      py::arg( "radius" ), py::arg( "height" ), py::arg( "transform" ), py::arg( "rgba" ) )
                .def( "DrawSolidArrowX",
                      []( TIVisualizer* self,
                          float length,
                          const py::array_t<TScalar>& arr_transform,
                          const py::array_t<TScalar>& arr_rgba )
                        {
                            self->DrawSolidArrowX( length,
                                                   tinymath::nparray_to_matrix<TScalar, 4>( arr_transform ),
                                                   tinymath::nparray_to_vector<TScalar, 4>( arr_rgba ) );
                        },
                      py::arg( "length" ), py::arg( "transform" ), py::arg( "rgba" ) )
                .def( "DrawSolidArrowY",
                      []( TIVisualizer* self,
                          float length,
                          const py::array_t<TScalar>& arr_transform,
                          const py::array_t<TScalar>& arr_rgba )
                        {
                            self->DrawSolidArrowY( length,
                                                   tinymath::nparray_to_matrix<TScalar, 4>( arr_transform ),
                                                   tinymath::nparray_to_vector<TScalar, 4>( arr_rgba ) );
                        },
                      py::arg( "length" ), py::arg( "transform" ), py::arg( "rgba" ) )
                .def( "DrawSolidArrowZ",
                      []( TIVisualizer* self,
                          float length,
                          const py::array_t<TScalar>& arr_transform,
                          const py::array_t<TScalar>& arr_rgba )
                        {
                            self->DrawSolidArrowZ( length,
                                                   tinymath::nparray_to_matrix<TScalar, 4>( arr_transform ),
                                                   tinymath::nparray_to_vector<TScalar, 4>( arr_rgba ) );
                        },
                      py::arg( "length" ), py::arg( "transform" ), py::arg( "rgba" ) )
                .def_property_readonly( "backendId", &TIVisualizer::backendId )
                .def( "__repr__", []( const TIVisualizer* self )
                    {
                        auto _strrep = std::string( "IVisualizer(\n" );
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "num-cameras : " + std::to_string( self->GetNumCameras() ) + "\n";
                        _strrep += "num-lights  : " + std::to_string( self->GetNumLights() ) + "\n";
                        _strrep += "cameras     :\n";
                        for ( size_t i = 0; i < self->GetNumCameras(); i++ )
                            _strrep += "\tname= " + self->get_camera( i )->name() + "\n";
                        _strrep += "lights      :\n";
                        for ( size_t i = 0; i < self->GetNumLights(); i++ )
                            _strrep += "\tname= " + self->get_light( i )->name() + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_<TNullVisualizer, TIVisualizer>( m, "NullVisualizer" )
                .def( py::init<TScenario*,size_t, size_t, bool, bool>(), 
                      py::arg( "scenario" ),
                      py::arg( "windowWidth" ) = 1024,
                      py::arg( "windowHeight" ) = 768,
                      py::arg( "windowResizable" ) = true,
                      py::arg( "renderOffscreen" ) = false ); // no ownership nor nurse-patient required
        }

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
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "dlpath-sim  : " + self->dlpathSim() + "\n";
                        _strrep += "dlpath-viz  : " + self->dlpathViz() + "\n";

                        auto _simulation = self->GetCurrentSimulation();
                        _strrep += "simulation  :\n";
                        _strrep += "\texists        : " + std::string( _simulation ? "yes" : "no" ) + "\n";
                        if ( _simulation )
                        {
                            _strrep += "\tcpp-address   : " + loco::PointerToHexAddress( _simulation ) + "\n";
                            _strrep += "\tbackend-id    : " + _simulation->backendId() + "\n";
                        }

                        auto _visualizer = self->GetCurrentVisualizer();
                        _strrep += "visualizer  :\n";
                        _strrep += "\texists        : " + std::string( _visualizer ? "yes" : "no" ) + "\n";
                        if ( _visualizer )
                        {
                            _strrep += "\tcpp-address   : " + loco::PointerToHexAddress( _visualizer ) + "\n";
                            _strrep += "\tbackend-id    : " + _visualizer->backendId() + "\n";
                        }
                        _strrep += ")";
                        return _strrep;
                    } );
        }
    }
}

#include <loco_common_py.h>
#include <visualizer/loco_visualizer.h>

namespace py = pybind11;

namespace loco
{
    void bindings_visualizer( py::module& m )
    {
        // Bindings for visualizer-related enumerators
        {
            py::enum_<loco::Keys>( m, "Keys", py::arithmetic() )
                .value( "KEY_SPACE", loco::Keys::KEY_SPACE )
                .value( "KEY_APOSTROPHE", loco::Keys::KEY_APOSTROPHE )
                .value( "KEY_COMMA", loco::Keys::KEY_COMMA )
                .value( "KEY_MINUS", loco::Keys::KEY_MINUS )
                .value( "KEY_PERIOD", loco::Keys::KEY_PERIOD )
                .value( "KEY_SLASH", loco::Keys::KEY_SLASH )
                .value( "KEY_0", loco::Keys::KEY_0 )
                .value( "KEY_1", loco::Keys::KEY_1 )
                .value( "KEY_2", loco::Keys::KEY_2 )
                .value( "KEY_3", loco::Keys::KEY_3 )
                .value( "KEY_4", loco::Keys::KEY_4 )
                .value( "KEY_5", loco::Keys::KEY_5 )
                .value( "KEY_6", loco::Keys::KEY_6 )
                .value( "KEY_7", loco::Keys::KEY_7 )
                .value( "KEY_8", loco::Keys::KEY_8 )
                .value( "KEY_9", loco::Keys::KEY_9 )
                .value( "KEY_SEMICOLON", loco::Keys::KEY_SEMICOLON )
                .value( "KEY_EQUAL", loco::Keys::KEY_EQUAL )
                .value( "KEY_A", loco::Keys::KEY_A )
                .value( "KEY_B", loco::Keys::KEY_B )
                .value( "KEY_C", loco::Keys::KEY_C )
                .value( "KEY_D", loco::Keys::KEY_D )
                .value( "KEY_E", loco::Keys::KEY_E )
                .value( "KEY_F", loco::Keys::KEY_F )
                .value( "KEY_G", loco::Keys::KEY_G )
                .value( "KEY_H", loco::Keys::KEY_H )
                .value( "KEY_I", loco::Keys::KEY_I )
                .value( "KEY_J", loco::Keys::KEY_J )
                .value( "KEY_K", loco::Keys::KEY_K )
                .value( "KEY_L", loco::Keys::KEY_L )
                .value( "KEY_M", loco::Keys::KEY_M )
                .value( "KEY_N", loco::Keys::KEY_N )
                .value( "KEY_O", loco::Keys::KEY_O )
                .value( "KEY_P", loco::Keys::KEY_P )
                .value( "KEY_Q", loco::Keys::KEY_Q )
                .value( "KEY_R", loco::Keys::KEY_R )
                .value( "KEY_S", loco::Keys::KEY_S )
                .value( "KEY_T", loco::Keys::KEY_T )
                .value( "KEY_U", loco::Keys::KEY_U )
                .value( "KEY_V", loco::Keys::KEY_V )
                .value( "KEY_W", loco::Keys::KEY_W )
                .value( "KEY_X", loco::Keys::KEY_X )
                .value( "KEY_Y", loco::Keys::KEY_Y )
                .value( "KEY_Z", loco::Keys::KEY_Z )
                .value( "KEY_LEFT_BRACKET", loco::Keys::KEY_LEFT_BRACKET )
                .value( "KEY_BACKSLASH", loco::Keys::KEY_BACKSLASH )
                .value( "KEY_RIGHT_BRACKET", loco::Keys::KEY_RIGHT_BRACKET )
                .value( "KEY_GRAVE_ACCENT", loco::Keys::KEY_GRAVE_ACCENT )
                .value( "KEY_WORLD_1", loco::Keys::KEY_WORLD_1 )
                .value( "KEY_WORLD_2", loco::Keys::KEY_WORLD_2 )
                .value( "KEY_ESCAPE", loco::Keys::KEY_ESCAPE )
                .value( "KEY_ENTER", loco::Keys::KEY_ENTER )
                .value( "KEY_TAB", loco::Keys::KEY_TAB )
                .value( "KEY_BACKSPACE", loco::Keys::KEY_BACKSPACE )
                .value( "KEY_INSERT", loco::Keys::KEY_INSERT )
                .value( "KEY_DELETE", loco::Keys::KEY_DELETE )
                .value( "KEY_RIGHT", loco::Keys::KEY_RIGHT )
                .value( "KEY_LEFT", loco::Keys::KEY_LEFT )
                .value( "KEY_DOWN", loco::Keys::KEY_DOWN )
                .value( "KEY_UP", loco::Keys::KEY_UP )
                .value( "KEY_PAGE_UP", loco::Keys::KEY_PAGE_UP )
                .value( "KEY_PAGE_DOWN", loco::Keys::KEY_PAGE_DOWN )
                .value( "KEY_HOME", loco::Keys::KEY_HOME )
                .value( "KEY_END", loco::Keys::KEY_END )
                .value( "KEY_CAPS_LOCK", loco::Keys::KEY_CAPS_LOCK )
                .value( "KEY_SCROLL_LOCK", loco::Keys::KEY_SCROLL_LOCK )
                .value( "KEY_NUM_LOCK", loco::Keys::KEY_NUM_LOCK )
                .value( "KEY_PRINT_SCREEN", loco::Keys::KEY_PRINT_SCREEN )
                .value( "KEY_PAUSE", loco::Keys::KEY_PAUSE )
                .value( "KEY_F1", loco::Keys::KEY_F1 )
                .value( "KEY_F2", loco::Keys::KEY_F2 )
                .value( "KEY_F3", loco::Keys::KEY_F3 )
                .value( "KEY_F4", loco::Keys::KEY_F4 )
                .value( "KEY_F5", loco::Keys::KEY_F5 )
                .value( "KEY_F6", loco::Keys::KEY_F6 )
                .value( "KEY_F7", loco::Keys::KEY_F7 )
                .value( "KEY_F8", loco::Keys::KEY_F8 )
                .value( "KEY_F9", loco::Keys::KEY_F9 )
                .value( "KEY_F10", loco::Keys::KEY_F10 )
                .value( "KEY_F11", loco::Keys::KEY_F11 )
                .value( "KEY_F12", loco::Keys::KEY_F12 )
                .value( "KEY_F13", loco::Keys::KEY_F13 )
                .value( "KEY_F14", loco::Keys::KEY_F14 )
                .value( "KEY_F15", loco::Keys::KEY_F15 )
                .value( "KEY_F16", loco::Keys::KEY_F16 )
                .value( "KEY_F17", loco::Keys::KEY_F17 )
                .value( "KEY_F18", loco::Keys::KEY_F18 )
                .value( "KEY_F19", loco::Keys::KEY_F19 )
                .value( "KEY_F20", loco::Keys::KEY_F20 )
                .value( "KEY_F21", loco::Keys::KEY_F21 )
                .value( "KEY_F22", loco::Keys::KEY_F22 )
                .value( "KEY_F23", loco::Keys::KEY_F23 )
                .value( "KEY_F24", loco::Keys::KEY_F24 )
                .value( "KEY_F25", loco::Keys::KEY_F25 )
                .value( "KEY_KP_0", loco::Keys::KEY_KP_0 )
                .value( "KEY_KP_1", loco::Keys::KEY_KP_1 )
                .value( "KEY_KP_2", loco::Keys::KEY_KP_2 )
                .value( "KEY_KP_3", loco::Keys::KEY_KP_3 )
                .value( "KEY_KP_4", loco::Keys::KEY_KP_4 )
                .value( "KEY_KP_5", loco::Keys::KEY_KP_5 )
                .value( "KEY_KP_6", loco::Keys::KEY_KP_6 )
                .value( "KEY_KP_7", loco::Keys::KEY_KP_7 )
                .value( "KEY_KP_8", loco::Keys::KEY_KP_8 )
                .value( "KEY_KP_9", loco::Keys::KEY_KP_9 )
                .value( "KEY_KP_DECIMAL", loco::Keys::KEY_KP_DECIMAL )
                .value( "KEY_KP_DIVIDE", loco::Keys::KEY_KP_DIVIDE )
                .value( "KEY_KP_MULTIPLY", loco::Keys::KEY_KP_MULTIPLY )
                .value( "KEY_KP_SUBTRACT", loco::Keys::KEY_KP_SUBTRACT )
                .value( "KEY_KP_ADD", loco::Keys::KEY_KP_ADD )
                .value( "KEY_KP_ENTER", loco::Keys::KEY_KP_ENTER )
                .value( "KEY_KP_EQUAL", loco::Keys::KEY_KP_EQUAL )
                .value( "KEY_LEFT_SHIFT", loco::Keys::KEY_LEFT_SHIFT )
                .value( "KEY_LEFT_CONTROL", loco::Keys::KEY_LEFT_CONTROL )
                .value( "KEY_LEFT_ALT", loco::Keys::KEY_LEFT_ALT )
                .value( "KEY_LEFT_SUPER", loco::Keys::KEY_LEFT_SUPER )
                .value( "KEY_RIGHT_SHIFT", loco::Keys::KEY_RIGHT_SHIFT )
                .value( "KEY_RIGHT_CONTROL", loco::Keys::KEY_RIGHT_CONTROL )
                .value( "KEY_RIGHT_ALT", loco::Keys::KEY_RIGHT_ALT )
                .value( "KEY_RIGHT_SUPER", loco::Keys::KEY_RIGHT_SUPER )
                .value( "KEY_MENU", loco::Keys::KEY_MENU );

            py::enum_<loco::eVizCameraType>( m, "VizCameraType", py::arithmetic() )
                .value( "ORBIT", loco::eVizCameraType::ORBIT )
                .value( "FPS", loco::eVizCameraType::FPS )
                .value( "FIXED", loco::eVizCameraType::FIXED );

            py::enum_<loco::eVizLightType>( m, "VizLightType", py::arithmetic() )
                .value( "DIRECTIONAL", loco::eVizLightType::DIRECTIONAL )
                .value( "POINT", loco::eVizLightType::POINT )
                .value( "SPOT", loco::eVizLightType::SPOT );
        }

        // Bindings for TDrawable class (user-exposed drawable)
        {
            py::class_<TDrawable, TObject>( m, "Drawable" )
                .def( py::init<const std::string&, const TVisualData&>() )
                .def_property( "visible",
                    []( const TDrawable* self ) -> bool
                        {
                            return self->visible();
                        },
                    []( TDrawable* self, bool visible ) -> void
                        {
                            self->SetVisible( visible );
                        } )
                .def_property( "wireframe",
                    []( const TDrawable* self ) -> bool
                        {
                            return self->wireframe();
                        },
                    []( TDrawable* self, bool wireframe ) -> void
                        {
                            self->SetWireframe( wireframe );
                        } )
                .def_property( "local_pos",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->local_pos() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_local_pos )
                        {
                            self->SetLocalPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_local_pos ) );
                        } )
                .def_property( "local_rot",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->local_rot() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_local_rot )
                        {
                            self->SetLocalRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_local_rot ) );
                        } )
                .def_property( "local_euler",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->local_euler() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_local_euler )
                        {
                            self->SetLocalEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_local_euler ) );
                        } )
                .def_property( "local_quat",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->local_quat() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_local_quat )
                        {
                            self->SetLocalQuat( tinymath::nparray_to_vector<TScalar, 4>( arr_local_quat ) );
                        } )
                .def_property( "local_tf",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->local_tf() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_local_tf )
                        {
                            self->SetLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ) );
                        } )
                .def_property( "size",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->size() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_size )
                        {
                            self->ChangeSize( tinymath::nparray_to_vector<TScalar, 3>( arr_size ) );
                        } )
                .def_property( "heights",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return loco::vecgrid_to_nparray<float>( self->data().hfield_data.heights );
                        },
                    []( TDrawable* self, const py::array_t<float>& arr_heights )
                        {
                            self->ChangeElevationData( loco::nparray_to_vecgrid<float>( arr_heights ) );
                        } )
                .def_property( "ambient",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->ambient() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_ambient )
                        {
                            self->ChangeAmbientColor( tinymath::nparray_to_vector<TScalar, 3>( arr_ambient ) );
                        } )
                .def_property( "diffuse",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->diffuse() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_diffuse )
                        {
                            self->ChangeDiffuseColor( tinymath::nparray_to_vector<TScalar, 3>( arr_diffuse ) );
                        } )
                .def_property( "specular",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->specular() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_specular )
                        {
                            self->ChangeSpecularColor( tinymath::nparray_to_vector<TScalar, 3>( arr_specular ) );
                        } )
                .def_property( "shininess",
                    []( const TDrawable* self ) -> TScalar
                        {
                            return self->shininess();
                        },
                    []( TDrawable* self, TScalar shininess )
                        {
                            self->ChangeShininess( shininess );
                        } )
                .def_property( "texture",
                    []( const TDrawable* self ) -> std::string
                        {
                            return self->texture();
                        },
                    []( TDrawable* self, const std::string& texture ) -> void
                        {
                            self->ChangeTexture( texture );
                        } )
                .def( "__repr__", []( const TDrawable* self )
                    {
                        auto _strrep = std::string( "Collision(\n" );
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "shape       : " + loco::ToString( self->shape() ) + "\n";
                        _strrep += "size        : " + loco::ToString( self->size() ) + "\n";
                        _strrep += "ambient     : " + loco::ToString( self->ambient() ) + "\n";
                        _strrep += "diffuse     : " + loco::ToString( self->diffuse() ) + "\n";
                        _strrep += "specular    : " + loco::ToString( self->specular() ) + "\n";
                        _strrep += "shininess   : " + std::to_string( self->shininess() ) + "\n";
                        _strrep += "parent      : " + ( self->parent() ? loco::PointerToHexAddress( self->parent() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::ToString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::ToString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::ToString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::ToString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::ToString( self->tf() ) + "\n";
                        _strrep += "localPos    : " + loco::ToString( self->local_pos() ) + "\n";
                        _strrep += "localRot    :\n" + loco::ToString( self->local_rot() ) + "\n";
                        _strrep += "localEuler  : " + loco::ToString( self->local_euler() ) + "\n";
                        _strrep += "localQuat   : " + loco::ToString( self->local_quat() ) + "\n";
                        _strrep += "localTf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for TVizCamera class (user-exposed camera)
        {
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
        }

        // Bindings for TVizLight class (user-exposed light)
        {
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
        }

        // Bindings for TIVisualizer interface
        {
            py::class_<TIVisualizer>( m, "IVisualizer" )
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
        }

        // Bindings for TNullVisualizer (default null-object in case no backend available)
        {
            py::class_<TNullVisualizer, TIVisualizer>( m, "NullVisualizer" )
                .def( py::init<TScenario*,size_t, size_t, bool, bool>(), 
                      py::arg( "scenario" ),
                      py::arg( "windowWidth" ) = 1024,
                      py::arg( "windowHeight" ) = 768,
                      py::arg( "windowResizable" ) = true,
                      py::arg( "renderOffscreen" ) = false ); // no ownership nor nurse-patient required
        }
    }
}
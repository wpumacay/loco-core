
#include <loco_common_py.h>
#include <components/loco_data.h>
#include <components/loco_collision.h>
#include <components/loco_visual.h>
#include <components/loco_joint.h>
//// #include <components/loco_sensors.h>
//// #include <components/loco_actuators.h>
#include <components/loco_body_base.h>

namespace py = pybind11;

namespace loco
{
    void bindings_components_elements( py::module& m )
    {
        // Bindings for TCollision class
        {
            py::class_< TCollision >( m, "Collision" )
                .def( py::init<const std::string&, const TCollisionData&>() )
                .def( "Initialize", &TCollision::Initialize )
                .def( "PreStep", &TCollision::PreStep )
                .def( "PostStep", &TCollision::PostStep )
                .def( "Reset", &TCollision::Reset )
                .def( "data", []( TCollision* self ) -> TCollisionData&
                    {
                        return self->data();
                    }, py::return_value_policy::reference )
                .def( "parent", []( TCollision* self ) -> TIBody*
                        {
                            return self->parent();
                        }, py::return_value_policy::reference )
                .def( "ChangeElevationData", []( TCollision* self, const py::array_t<float>& arr_heights )
                    {
                        self->ChangeElevationData( loco::nparray_to_vecgrid<float>( arr_heights ) );
                    } )
                .def_property( "size",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->size() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_size )
                        {
                            self->ChangeSize( tinymath::nparray_to_vector<TScalar, 3>( arr_size ) );
                        } )
                .def_property( "visible",
                    []( const TCollision* self )
                        {
                            return self->visible();
                        },
                    []( TCollision* self, bool visible )
                        {
                            self->SetVisible( visible );
                        } )
                .def_property( "wireframe",
                    []( const TCollision* self )
                        {
                            return self->wireframe();
                        },
                    []( TCollision* self, bool wireframe )
                        {
                            self->SetWireframe( wireframe );
                        } )
                .def_property( "collisionGroup",
                    []( const TCollision* self )
                        {
                            return self->collisionGroup();
                        },
                    []( TCollision* self, int collisionGroup )
                        {
                            self->ChangeCollisionGroup( collisionGroup );
                        } )
                .def_property( "collisionMask",
                    []( const TCollision* self )
                        {
                            return self->collisionMask();
                        },
                    []( TCollision* self, int collisionMask )
                        {
                            self->ChangeCollisionMask( collisionMask );
                        } )
                .def_property( "localPos",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localPos() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localPos ) -> void
                        {
                            self->SetLocalPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_localPos ) );
                        } )
                .def_property( "localRot",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->localRot() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localRot ) -> void
                        {
                            self->SetLocalRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_localRot ) );
                        } )
                .def_property( "localEuler",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localEuler() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localEuler ) -> void
                        {
                            self->SetLocalEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_localEuler ) );
                        } )
                .def_property( "localQuat",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->localQuat() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localQuat ) -> void
                        {
                            self->SetLocalQuat( tinymath::nparray_to_vector<TScalar, 4>( arr_localQuat ) );
                        } )
                .def_property( "localTf",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTf() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localTf )
                        {
                            self->SetLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf ) );
                        } )
                .def_property_readonly( "name", &TCollision::name )
                .def_property_readonly( "shape", &TCollision::shape )
                .def_property_readonly( "pos", []( const TCollision* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->pos() );
                    } )
                .def_property_readonly( "rot", []( const TCollision* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 3>( self->rot() );
                    } )
                .def_property_readonly( "euler", []( const TCollision* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->euler() );
                    } )
                .def_property_readonly( "quat", []( const TCollision* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 4>( self->quat() );
                    } )
                .def_property_readonly( "tf", []( const TCollision* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 4>( self->tf() );
                    } )
                .def( "__repr__", []( const TCollision* self )
                    {
                        auto _strrep = std::string( "Collision(\n" );
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "shape       : " + loco::ToString( self->shape() ) + "\n";
                        _strrep += "size        : " + loco::ToString( self->size() ) + "\n";
                        _strrep += "colGroup    : " + std::to_string( self->collisionGroup() ) + "\n";
                        _strrep += "colMask     : " + std::to_string( self->collisionMask() ) + "\n";
                        _strrep += "parent      : " + ( self->parent() ? loco::PointerToHexAddress( self->parent() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::ToString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::ToString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::ToString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::ToString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::ToString( self->tf() ) + "\n";
                        _strrep += "localPos    : " + loco::ToString( self->localPos() ) + "\n";
                        _strrep += "localRot    :\n" + loco::ToString( self->localRot() ) + "\n";
                        _strrep += "localEuler  : " + loco::ToString( self->localEuler() ) + "\n";
                        _strrep += "localQuat   : " + loco::ToString( self->localQuat() ) + "\n";
                        _strrep += "localTf     :\n" + loco::ToString( self->localTf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for TDrawable class
        {
            py::class_< TDrawable >( m, "Visual" )
                .def( py::init<const std::string&, const TVisualData&>() )
                .def( "Initialize", &TDrawable::Initialize )
                .def( "PreStep", &TDrawable::PreStep )
                .def( "PostStep", &TDrawable::PostStep )
                .def( "Reset", &TDrawable::Reset )
                .def( "data", []( TDrawable* self ) -> TVisualData&
                    {
                        return self->data();
                    }, py::return_value_policy::reference )
                .def( "parent", []( TDrawable* self ) -> TIBody*
                    {
                        return self->parent();
                    }, py::return_value_policy::reference )
                .def( "ChangeElevationData", []( TDrawable* self, const py::array_t<float>& arr_heights )
                    {
                        self->ChangeElevationData( loco::nparray_to_vecgrid<float>( arr_heights ) );
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
                .def_property( "visible",
                    []( const TDrawable* self )
                        {
                            return self->visible();
                        },
                    []( TDrawable* self, bool visible )
                        {
                            self->SetVisible( visible );
                        } )
                .def_property( "wireframe",
                    []( const TDrawable* self )
                        {
                            return self->wireframe();
                        },
                    []( TDrawable* self, bool wireframe )
                        {
                            self->SetWireframe( wireframe );
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
                .def_property( "localPos",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localPos() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_localPos ) -> void
                        {
                            self->SetLocalPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_localPos ) );
                        } )
                .def_property( "localRot",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->localRot() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_localRot ) -> void
                        {
                            self->SetLocalRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_localRot ) );
                        } )
                .def_property( "localEuler",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localEuler() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_localEuler ) -> void
                        {
                            self->SetLocalEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_localEuler ) );
                        } )
                .def_property( "localQuat",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->localQuat() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_localQuat ) -> void
                        {
                            self->SetLocalQuat( tinymath::nparray_to_vector<TScalar, 4>( arr_localQuat ) );
                        } )
                .def_property( "localTf",
                    []( const TDrawable* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTf() );
                        },
                    []( TDrawable* self, const py::array_t<TScalar>& arr_localTf )
                        {
                            self->SetLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf ) );
                        } )
                .def_property_readonly( "name", &TDrawable::name )
                .def_property_readonly( "shape", &TDrawable::shape )
                .def_property_readonly( "pos", []( const TDrawable* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->pos() );
                    } )
                .def_property_readonly( "rot", []( const TDrawable* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 3>( self->rot() );
                    } )
                .def_property_readonly( "euler", []( const TDrawable* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->euler() );
                    } )
                .def_property_readonly( "quat", []( const TDrawable* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 4>( self->quat() );
                    } )
                .def_property_readonly( "tf", []( const TDrawable* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 4>( self->tf() );
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
                        _strrep += "localPos    : " + loco::ToString( self->localPos() ) + "\n";
                        _strrep += "localRot    :\n" + loco::ToString( self->localRot() ) + "\n";
                        _strrep += "localEuler  : " + loco::ToString( self->localEuler() ) + "\n";
                        _strrep += "localQuat   : " + loco::ToString( self->localQuat() ) + "\n";
                        _strrep += "localTf     :\n" + loco::ToString( self->localTf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for TIBody class
        {
            py::class_< TIBody >( m, "IBody" )
                .def( "Initialize", &TIBody::Initialize )
                .def( "PreStep", &TIBody::PreStep )
                .def( "PostStep", &TIBody::PostStep )
                .def( "Reset", &TIBody::Reset )
                .def( "data", []( TIBody* self ) -> TBodyData&
                    {
                        return self->data();
                    }, py::return_value_policy::reference )
                .def_property( "collision",
                    []( TIBody* self ) -> TCollision*
                        {
                            return self->collision();
                        },
                    []( TIBody* self, std::unique_ptr<TCollision> collision )
                        {
                            self->SetCollision( std::move( collision ) );
                        },
                    py::return_value_policy::reference, py::keep_alive<1, 0>() )
                .def_property( "visual",
                    []( TIBody* self ) -> TDrawable*
                        {
                            return self->visual();
                        },
                    []( TIBody* self, std::unique_ptr<TDrawable> visual )
                        {
                            self->SetVisual( std::move( visual ) );
                        },
                    py::return_value_policy::reference, py::keep_alive<1, 0>() )
                .def_property( "pos",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->pos() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_pos )
                        {
                            self->SetPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_pos ) );
                        } )
                .def_property( "rot",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->rot() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_rot )
                        {
                            self->SetRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_rot ) );
                        } )
                .def_property( "euler",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->euler() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_euler )
                        {
                            self->SetEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_euler ) );
                        } )
                .def_property( "quat",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->quat() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_quat )
                        {
                            self->SetQuaternion( tinymath::nparray_to_vector<TScalar, 4>( arr_quat ) );
                        } )
                .def_property( "tf",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->tf() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_tf )
                        {
                            self->SetTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_tf ) );
                        } )
                .def_property( "localPos",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localPos() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_localPos )
                        {
                            self->SetLocalPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_localPos ) );
                        } )
                .def_property( "localRot",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->localRot() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_localRot )
                        {
                            self->SetLocalRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_localRot ) );
                        } )
                .def_property( "localEuler",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localEuler() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_localEuler )
                        {
                            self->SetLocalEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_localEuler ) );
                        } )
                .def_property( "localQuat",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->localQuat() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_localQuat )
                        {
                            self->SetLocalQuaternion( tinymath::nparray_to_vector<TScalar, 4>( arr_localQuat ) );
                        } )
                .def_property( "localTf",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTf() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_localTf )
                        {
                            self->SetLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf ) );
                        } )
                .def_property_readonly( "localPos0",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localPos0() );
                        } )
                .def_property_readonly( "localRot0",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->localRot0() );
                        } )
                .def_property_readonly( "localEuler0",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localEuler0() );
                        } )
                .def_property_readonly( "localQuat0",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->localQuat0() );
                        } )
                .def_property_readonly( "name", &TIBody::name )
                .def_property_readonly( "classType", &TIBody::classType )
                .def_property_readonly( "dyntype", &TIBody::dyntype )
                .def( "__repr__", []( const TIBody* self )
                    {
                        auto _strrep = std::string( "Body(\n" );
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "classType   : " + loco::ToString( self->classType() ) + "\n";
                        _strrep += "dyntype     : " + loco::ToString( self->dyntype() ) + "\n";
                        _strrep += "collision   : " + ( self->collision() ? loco::PointerToHexAddress( self->collision() ) : std::string( "null" ) ) + "\n";
                        _strrep += "visual      : " + ( self->visual() ? loco::PointerToHexAddress( self->visual() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::ToString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::ToString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::ToString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::ToString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::ToString( self->tf() ) + "\n";
                        _strrep += "localPos    : " + loco::ToString( self->localPos() ) + "\n";
                        _strrep += "localRot    :\n" + loco::ToString( self->localRot() ) + "\n";
                        _strrep += "localEuler  : " + loco::ToString( self->localEuler() ) + "\n";
                        _strrep += "localQuat   : " + loco::ToString( self->localQuat() ) + "\n";
                        _strrep += "localTf     :\n" + loco::ToString( self->localTf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }
    }
}
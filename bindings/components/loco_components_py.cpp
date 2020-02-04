
#include <loco_common_py.h>
#include <components/data.h>
#include <components/collision.h>
#include <components/visual.h>
#include <components/joint.h>
//// #include <components/sensors.h>
//// #include <components/actuators.h>
#include <components/body.h>

namespace py = pybind11;

namespace loco
{
    void bindings_components_elements( py::module& m )
    {
        // Bindings for TCollision class
        {
            py::class_< TCollision >( m, "Collision" )
                .def( py::init<const std::string&, const TCollisionData&>() )
                .def( "preStep", &TCollision::preStep )
                .def( "postStep", &TCollision::postStep )
                .def( "reset", &TCollision::reset )
                .def( "data", &TCollision::data )
                .def( "parent", []( TCollision* self ) -> TIBody*
                        {
                            return self->parent();
                        }, py::return_value_policy::reference )
                .def( "changeElevationData", []( TCollision* self, const py::array_t<float>& arr_heights )
                    {
                        self->changeElevationData( loco::nparray_to_vecgrid<float>( arr_heights ) );
                    } )
                .def_property( "size",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->size() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_size )
                        {
                            self->changeSize( tinymath::nparray_to_vector<TScalar, 3>( arr_size ) );
                        } )
                .def_property( "visible",
                    []( const TCollision* self )
                        {
                            return self->isVisible();
                        },
                    []( TCollision* self, bool visible )
                        {
                            self->show( visible );
                        } )
                .def_property( "wireframe",
                    []( const TCollision* self )
                        {
                            return self->isWireframe();
                        },
                    []( TCollision* self, bool wireframe )
                        {
                            self->wireframe( wireframe );
                        } )
                .def_property( "collisionGroup",
                    []( const TCollision* self )
                        {
                            return self->collisionGroup();
                        },
                    []( TCollision* self, int collisionGroup )
                        {
                            self->changeCollisionGroup( collisionGroup );
                        } )
                .def_property( "collisionMask",
                    []( const TCollision* self )
                        {
                            return self->collisionMask();
                        },
                    []( TCollision* self, int collisionMask )
                        {
                            self->changeCollisionMask( collisionMask );
                        } )
                .def_property( "localPos",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localPos() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localPos ) -> void
                        {
                            self->setLocalPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_localPos ) );
                        } )
                .def_property( "localRot",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->localRot() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localRot ) -> void
                        {
                            self->setLocalRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_localRot ) );
                        } )
                .def_property( "localEuler",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localEuler() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localEuler ) -> void
                        {
                            self->setLocalEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_localEuler ) );
                        } )
                .def_property( "localQuat",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->localQuat() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localQuat ) -> void
                        {
                            self->setLocalQuat( tinymath::nparray_to_vector<TScalar, 4>( arr_localQuat ) );
                        } )
                .def_property( "localTf",
                    []( const TCollision* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTf() );
                        },
                    []( TCollision* self, const py::array_t<TScalar>& arr_localTf )
                        {
                            self->setLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf ) );
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
                        _strrep += "cpp-address : " + loco::pointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "shape       : " + loco::toString( self->shape() ) + "\n";
                        _strrep += "size        : " + loco::toString( self->size() ) + "\n";
                        _strrep += "colGroup    : " + std::to_string( self->collisionGroup() ) + "\n";
                        _strrep += "colMask     : " + std::to_string( self->collisionMask() ) + "\n";
                        _strrep += "parent      : " + ( self->parent() ? loco::pointerToHexAddress( self->parent() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::toString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::toString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::toString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::toString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::toString( self->tf() ) + "\n";
                        _strrep += "localPos    : " + loco::toString( self->localPos() ) + "\n";
                        _strrep += "localRot    :\n" + loco::toString( self->localRot() ) + "\n";
                        _strrep += "localEuler  : " + loco::toString( self->localEuler() ) + "\n";
                        _strrep += "localQuat   : " + loco::toString( self->localQuat() ) + "\n";
                        _strrep += "localTf     :\n" + loco::toString( self->localTf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for TVisual class
        {
            py::class_< TVisual >( m, "Visual" )
                .def( py::init<const std::string&, const TVisualData&>() )
                .def( "preStep", &TVisual::preStep )
                .def( "postStep", &TVisual::postStep )
                .def( "reset", &TVisual::reset )
                .def( "data", &TVisual::data )
                .def( "parent", []( TVisual* self ) -> TIBody*
                    {
                        return self->parent();
                    }, py::return_value_policy::reference )
                .def( "changeElevationData", []( TVisual* self, const py::array_t<float>& arr_heights )
                    {
                        self->changeElevationData( loco::nparray_to_vecgrid<float>( arr_heights ) );
                    } )
                .def_property( "size",
                    []( const TVisual* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->size() );
                        },
                    []( TVisual* self, const py::array_t<TScalar>& arr_size )
                        {
                            self->changeSize( tinymath::nparray_to_vector<TScalar, 3>( arr_size ) );
                        } )
                .def_property( "visible",
                    []( const TVisual* self )
                        {
                            return self->isVisible();
                        },
                    []( TVisual* self, bool visible )
                        {
                            self->show( visible );
                        } )
                .def_property( "wireframe",
                    []( const TVisual* self )
                        {
                            return self->isWireframe();
                        },
                    []( TVisual* self, bool wireframe )
                        {
                            self->wireframe( wireframe );
                        } )
                .def_property( "ambient",
                    []( const TVisual* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->ambient() );
                        },
                    []( TVisual* self, const py::array_t<TScalar>& arr_ambient )
                        {
                            self->changeAmbientColor( tinymath::nparray_to_vector<TScalar, 3>( arr_ambient ) );
                        } )
                .def_property( "diffuse",
                    []( const TVisual* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->diffuse() );
                        },
                    []( TVisual* self, const py::array_t<TScalar>& arr_diffuse )
                        {
                            self->changeDiffuseColor( tinymath::nparray_to_vector<TScalar, 3>( arr_diffuse ) );
                        } )
                .def_property( "specular",
                    []( const TVisual* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->specular() );
                        },
                    []( TVisual* self, const py::array_t<TScalar>& arr_specular )
                        {
                            self->changeSpecularColor( tinymath::nparray_to_vector<TScalar, 3>( arr_specular ) );
                        } )
                .def_property( "shininess",
                    []( const TVisual* self ) -> TScalar
                        {
                            return self->shininess();
                        },
                    []( TVisual* self, TScalar shininess )
                        {
                            self->changeShininess( shininess );
                        } )
                .def_property( "localPos",
                    []( const TVisual* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localPos() );
                        },
                    []( TVisual* self, const py::array_t<TScalar>& arr_localPos ) -> void
                        {
                            self->setLocalPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_localPos ) );
                        } )
                .def_property( "localRot",
                    []( const TVisual* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->localRot() );
                        },
                    []( TVisual* self, const py::array_t<TScalar>& arr_localRot ) -> void
                        {
                            self->setLocalRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_localRot ) );
                        } )
                .def_property( "localEuler",
                    []( const TVisual* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localEuler() );
                        },
                    []( TVisual* self, const py::array_t<TScalar>& arr_localEuler ) -> void
                        {
                            self->setLocalEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_localEuler ) );
                        } )
                .def_property( "localQuat",
                    []( const TVisual* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->localQuat() );
                        },
                    []( TVisual* self, const py::array_t<TScalar>& arr_localQuat ) -> void
                        {
                            self->setLocalQuat( tinymath::nparray_to_vector<TScalar, 4>( arr_localQuat ) );
                        } )
                .def_property( "localTf",
                    []( const TVisual* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTf() );
                        },
                    []( TVisual* self, const py::array_t<TScalar>& arr_localTf )
                        {
                            self->setLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf ) );
                        } )
                .def_property_readonly( "name", &TVisual::name )
                .def_property_readonly( "shape", &TVisual::shape )
                .def_property_readonly( "pos", []( const TVisual* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->pos() );
                    } )
                .def_property_readonly( "rot", []( const TVisual* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 3>( self->rot() );
                    } )
                .def_property_readonly( "euler", []( const TVisual* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->euler() );
                    } )
                .def_property_readonly( "quat", []( const TVisual* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 4>( self->quat() );
                    } )
                .def_property_readonly( "tf", []( const TVisual* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 4>( self->tf() );
                    } )
                .def( "__repr__", []( const TVisual* self )
                    {
                        auto _strrep = std::string( "Collision(\n" );
                        _strrep += "cpp-address : " + loco::pointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "shape       : " + loco::toString( self->shape() ) + "\n";
                        _strrep += "size        : " + loco::toString( self->size() ) + "\n";
                        _strrep += "ambient     : " + loco::toString( self->ambient() ) + "\n";
                        _strrep += "diffuse     : " + loco::toString( self->diffuse() ) + "\n";
                        _strrep += "specular    : " + loco::toString( self->specular() ) + "\n";
                        _strrep += "shininess   : " + std::to_string( self->shininess() ) + "\n";
                        _strrep += "parent      : " + ( self->parent() ? loco::pointerToHexAddress( self->parent() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::toString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::toString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::toString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::toString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::toString( self->tf() ) + "\n";
                        _strrep += "localPos    : " + loco::toString( self->localPos() ) + "\n";
                        _strrep += "localRot    :\n" + loco::toString( self->localRot() ) + "\n";
                        _strrep += "localEuler  : " + loco::toString( self->localEuler() ) + "\n";
                        _strrep += "localQuat   : " + loco::toString( self->localQuat() ) + "\n";
                        _strrep += "localTf     :\n" + loco::toString( self->localTf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for TJoint class
        {
            py::class_< TJoint >( m, "Joint" )
                .def( py::init<const std::string&, const TJointData&>() )
                .def( "preStep", &TJoint::preStep )
                .def( "postStep", &TJoint::postStep )
                .def( "reset", &TJoint::reset )
                .def( "data", &TJoint::data )
                .def( "owner", []( TJoint* self ) -> TIBody*
                    {
                        return self->owner();
                    }, py::return_value_policy::reference )
                .def( "ownerParent", []( TJoint* self ) -> TIBody*
                    {
                        return self->ownerParent();
                    }, py::return_value_policy::reference )
                .def_property( "limits",
                    []( const TJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 2>( self->limits() );
                        },
                    []( TJoint* self, const py::array_t<TScalar>& arr_limits )
                        {
                            self->changeLimits( tinymath::nparray_to_vector<TScalar, 2>( arr_limits ) );
                        } )
                .def_property( "qpos",
                    []( const TJoint* self ) -> py::array_t<TScalar>
                        {
                            return loco::stdvec_to_nparray( self->getQpos() );
                        },
                    []( TJoint* self, const py::array_t<TScalar>& arr_qpos )
                        {
                            self->setQpos( loco::nparray_to_stdvec( arr_qpos ) );
                        } )
                .def_property( "qvel",
                    []( const TJoint* self ) -> py::array_t<TScalar>
                        {
                            return loco::stdvec_to_nparray( self->getQvel() );
                        },
                    []( TJoint* self, const py::array_t<TScalar>& arr_qvel )
                        {
                            self->setQvel( loco::nparray_to_stdvec( arr_qvel ) );
                        } )
                .def_property( "localPos",
                    []( const TJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localPos() );
                        },
                    []( TJoint* self, const py::array_t<TScalar>& arr_localPos ) -> void
                        {
                            self->setLocalPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_localPos ) );
                        } )
                .def_property( "localRot",
                    []( const TJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->localRot() );
                        },
                    []( TJoint* self, const py::array_t<TScalar>& arr_localRot ) -> void
                        {
                            self->setLocalRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_localRot ) );
                        } )
                .def_property( "localEuler",
                    []( const TJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localEuler() );
                        },
                    []( TJoint* self, const py::array_t<TScalar>& arr_localEuler ) -> void
                        {
                            self->setLocalEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_localEuler ) );
                        } )
                .def_property( "localQuat",
                    []( const TJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->localQuat() );
                        },
                    []( TJoint* self, const py::array_t<TScalar>& arr_localQuat ) -> void
                        {
                            self->setLocalQuat( tinymath::nparray_to_vector<TScalar, 4>( arr_localQuat ) );
                        } )
                .def_property( "localTf",
                    []( const TJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTf() );
                        },
                    []( TJoint* self, const py::array_t<TScalar>& arr_localTf )
                        {
                            self->setLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf ) );
                        } )
                .def_property_readonly( "name", &TJoint::name )
                .def_property_readonly( "type", &TJoint::type )
                .def_property_readonly( "axis", []( const TJoint* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->axis() );
                    } )
                .def_property_readonly( "qpos0", []( const TJoint* self ) -> py::array_t<TScalar>
                    {
                        return loco::stdvec_to_nparray( self->getQpos0() );
                    } )
                .def_property_readonly( "qvel0", []( const TJoint* self ) -> py::array_t<TScalar>
                    {
                        return loco::stdvec_to_nparray( self->getQvel0() );
                    } )
                .def_property_readonly( "pos", []( const TJoint* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->pos() );
                    } )
                .def_property_readonly( "rot", []( const TJoint* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 3>( self->rot() );
                    } )
                .def_property_readonly( "euler", []( const TJoint* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->euler() );
                    } )
                .def_property_readonly( "quat", []( const TJoint* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 4>( self->quat() );
                    } )
                .def_property_readonly( "tf", []( const TJoint* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 4>( self->tf() );
                    } )
                .def( "__repr__", []( const TJoint* self )
                    {
                        auto _strrep = std::string( "Joint(\n" );
                        _strrep += "cpp-address : " + loco::pointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "type        : " + loco::toString( self->type() ) + "\n";
                        _strrep += "axis        : " + loco::toString( self->axis() ) + "\n";
                        _strrep += "limits      : " + loco::toString( self->limits() ) + "\n";
                        _strrep += "qpos        : " + loco::toString( self->getQpos() ) + "\n";
                        _strrep += "qvel        : " + loco::toString( self->getQvel() ) + "\n";
                        _strrep += "qpos0       : " + loco::toString( self->getQpos0() ) + "\n";
                        _strrep += "qvel0       : " + loco::toString( self->getQvel0() ) + "\n";
                        _strrep += "owner       : " + ( self->owner() ? loco::pointerToHexAddress( self->owner() ) : std::string( "null" ) ) + "\n";
                        _strrep += "ownerParent : " + ( self->ownerParent() ? loco::pointerToHexAddress( self->ownerParent() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::toString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::toString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::toString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::toString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::toString( self->tf() ) + "\n";
                        _strrep += "localPos    : " + loco::toString( self->localPos() ) + "\n";
                        _strrep += "localRot    :\n" + loco::toString( self->localRot() ) + "\n";
                        _strrep += "localEuler  : " + loco::toString( self->localEuler() ) + "\n";
                        _strrep += "localQuat   : " + loco::toString( self->localQuat() ) + "\n";
                        _strrep += "localTf     :\n" + loco::toString( self->localTf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for TIBody class
        {
            py::class_< TIBody >( m, "IBody" )
                .def( "preStep", &TIBody::preStep )
                .def( "postStep", &TIBody::postStep )
                .def( "reset", &TIBody::reset )
                .def( "data", &TIBody::data )
                .def_property( "collision",
                    []( TIBody* self ) -> TCollision*
                        {
                            return self->collision();
                        },
                    []( TIBody* self, std::unique_ptr<TCollision> collision )
                        {
                            self->setCollision( std::move( collision ) );
                        },
                    py::return_value_policy::reference, py::keep_alive<1, 0>() )
                .def_property( "visual",
                    []( TIBody* self ) -> TVisual*
                        {
                            return self->visual();
                        },
                    []( TIBody* self, std::unique_ptr<TVisual> visual )
                        {
                            self->setVisual( std::move( visual ) );
                        },
                    py::return_value_policy::reference, py::keep_alive<1, 0>() )
                .def_property( "pos",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->pos() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_pos )
                        {
                            self->setPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_pos ) );
                        } )
                .def_property( "rot",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->rot() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_rot )
                        {
                            self->setRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_rot ) );
                        } )
                .def_property( "euler",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->euler() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_euler )
                        {
                            self->setEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_euler ) );
                        } )
                .def_property( "quat",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->quat() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_quat )
                        {
                            self->setQuaternion( tinymath::nparray_to_vector<TScalar, 4>( arr_quat ) );
                        } )
                .def_property( "localPos",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localPos() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_localPos )
                        {
                            self->setLocalPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_localPos ) );
                        } )
                .def_property( "localRot",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->localRot() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_localRot )
                        {
                            self->setLocalRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_localRot ) );
                        } )
                .def_property( "localEuler",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->localEuler() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_localEuler )
                        {
                            self->setLocalEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_localEuler ) );
                        } )
                .def_property( "localQuat",
                    []( const TIBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->localQuat() );
                        },
                    []( TIBody* self, const py::array_t<TScalar>& arr_localQuat )
                        {
                            self->setLocalQuaternion( tinymath::nparray_to_vector<TScalar, 4>( arr_localQuat ) );
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
                        _strrep += "cpp-address : " + loco::pointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "classType   : " + loco::toString( self->classType() ) + "\n";
                        _strrep += "dyntype     : " + loco::toString( self->dyntype() ) + "\n";
                        _strrep += "collision   : " + ( self->collision() ? loco::pointerToHexAddress( self->collision() ) : std::string( "null" ) ) + "\n";
                        _strrep += "visual      : " + ( self->visual() ? loco::pointerToHexAddress( self->visual() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::toString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::toString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::toString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::toString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::toString( self->tf() ) + "\n";
                        _strrep += "localPos    : " + loco::toString( self->localPos() ) + "\n";
                        _strrep += "localRot    :\n" + loco::toString( self->localRot() ) + "\n";
                        _strrep += "localEuler  : " + loco::toString( self->localEuler() ) + "\n";
                        _strrep += "localQuat   : " + loco::toString( self->localQuat() ) + "\n";
                        _strrep += "localTf     :\n" + loco::toString( self->localTf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }
    }
}
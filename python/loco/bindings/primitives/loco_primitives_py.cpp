
#include <loco_common_py.h>
#include <primitives/loco_single_body.h>

namespace py = pybind11;

namespace loco
{
    void bindings_primitives( py::module& m )
    {
        // Bindings for TSingleBodyCollider class
        {
            py::class_< TSingleBodyCollider, TObject >( m, "SingleBodyCollider" )
                .def( py::init<const std::string&, const TCollisionData&>() )
                .def( "data", []( TSingleBodyCollider* self ) -> TCollisionData&
                    {
                        return self->data();
                    }, py::return_value_policy::reference )
                .def( "parent", []( TSingleBodyCollider* self ) -> TSingleBody*
                        {
                            return self->parent();
                        }, py::return_value_policy::reference )
                .def_property( "visible",
                    []( const TSingleBodyCollider* self )
                        {
                            return self->visible();
                        },
                    []( TSingleBodyCollider* self, bool visible )
                        {
                            self->SetVisible( visible );
                        } )
                .def_property( "wireframe",
                    []( const TSingleBodyCollider* self )
                        {
                            return self->wireframe();
                        },
                    []( TSingleBodyCollider* self, bool wireframe )
                        {
                            self->SetWireframe( wireframe );
                        } )
                .def_property( "size",
                    []( const TSingleBodyCollider* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->size() );
                        },
                    []( TSingleBodyCollider* self, const py::array_t<TScalar>& arr_size )
                        {
                            self->ChangeSize( tinymath::nparray_to_vector<TScalar, 3>( arr_size ) );
                        } )
                .def_property( "heights",
                    []( const TSingleBodyCollider* self ) -> py::array_t<TScalar>
                        {
                            return loco::vecgrid_to_nparray<float>( self->data().hfield_data.heights );
                        },
                    []( TSingleBodyCollider* self, const py::array_t<float>& arr_heights )
                        {
                            self->ChangeElevationData( loco::nparray_to_vecgrid<float>( arr_heights ) );
                        } )
                .def_property( "collisionGroup",
                    []( const TSingleBodyCollider* self )
                        {
                            return self->collisionGroup();
                        },
                    []( TSingleBodyCollider* self, int collisionGroup )
                        {
                            self->ChangeCollisionGroup( collisionGroup );
                        } )
                .def_property( "collisionMask",
                    []( const TSingleBodyCollider* self )
                        {
                            return self->collisionMask();
                        },
                    []( TSingleBodyCollider* self, int collisionMask )
                        {
                            self->ChangeCollisionMask( collisionMask );
                        } )
                .def_property_readonly( "shape", &TSingleBodyCollider::shape )
                .def( "__repr__", []( const TSingleBodyCollider* self )
                    {
                        auto _strrep = std::string( "SingleBodyCollider(\n" );
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
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for TSingleBody class
        {
            py::class_< TSingleBody, TObject >( m, "SingleBody" )
                .def( py::init( []( const std::string& name,
                                    const TBodyData& data,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation ) 
                    {
                        return std::make_unique<TSingleBody>( name, data,
                                                              tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                              tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ) );
                    } ) )
                .def( "data", []( TSingleBody* self ) -> TBodyData&
                    {
                        return self->data();
                    }, py::return_value_policy::reference )
                //// .def_property( "collider",
                ////     []( TSingleBody* self ) -> TSingleBodyCollider*
                ////         {
                ////             return self->collider();
                ////         },
                ////     []( TSingleBody* self, std::unique_ptr<TSingleBodyCollider> collider ) -> void
                ////         {
                ////             self->SetCollider( std::move( collider ) );
                ////         }, py::keep_alive<1, 2>() )
                //// .def_property( "drawable",
                ////     []( TSingleBody* self ) -> TDrawable*
                ////         {
                ////             return self->drawable();
                ////         },
                ////     []( TSingleBody* self, std::unique_ptr<TDrawable> drawable ) -> void
                ////         {
                ////             self->SetDrawable( std::move( drawable ) );
                ////         }, py::keep_alive<1, 2>() )
                .def_property( "linear_vel",
                    []( const TSingleBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->linear_vel() );
                        },
                    []( TSingleBody* self, const py::array_t<TScalar>& arr_linear_vel ) -> void
                        {
                            self->SetLinearVelocity( tinymath::nparray_to_vector<TScalar, 3>( arr_linear_vel ) );
                        } )
                .def_property( "angular_vel",
                    []( const TSingleBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->angular_vel() );
                        },
                    []( TSingleBody* self, const py::array_t<TScalar>& arr_angular_vel ) -> void
                        {
                            self->SetAngularVelocity( tinymath::nparray_to_vector<TScalar, 3>( arr_angular_vel ) );
                        } )
                .def( "AddForceCOM", []( TSingleBody* self, const py::array_t<TScalar>& arr_force_com ) -> void
                    {
                        self->AddForceCOM( tinymath::nparray_to_vector<TScalar, 3>( arr_force_com ) );
                    }, py::arg( "forceCOM" ) )
                .def( "AddTorqueCOM", []( TSingleBody* self, const py::array_t<TScalar>& arr_torque_com ) -> void
                    {
                        self->AddTorqueCOM( tinymath::nparray_to_vector<TScalar, 3>( arr_torque_com ) );
                    }, py::arg( "torqueCOM" ) )
                .def_property( "pos0",
                    []( const TSingleBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->pos0() );
                        },
                    []( TSingleBody* self, const py::array_t<TScalar>& arr_initial_pos ) -> void
                        {
                            self->SetInitialPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_initial_pos ) );
                        } )
                .def_property( "rot0",
                    []( const TSingleBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->rot0() );
                        },
                    []( TSingleBody* self, const py::array_t<TScalar>& arr_initial_rot ) -> void
                        {
                            self->SetInitialRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_initial_rot ) );
                        } )
                .def_property( "euler0",
                    []( const TSingleBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->euler0() );
                        },
                    []( TSingleBody* self, const py::array_t<TScalar>& arr_initial_euler ) -> void
                        {
                            self->SetInitialEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_initial_euler ) );
                        } )
                .def_property( "quat0",
                    []( const TSingleBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->quat0() );
                        },
                    []( TSingleBody* self, const py::array_t<TScalar>& arr_initial_quat ) -> void
                        {
                            self->SetInitialQuaternion( tinymath::nparray_to_vector<TScalar, 4>( arr_initial_quat ) );
                        } )
                .def_property( "tf0",
                    []( const TSingleBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->tf0() );
                        },
                    []( TSingleBody* self, const py::array_t<TScalar>& arr_initial_tf ) -> void
                        {
                            self->SetInitialTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_initial_tf ) );
                        } )
                .def_property( "linear_vel0",
                    []( const TSingleBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->linear_vel0() );
                        },
                    []( TSingleBody* self, const py::array_t<TScalar>& arr_linear_vel0 ) -> void
                        {
                            self->SetInitialLinearVelocity( tinymath::nparray_to_vector<TScalar, 3>( arr_linear_vel0 ) );
                        } )
                .def_property( "angular_vel0",
                    []( const TSingleBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->angular_vel0() );
                        },
                    []( TSingleBody* self, const py::array_t<TScalar>& arr_angular_vel0 ) -> void
                        {
                            self->SetInitialAngularVelocity( tinymath::nparray_to_vector<TScalar, 3>( arr_angular_vel0 ) );
                        } )
                .def_property_readonly( "dyntype", &TSingleBody::dyntype )
                .def( "__repr__", []( const TSingleBody* self )
                    {
                        auto _strrep = std::string( "SingleBody(\n" );
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "dyntype     : " + loco::ToString( self->dyntype() ) + "\n";
                        _strrep += "collision   : " + ( self->collider() ? loco::PointerToHexAddress( self->collider() ) : std::string( "null" ) ) + "\n";
                        _strrep += "visual      : " + ( self->drawable() ? loco::PointerToHexAddress( self->drawable() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::ToString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::ToString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::ToString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::ToString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::ToString( self->tf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }
    }
}
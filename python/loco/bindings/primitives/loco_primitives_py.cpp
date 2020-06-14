
#include <loco_common_py.h>
#include <primitives/loco_single_body_primitives.h>

namespace py = pybind11;

namespace loco {
namespace primitives {

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
                .def_property( "friction",
                    []( const TSingleBodyCollider* self ) -> TScalar
                        {
                            return self->friction();
                        },
                    []( TSingleBodyCollider* self, TScalar friction )
                        {
                            self->ChangeFriction( friction );
                        } )
                .def_property_readonly( "contacts",
                    []( TSingleBodyCollider* self ) -> std::vector<TContactData>
                        {
                            return self->contacts();
                        } )
                .def_property_readonly( "shape", &TSingleBodyCollider::shape )
                .def( "__repr__", []( const TSingleBodyCollider* self )
                    {
                        auto _strrep = std::string( "SingleBodyCollider(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "shape       : " + loco::ToString( self->shape() ) + "\n";
                        _strrep += "size        : " + loco::ToString( self->size() ) + "\n";
                        _strrep += "colGroup    : " + std::to_string( self->collisionGroup() ) + "\n";
                        _strrep += "colMask     : " + std::to_string( self->collisionMask() ) + "\n";
                        _strrep += "parent      : " + ( self->parent() ? tinyutils::PointerToHexAddress( self->parent() ) : std::string( "null" ) ) + "\n";
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
                .def_property( "collider",
                    []( TSingleBody* self ) -> TSingleBodyCollider*
                        {
                            return self->collider();
                        },
                    []( TSingleBody* self, std::unique_ptr<TSingleBodyCollider> collider ) -> void
                        {
                            self->SetCollider( std::move( collider ) );
                        }, py::keep_alive<1, 2>() )
                .def_property( "drawable",
                    []( TSingleBody* self ) -> visualizer::TDrawable*
                        {
                            return self->drawable();
                        },
                    []( TSingleBody* self, std::unique_ptr<visualizer::TDrawable> drawable ) -> void
                        {
                            self->SetDrawable( std::move( drawable ) );
                        }, py::keep_alive<1, 2>() )
                .def_property( "constraint",
                    []( TSingleBody* self ) -> TISingleBodyConstraint*
                        {
                            return self->constraint();
                        },
                    []( TSingleBody* self, std::unique_ptr<TISingleBodyConstraint> constraint ) -> void
                        {
                            self->SetConstraint( std::move( constraint ) );
                        }, py::keep_alive<1, 2>() )
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
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "dyntype     : " + loco::ToString( self->dyntype() ) + "\n";
                        _strrep += "collision   : " + ( self->collider() ? tinyutils::PointerToHexAddress( self->collider() ) : std::string( "null" ) ) + "\n";
                        _strrep += "visual      : " + ( self->drawable() ? tinyutils::PointerToHexAddress( self->drawable() ) : std::string( "null" ) ) + "\n";
                        _strrep += "constraint  : " + ( self->constraint() ? tinyutils::PointerToHexAddress( self->constraint() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::ToString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::ToString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::ToString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::ToString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::ToString( self->tf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for primitives helper classes
        {
            py::class_< TBox, TSingleBody >( m, "Box", R"mydelimiter(
                Box primitive, corresponding to a single-body with box-shaped collider and drawable

                This helper class creates a single body with a box-shaped collider, defined
                by its x-y-z extents (width|depth|height). The body exposes its extents as
                user modifiable parameters to the user, which can be changed at runtime.
                )mydelimiter" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_extents,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation,
                                    const eDynamicsType& dyntype,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TBox>( name,
                                                       tinymath::nparray_to_vector<TScalar, 3>( arr_extents ),
                                                       tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                       tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ),
                                                       dyntype, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "extents" ), py::arg( "position" ), py::arg( "rotation" ),
                     py::arg( "dyntype" ) = eDynamicsType::DYNAMIC, py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
                .def_property( "extents",
                    []( const TBox* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->extents() );
                        },
                    []( TBox* self, const py::array_t<TScalar>& arr_extents ) -> void
                        {
                            self->SetExtents( tinymath::nparray_to_vector<TScalar, 3>( arr_extents ) );
                        }, R"mydelimiter(
                        Width (x), depth (y) and height (z) extensions of the box collider, grouped into a vec3
                        )mydelimiter" )
                .def_property( "width",
                    []( const TBox* self ) -> TScalar
                        {
                            return self->width();
                        },
                    []( TBox* self, const TScalar& width ) -> void
                        {
                            self->SetWidth( width );
                        }, R"mydelimiter(
                        Width (x) of the box collider associated with this single-body
                        )mydelimiter" )
                .def_property( "depth",
                    []( const TBox* self ) -> TScalar
                        {
                            return self->depth();
                        },
                    []( TBox* self, const TScalar& depth ) -> void
                        {
                            self->SetDepth( depth );
                        }, R"mydelimiter(
                        Depth (y) of the box collider associated with this single-body
                        )mydelimiter" )
                .def_property( "height",
                    []( const TBox* self ) -> TScalar
                        {
                            return self->height();
                        },
                    []( TBox* self, const TScalar& height ) -> void
                        {
                            self->SetHeight( height );
                        }, R"mydelimiter(
                        Height (z) of the box collider associated with this single-body
                        )mydelimiter" )
                .def( "__repr__", []( const TBox* self )
                    {
                        std::string strrep = "Box(\n";
                        strrep += "width        : " + std::to_string( self->width() ) + "\n";
                        strrep += "depth        : " + std::to_string( self->depth() ) + "\n";
                        strrep += "height       : " + std::to_string( self->height() ) + "\n";
                        strrep += "position     : " + loco::ToString( self->pos() ) + "\n";
                        strrep += "rot-matrix   :\n" + loco::ToString( self->rot() ) + "\n";
                        strrep += "rot-euler    : " + loco::ToString( self->euler() ) + "\n";
                        strrep += "rot-quat     : " + loco::ToString( self->quat() ) + "\n";
                        strrep += "transform    :\n" + loco::ToString( self->tf() ) + "\n";
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );

            py::class_< TPlane, TSingleBody >( m, "Plane" )
                .def( py::init( []( const std::string& name,
                                    const TScalar& width,
                                    const TScalar& depth,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TPlane>( name,
                                                         width, depth,
                                                         tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                         tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ),
                                                         collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "width" ), py::arg( "depth" ), py::arg( "position" ),
                     py::arg( "rotation" ), py::arg( "collision_group" ) = 0x7fffffff, py::arg( "collision_mask" ) = 0x7fffffff )
                .def_property( "width",
                    []( const TPlane* self ) -> TScalar
                        {
                            return self->width();
                        },
                    []( TPlane* self, const TScalar& width ) -> void
                        {
                            self->SetWidth( width );
                        } )
                .def_property( "depth",
                    []( const TPlane* self ) -> TScalar
                        {
                            return self->depth();
                        },
                    []( TPlane* self, const TScalar& depth ) -> void
                        {
                            self->SetDepth( depth );
                        } );

            py::class_< TSphere, TSingleBody >( m, "Sphere" )
                .def( py::init( []( const std::string& name,
                                    const TScalar& radius,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation,
                                    const eDynamicsType& dyntype,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TSphere>( name,
                                                          radius,
                                                          tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                          tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ),
                                                          dyntype, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "radius" ), py::arg( "position" ), py::arg( "rotation" ),
                     py::arg( "dyntype" ) = eDynamicsType::DYNAMIC, py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
                .def_property( "radius",
                    []( const TSphere* self ) -> TScalar
                        {
                            return self->radius();
                        },
                    []( TSphere* self, const TScalar& radius ) -> void
                        {
                            self->SetRadius( radius );
                        } );

            py::class_< TCylinder, TSingleBody >( m, "Cylinder" )
                .def( py::init( []( const std::string& name,
                                    const TScalar& radius,
                                    const TScalar& height,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation,
                                    const eDynamicsType& dyntype,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TCylinder>( name,
                                                            radius, height,
                                                            tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                            tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ),
                                                            dyntype, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "radius" ), py::arg( "height" ), py::arg( "position" ), py::arg( "rotation" ),
                     py::arg( "dyntype" ) = eDynamicsType::DYNAMIC, py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
                .def_property( "radius",
                    []( const TCylinder* self ) -> TScalar
                        {
                            return self->radius();
                        },
                    []( TCylinder* self, const TScalar& radius ) -> void
                        {
                            self->SetRadius( radius );
                        } )
                .def_property( "height",
                    []( const TCylinder* self ) -> TScalar
                        {
                            return self->height();
                        },
                    []( TCylinder* self, const TScalar& height ) -> void
                        {
                            self->SetHeight( height );
                        } );

            py::class_< TCapsule, TSingleBody >( m, "Capsule" )
                .def( py::init( []( const std::string& name,
                                    const TScalar& radius,
                                    const TScalar& height,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation,
                                    const eDynamicsType& dyntype,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TCapsule>( name,
                                                            radius, height,
                                                            tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                            tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ),
                                                            dyntype, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "radius" ), py::arg( "height" ), py::arg( "position" ), py::arg( "rotation" ),
                     py::arg( "dyntype" ) = eDynamicsType::DYNAMIC, py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
                .def_property( "radius",
                    []( const TCapsule* self ) -> TScalar
                        {
                            return self->radius();
                        },
                    []( TCapsule* self, const TScalar& radius ) -> void
                        {
                            self->SetRadius( radius );
                        } )
                .def_property( "height",
                    []( const TCapsule* self ) -> TScalar
                        {
                            return self->height();
                        },
                    []( TCapsule* self, const TScalar& height ) -> void
                        {
                            self->SetHeight( height );
                        } );

            py::class_< TEllipsoid, TSingleBody >( m, "Ellipsoid" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_radii,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation,
                                    const eDynamicsType& dyntype,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TEllipsoid>( name,
                                                             tinymath::nparray_to_vector<TScalar, 3>( arr_radii ),
                                                             tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                             tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ),
                                                             dyntype, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "radii" ), py::arg( "position" ), py::arg( "rotation" ),
                     py::arg( "dyntype" ) = eDynamicsType::DYNAMIC, py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
                .def_property( "radii",
                    []( const TEllipsoid* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->radii() );
                        },
                    []( TEllipsoid* self, const py::array_t<TScalar>& arr_radii ) -> void
                        {
                            self->SetRadii( tinymath::nparray_to_vector<TScalar, 3>( arr_radii ) );
                        } )
                .def_property( "radius_x",
                    []( const TEllipsoid* self ) -> TScalar
                        {
                            return self->radius_x();
                        },
                    []( TEllipsoid* self, const TScalar& radius_x ) -> void
                        {
                            self->SetRadiusX( radius_x );
                        } )
                .def_property( "radius_y",
                    []( const TEllipsoid* self ) -> TScalar
                        {
                            return self->radius_y();
                        },
                    []( TEllipsoid* self, const TScalar& radius_y ) -> void
                        {
                            self->SetRadiusY( radius_y );
                        } )
                .def_property( "radius_z",
                    []( const TEllipsoid* self ) -> TScalar
                        {
                            return self->radius_z();
                        },
                    []( TEllipsoid* self, const TScalar& radius_z ) -> void
                        {
                            self->SetRadiusZ( radius_z );
                        } );

            py::class_< TMesh, TSingleBody >( m, "Mesh" )
                .def( py::init( []( const std::string& name,
                                    const std::string& mesh_collider_filepath,
                                    const std::string& mesh_visual_filepath,
                                    const TScalar& mesh_scale,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation,
                                    const eDynamicsType& dyntype,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TMesh>( name,
                                                        mesh_collider_filepath,
                                                        mesh_visual_filepath,
                                                        mesh_scale,
                                                        tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                        tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ),
                                                        dyntype, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "mesh_collider_filepath" ), py::arg( "mesh_visual_filepath" ),
                     py::arg( "mesh_scale" ), py::arg( "position" ), py::arg( "rotation" ),
                     py::arg( "dyntype" ) = eDynamicsType::DYNAMIC, py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<float>& arr_mesh_vertices,
                                    const py::array_t<int>& arr_mesh_faces,
                                    const TScalar& mesh_scale,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation,
                                    const eDynamicsType& dyntype,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TMesh>( name,
                                                        loco::nparray_to_stdvec<float>( arr_mesh_vertices ),
                                                        loco::nparray_to_stdvec<int>( arr_mesh_faces ),
                                                        mesh_scale,
                                                        tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                        tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ),
                                                        dyntype, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "mesh_vertices" ), py::arg( "mesh_faces" ),
                     py::arg( "mesh_scale" ), py::arg( "position" ), py::arg( "rotation" ),
                     py::arg( "dyntype" ) = eDynamicsType::DYNAMIC, py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
                .def_property( "scale",
                    []( const TMesh* self ) -> TScalar
                        {
                            return self->scale();
                        },
                    []( TMesh* self, const TScalar& scale ) -> void
                        {
                            self->SetScale( scale );
                        } );

            py::class_< THeightfield, TSingleBody >( m, "Heightfield" )
                .def( py::init( []( const std::string& name,
                                    const ssize_t& num_width_samples,
                                    const ssize_t& num_depth_samples,
                                    const TScalar& width_extent,
                                    const TScalar& depth_extent,
                                    const py::array_t<float>& arr_heights,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<THeightfield>( name,
                                                               num_width_samples, num_depth_samples,
                                                               width_extent, depth_extent,
                                                               loco::nparray_to_stdvec<float>( arr_heights ),
                                                               tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                               tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ),
                                                               collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "num_width_samples" ), py::arg( "num_depth_samples" ),
                     py::arg( "width_extent" ), py::arg( "depth_extent" ), py::arg( "heights" ),
                     py::arg( "position" ), py::arg( "rotation" ), py::arg( "collision_group" ) = 0x7fffffff, py::arg( "collision_mask" ) = 0x7fffffff )
                .def_property( "heights",
                    []( const THeightfield* self ) -> py::array_t<float>
                        {
                            return loco::stdvec_to_nparray<float>( self->heights() );
                        },
                    []( THeightfield* self, const py::array_t<float>& arr_heights ) -> void
                        {
                            self->SetHeights( loco::nparray_to_stdvec<float>( arr_heights ) );
                        } )
                .def_property_readonly( "num_width_samples", []( const THeightfield* self ) { return self->num_width_samples(); } )
                .def_property_readonly( "num_depth_samples", []( const THeightfield* self ) { return self->num_depth_samples(); } )
                .def_property_readonly( "width_extent", []( const THeightfield* self ) { return self->width_extent(); } )
                .def_property_readonly( "depth_extent", []( const THeightfield* self ) { return self->depth_extent(); } );
        }

        // Bindings for constraints
        {
            py::class_< TISingleBodyConstraint, TObject >( m, "ISingleBodyConstraint" )
                .def_property_readonly( "local_tf",
                    []( const TISingleBodyConstraint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->local_tf() );
                        } )
                .def_property_readonly( "constraint_type",
                    []( const TISingleBodyConstraint* self ) -> eConstraintType
                        {
                            return self->constraint_type();
                        } );

            py::class_< TSingleBodyRevoluteConstraint, TISingleBodyConstraint >( m, "SingleBodyRevoluteConstraint" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_local_tf,
                                    const py::array_t<TScalar>& arr_axis )
                    {
                        return std::make_unique<TSingleBodyRevoluteConstraint>( name,
                                                                                tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ),
                                                                                tinymath::nparray_to_vector<TScalar, 3>( arr_axis ) );
                    } ),
                     py::arg( "name" ), py::arg( "local_tf" ), py::arg( "axis" ) )
                .def_property( "hinge_angle",
                    []( const TSingleBodyRevoluteConstraint* self ) -> TScalar
                        {
                            return self->hinge_angle();
                        },
                    []( TSingleBodyRevoluteConstraint* self, TScalar hinge_angle ) -> void
                        {
                            self->SetHingeAngle( hinge_angle );
                        } )
                .def_property( "limits",
                    []( const TSingleBodyRevoluteConstraint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 2>( self->limits() );
                        },
                    []( TSingleBodyRevoluteConstraint* self, const py::array_t<TScalar>& arr_limits ) -> void
                        {
                            self->SetLimits( tinymath::nparray_to_vector<TScalar, 2>( arr_limits ) );
                        } )
                .def_property_readonly( "axis",
                    []( const TSingleBodyRevoluteConstraint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->axis() );
                        } );

            py::class_< TSingleBodyPrismaticConstraint, TISingleBodyConstraint >( m, "SingleBodyPrismaticConstraint" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_local_tf,
                                    const py::array_t<TScalar>& arr_axis )
                    {
                        return std::make_unique<TSingleBodyPrismaticConstraint>( name,
                                                                                 tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ),
                                                                                 tinymath::nparray_to_vector<TScalar, 3>( arr_axis ) );
                    } ),
                     py::arg( "name" ), py::arg( "local_tf" ), py::arg( "axis" ) )
                .def_property( "slide_position",
                    []( const TSingleBodyPrismaticConstraint* self ) -> TScalar
                        {
                            return self->slide_position();
                        },
                    []( TSingleBodyPrismaticConstraint* self, TScalar slide_position ) -> void
                        {
                            self->SetSlidePosition( slide_position );
                        } )
                .def_property( "limits",
                    []( const TSingleBodyPrismaticConstraint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 2>( self->limits() );
                        },
                    []( TSingleBodyPrismaticConstraint* self, const py::array_t<TScalar>& arr_limits ) -> void
                        {
                            self->SetLimits( tinymath::nparray_to_vector<TScalar, 2>( arr_limits ) );
                        } )
                .def_property_readonly( "axis",
                    []( const TSingleBodyPrismaticConstraint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->axis() );
                        } );

            py::class_< TSingleBodySphericalConstraint, TISingleBodyConstraint >( m, "SingleBodySphericalConstraint" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_local_tf )
                    {
                        return std::make_unique<TSingleBodySphericalConstraint>( name, tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ) );
                    } ),
                     py::arg( "name" ), py::arg( "loca_tf" ) );

            py::class_< TSingleBodyTranslational3dConstraint, TISingleBodyConstraint >( m, "SingleBodyTranslational3dConstraint" )
                .def( py::init( []( const std::string& name )
                    {
                        return std::make_unique<TSingleBodyTranslational3dConstraint>( name );
                    } ),
                     py::arg( "name" ) );

            py::class_< TSingleBodyUniversal3dConstraint, TISingleBodyConstraint >( m, "SingleBodyUniversal3dConstraint" )
                .def( py::init( []( const std::string& name )
                    {
                        return std::make_unique<TSingleBodyUniversal3dConstraint>( name );
                    } ),
                     py::arg( "name" ) );

            py::class_< TSingleBodyPlanarConstraint, TISingleBodyConstraint >( m, "SingleBodyPlanarConstraint" )
                .def( py::init( []( const std::string& name )
                    {
                        return std::make_unique<TSingleBodyPlanarConstraint>( name );
                    } ),
                     py::arg( "name" ) );
        }
    }
}}
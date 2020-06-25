
#include <loco_common_py.h>
#include <kinematic_trees/loco_kinematic_tree.h>

namespace py = pybind11;

namespace loco {
namespace kintree {

    void bindings_kinematic_trees( py::module& m )
    {
        // Bindings for TKinematicTreeCollider
        {
            py::class_<TKinematicTreeCollider, TObject>( m, "Collider" )
                .def( py::init<const std::string&, const TCollisionData&>() )
                .def( "data", []( TKinematicTreeCollider* self ) -> TCollisionData&
                    { 
                        return self->data();
                    }, py::return_value_policy::reference )
                //// .def( "parent", []( TKinematicTreeCollider* self ) -> TKinematicTreeBody*
                ////     {
                ////         return self->parent();
                ////     }, py::return_value_policy::reference )
                .def_property( "visible",
                    []( const TKinematicTreeCollider* self )
                        {
                            return self->visible();
                        },
                    []( TKinematicTreeCollider* self, bool visible )
                        {
                            self->SetVisible( visible );
                        } )
                .def_property( "wireframe",
                    []( const TKinematicTreeCollider* self )
                        {
                            return self->wireframe();
                        },
                    []( TKinematicTreeCollider* self, bool wireframe )
                        {
                            self->SetWireframe( wireframe );
                        } )
                .def_property( "local_tf",
                    []( const TKinematicTreeCollider* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->local_tf() );
                        },
                    []( TKinematicTreeCollider* self, const py::array_t<TScalar>& arr_local_tf )
                        {
                            self->SetLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ) );
                        } )
                .def_property( "size",
                    []( const TKinematicTreeCollider* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->size() );
                        },
                    []( TKinematicTreeCollider* self, const py::array_t<TScalar>& arr_size )
                        {
                            self->ChangeSize( tinymath::nparray_to_vector<TScalar, 3>( arr_size ) );
                        } )
                .def_property( "collisionGroup",
                    []( const TKinematicTreeCollider* self )
                        {
                            return self->collisionGroup();
                        },
                    []( TKinematicTreeCollider* self, ssize_t collision_group )
                        {
                            self->ChangeCollisionGroup( collision_group );
                        } )
                .def_property( "collisionMask",
                    []( const TKinematicTreeCollider* self )
                        {
                            return self->collisionMask();
                        },
                    []( TKinematicTreeCollider* self, ssize_t collision_mask )
                        {
                            self->ChangeCollisionMask( collision_mask );
                        } )
                .def_property( "friction",
                    []( const TKinematicTreeCollider* self )
                        {
                            return self->friction();
                        },
                    []( TKinematicTreeCollider* self, TScalar friction )
                        {
                            self->ChangeFriction( friction );
                        } )
                .def_property_readonly( "contacts",
                    []( TKinematicTreeCollider* self ) -> std::vector<TContactData>
                        {
                            return self->contacts();
                        } )
                .def_property_readonly( "shape", &TKinematicTreeCollider::shape )
                .def( "__repr__", []( const TKinematicTreeCollider* self )
                    {
                        auto strrep = std::string( "kintree.Collider(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "shape        : " + loco::ToString( self->shape() ) + "\n";
                        strrep += "size         : " + loco::ToString( self->size() ) + "\n";
                        strrep += "parent       : " + ( self->parent() ? self->parent()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "col_group    : " + std::to_string( self->collisionGroup() ) + "\n";
                        strrep += "col_mask     : " + std::to_string( self->collisionMask() ) + "\n";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        strrep += "world_tf     :\n" + loco::ToString( self->tf() ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );
        }

        // Bindings for TKinematicTreeJoint and related classes
        {
            //------------------------------------------------------------------------------------//
            //                       TKinematicTreeJointData struct bindings                      //
            //------------------------------------------------------------------------------------//
            py::class_<TKinematicTreeJointData>( m, "JointData" )
                .def( py::init<>() )
                .def_readwrite( "type", &TKinematicTreeJointData::type )
                .def_property( "limits",
                    []( const TKinematicTreeJointData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 2>( self->limits );
                        },
                    []( TKinematicTreeJointData* self, const py::array_t<TScalar>& arr_limits )
                        {
                            self->limits = tinymath::nparray_to_vector<TScalar, 2>( arr_limits );
                        } )
                .def_readwrite( "stiffness", &TKinematicTreeJointData::stiffness )
                .def_readwrite( "armature", &TKinematicTreeJointData::armature )
                .def_readwrite( "damping", &TKinematicTreeJointData::damping )
                .def_property( "plane_axis_1",
                    []( const TKinematicTreeJointData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->plane_axis_1 );
                        },
                    []( TKinematicTreeJointData* self, const py::array_t<TScalar>& arr_plane_axis_1 )
                        {
                            self->plane_axis_1 = tinymath::nparray_to_vector<TScalar, 3>( arr_plane_axis_1 );
                        } )
                .def_property( "plane_axis_2",
                    []( const TKinematicTreeJointData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->plane_axis_2 );
                        },
                    []( TKinematicTreeJointData* self, const py::array_t<TScalar>& arr_plane_axis_2 )
                        {
                            self->plane_axis_2 = tinymath::nparray_to_vector<TScalar, 3>( arr_plane_axis_2 );
                        } )
                .def_property( "local_axis",
                    []( const TKinematicTreeJointData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->local_axis );
                        },
                    []( TKinematicTreeJointData* self, const py::array_t<TScalar>& arr_local_axis )
                        {
                            self->local_axis = tinymath::nparray_to_vector<TScalar, 3>( arr_local_axis );
                        } )
                .def_property( "local_tf",
                    []( const TKinematicTreeJointData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->local_tf );
                        },
                    []( TKinematicTreeJointData* self, const py::array_t<TScalar>& arr_local_tf )
                        {
                            self->local_tf = tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf );
                        } )
                .def( "__repr__", []( const TKinematicTreeJointData* self )
                    {
                        auto strrep = std::string( "kintree.JointData(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "type         : " + loco::ToString( self->type ) + "\n";
                        strrep += "limits       : " + loco::ToString( self->limits ) + "\n";
                        strrep += "stiffness    : " + std::to_string( self->stiffness ) + "\n";
                        strrep += "armature     : " + std::to_string( self->armature ) + "\n";
                        strrep += "damping      : " + std::to_string( self->damping ) + "\n";
                        strrep += "plane_axis_1 : " + loco::ToString( self->plane_axis_1 ) + "\n";
                        strrep += "plane_axis_2 : " + loco::ToString( self->plane_axis_2 ) + "\n";
                        strrep += "local_axis   : " + loco::ToString( self->local_axis ) + "\n";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );

            //------------------------------------------------------------------------------------//
            //                          TKinematicTreeJoint class bindings                        //
            //------------------------------------------------------------------------------------//
            py::class_<TKinematicTreeJoint, TObject>( m, "Joint" )
                .def( py::init<const std::string&, const TKinematicTreeJointData&>() )
                .def( "data", []( TKinematicTreeJoint* self ) -> TKinematicTreeJointData&
                    {
                        return self->data();
                    }, py::return_value_policy::reference )
                .def_property( "qpos",
                    []( const TKinematicTreeJoint* self ) -> py::array_t<TScalar>
                        {
                            return loco::stdvec_to_nparray<TScalar>( self->qpos() );
                        },
                    []( TKinematicTreeJoint* self, const py::array_t<TScalar>& arr_qpos )
                        {
                            self->SetQpos( loco::nparray_to_stdvec<TScalar>( arr_qpos ) );
                        } )
                .def_property( "qpos0",
                    []( const TKinematicTreeJoint* self ) -> py::array_t<TScalar>
                        {
                            return loco::stdvec_to_nparray<TScalar>( self->qpos0() );
                        },
                    []( TKinematicTreeJoint* self, const py::array_t<TScalar>& arr_qpos0 )
                        {
                            self->SetQpos0( loco::nparray_to_stdvec<TScalar>( arr_qpos0 ) );
                        } )
                .def_property( "qvel",
                    []( const TKinematicTreeJoint* self ) -> py::array_t<TScalar>
                        {
                            return loco::stdvec_to_nparray<TScalar>( self->qvel() );
                        },
                    []( TKinematicTreeJoint* self, const py::array_t<TScalar>& arr_qvel )
                        {
                            self->SetQvel( loco::nparray_to_stdvec<TScalar>( arr_qvel ) );
                        } )
                .def_property( "qvel0",
                    []( const TKinematicTreeJoint* self ) -> py::array_t<TScalar>
                        {
                            return loco::stdvec_to_nparray<TScalar>( self->qvel0() );
                        },
                    []( TKinematicTreeJoint* self, const py::array_t<TScalar>& arr_qvel0 )
                        {
                            self->SetQvel0( loco::nparray_to_stdvec<TScalar>( arr_qvel0 ) );
                        } )
                .def_property( "stiffness",
                    []( const TKinematicTreeJoint* self )
                        {
                            return self->stiffness();
                        },
                    []( TKinematicTreeJoint* self, TScalar stiffness )
                        {
                            self->SetStiffness( stiffness );
                        } )
                .def_property( "armature",
                    []( const TKinematicTreeJoint* self )
                        {
                            return self->armature();
                        },
                    []( TKinematicTreeJoint* self, TScalar armature )
                        {
                            self->SetArmature( armature );
                        } )
                .def_property( "damping",
                    []( const TKinematicTreeJoint* self )
                        {
                            return self->damping();
                        },
                    []( TKinematicTreeJoint* self, TScalar damping )
                        {
                            self->SetDamping( damping );
                        } )
                .def_property( "local_tf",
                    []( const TKinematicTreeJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->local_tf() );
                        },
                    []( TKinematicTreeJoint* self, const py::array_t<TScalar>& arr_local_tf )
                        {
                            self->SetLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ) );
                        } )
                .def_property_readonly( "num_qpos", &TKinematicTreeJoint::num_qpos )
                .def_property_readonly( "num_qvel", &TKinematicTreeJoint::num_qvel )
                .def_property_readonly( "type", &TKinematicTreeJoint::type )
                .def( "__repr__", []( const TKinematicTreeJoint* self )
                    {
                        auto strrep = std::string( "kintree.Joint(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "type         : " + loco::ToString( self->type() ) + "\n";
                        strrep += "parent       : " + ( self->parent() ? self->parent()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "stiffness    : " + std::to_string( self->stiffness() ) + "\n";
                        strrep += "armature     : " + std::to_string( self->armature() ) + "\n";
                        strrep += "damping      : " + std::to_string( self->damping() ) + "\n";
                        strrep += "num_qpos     : " + std::to_string( self->num_qpos() ) + "\n";
                        strrep += "num_qvel     : " + std::to_string( self->num_qvel() ) + "\n";
                        strrep += "qpos         : [ ";
                            for ( ssize_t i = 0; i < self->qpos().size(); i++ )
                                strrep += std::to_string( self->qpos()[i] ) + " ";
                            strrep += "]";
                        strrep += "qvel         : [";
                            for ( ssize_t i = 0; i < self->qvel().size(); i++ )
                                strrep += std::to_string( self->qvel()[i] ) + " ";
                            strrep += "]";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );

            //------------------------------------------------------------------------------------//
            //                          Typed-kintree-joint class bindings                        //
            //------------------------------------------------------------------------------------//

            py::class_<TKinematicTreeRevoluteJoint, TKinematicTreeJoint>( m, "RevoluteJoint" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_local_axis,
                                    const py::array_t<TScalar>& arr_limits,
                                    TScalar stiffness,
                                    TScalar armature,
                                    TScalar damping )
                    {
                        return std::make_unique<TKinematicTreeRevoluteJoint>( name,
                                                                              tinymath::nparray_to_vector<TScalar, 3>( arr_local_axis ),
                                                                              tinymath::nparray_to_vector<TScalar, 2>( arr_limits ),
                                                                              stiffness, armature, damping );
                    } ),
                     py::arg( "name" ), py::arg( "local_axis" ), py::arg( "limits" ),
                     py::arg( "stiffness" ) = 0.0f, py::arg( "armature" ) = 0.0f, py::arg( "damping" ) = 0.0f )
                .def_property( "angle", &TKinematicTreeRevoluteJoint::angle, &TKinematicTreeRevoluteJoint::SetAngle )
                .def_property( "axis",
                    []( const TKinematicTreeRevoluteJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->axis() );
                        },
                    []( TKinematicTreeRevoluteJoint* self, const py::array_t<TScalar>& arr_axis )
                        {
                            self->SetAxis( tinymath::nparray_to_vector<TScalar, 3>( arr_axis ) );
                        } )
                .def_property( "limits",
                    []( const TKinematicTreeRevoluteJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 2>( self->limits() );
                        },
                    []( TKinematicTreeRevoluteJoint* self, const py::array_t<TScalar>& arr_limits )
                        {
                            self->SetLimits( tinymath::nparray_to_vector<TScalar, 2>( arr_limits ) );
                        } )
                .def( "__repr__", []( const TKinematicTreeRevoluteJoint* self )
                    {
                        auto strrep = std::string( "kintree.RevoluteJoint(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "parent       : " + ( self->parent() ? self->parent()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "stiffness    : " + std::to_string( self->stiffness() ) + "\n";
                        strrep += "armature     : " + std::to_string( self->armature() ) + "\n";
                        strrep += "damping      : " + std::to_string( self->damping() ) + "\n";
                        strrep += "angle        : " + std::to_string( self->angle() ) + "\n";
                        strrep += "axis         : " + loco::ToString( self->axis() ) + "\n";
                        strrep += "limits       : " + loco::ToString( self->limits() ) + "\n";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        strrep += "world_tf     :\n" + loco::ToString( self->tf() ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );

            py::class_<TKinematicTreePrismaticJoint, TKinematicTreeJoint>( m, "PrismaticJoint" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_local_axis,
                                    const py::array_t<TScalar>& arr_limits,
                                    TScalar stiffness,
                                    TScalar armature,
                                    TScalar damping )
                    {
                        return std::make_unique<TKinematicTreePrismaticJoint>( name,
                                                                               tinymath::nparray_to_vector<TScalar, 3>( arr_local_axis ),
                                                                               tinymath::nparray_to_vector<TScalar, 2>( arr_limits ),
                                                                               stiffness, armature, damping );
                    } ),
                     py::arg( "name" ), py::arg( "local_axis" ), py::arg( "limits" ),
                     py::arg( "stiffness" ) = 0.0f, py::arg( "armature" ) = 0.0f, py::arg( "damping" ) = 0.0f )
                .def_property( "position", &TKinematicTreePrismaticJoint::position, &TKinematicTreePrismaticJoint::SetPosition )
                .def_property( "axis",
                    []( const TKinematicTreePrismaticJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->axis() );
                        },
                    []( TKinematicTreePrismaticJoint* self, const py::array_t<TScalar>& arr_axis )
                        {
                            self->SetAxis( tinymath::nparray_to_vector<TScalar, 3>( arr_axis ) );
                        } )
                .def_property( "limits",
                    []( const TKinematicTreePrismaticJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 2>( self->limits() );
                        },
                    []( TKinematicTreePrismaticJoint* self, const py::array_t<TScalar>& arr_limits )
                        {
                            self->SetLimits( tinymath::nparray_to_vector<TScalar, 2>( arr_limits ) );
                        } )
                .def( "__repr__", []( const TKinematicTreePrismaticJoint* self )
                    {
                        auto strrep = std::string( "kintree.PrismaticJoint(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "parent       : " + ( self->parent() ? self->parent()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "stiffness    : " + std::to_string( self->stiffness() ) + "\n";
                        strrep += "armature     : " + std::to_string( self->armature() ) + "\n";
                        strrep += "damping      : " + std::to_string( self->damping() ) + "\n";
                        strrep += "position     : " + std::to_string( self->position() ) + "\n";
                        strrep += "axis         : " + loco::ToString( self->axis() ) + "\n";
                        strrep += "limits       : " + loco::ToString( self->limits() ) + "\n";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        strrep += "world_tf     :\n" + loco::ToString( self->tf() ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );

            py::class_<TKinematicTreeSphericalJoint, TKinematicTreeJoint>( m, "SphericalJoint" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_limits,
                                    TScalar stiffness,
                                    TScalar armature,
                                    TScalar damping )
                    {
                        return std::make_unique<TKinematicTreeSphericalJoint>( name,
                                                                               tinymath::nparray_to_vector<TScalar, 2>( arr_limits ),
                                                                               stiffness, armature, damping );
                    } ),
                     py::arg( "name" ), py::arg( "limits" ), py::arg( "stiffness" ) = 0.0f, py::arg( "armature" ) = 0.0f, py::arg( "damping" ) = 0.0f )
                .def_property( "quat",
                    []( const TKinematicTreeSphericalJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->quat() );
                        },
                    []( TKinematicTreeSphericalJoint* self, const py::array_t<TScalar>& arr_quat )
                        {
                            self->SetQuat( tinymath::nparray_to_vector<TScalar, 4>( arr_quat ) );
                        } )
                .def_property( "limits",
                    []( const TKinematicTreeSphericalJoint* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 2>( self->limits() );
                        },
                    []( TKinematicTreeSphericalJoint* self, const py::array_t<TScalar>& arr_limits )
                        {
                            self->SetLimits( tinymath::nparray_to_vector<TScalar, 2>( arr_limits ) );
                        } )
                .def( "__repr__", []( const TKinematicTreeSphericalJoint* self )
                    {
                        auto strrep = std::string( "kintree.SphericalJoint(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "parent       : " + ( self->parent() ? self->parent()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "stiffness    : " + std::to_string( self->stiffness() ) + "\n";
                        strrep += "armature     : " + std::to_string( self->armature() ) + "\n";
                        strrep += "damping      : " + std::to_string( self->damping() ) + "\n";
                        strrep += "quat         : " + loco::ToString( self->quat() ) + "\n";
                        strrep += "limits       : " + loco::ToString( self->limits() ) + "\n";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        strrep += "world_tf     :\n" + loco::ToString( self->tf() ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );

            py::class_<TKinematicTreePlanarJoint, TKinematicTreeJoint>( m, "PlanarJoint" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_plane_axis_1,
                                    const py::array_t<TScalar>& arr_plane_axis_2 )
                    {
                        return std::make_unique<TKinematicTreePlanarJoint>( name,
                                                                            tinymath::nparray_to_vector<TScalar, 3>( arr_plane_axis_1 ),
                                                                            tinymath::nparray_to_vector<TScalar, 3>( arr_plane_axis_2 ) );
                    } ),
                     py::arg( "name" ), py::arg( "plane_axis_1" ), py::arg( "plane_axis_2" ) )
                .def( "__repr__", []( const TKinematicTreePlanarJoint* self )
                    {
                        auto strrep = std::string( "kintree.PlanarJoint(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "parent       : " + ( self->parent() ? self->parent()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        strrep += "world_tf     :\n" + loco::ToString( self->tf() ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );

            py::class_<TKinematicTreeFixedJoint, TKinematicTreeJoint>( m, "FixedJoint" )
                .def( py::init<const std::string&>() )
                .def( "__repr__", []( const TKinematicTreeFixedJoint* self )
                    {
                        auto strrep = std::string( "kintree.FixedJoint(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "parent       : " + ( self->parent() ? self->parent()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        strrep += "world_tf     :\n" + loco::ToString( self->tf() ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );

            py::class_<TKinematicTreeFreeJoint, TKinematicTreeJoint>( m, "FreeJoint" )
                .def( py::init<const std::string&>() )
                .def( "__repr__", []( const TKinematicTreeFreeJoint* self )
                    {
                        auto strrep = std::string( "kintree.FreeJoint(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "parent       : " + ( self->parent() ? self->parent()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        strrep += "world_tf     :\n" + loco::ToString( self->tf() ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );
        }

        // Bindings for TKinematicTreeBody and related classes
        {

        }
    }

}}

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
            //------------------------------------------------------------------------------------//
            //                         Bindings for TKinematicTreeBodyData                        //
            //------------------------------------------------------------------------------------//
            py::class_<TKinematicTreeBodyData>( m, "BodyData" )
                .def( py::init<>() )
                .def_property_readonly( "colliders", []( TKinematicTreeBodyData* self ) -> std::vector<TCollisionData>
                    { return self->colliders; } )
                .def_property_readonly( "drawables", []( TKinematicTreeBodyData* self ) -> std::vector<TVisualData>
                    { return self->drawables; } )
                .def_property_readonly( "joints", []( TKinematicTreeBodyData* self ) -> std::vector<TKinematicTreeJointData>
                    { return self->joints; } )
                .def_property_readonly( "inertia", []( TKinematicTreeBodyData* self ) -> TInertialData
                    { return self->inertia; } )
                .def_property( "local_tf",
                    []( const TKinematicTreeBodyData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->local_tf );
                        },
                    []( TKinematicTreeBodyData* self, const py::array_t<TScalar>& arr_local_tf )
                        {
                            self->local_tf = tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf );
                        } );

            //------------------------------------------------------------------------------------//
            //                           Bindings for TKinematicTreeBody                          //
            //------------------------------------------------------------------------------------//
            py::class_<TKinematicTreeBody, TObject>( m, "Body" )
                .def( py::init<const std::string&>() )
                .def( "AddDrawable", []( TKinematicTreeBody* self, 
                                         std::unique_ptr<visualizer::TDrawable> drawable,
                                         const py::array_t<TScalar>& arr_local_tf )
                    {
                        self->AddDrawable( std::move( drawable ), tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ) );
                    }, py::keep_alive<1, 2>() )
                .def( "AddCollider", []( TKinematicTreeBody* self,
                                         std::unique_ptr<TKinematicTreeCollider> collider,
                                         const py::array_t<TScalar>& arr_local_tf )
                    {
                        self->AddCollider( std::move( collider ), tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ) );
                    }, py::keep_alive<1, 2>() )
                .def( "AddJoint", []( TKinematicTreeBody* self,
                                      std::unique_ptr<TKinematicTreeJoint> joint,
                                      const py::array_t<TScalar>& arr_local_tf )
                    {
                        self->AddJoint( std::move( joint ), tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ) );
                    }, py::keep_alive<1, 2>() )
                .def( "AddChild", []( TKinematicTreeBody* self,
                                      std::unique_ptr<TKinematicTreeBody> body,
                                      const py::array_t<TScalar>& arr_local_tf )
                    {
                        self->AddChild( std::move( body ), tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ) );
                    } )
                .def( "AddForceCOM", []( TKinematicTreeBody* self, const py::array_t<TScalar>& arr_force )
                    {
                        self->AddForceCOM( tinymath::nparray_to_vector<TScalar, 3>( arr_force ) );
                    }, py::arg( "force" ) )
                .def( "AddTorqueCOM", []( TKinematicTreeBody* self, const py::array_t<TScalar>& arr_torque )
                    {
                        self->AddTorqueCOM( tinymath::nparray_to_vector<TScalar, 3>( arr_torque ) );
                    }, py::arg( "torque" ) )
                .def( "RemoveChild", &TKinematicTreeBody::RemoveChild )
                .def( "Forward", &TKinematicTreeBody::Forward )
                .def( "data", []( TKinematicTreeBody* self ) -> TKinematicTreeBodyData&
                    { return self->data(); }, py::return_value_policy::reference )
                .def( "drawables", []( TKinematicTreeBody* self ) -> std::vector<visualizer::TDrawable*>
                    {
                        return self->drawables();
                    }, py::return_value_policy::reference )
                .def( "colliders", []( TKinematicTreeBody* self ) -> std::vector<TKinematicTreeCollider*>
                    {
                        return self->colliders();
                    }, py::return_value_policy::reference )
                .def( "joints", []( TKinematicTreeBody* self ) -> std::vector<TKinematicTreeJoint*>
                    {
                        return self->joints();
                    }, py::return_value_policy::reference )
                .def( "children_names", []( TKinematicTreeBody* self ) -> std::vector<std::string>
                    {
                        auto children = self->children();
                        std::vector<std::string> vec_children_names( children.size() );
                        for ( ssize_t i = 0; i < children.size(); i++ )
                            if ( children[i] )
                                vec_children_names[i] = children[i]->name();
                        return vec_children_names;
                    } )
                .def( "parent_name", []( TKinematicTreeBody* self ) -> std::string
                    {
                        return ( self->parent() ? self->parent()->name() : "" );
                    } )
                .def( "kintree_name", []( TKinematicTreeBody* self ) -> std::string
                    {
                        return ( self->kintree() ? self->kintree()->name() : "" );
                    } )
                .def_property( "local_tf",
                    []( const TKinematicTreeBody* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->local_tf() );
                        },
                    []( TKinematicTreeBody* self, const py::array_t<TScalar>& arr_local_tf )
                        {
                            self->SetLocalTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_local_tf ) );
                        } )
                .def( "__repr__", []( const TKinematicTreeBody* self )
                    {
                        auto strrep = std::string( "kintree.Body(\n" );
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "parent       : " + ( self->parent() ? self->parent()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "kintree      : " + ( self->kintree() ? self->kintree()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "drawables    : [";
                            auto drawables = self->drawables();
                            for ( ssize_t i = 0; i < drawables.size(); i++ )
                                if ( drawables[i] )
                                    strrep += drawables[i]->name() + " ";
                            strrep += "]";
                        strrep += "colliders    : [";
                            auto colliders = self->colliders();
                            for ( ssize_t i = 0; i < colliders.size(); i++ )
                                if ( colliders[i] )
                                    strrep += colliders[i]->name() + " ";
                            strrep += "]";
                        strrep += "joints       : [";
                            auto joints = self->joints();
                            for ( ssize_t i = 0; i < joints.size(); i++ )
                                if ( joints[i] )
                                    strrep += joints[i]->name() + " ";
                            strrep += "]";
                        strrep += "children     : [";
                            auto children = self->children();
                            for ( ssize_t i = 0; i < children.size(); i++ )
                                if ( children[i] )
                                    strrep += children[i]->name() + " ";
                            strrep += "]";
                        strrep += "local_tf     :\n" + loco::ToString( self->local_tf() ) + "\n";
                        strrep += "world_tf     :\n" + loco::ToString( self->tf() ) + "\n";
                        strrep += ")";
                        return strrep;
                    } );

            //------------------------------------------------------------------------------------//
            //                            Typed bodies class bindings                             //
            //------------------------------------------------------------------------------------//

            py::class_<TBox, TKinematicTreeBody>( m, "Box", R"mydelimiter(
                Box primitive, corresponding to a single-body with box-shaped collider and drawable

                This helper class creates a single body with a box-shaped collider, defined
                by its x-y-z extents (width|depth|height). The body exposes its extents as
                user modifiable parameters to the user, which can be changed at runtime.
                )mydelimiter" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_extents,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TBox>( name,
                                                       tinymath::nparray_to_vector<TScalar, 3>( arr_extents ),
                                                       collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "extents" ), py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
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
                        std::string strrep = "kintree.Box(\n";
                        strrep += "name         : " + self->name() + "\n";
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

            py::class_<TSphere, TKinematicTreeBody>( m, "Sphere" )
                .def( py::init( []( const std::string& name,
                                    const TScalar& radius,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TSphere>( name, radius, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "radius" ), py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
                .def_property( "radius",
                    []( const TSphere* self ) -> TScalar
                        {
                            return self->radius();
                        },
                    []( TSphere* self, const TScalar& radius ) -> void
                        {
                            self->SetRadius( radius );
                        } );

            py::class_<TCylinder, TKinematicTreeBody>( m, "Cylinder" )
                .def( py::init( []( const std::string& name,
                                    const TScalar& radius,
                                    const TScalar& height,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TCylinder>( name, radius, height, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "radius" ), py::arg( "height" ), py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
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

            py::class_<TCapsule, TKinematicTreeBody>( m, "Capsule" )
                .def( py::init( []( const std::string& name,
                                    const TScalar& radius,
                                    const TScalar& height,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TCapsule>( name, radius, height, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "radius" ), py::arg( "height" ), py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
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

            py::class_<TEllipsoid, TKinematicTreeBody>( m, "Ellipsoid" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_radii,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TEllipsoid>( name,
                                                             tinymath::nparray_to_vector<TScalar, 3>( arr_radii ),
                                                             collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "radii" ), py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
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

            py::class_<TConvexMesh, TKinematicTreeBody>( m, "Mesh" )
                .def( py::init( []( const std::string& name,
                                    const std::string& mesh_collider_filepath,
                                    const std::string& mesh_visual_filepath,
                                    const TScalar& mesh_scale,
                                    const int& collision_group,
                                    const int& collision_mask )
                    {
                        return std::make_unique<TConvexMesh>( name,
                                                        mesh_collider_filepath,
                                                        mesh_visual_filepath,
                                                        mesh_scale, collision_group, collision_mask );
                    } ),
                     py::arg( "name" ), py::arg( "mesh_collider_filepath" ), py::arg( "mesh_visual_filepath" ),
                     py::arg( "mesh_scale" ), py::arg( "collision_group" ) = 1, py::arg( "collision_mask" ) = 1 )
                .def_property( "scale",
                    []( const TConvexMesh* self ) -> TScalar
                        {
                            return self->scale();
                        },
                    []( TConvexMesh* self, const TScalar& scale ) -> void
                        {
                            self->SetScale( scale );
                        } );
        }

        // Bindings for TKinematicTree
        {
            py::class_<TKinematicTree, TObject>( m, "KinematicTree" )
                .def( py::init( []( const std::string& name,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation )
                    {
                        return std::make_unique<TKinematicTree>( name,
                                                                 tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                                 tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ) );
                    } ),
                     py::arg( "name" ), py::arg( "position" ), py::arg( "rotation" ) )
                .def( "LoadFromMjcf", &TKinematicTree::LoadFromMjcf )
                .def( "LoadFromUrdf", &TKinematicTree::LoadFromUrdf )
                .def( "LoadFromRlsim", &TKinematicTree::LoadFromRlsim )
                .def( "SetRoot", []( TKinematicTree* self, std::unique_ptr<TKinematicTreeBody> body )
                    {
                        self->SetRoot( std::move( body ) );
                    }, py::keep_alive<1, 2>() )
                .def_property( "pos0",
                    []( const TKinematicTree* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->pos0() );
                        },
                    []( TKinematicTree* self, const py::array_t<TScalar>& arr_pos0 )
                        {
                            self->SetInitialPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_pos0 ) );
                        } )
                .def_property( "rot0",
                    []( const TKinematicTree* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 3>( self->rot0() );
                        },
                    []( TKinematicTree* self, const py::array_t<TScalar>& arr_rot0 )
                        {
                            self->SetInitialRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_rot0 ) );
                        } )
                .def_property( "euler0",
                    []( const TKinematicTree* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->euler0() );
                        },
                    []( TKinematicTree* self, const py::array_t<TScalar>& arr_euler0 )
                        {
                            self->SetInitialEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_euler0 ) );
                        } )
                .def_property( "quat0",
                    []( const TKinematicTree* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 4>( self->quat0() );
                        },
                    []( TKinematicTree* self, const py::array_t<TScalar>& arr_quat0 )
                        {
                            self->SetInitialQuaternion( tinymath::nparray_to_vector<TScalar, 4>( arr_quat0 ) );
                        } )
                .def_property( "tf0",
                    []( const TKinematicTree* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->tf0() );
                        },
                    []( TKinematicTree* self, const py::array_t<TScalar>& arr_tf0 )
                        {
                            self->SetInitialTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_tf0 ) );
                        } )
                .def( "GetNumBodies", &TKinematicTree::GetNumBodies )
                .def( "HasBody", &TKinematicTree::HasBody )
                .def( "RemoveBodyByName", &TKinematicTree::RemoveBodyByName )
                //// .def( "GetBodyByName", []( TKinematicTree* self, const std::string& name ) -> TKinematicTreeBody*
                ////     {
                ////         return self->GetBodyByName( name );
                ////     }, py::return_value_policy::reference )
                //// .def( "GetBodiesList", []( TKinematicTree* self ) -> std::vector<TKinematicTreeBody*>
                ////     {
                ////         return self->GetBodiesList();
                ////     }, py::return_value_policy::reference )
                .def( "__repr__", []( const TKinematicTree* self )
                    {
                        std::string strrep = "kintree.KinematicTree(\n";
                        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        strrep += "name         : " + self->name() + "\n";
                        strrep += "position     : " + loco::ToString( self->pos() ) + "\n";
                        strrep += "euler        : " + loco::ToString( self->euler() ) + "\n";
                        strrep += "quaternion   : " + loco::ToString( self->quat() ) + "\n";
                        strrep += "world_tf     : " + loco::ToString( self->tf() ) + "\n";
                        strrep += "root         : " + ( self->root() ? self->root()->name() : std::string( "nullptr" ) ) + "\n";
                        strrep += "bodies       : [ ";
                            auto bodies = self->GetBodiesList();
                            for ( ssize_t i = 0; i < bodies.size(); i++ )
                                if ( bodies[i] )
                                    strrep += bodies[i]->name() + " ";
                            strrep += "]";
                        strrep += ")";
                        return strrep;
                    } );
        }
    }
}}
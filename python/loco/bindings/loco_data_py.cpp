
#include <loco_common_py.h>
#include <loco_data.h>

namespace py = pybind11;

namespace loco
{
    void bindings_data( py::module& m )
    {   
        // Bindings for enumerators|constants
        {
            py::enum_< loco::eShapeType >( m, "ShapeType", py::arithmetic() )
                .value( "NONE", loco::eShapeType::NONE )
                .value( "BOX", loco::eShapeType::BOX )
                .value( "PLANE", loco::eShapeType::PLANE )
                .value( "SPHERE", loco::eShapeType::SPHERE )
                .value( "CYLINDER", loco::eShapeType::CYLINDER )
                .value( "CAPSULE", loco::eShapeType::CAPSULE )
                .value( "ELLIPSOID", loco::eShapeType::ELLIPSOID )
                .value( "MESH", loco::eShapeType::MESH )
                .value( "HFIELD", loco::eShapeType::HFIELD );

            py::enum_< loco::eJointType >( m, "JointType", py::arithmetic() )
                .value( "FREE", loco::eJointType::FREE )
                .value( "REVOLUTE", loco::eJointType::REVOLUTE )
                .value( "PRISMATIC", loco::eJointType::PRISMATIC )
                .value( "SPHERICAL", loco::eJointType::SPHERICAL )
                .value( "PLANAR", loco::eJointType::PLANAR )
                .value( "FIXED", loco::eJointType::FIXED );

            py::enum_< loco::eDynamicsType >( m, "DynamicsType", py::arithmetic() )
                .value( "DYNAMIC", loco::eDynamicsType::DYNAMIC )
                .value( "STATIC", loco::eDynamicsType::STATIC );

            py::enum_< loco::eSensorType >( m, "SensorType", py::arithmetic() )
                .value( "NONE", loco::eSensorType::NONE )
                .value( "PROP_JOINT", loco::eSensorType::PROP_JOINT )
                .value( "PROP_BODY", loco::eSensorType::PROP_BODY )
                .value( "EXT_HEIGHTFIELD_1D", loco::eSensorType::EXT_HEIGHTFIELD_1D )
                .value( "EXT_HEIGHTFIELD_2D", loco::eSensorType::EXT_HEIGHTFIELD_2D )
                .value( "EXT_CAMERA_RGB", loco::eSensorType::EXT_CAMERA_RGB )
                .value( "EXT_CAMERA_DEPTH", loco::eSensorType::EXT_CAMERA_DEPTH )
                .value( "EXT_CAMERA_SEGMENTATION", loco::eSensorType::EXT_CAMERA_SEGMENTATION );

            py::enum_< loco::eActuatorType >( m, "ActuatorType", py::arithmetic() )
                .value( "TORQUE", loco::eActuatorType::TORQUE )
                .value( "POSITION", loco::eActuatorType::POSITION )
                .value( "VELOCITY", loco::eActuatorType::VELOCITY )
                .value( "PD_CONTROLLER", loco::eActuatorType::PD_CONTROLLER );
        }

        // Bindings for base structs
        {
            py::class_< THeightFieldData >( m, "HeightFieldData" )
                .def( py::init<>() )
                .def_readwrite( "nWidthSamples", &THeightFieldData::nWidthSamples )
                .def_readwrite( "nDepthSamples", &THeightFieldData::nDepthSamples )
                .def_property( "heights",
                    []( const THeightFieldData* self ) -> py::array_t<float>
                        {
                            return loco::vecgrid_to_nparray<float>( self->heights );
                        },
                    []( THeightFieldData* self, const py::array_t<float>& arr_heightData ) -> void
                        {
                            self->heights = loco::nparray_to_vecgrid<float>( arr_heightData );
                        } )
                .def( "__repr__", []( const THeightFieldData* self )
                    {
                        auto _strrep = std::string( "HeightFieldData(\n" );
                        _strrep += "cpp-address     : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "nWidthSamples   : " + std::to_string( self->nWidthSamples ) + "\n";
                        _strrep += "nDepthSamples   : " + std::to_string( self->nDepthSamples ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_< TShapeData >( m, "ShapeData" )
                .def( py::init<>() )
                .def_readwrite( "type", &TShapeData::type )
                .def_property( "size",
                    []( const TShapeData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->size );
                        },
                    []( TShapeData* self, const py::array_t<TScalar>& arr_size ) -> void
                        {
                            self->size = tinymath::nparray_to_vector<TScalar, 3>( arr_size );
                        } )
                .def_readwrite( "mesh_data", &TShapeData::mesh_data, py::return_value_policy::reference )
                .def_readwrite( "hfield_data", &TShapeData::hfield_data, py::return_value_policy::reference )
                .def_property( "localTf",
                    []( const TShapeData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTransform );
                        },
                    []( TShapeData* self, const py::array_t<TScalar>& arr_localTf )
                        {
                            self->localTransform = tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf );
                        } )
                .def( "__repr__", []( const TShapeData* self )
                    {
                        auto _strrep = std::string( "ShapeData(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "type        : " + loco::ToString( self->type ) + "\n";
                        _strrep += "size        : " + loco::ToString( self->size ) + "\n";
                        if ( self->type == eShapeType::MESH )
                        {
                            auto& mesh_data = self->mesh_data;
                            _strrep += "mesh-file   : " + mesh_data.filename + "\n";
                            _strrep += "mesh-nverts : " + std::to_string( mesh_data.vertices.size() ) + "\n";
                            _strrep += "mesh-nfaces : " + std::to_string( mesh_data.faces.size() ) + "\n";
                        }
                        if ( self->type == eShapeType::HFIELD )
                        {
                            auto& hfield_data = self->hfield_data;
                            _strrep += "nwidth-sampl: " + std::to_string( hfield_data.nWidthSamples ) + "\n";
                            _strrep += "ndepth-sampl: " + std::to_string( hfield_data.nDepthSamples ) + "\n";
                        }
                        _strrep += "localTf     : \n" + loco::ToString( self->localTransform ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for core structs (used for constructing core components)
        {
            py::class_< TCollisionData, TShapeData >( m, "CollisionData" )
                .def( py::init<>() )
                .def_readwrite( "collisionGroup", &TCollisionData::collisionGroup )
                .def_readwrite( "collisionMask", &TCollisionData::collisionMask )
                .def_property( "friction",
                    []( const TCollisionData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->friction );
                        },
                    []( TCollisionData* self, const py::array_t<TScalar>& arr_friction ) -> void
                        {
                            self->friction = tinymath::nparray_to_vector<TScalar, 3>( arr_friction );
                        } )
                .def_readwrite( "density", &TCollisionData::density )
                .def( "__repr__", []( const TCollisionData* self )
                    {
                        auto _strrep = std::string( "CollisionData(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "type        : " + loco::ToString( self->type ) + "\n";
                        _strrep += "size        : " + loco::ToString( self->size ) + "\n";
                        if ( self->type == eShapeType::MESH )
                        {
                            auto& mesh_data = self->mesh_data;
                            _strrep += "mesh-file   : " + mesh_data.filename + "\n";
                            _strrep += "mesh-nverts : " + std::to_string( mesh_data.vertices.size() ) + "\n";
                            _strrep += "mesh-nfaces : " + std::to_string( mesh_data.faces.size() ) + "\n";
                        }
                        if ( self->type == eShapeType::HFIELD )
                        {
                            auto& hfield_data = self->hfield_data;
                            _strrep += "nwidth-sampl: " + std::to_string( hfield_data.nWidthSamples ) + "\n";
                            _strrep += "ndepth-sampl: " + std::to_string( hfield_data.nDepthSamples ) + "\n";
                        }
                        _strrep += "localTf     : \n" + loco::ToString( self->localTransform ) + "\n";
                        _strrep += "colGroup    : " + std::to_string( self->collisionGroup ) + "\n";
                        _strrep += "colMask     : " + std::to_string( self->collisionMask ) + "\n";
                        _strrep += "friction    : " + loco::ToString( self->friction ) + "\n";
                        _strrep += "density     : " + std::to_string( self->density ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_< TVisualData, TShapeData >( m, "VisualData" )
                .def( py::init<>() )
                .def_property( "ambient",
                    []( const TVisualData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->ambient );
                        },
                    []( TVisualData* self, const py::array_t<TScalar>& arr_ambient ) -> void
                        {
                            self->ambient = tinymath::nparray_to_vector<TScalar, 3>( arr_ambient );
                        } )
                .def_property( "diffuse",
                    []( const TVisualData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->diffuse );
                        },
                    []( TVisualData* self, const py::array_t<TScalar>& arr_diffuse ) -> void
                        {
                            self->diffuse = tinymath::nparray_to_vector<TScalar, 3>( arr_diffuse );
                        } )
                .def_property( "specular",
                    []( const TVisualData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->specular );
                        },
                    []( TVisualData* self, const py::array_t<TScalar>& arr_specular ) -> void
                        {
                            self->specular = tinymath::nparray_to_vector<TScalar, 3>( arr_specular );
                        } )
                .def_readwrite( "shininess", &TVisualData::shininess )
                .def_readwrite( "texture", &TVisualData::texture )
                .def_readwrite( "usesMaterialFromMesh", &TVisualData::usesMaterialFromMesh )
                .def( "__repr__", []( const TVisualData* self )
                    {
                        auto _strrep = std::string( "VisualData(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "type        : " + loco::ToString( self->type ) + "\n";
                        _strrep += "size        : " + loco::ToString( self->size ) + "\n";
                        if ( self->type == eShapeType::MESH )
                        {
                            auto& mesh_data = self->mesh_data;
                            _strrep += "mesh-file   : " + mesh_data.filename + "\n";
                            _strrep += "mesh-nverts : " + std::to_string( mesh_data.vertices.size() ) + "\n";
                            _strrep += "mesh-nfaces : " + std::to_string( mesh_data.faces.size() ) + "\n";
                        }
                        if ( self->type == eShapeType::HFIELD )
                        {
                            auto& hfield_data = self->hfield_data;
                            _strrep += "nwidth-sampl: " + std::to_string( hfield_data.nWidthSamples ) + "\n";
                            _strrep += "ndepth-sampl: " + std::to_string( hfield_data.nDepthSamples ) + "\n";
                        }
                        _strrep += "localTf     : \n" + loco::ToString( self->localTransform ) + "\n";
                        _strrep += "ambient     : " + loco::ToString( self->ambient ) + "\n";
                        _strrep += "diffuse     : " + loco::ToString( self->diffuse ) + "\n";
                        _strrep += "specular    : " + loco::ToString( self->specular ) + "\n";
                        _strrep += "shininess   : " + std::to_string( self->shininess ) + "\n";
                        _strrep += "texture     : " + self->texture + "\n";
                        _strrep += "useMeshMatrl: " + std::string( self->usesMaterialFromMesh ? "True" : "False" ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_< TJointData >( m, "JointData" )
                .def( py::init<>() )
                .def_readwrite( "type", &TJointData::type )
                .def_property( "axis",
                    []( const TJointData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->axis );
                        },
                    []( TJointData* self, const py::array_t<TScalar>& arr_axis ) -> void
                        {
                            self->axis = tinymath::nparray_to_vector<TScalar, 3>( arr_axis );
                        } )
                .def_property( "limits",
                    []( const TJointData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 2>( self->limits );
                        },
                    []( TJointData* self, const py::array_t<TScalar>& arr_limits ) -> void
                        {
                            self->limits = tinymath::nparray_to_vector<TScalar, 2>( arr_limits );
                        } )
                .def_readwrite( "stiffness", &TJointData::stiffness )
                .def_readwrite( "armature", &TJointData::armature )
                .def_readwrite( "damping", &TJointData::damping )
                .def_readwrite( "ref", &TJointData::ref )
                .def_property_readonly( "nqpos", []( const TJointData* self ) -> size_t
                    {
                        return self->nqpos;
                    } )
                .def_property_readonly( "nqvel", []( const TJointData* self ) -> size_t
                    {
                        return self->nqvel;
                    } )
                .def_property( "localTf",
                    []( const TJointData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTransform );
                        },
                    []( TJointData* self, const py::array_t<TScalar>& arr_localTf ) -> void
                        {
                            self->localTransform = tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf );
                        } )
                .def( "__repr__", []( const TJointData* self )
                    {
                        auto _strrep = std::string( "JointData(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "type        : " + loco::ToString( self->type ) + "\n";
                        _strrep += "axis        : " + loco::ToString( self->axis ) + "\n";
                        _strrep += "limits      : " + loco::ToString( self->limits ) + "\n";
                        _strrep += "stiffness   : " + std::to_string( self->stiffness ) + "\n";
                        _strrep += "armature    : " + std::to_string( self->armature ) + "\n";
                        _strrep += "damping     : " + std::to_string( self->damping ) + "\n";
                        _strrep += "ref         : " + std::to_string( self->ref ) + "\n";
                        _strrep += "nqpos       : " + std::to_string( self->nqpos ) + "\n";
                        _strrep += "nqvel       : " + std::to_string( self->nqvel ) + "\n";
                        _strrep += "localTf     : \n" + loco::ToString( self->localTransform ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_< TActuatorData >( m, "ActuatorData" )
                .def( py::init<>() )
                .def_readwrite( "type", &TActuatorData::type )
                .def_property( "limits",
                    []( const TActuatorData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 2>( self->limits );
                        },
                    []( TActuatorData* self, const py::array_t<TScalar>& arr_limits ) -> void
                        {
                            self->limits = tinymath::nparray_to_vector<TScalar, 2>( arr_limits );
                        } )
                .def_property( "gear",
                    []( const TActuatorData* self ) -> py::array_t<TScalar>
                        {
                            return loco::sizef_to_nparray( self->gear );
                        },
                    []( TActuatorData* self, const py::array_t<TScalar>& arr_gear ) -> void
                        {
                            self->gear = loco::nparray_to_sizef( arr_gear );
                        } )
                .def_readwrite( "kp", &TActuatorData::kp )
                .def_readwrite( "kv", &TActuatorData::kv )
                .def_property( "localTf",
                    []( const TActuatorData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTransform );
                        },
                    []( TActuatorData* self, const py::array_t<TScalar>& arr_localTf ) -> void
                        {
                            self->localTransform = tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf );
                        } )
                .def( "__repr__", []( const TActuatorData* self )
                    {
                        auto _strrep = std::string( "ActuatorData(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "type        : " + loco::ToString( self->type ) + "\n";
                        _strrep += "limits      : " + loco::ToString( self->limits ) + "\n";
                        _strrep += "gear        : " + loco::ToString( self->gear ) + "\n";
                        _strrep += "kp          : " + std::to_string( self->kp ) + "\n";
                        _strrep += "kv          : " + std::to_string( self->kv ) + "\n";
                        _strrep += "localTf     : \n" + loco::ToString( self->localTransform ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_< TSensorData >( m, "SensorData" )
                .def( py::init<>() )
                .def_readwrite( "type", &TSensorData::type )
                .def_property( "localTf",
                    []( const TSensorData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTransform );
                        },
                    []( TSensorData* self, const py::array_t<TScalar>& arr_localTf ) -> void
                        {
                            self->localTransform = tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf );
                        } )
                .def( "__repr__", []( const TSensorData* self )
                    {
                        auto _strrep = std::string( "SensorData(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "type        : " + loco::ToString( self->type ) + "\n";
                        _strrep += "localTf     : \n" + loco::ToString( self->localTransform ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_< TContactData >( m, "ContactData" )
                .def( py::init<>() )
                .def_readwrite( "name", &TContactData::name )
                .def_property( "position",
                    []( const TContactData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->position );
                        },
                    []( TContactData* self, const py::array_t<TScalar>& arr_position ) -> void
                        {
                            self->position = tinymath::nparray_to_vector<TScalar, 3>( arr_position );
                        } )
                .def_property( "normal",
                    []( const TContactData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::vector_to_nparray<TScalar, 3>( self->normal );
                        },
                    []( TContactData* self, const py::array_t<TScalar>& arr_normal ) -> void
                        {
                            self->normal = tinymath::nparray_to_vector<TScalar, 3>( arr_normal );
                        } )
                .def( "__repr__", []( const TContactData* self )
                    {
                        auto _strrep = std::string( "ContactData(\n" );
                        _strrep += "position    : " + loco::ToString( self->position ) + "\n";
                        _strrep += "normal      : " + loco::ToString( self->normal ) + "\n";
                        _strrep += "name        : " + self->name + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_< TInertialData >( m, "InertialData" )
                .def( py::init<>() )
                .def_readwrite( "mass", &TInertialData::mass )
                .def_readwrite( "ixx", &TInertialData::ixx )
                .def_readwrite( "ixy", &TInertialData::ixy )
                .def_readwrite( "ixz", &TInertialData::ixz )
                .def_readwrite( "iyy", &TInertialData::iyy )
                .def_readwrite( "iyz", &TInertialData::iyz )
                .def_readwrite( "izz", &TInertialData::izz )
                .def_property( "localTf",
                    []( const TInertialData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTransform );
                        },
                    []( TInertialData* self, const py::array_t<TScalar>& arr_localTf ) -> void
                        {
                            self->localTransform = tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf );
                        } )
                .def( "__repr__", []( const TInertialData* self )
                    {
                        auto _strrep = std::string( "InertialData(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "mass        : " + std::to_string( self->mass ) + "\n";
                        _strrep += "ixx         : " + std::to_string( self->ixx ) + "\n";
                        _strrep += "ixy         : " + std::to_string( self->ixy ) + "\n";
                        _strrep += "ixz         : " + std::to_string( self->ixz ) + "\n";
                        _strrep += "iyy         : " + std::to_string( self->iyy ) + "\n";
                        _strrep += "iyz         : " + std::to_string( self->iyz ) + "\n";
                        _strrep += "izz         : " + std::to_string( self->izz ) + "\n";
                        _strrep += "localTf     : \n" + loco::ToString( self->localTransform ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );

            py::class_< TBodyData >( m, "BodyData" )
                .def( py::init<>() )
                .def_readwrite( "dyntype", &TBodyData::dyntype )
                .def_readwrite( "inertia", &TBodyData::inertia, py::return_value_policy::reference )
                .def_property( "localTf0",
                    []( const TBodyData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar, 4>( self->localTransformZero );
                        },
                    []( TBodyData* self, const py::array_t<TScalar>& arr_localTf0 ) -> void
                        {
                            self->localTransformZero = tinymath::nparray_to_matrix<TScalar, 4>( arr_localTf0 );
                        } )
                .def_readwrite( "collision", &TBodyData::collision, py::return_value_policy::reference )
                .def_readwrite( "visual", &TBodyData::visual, py::return_value_policy::reference )
                .def( "__repr__", []( const TBodyData* self )
                    {
                        auto _strrep = std::string( "BodyData(\n" );
                        _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                        _strrep += "dyntype     : " + loco::ToString( self->dyntype ) + "\n";
                        _strrep += "localTf0    : \n" + loco::ToString( self->localTransformZero ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Bindings for helper functions
        {
            m.def( "ComputeVolumeFromShape", loco::ComputeVolumeFromShape );
        }
    }
}
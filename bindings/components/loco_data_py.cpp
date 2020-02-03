
#include <loco_common_py.h>
#include <components/data.h>

namespace py = pybind11;

namespace loco
{
    void bindings_components( py::module& m )
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
                .value( "STATIC", loco::eDynamicsType::STATIC )
                .value( "KINEMATIC", loco::eDynamicsType::KINEMATIC );

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

            py::enum_< loco::eBodyClassType >( m, "BodyClassType", py::arithmetic() )
                .value( "NONE", loco::eBodyClassType::NONE )
                .value( "SINGLE_BODY", loco::eBodyClassType::SINGLE_BODY )
                .value( "COMPOUND_BODY", loco::eBodyClassType::COMPOUND_BODY )
                .value( "KINEMATIC_TREE_BODY", loco::eBodyClassType::KINEMATIC_TREE_BODY );
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
                            return loco::vecgrid_to_nparray<float>( self->heightData );
                        },
                    []( THeightFieldData* self, const py::array_t<float>& arr_heightData ) -> void
                        {
                            self->heightData = loco::nparray_to_vecgrid<float>( arr_heightData );
                        } )
                .def( "__repr__", []( const THeightFieldData* self )
                    {
                        auto _strrep = std::string( "HeightFieldData(\n" );
                        _strrep += "cpp-address     : " + loco::pointerToHexAddress( self ) + "\n";
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
                            return tinymath::vector_to_nparray<TScalar,3>( self->size );
                        },
                    []( TShapeData* self, const py::array_t<TScalar>& arr_size ) -> void
                        {
                            self->size = tinymath::nparray_to_vector<TScalar,3>( arr_size );
                        } )
                .def_readwrite( "filename", &TShapeData::filename )
                .def_readwrite( "hdata", &TShapeData::hdata )
                .def_property( "localTf",
                    []( const TShapeData* self ) -> py::array_t<TScalar>
                        {
                            return tinymath::matrix_to_nparray<TScalar,4>( self->localTransform );
                        },
                    []( TShapeData* self, const py::array_t<TScalar>& arr_localTf )
                        {
                            self->localTransform = tinymath::nparray_to_matrix<TScalar,4>( arr_localTf );
                        } )
                .def( "__repr__", []( const TShapeData* self )
                    {
                        auto _strrep = std::string( "TShapeData(\n" );
                        _strrep += "cpp-address : " + loco::pointerToHexAddress( self ) + "\n";
                        _strrep += "type        : " + loco::toString( self->type ) + "\n";
                        _strrep += "size        : " + loco::toString( self->size ) + "\n";
                        _strrep += "filename    : " + self->filename + "\n";
                        _strrep += "localTf     : \n" + loco::toString( self->localTransform ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }
    }
}
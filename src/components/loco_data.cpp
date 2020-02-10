
#include <components/loco_data.h>

namespace loco
{
    std::string ToString( const eShapeType& type )
    {
        if ( type == eShapeType::BOX ) return "box";
        if ( type == eShapeType::SPHERE ) return "sphere";
        if ( type == eShapeType::CYLINDER ) return "cylinder";
        if ( type == eShapeType::CAPSULE ) return "capsule";
        if ( type == eShapeType::MESH ) return "mesh";
        if ( type == eShapeType::HFIELD ) return "hfield";
        if ( type == eShapeType::NONE ) return "none";

        return "undefined";
    }

    std::string ToString( const eJointType& type )
    {
        if ( type == eJointType::FREE ) return "free";
        if ( type == eJointType::FIXED ) return "fixed";
        if ( type == eJointType::REVOLUTE ) return "revolute";
        if ( type == eJointType::PRISMATIC ) return "prismatic";
        if ( type == eJointType::SPHERICAL ) return "spherical";
        if ( type == eJointType::PLANAR ) return "planar";

        return "undefined";
    }

    std::string ToString( const eDynamicsType& type )
    {
        if ( type == eDynamicsType::DYNAMIC ) return "dynamic";
        if ( type == eDynamicsType::STATIC ) return "static";
        if ( type == eDynamicsType::KINEMATIC ) return "kinematic";

        return "undefined";
    }

    std::string ToString( const eSensorType& type )
    {
        if ( type ==  eSensorType::PROP_JOINT ) return "proprioceptive_joint";
        if ( type ==  eSensorType::PROP_BODY ) return "proprioceptive_body";
        if ( type ==  eSensorType::EXT_HEIGHTFIELD_1D ) return "exteroceptive_hfield_1d";
        if ( type ==  eSensorType::EXT_HEIGHTFIELD_2D ) return "exteroceptive_hfield_2d";
        if ( type ==  eSensorType::EXT_CAMERA_RGB ) return "exteroceptive_camera_rgb";
        if ( type ==  eSensorType::EXT_CAMERA_DEPTH ) return "exteroceptive_camera_depth";
        if ( type ==  eSensorType::EXT_CAMERA_SEGMENTATION ) return "exteroceptive_camera_segm";

        return "undefined";
    }

    std::string ToString( const eActuatorType& type )
    {
        if ( type == eActuatorType::TORQUE ) return "torque";
        if ( type == eActuatorType::POSITION ) return "position";
        if ( type == eActuatorType::VELOCITY ) return "velocity";
        if ( type == eActuatorType::PD_CONTROLLER ) return "pd_controller";

        return "undefined";
    }

    std::string ToString( const eBodyClassType& type )
    {
        if ( type == eBodyClassType::NONE ) return "none";
        if ( type == eBodyClassType::SINGLE_BODY ) return "single_body";
        if ( type == eBodyClassType::COMPOUND_BODY ) return "compound_body";
        if ( type == eBodyClassType::KINEMATIC_TREE_BODY ) return "kinematic_tree_body";

        return "undefined";
    }

    eShapeType toEnumShape( const std::string& shape )
    {
        if ( shape == "box" ) return eShapeType::BOX;
        if ( shape == "plane" ) return eShapeType::PLANE;
        if ( shape == "sphere" ) return eShapeType::SPHERE;
        if ( shape == "capsule" ) return eShapeType::CAPSULE;
        if ( shape == "cylinder" ) return eShapeType::CYLINDER;
        if ( shape == "ellipsoid" ) return eShapeType::ELLIPSOID;
        if ( shape == "mesh" ) return eShapeType::MESH;
        if ( shape == "hfield" ) return eShapeType::HFIELD;

        std::cout << "ERROR> not supported shape type: " << shape << std::endl;

        return eShapeType::NONE;
    }

    eJointType toEnumJoint( const std::string& type )
    {
        if ( type == "free" || type == "floating" || type == "none" ) return eJointType::FREE;
        if ( type == "hinge" || type == "revolute" || type == "continuous" ) return eJointType::REVOLUTE;
        if ( type == "slide" || type == "prismatic" ) return eJointType::PRISMATIC;
        if ( type == "ball" || type == "spherical" ) return eJointType::SPHERICAL;
        if ( type == "fixed" || type == "world" ) return eJointType::FIXED;
        if ( type == "planar" ) return eJointType::PLANAR;

        std::cout << "ERROR> not supported joint type: " << type << std::endl;

        return eJointType::FIXED;
    }

    eActuatorType toEnumActuator( const std::string& type )
    {
        if ( type == "motor" ) return eActuatorType::TORQUE;
        if ( type == "position" ) return eActuatorType::POSITION;
        if ( type == "velocity" ) return eActuatorType::VELOCITY;

        std::cout << "ERROR> not supported actuator type: " << type << std::endl;

        return eActuatorType::TORQUE;
    }

    std::string ToString( const TShapeData& shapeData )
    {
        std::string _res;

        _res += "type       : " + ToString( shapeData.type ) + "\n\r";
        _res += "size       : " + ToString( shapeData.size ) + "\n\r";
        _res += "localTf    : \n\r" + ToString( shapeData.localTransform ) + "\n\r";

        return _res;
    }

    TScalar ComputeVolumeFromShape( const TShapeData& shapeData )
    {
        TScalar _volume = 0.1f;

        if ( shapeData.type == eShapeType::BOX )
        {
            _volume = ( shapeData.size.x() * shapeData.size.y() * shapeData.size.z() );
        }
        else if ( shapeData.type == eShapeType::PLANE )
        {
            _volume = 0.0f; // planes are only static|kinematic
        }
        else if ( shapeData.type == eShapeType::SPHERE )
        {
            _volume = ( 4.0f / 3.0f ) * loco::PI * ( shapeData.size.x() * shapeData.size.x() * shapeData.size.x() );
        }
        else if ( shapeData.type == eShapeType::CYLINDER )
        {
            auto _cylinderRadius = shapeData.size.x();
            auto _cylinderHeight = shapeData.size.y();

            _volume = ( loco::PI * _cylinderRadius * _cylinderRadius ) * _cylinderHeight;
        }
        else if ( shapeData.type == eShapeType::CAPSULE )
        {
            auto _capsuleRadius = shapeData.size.x();
            auto _capsuleHeight = shapeData.size.y();

            _volume = ( loco::PI * _capsuleRadius * _capsuleRadius ) * _capsuleHeight +
                      ( 4.0f / 3.0f ) * ( loco::PI * _capsuleRadius * _capsuleRadius * _capsuleRadius );
        }
        else if ( shapeData.type == eShapeType::ELLIPSOID )
        {
            _volume = ( 4.0f / 3.0f ) * loco::PI * ( shapeData.size.x() * shapeData.size.y() * shapeData.size.z() );
        }
        else if ( shapeData.type == eShapeType::MESH )
        {
            std::pair<TVec3, TVec3> _aabb = ComputeMeshAABB( shapeData.filename );
            _volume = std::abs( ( _aabb.second.x() - _aabb.first.x() ) * shapeData.size.x() *
                                ( _aabb.second.y() - _aabb.first.y() ) * shapeData.size.y() *
                                ( _aabb.second.z() - _aabb.first.z() ) * shapeData.size.z() );
        }
        else if ( shapeData.type == eShapeType::HFIELD )
        {
            _volume = 0.0f; // heightfields are only static|kinematic
        }
        else
        {
            LOCO_CORE_WARN( "Unsupported type \"{0}\" for volume computation", ToString( shapeData.type ) );
        }

        return _volume;
    }

    std::pair<TVec3, TVec3> ComputeMeshAABB( const std::string& filename )
    {
        static std::unordered_map<std::string, std::pair< TVec3, TVec3 >> _cachedBounds;
        if ( _cachedBounds.find( filename ) != _cachedBounds.end() )
            return _cachedBounds[filename];

        /* load mesh vertices to compute the aabb (adapted from DART's MeshShape::loadMesh method) */

        // hint loader to remove points and lines
        aiPropertyStore* _propertyStore = aiCreatePropertyStore();
        aiSetImportPropertyInteger( _propertyStore, 
                                    AI_CONFIG_PP_SBP_REMOVE, 
                                    aiPrimitiveType_POINT | aiPrimitiveType_LINE );

        // load mesh with some extra properties
        const aiScene* _sceneMesh = aiImportFileExWithProperties( filename.c_str(),
                                                                  aiProcess_GenNormals | aiProcess_Triangulate |
                                                                    aiProcess_JoinIdenticalVertices |
                                                                    aiProcess_SortByPType | aiProcess_OptimizeMeshes |
                                                                    aiProcess_PreTransformVertices,
                                                                  nullptr,
                                                                  _propertyStore );
        aiReleasePropertyStore( _propertyStore );

        if ( !_sceneMesh )
        {
            LOCO_CORE_ERROR( "Couldn't load mesh {0}, while trying to compute aabb for volume-computation", filename );
            _cachedBounds[filename] = { TVec3( 0.0f, 0.0f, 0.0f ), TVec3( 0.1f, 0.1f, 0.1f ) };
            return _cachedBounds[filename];
        }

        // compute min-max extensions by traversing all vertices
        TVec3 _aabbMin, _aabbMax;
        for ( int i = 0; i < _sceneMesh->mNumMeshes; i++ )
        {
            for ( int j = 0; j < _sceneMesh->mMeshes[i]->mNumFaces; j++ )
            {
                for ( int k = 0; k < 3; k++ )
                {
                    const aiVector3D& _vertex = 
                            _sceneMesh->mMeshes[i]->mVertices[_sceneMesh->mMeshes[i]->mFaces[j].mIndices[k]];

                    if ( i == 0 && j == 0 && k == 0 )
                    {
                        _aabbMin = _aabbMax = { _vertex.x, _vertex.y, _vertex.z };
                        continue;
                    }

                    _aabbMin.x() = std::min( (float)_vertex.x, _aabbMin.x() );
                    _aabbMin.y() = std::min( (float)_vertex.y, _aabbMin.y() );
                    _aabbMin.z() = std::min( (float)_vertex.z, _aabbMin.z() );

                    _aabbMax.x() = std::max( (float)_vertex.x, _aabbMax.x() );
                    _aabbMax.y() = std::max( (float)_vertex.y, _aabbMax.y() );
                    _aabbMax.z() = std::max( (float)_vertex.z, _aabbMax.z() );
                }
            }
        }

        _cachedBounds[filename] = { _aabbMin, _aabbMax };
        return _cachedBounds[filename];
    }
}
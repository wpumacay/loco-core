
#include <components/data.h>

namespace tysoc {

    std::string toString( const eShapeType& type )
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

    std::string toString( const eJointType& type )
    {
        if ( type == eJointType::FREE ) return "free";
        if ( type == eJointType::FIXED ) return "fixed";
        if ( type == eJointType::REVOLUTE ) return "revolute";
        if ( type == eJointType::PRISMATIC ) return "prismatic";
        if ( type == eJointType::SPHERICAL ) return "spherical";
        if ( type == eJointType::PLANAR ) return "planar";

        return "undefined";
    }

    std::string toString( const eDynamicsType& type )
    {
        if ( type == eDynamicsType::DYNAMIC ) return "dynamic";
        if ( type == eDynamicsType::STATIC ) return "static";
        if ( type == eDynamicsType::KINEMATIC ) return "kinematic";

        return "undefined";
    }

    std::string toString( const eSensorType& type )
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

    std::string toString( const eActuatorType& type )
    {
        if ( type == eActuatorType::TORQUE ) return "torque";
        if ( type == eActuatorType::POSITION ) return "position";
        if ( type == eActuatorType::VELOCITY ) return "velocity";
        if ( type == eActuatorType::PD_CONTROLLER ) return "pd_controller";

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

    std::string toString( const TShapeData& shapeData )
    {
        std::string _res;

        _res += "type       : " + toString( shapeData.type ) + "\n\r";
        _res += "size       : " + tysoc::TVec3::toString( shapeData.size ) + "\n\r";
        _res += "localTf    : \n\r" + tysoc::TMat4::toString( shapeData.localTransform ) + "\n\r";

        return _res;
    }

    TScalar computeVolumeFromShape( const TShapeData& shapeData )
    {
        TScalar _volume = 0.1f;

        if ( shapeData.type == eShapeType::BOX )
        {
            _volume = ( shapeData.size.x * shapeData.size.y * shapeData.size.z );
        }
        else if ( shapeData.type == eShapeType::SPHERE )
        {
            _volume = ( 4.0f / 3.0f ) * TYSOC_PI * ( shapeData.size.x * shapeData.size.x * shapeData.size.x );
        }
        else if ( shapeData.type == eShapeType::CYLINDER )
        {
            auto _cylinderRadius = shapeData.size.x;
            auto _cylinderHeight = shapeData.size.y;

            _volume = ( TYSOC_PI * _cylinderRadius * _cylinderRadius ) * _cylinderHeight;
        }
        else if ( shapeData.type == eShapeType::CAPSULE )
        {
            auto _capsuleRadius = shapeData.size.x;
            auto _capsuleHeight = shapeData.size.y;

            _volume = ( TYSOC_PI * _capsuleRadius * _capsuleRadius ) * _capsuleHeight +
                      ( 4.0f / 3.0f ) * ( TYSOC_PI * _capsuleRadius * _capsuleRadius * _capsuleRadius );
        }
        else
        {
            std::cout << "WARNING> unsupported type: " << tysoc::toString( shapeData.type ) << " for volume computation" << std::endl;
        }

        return _volume;
    }

}
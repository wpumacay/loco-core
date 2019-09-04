
#include <components/data.h>

namespace tysoc {

    std::string toString( const eShapeType& type )
    {
        if ( type == eShapeType::BOX )
            return "box";

        if ( type == eShapeType::SPHERE )
            return "sphere";

        if ( type == eShapeType::CYLINDER )
            return "cylinder";

        if ( type == eShapeType::CAPSULE )
            return "capsule";

        if ( type == eShapeType::MESH )
            return "mesh";

        if ( type == eShapeType::NONE )
            return "none";

        return "undefined";
    }

    std::string toString( const eJointType& type )
    {
        if ( type == eJointType::FREE )
            return "free";

        if ( type == eJointType::FIXED )
            return "fixed";

        if ( type == eJointType::REVOLUTE )
            return "revolute";

        if ( type == eJointType::PRISMATIC )
            return "prismatic";

        if ( type == eJointType::SPHERICAL )
            return "spherical";

        if ( type == eJointType::PLANAR )
            return "planar";

        return "undefined";
    }

    std::string toString( const TShapeData& shapeData )
    {
        std::string _res;

        _res += "type       : " + toString( shapeData.type ) + "\n\r";
        _res += "size       : " + tysoc::TVec3::toString( shapeData.size ) + "\n\r";
        _res += "local-pos  : " + tysoc::TVec3::toString( shapeData.localPos ) + "\n\r";
        _res += "local-rot  : \n\r" + tysoc::TMat3::toString( shapeData.localRot ) + "\n\r";

        return _res;
    }

}
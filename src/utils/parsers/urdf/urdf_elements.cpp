
#include <utils/parsers/urdf/urdf.h>


namespace tysoc {
namespace urdf {

    void UrdfMaterial::collectAttribs( tinyxml2::XMLElement* xmlElement )
    {
        if ( !xmlElement->Attribute( "name" ) )
        {
            parsing::logError( "Material must contain a name attribute" );
            return;
        }

        // name
        this->name = xml::safeParseString( xmlElement, "name" );

        // Check if the material has already been cached
        if ( UrdfMaterial::MATERIALS.find( this->name ) != UrdfMaterial::MATERIALS.end() )
        {
            this->filename  = UrdfMaterial::MATERIALS[ this->name ]->filename;
            this->color     = UrdfMaterial::MATERIALS[ this->name ]->color;
        }
        else
        {
            // texture
            auto _textureElement = xmlElement->FirstChildElement( "texture" );
            if ( _textureElement )
                this->filename = xml::safeParseString( _textureElement, "filename" );

            // color
            auto _colorElement = xmlElement->FirstChildElement( "color" );
            if ( _colorElement )
                this->color = xml::safeParseVec4( _colorElement, "rgba" );

            UrdfMaterial::MATERIALS[ this->name ] = this;
        }
    }

    void UrdfInertia::collectAttribs( tinyxml2::XMLElement* xmlElement )
    {
        // Grab the local transform
        auto _originElement = xmlElement->FirstChildElement( "origin" );
        if ( _originElement )
        {
            // grab origin(xyz) and euler(rpy)
            auto _xyz = xml::safeParseVec3( _originElement, "xyz" );
            auto _rpy = xml::safeParseVec3( _originElement, "rpy" );

            // create the transform
            this->localTransform.setPosition( _xyz );
            this->localTransform.setRotation( TMat3::fromEuler( _rpy ) );
        }

        // Mass
        auto _massElement = xmlElement->FirstChildElement( "mass" );
        if ( _massElement )
            this->mass = xml::safeParseFloat( _massElement, "value" );

        // Inertia
        auto _inertiaElement = xmlElement->FirstChildElement( "inertia" );
        if ( _inertiaElement )
        {
            this->ixx = xml::safeParseFloat( _inertiaElement, "ixx" );
            this->ixy = xml::safeParseFloat( _inertiaElement, "ixy" );
            this->ixz = xml::safeParseFloat( _inertiaElement, "ixz" );
            this->iyy = xml::safeParseFloat( _inertiaElement, "iyy" );
            this->iyz = xml::safeParseFloat( _inertiaElement, "iyz" );
            this->izz = xml::safeParseFloat( _inertiaElement, "izz" );
        }
    }

    void UrdfGeometry::collectAttribs( tinyxml2::XMLElement* xmlElement )
    {
        auto _shapeElement = xmlElement->FirstChildElement();

        if ( !_shapeElement )
        {
            parsing::logError( "Geometry tag contains no child element" );
            return;
        }

        std::string _typeName = _shapeElement->Value();
        if ( _typeName == "sphere" )
        {
            this->type      = "sphere";
            this->size.x    = xml::safeParseFloat( _shapeElement, "radius" );
        }
        else if ( _typeName == "box" )
        {
            this->type  = "box";
            this->size  = xml::safeParseVec3( _shapeElement, "size" );
        }
        else if ( _typeName == "cylinder" )
        {
            this->type      = "cylinder";
            this->size.x    = xml::safeParseFloat( _shapeElement, "radius" );
            this->size.y    = xml::safeParseFloat( _shapeElement, "length" );
        }
        else if ( _typeName == "capsule" )
        {
            this->type      = "capsule";
            this->size.x    = xml::safeParseFloat( _shapeElement, "radius" );
            this->size.y    = xml::safeParseFloat( _shapeElement, "length" );
        }
        else if ( _typeName == "mesh" )
        {
            this->type      = "mesh";
            this->size      = xml::safeParseVec3( _shapeElement, "scale", { 1, 1, 1 } );
            this->filename  = xml::safeParseString( _shapeElement, "filename", "" );
        }
        else if ( _typeName == "plane" )
        {
            this->type      = "plane";
            this->size.x    = xml::safeParseFloat( _shapeElement, "width", 1 );
            this->size.y    = xml::safeParseFloat( _shapeElement, "depth", 1 );
        }
        else
        {
            std::string _msg;

            _msg += "Unknown geometry type: ";
            _msg += _typeName;

            parsing::logError( _msg );
        }
    }

    void UrdfShape::collectAttribs( tinyxml2::XMLElement* xmlElement )
    {
        // Grab the local transform
        auto _originElement = xmlElement->FirstChildElement( "origin" );
        if ( _originElement )
        {
            // grab origin(xyz) and euler(rpy)
            auto _xyz = xml::safeParseVec3( _originElement, "xyz" );
            auto _rpy = xml::safeParseVec3( _originElement, "rpy" );

            // create transform
            this->localTransform.setPosition( _xyz );
            this->localTransform.setRotation( TMat3::fromEuler( _rpy ) );
        }

        // Grab the geometry info
        auto _geometryElement = xmlElement->FirstChildElement( "geometry" );
        if ( _geometryElement )
            this->geometry->collectAttribs( _geometryElement );
    }

    void UrdfVisual::collectAttribs( tinyxml2::XMLElement* xmlElement )
    {
        UrdfShape::collectAttribs( xmlElement );

        // Grab the name (optional)
        this->name = xml::safeParseString( xmlElement, "name" );

        // Grab the material
        auto _materialElement = xmlElement->FirstChildElement( "material" );
        if ( _materialElement )
            this->material->collectAttribs( _materialElement );
    }

    void UrdfCollision::collectAttribs( tinyxml2::XMLElement* xmlElement )
    {
        UrdfShape::collectAttribs( xmlElement );

        // Grab the name (optional)
        this->name = xml::safeParseString( xmlElement, "name" );
    }

    void UrdfLink::collectAttribs( tinyxml2::XMLElement* xmlElement )
    {
        // Grab the name
        this->name = xml::safeParseString( xmlElement, "name" );

        // Inertial (optional)
        auto _inertialElement = xmlElement->FirstChildElement( "inertial" );
        if ( _inertialElement )
        {
            this->inertia = new UrdfInertia();
            this->inertia->collectAttribs( _inertialElement );
        }

        // Multiple Visuals (optional)
        for ( auto _visualElement = xmlElement->FirstChildElement( "visual" ); 
                   _visualElement != NULL; 
                   _visualElement = _visualElement->NextSiblingElement( "visual" ) )
        {
            auto _visual = new UrdfVisual();
            _visual->collectAttribs( _visualElement );
            
            this->visuals.push_back( _visual );
        }

        // Multiple Collisions (optional)
        for ( auto _collisionElement = xmlElement->FirstChildElement( "collision" ); 
                   _collisionElement != NULL; 
                   _collisionElement = _collisionElement->NextSiblingElement( "collision" ) )
        {
            auto _collision = new UrdfCollision();
            _collision->collectAttribs( _collisionElement );
        }
    }

    void UrdfJoint::collectAttribs( tinyxml2::XMLElement* xmlElement )
    {
        // Grab Joint Name and type
        this->name = xml::safeParseString( xmlElement, "name" );
        this->type = xml::safeParseString( xmlElement, "type" );

        // Grab the local transform (parentLinkToJointTransform)
        auto _originElement = xmlElement->FirstChildElement( "origin" );
        if ( _originElement )
        {
            // grab origin(xyz) and euler(rpy)
            auto _xyz = xml::safeParseVec3( _originElement, "xyz" );
            auto _rpy = xml::safeParseVec3( _originElement, "rpy" );

            // create transform
            this->parentLinkToJointTransform.setPosition( _xyz );
            this->parentLinkToJointTransform.setRotation( TMat3::fromEuler( _rpy ) );
        }

        // Grab parent link name
        auto _parentElement = xmlElement->FirstChildElement( "parent" );
        if ( _parentElement )
            this->parentLinkName = xml::safeParseString( _parentElement, "link" );

        // Grab child link name
        auto _childElement = xmlElement->FirstChildElement( "child" );
        if ( _childElement )
            this->childLinkName = xml::safeParseString( _childElement, "link" );

        // Grab Joint Axis
        if ( this->type != "floating" && this->type != "fixed" )
        {
            auto _axisElement = xmlElement->FirstChildElement( "axis" );
            if ( _axisElement )
                this->localJointAxis = xml::safeParseVec3( _axisElement, "xyz", { 1, 0, 0 } );
        }

        // Get limit
        auto _limitElement = xmlElement->FirstChildElement( "limit" );
        if ( _limitElement )
        {
            this->lowerLimit    = xml::safeParseFloat( _limitElement, "lower" );
            this->upperLimit    = xml::safeParseFloat( _limitElement, "upper" );
            this->effortLimit   = xml::safeParseFloat( _limitElement, "effort" );
            this->velocityLimit = xml::safeParseFloat( _limitElement, "velocityLimit" );
        }

        // Sanity check, make the user clean its urdf file
        if ( xmlElement->FirstChildElement( "joint_properties" ) )
            parsing::logWarning( "replace non-standard joint_properties node by dynamics node" );

        // Get Dynamics
        auto _dynamicsElement = xmlElement->FirstChildElement( "dynamics" );
        if ( _dynamicsElement )
        {
            this->jointDamping  = xml::safeParseFloat( _dynamicsElement, "damping" );
            this->jointFriction = xml::safeParseFloat( _dynamicsElement, "friction" );
        }
    }
}}

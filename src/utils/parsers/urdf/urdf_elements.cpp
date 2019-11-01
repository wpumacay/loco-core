
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
            this->filename  = UrdfMaterial::MATERIALS[ this->name ].filename;
            this->color     = UrdfMaterial::MATERIALS[ this->name ].color;
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

            UrdfMaterial::MATERIALS[ this->name ] = *this;
        }
    }

    void UrdfInertia::collectAttribs( tinyxml2::XMLElement* xmlElement,
                                      const std::string& worldUp )
    {
        // Grab the local transform
        auto _originElement = xmlElement->FirstChildElement( "origin" );
        if ( _originElement )
        {
            // grab origin(xyz) and euler(rpy)
            auto _xyz = rearrange( xml::safeParseVec3( _originElement, "xyz" ), worldUp );
            auto _rpy = rearrange( xml::safeParseVec3( _originElement, "rpy" ), worldUp );

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
            if ( worldUp == "x" )
            {
                this->ixx = xml::safeParseFloat( _inertiaElement, "iyy" );
                this->iyy = xml::safeParseFloat( _inertiaElement, "izz" );
                this->izz = xml::safeParseFloat( _inertiaElement, "ixx" );

                this->ixy = xml::safeParseFloat( _inertiaElement, "iyz" );
                this->ixz = xml::safeParseFloat( _inertiaElement, "ixy" );
                this->iyz = xml::safeParseFloat( _inertiaElement, "ixz" );
            }
            else if ( worldUp == "y" )
            {
                this->ixx = xml::safeParseFloat( _inertiaElement, "izz" );
                this->iyy = xml::safeParseFloat( _inertiaElement, "ixx" );
                this->izz = xml::safeParseFloat( _inertiaElement, "iyy" );

                this->ixy = xml::safeParseFloat( _inertiaElement, "ixz" );
                this->ixz = xml::safeParseFloat( _inertiaElement, "iyz" );
                this->iyz = xml::safeParseFloat( _inertiaElement, "ixy" );
            }
            else // Assumming z
            {
                this->ixx = xml::safeParseFloat( _inertiaElement, "ixx" );
                this->iyy = xml::safeParseFloat( _inertiaElement, "iyy" );
                this->izz = xml::safeParseFloat( _inertiaElement, "izz" );

                this->ixy = xml::safeParseFloat( _inertiaElement, "ixy" );
                this->ixz = xml::safeParseFloat( _inertiaElement, "ixz" );
                this->iyz = xml::safeParseFloat( _inertiaElement, "iyz" );
            }
            
        }
    }

    void UrdfShape::collectAttribs( tinyxml2::XMLElement* xmlElement,
                                    const std::string& worldUp )
    {
        // Grab the local transform
        auto _originElement = xmlElement->FirstChildElement( "origin" );
        if ( _originElement )
        {
            // grab origin(xyz) and euler(rpy)
            auto _xyz = rearrange( xml::safeParseVec3( _originElement, "xyz" ), worldUp );
            auto _rpy = rearrange( xml::safeParseVec3( _originElement, "rpy" ), worldUp );

            // create transform
            this->localTransform.setPosition( _xyz );
            this->localTransform.setRotation( TMat3::fromEuler( _rpy ) );
        }

        // Grab the geometry info
        auto _geometryElement = xmlElement->FirstChildElement( "geometry" );
        if ( _geometryElement )
        {
            auto _geometryDataElement = _geometryElement->FirstChildElement();

            if ( !_geometryDataElement )
            {
                parsing::logError( "Geometry tag contains no child element with required geometry data" );
                return;
            }

            std::string _typeName = _geometryDataElement->Value();
            if ( _typeName == "sphere" )
            {
                this->type      = "sphere";
                this->size.x    = xml::safeParseFloat( _geometryDataElement, "radius" );
            }
            else if ( _typeName == "box" )
            {
                this->type  = "box";
                this->size  = rearrange( xml::safeParseVec3( _geometryDataElement, "size" ), worldUp );
            }
            else if ( _typeName == "cylinder" )
            {
                this->type      = "cylinder";
                this->size.x    = xml::safeParseFloat( _geometryDataElement, "radius" );
                this->size.y    = xml::safeParseFloat( _geometryDataElement, "length" );
            }
            else if ( _typeName == "capsule" )
            {
                this->type      = "capsule";
                this->size.x    = xml::safeParseFloat( _geometryDataElement, "radius" );
                this->size.y    = xml::safeParseFloat( _geometryDataElement, "length" );
            }
            else if ( _typeName == "mesh" )
            {
                this->type      = "mesh";
                this->size      = xml::safeParseVec3( _geometryDataElement, "scale", { 1, 1, 1 } );
                this->filename  = xml::safeParseString( _geometryDataElement, "filename", "" );
            }
            else if ( _typeName == "plane" )
            {
                this->type      = "plane";
                this->size.x    = xml::safeParseFloat( _geometryDataElement, "width", 1 );
                this->size.y    = xml::safeParseFloat( _geometryDataElement, "depth", 1 );
            }
            else
            {
                parsing::logError( std::string( "Unknown geometry type: " ) + _typeName );
            }
        }
    }

    void UrdfVisual::collectAttribs( tinyxml2::XMLElement* xmlElement,
                                     const std::string& worldUp )
    {
        UrdfShape::collectAttribs( xmlElement, worldUp );

        // Grab the name (optional)
        this->name = xml::safeParseString( xmlElement, "name" );

        // Grab the material
        auto _materialElement = xmlElement->FirstChildElement( "material" );
        if ( _materialElement )
            this->material.collectAttribs( _materialElement );
    }

    void UrdfCollision::collectAttribs( tinyxml2::XMLElement* xmlElement,
                                        const std::string& worldUp )
    {
        UrdfShape::collectAttribs( xmlElement, worldUp );

        // Grab the name (optional)
        this->name = xml::safeParseString( xmlElement, "name" );
    }

    void UrdfLink::collectAttribs( tinyxml2::XMLElement* xmlElement,
                                   const std::string& worldUp )
    {
        // Grab the name
        this->name = xml::safeParseString( xmlElement, "name" );

        // Inertial (optional)
        auto _inertialElement = xmlElement->FirstChildElement( "inertial" );
        if ( _inertialElement )
            this->inertia.collectAttribs( _inertialElement, worldUp );

        // Multiple Visuals (optional)
        for ( auto _visualElement = xmlElement->FirstChildElement( "visual" ); 
                   _visualElement != NULL; 
                   _visualElement = _visualElement->NextSiblingElement( "visual" ) )
        {
            UrdfVisual _visual;
            _visual.collectAttribs( _visualElement, worldUp );
            this->visuals.push_back( _visual );
        }

        // Multiple Collisions (optional)
        for ( auto _collisionElement = xmlElement->FirstChildElement( "collision" ); 
                   _collisionElement != NULL; 
                   _collisionElement = _collisionElement->NextSiblingElement( "collision" ) )
        {
            UrdfCollision _collision;
            _collision.collectAttribs( _collisionElement, worldUp );
            this->collisions.push_back( _collision );
        }
    }

    void UrdfJoint::collectAttribs( tinyxml2::XMLElement* xmlElement,
                                    const std::string& worldUp )
    {
        // Grab Joint Name and type
        this->name = xml::safeParseString( xmlElement, "name" );
        this->type = xml::safeParseString( xmlElement, "type" );

        // Grab the local transform (parentLinkToJointTransform)
        auto _originElement = xmlElement->FirstChildElement( "origin" );
        if ( _originElement )
        {
            // grab origin(xyz) and euler(rpy)
            auto _xyz = rearrange( xml::safeParseVec3( _originElement, "xyz" ), worldUp );
            auto _rpy = rearrange( xml::safeParseVec3( _originElement, "rpy" ), worldUp );

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
                this->localJointAxis = rearrange( xml::safeParseVec3( _axisElement, "xyz", { 1, 0, 0 } ), worldUp );
        }

        // Get limit
        auto _limitElement = xmlElement->FirstChildElement( "limit" );
        if ( _limitElement )
        {
            this->lowerLimit    = xml::safeParseFloat( _limitElement, "lower", -45. );
            this->upperLimit    = xml::safeParseFloat( _limitElement, "upper", 45. );
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

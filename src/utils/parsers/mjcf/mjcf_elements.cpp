
#include <utils/parsers/mjcf/mjcf_elements.h>

namespace tysoc {
namespace mjcf {

    GenericElement::GenericElement()
    {
        etype  = "UNDEFINED";
        parent = NULL;
    }

    GenericElement::GenericElement( const std::string& elementType )
    {
        etype  = elementType;
        parent = NULL;
    }

    void GenericElement::collectAttribs( Schema* schema, tinyxml2::XMLElement* xmlElement )
    {
        etype = xmlElement->Value();

        auto _possibleAttribs = schema->getPossibleAttribs( etype );
        for ( auto const& it : _possibleAttribs )
        {
            auto _attribName = it.first;
            auto _attribType = it.second;

            if ( !xmlElement->Attribute( _attribName.c_str() ) )
            {
                continue;
            }

            if ( _attribType == TYPE_INT )
            {
                _ints[ _attribName ] = safeParseInt( xmlElement, _attribName );
            }
            else if ( _attribType == TYPE_FLOAT )
            {
                _floats[ _attribName ] = safeParseFloat( xmlElement, _attribName );
            }
            else if ( _attribType == TYPE_ARRAY_INT )
            {
                _sizeis[ _attribName ] = safeParseSizei( xmlElement, _attribName );
            }
            else if ( _attribType == TYPE_ARRAY_FLOAT )
            {
                _sizefs[ _attribName ] = safeParseSizef( xmlElement, _attribName );
            }
            else if ( _attribType == TYPE_STRING )
            {
                _strings[ _attribName ] = std::string( xmlElement->Attribute( _attribName.c_str() ) );
            }
        }
    }
    
    void GenericElement::insertAttribs( tinyxml2::XMLElement* xmlElement )
    {
        // insert ints
        for ( auto const& it : _ints )
        {
            xmlElement->SetAttribute( it.first.c_str(), std::to_string( it.second ).c_str() );
        }
        // insert floats
        for ( auto const& it : _floats )
        {
            xmlElement->SetAttribute( it.first.c_str(), std::to_string( it.second ).c_str() );
        }
        // insert sizeis
        for ( auto const& it : _sizeis )
        {
            xmlElement->SetAttribute( it.first.c_str(), toString( it.second ).c_str() );
        }
        // insert sizefs
        for ( auto const& it : _sizefs )
        {
            xmlElement->SetAttribute( it.first.c_str(), toString( it.second ).c_str() );
        }
        // insert strings
        for ( auto const& it : _strings )
        {
            xmlElement->SetAttribute( it.first.c_str(), it.second.c_str() );
        }
    }

    void GenericElement::print()
    {
        std::cout << "<" << etype;

        // insert ints
        for ( auto const& it : _ints )
        {
            std::cout << it.first << "=\"" << std::to_string( it.second ) << "\" ";
        }
        // insert floats
        for ( auto const& it : _floats )
        {
            std::cout << it.first << "=\"" << std::to_string( it.second ) << "\" ";
        }
        // insert sizeis
        for ( auto const& it : _sizeis )
        {
            std::cout << it.first << "=\"" << toString( it.second ) << "\" ";
        }
        // insert sizefs
        for ( auto const& it : _sizefs )
        {
            std::cout << it.first << "=\"" << toString( it.second ) << "\" ";
        }
        // insert strings
        for ( auto const& it : _strings )
        {
            std::cout << it.first << "=\"" << it.second << "\" ";
        }

        std::cout << "/>" << std::endl;
    }

    void GenericElement::setAttributeInt( const std::string& attribName, int val )
    {
        _ints[ attribName ] = val;
    }

    void GenericElement::setAttributeFloat( const std::string& attribName, float val )
    {
        _floats[ attribName ] = val;
    }

    void GenericElement::setAttributeArrayInt( const std::string& attribName, const Sizei& arrayint )
    {
        _sizeis[ attribName ] = arrayint;
    }

    void GenericElement::setAttributeArrayFloat( const std::string& attribName, const Sizef& arrayfloat )
    {
        // @TODO: Should check with the schema
        _sizefs[ attribName ] = arrayfloat;
    }

    void GenericElement::setAttributeString( const std::string& attribName, const std::string& val )
    {
        _strings[ attribName ] = val;
    }

    void GenericElement::setAttributeVec3( const std::string& attribName, const Vec3& vec )
    {
        Sizef _svec;
        _svec.ndim = 3;
        _svec.buff[0] = vec.x;
        _svec.buff[1] = vec.y;
        _svec.buff[2] = vec.z;

        _sizefs[ attribName ] = _svec;
    }

    void GenericElement::setAttributeVec4( const std::string& attribName, const Vec4& vec )
    {
        Sizef _svec;
        _svec.ndim = 4;
        _svec.buff[0] = vec.x;
        _svec.buff[1] = vec.y;
        _svec.buff[2] = vec.z;
        _svec.buff[3] = vec.w;

        _sizefs[ attribName ] = _svec;
    }
    
    int GenericElement::getAttributeInt( const std::string& attribName )
    {
        if ( _ints.find( attribName ) != _ints.end() )
        {
            return _ints[ attribName ];
        }

        return 0;
    }

    float GenericElement::getAttributeFloat( const std::string& attribName )
    {
        if ( _floats.find( attribName ) != _floats.end() )
        {
            return _floats[ attribName ];
        }

        return 0.0f;
    }

    Sizef GenericElement::getAttributeArrayFloat( const std::string& attribName )
    {
        if ( _sizefs.find( attribName ) != _sizefs.end() )
        {
            return _sizefs[ attribName ];
        }

        return { 0, { 0.0f } };
    }

    Sizei GenericElement::getAttributeArrayInt( const std::string& attribName )
    {
        if ( _sizeis.find( attribName ) != _sizeis.end() )
        {
            return _sizeis[ attribName ];
        }

        return { 0, { 0 } };
    }

    std::string GenericElement::getAttributeString( const std::string& attribName )
    {
        if ( _strings.find( attribName ) != _strings.end() )
        {
            return _strings[ attribName ];
        }

        return "";
    }

    Vec2 GenericElement::getAttributeVec2( const std::string& attribName, const Vec2& def )
    {
        Vec2 _res;

        if ( _sizefs.find( attribName ) != _sizefs.end() )
        {
            Sizef _vec2 = _sizefs[ attribName ];

            _res.x = _vec2.buff[0];
            _res.y = _vec2.buff[1];
        }
        else
        {
            _res = def;
        }

        return _res;
    }

    Vec3 GenericElement::getAttributeVec3( const std::string& attribName, const Vec3& def )
    {
        Vec3 _res;

        if ( _sizefs.find( attribName ) != _sizefs.end() )
        {
            Sizef _vec3 = _sizefs[ attribName ];

            _res.x = _vec3.buff[0];
            _res.y = _vec3.buff[1];
            _res.z = _vec3.buff[2];
        }
        else
        {
            _res = def;
        }

        return _res;
    }

    Vec4 GenericElement::getAttributeVec4( const std::string& attribName, const Vec4& def )
    {
        Vec4 _res;

        if ( _sizefs.find( attribName ) != _sizefs.end() )
        {
            Sizef _vec4 = _sizefs[ attribName ];
            
            _res.x = _vec4.buff[0];
            _res.y = _vec4.buff[1];
            _res.z = _vec4.buff[2];
            _res.w = _vec4.buff[3];
        }
        else
        {
            _res = def;
        }

        return _res;
    }

    bool GenericElement::hasAttributeVec4( const std::string& attribName )
    {
        if ( _sizefs.find( attribName ) != _sizefs.end() )
        {
            return true;
        }

        return false;
    }

    bool GenericElement::hasAttributeVec3( const std::string& attribName )
    {
        if ( _sizefs.find( attribName ) != _sizefs.end() )
        {
            return true;
        }

        return false;
    }



    GenericElement* createWorldBody()
    {
        return new GenericElement( "worldbody" );
    }

    GenericElement* createBody( const std::string& name,
                                 const Vec3& pos,
                                 const Vec4& quat )
    {
        auto _body = new GenericElement( "body" );

        _body->setAttributeString( "name", name );
        _body->setAttributeVec3( "pos", pos );
        _body->setAttributeVec4( "quat", quat );

        return _body;
    }

    GenericElement* createGeometry( const std::string& name,
                                    const std::string& type,
                                    const Sizef& size,
                                    float mass,
                                    const Vec3& pos,
                                    const Vec4& quat )
    {
        auto _geometry = new GenericElement( "geom" );

        _geometry->setAttributeString( "name", name );
        _geometry->setAttributeString( "type", type );
        _geometry->setAttributeArrayFloat( "size", size );
        _geometry->setAttributeFloat( "mass", mass );
        _geometry->setAttributeVec3( "pos", pos );
        _geometry->setAttributeVec4( "quat", quat );

        return _geometry;
    }

    GenericElement* createGeometry( const std::string& name,
                                    const std::string& type,
                                    const Sizef& size,
                                    const Sizef& fromto,
                                    float mass )
    {
        auto _geometry = new GenericElement( "geom" );

        _geometry->setAttributeString( "name", name );
        _geometry->setAttributeString( "type", type );
        _geometry->setAttributeArrayFloat( "size", size );
        _geometry->setAttributeFloat( "mass", mass );
        _geometry->setAttributeArrayFloat( "fromto", fromto );

        return _geometry;
    }


}}
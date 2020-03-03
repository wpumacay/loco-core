
#include <utils/parsers/mjcf/mjcf_common.h>

namespace tysoc {
namespace mjcf {

    Schema::Schema()
    {

    }

    Schema::~Schema()
    {
        m_attribsPerElement.clear();
    }

    void Schema::load( const std::string& filename )
    {
        tinyxml2::XMLDocument _doc;
        _doc.LoadFile( filename.c_str() );

        _traverse( _doc.FirstChildElement() );
    }

    void Schema::_collectAttributesSchema( const std::string& elementName, 
                                           tinyxml2::XMLElement* xmlAttribsElement )
    {
        // just in case, initialize dictionary
        if ( m_attribsPerElement.find( elementName ) == 
             m_attribsPerElement.end() )
        {
            m_attribsPerElement[ elementName ] = std::map< std::string, AttribType >();
        }

        auto _attribXML = xmlAttribsElement->FirstChildElement( "attribute" );
        while( _attribXML != NULL )
        {
            std::string _attribName = _attribXML->Attribute( "name" );
            std::string _attribType = _attribXML->Attribute( "type" );

            if ( _attribType == "int" )
            {
                m_attribsPerElement[ elementName ][ _attribName ] = TYPE_INT;
            }
            else if ( _attribType == "float" )
            {
                m_attribsPerElement[ elementName ][ _attribName ] = TYPE_FLOAT;
            }
            else if ( _attribType == "array" )
            {
                // get array type
                std::string _attribArrayType = _attribXML->Attribute( "array_type" );
                if ( _attribArrayType == "int" )
                {
                    m_attribsPerElement[ elementName ][ _attribName ] = TYPE_ARRAY_INT;
                }
                else
                {
                    m_attribsPerElement[ elementName ][ _attribName ] = TYPE_ARRAY_FLOAT;
                }
            }
            else if ( _attribType == "keyword" )
            {
                m_attribsPerElement[ elementName ][ _attribName ] = TYPE_STRING;
            }
            else
            {
                // Everything else, just assume string
                m_attribsPerElement[ elementName ][ _attribName ] = TYPE_STRING;
            }

            _attribXML = _attribXML->NextSiblingElement();
        }
    }

    void Schema::_traverse( tinyxml2::XMLElement* xmlElement )
    {
        // parse the attributes that this element may have
        auto _xmlAttribsElement = xmlElement->FirstChildElement( "attributes" );
        // check if this element has attributes
        if ( _xmlAttribsElement )
        {
            auto _elementName = xmlElement->Attribute( "name" );
            _collectAttributesSchema( _elementName, _xmlAttribsElement );
        }

        // traverse the children recursively
        auto _xmlChildrenElement = xmlElement->FirstChildElement( "children" );
        // if there are children to traverse, recurse
        if ( _xmlChildrenElement )
        {
            auto _xmlChildElement = _xmlChildrenElement->FirstChildElement( "element" );
            while ( _xmlChildElement != NULL )
            {
                // traverse to the child
                _traverse( _xmlChildElement );
                // go to the next chold
                _xmlChildElement = _xmlChildElement->NextSiblingElement();
            }
        }
    }

    std::map< std::string, AttribType > Schema::getPossibleAttribs( const std::string& elementName )
    {
        if ( m_attribsPerElement.find( elementName ) == m_attribsPerElement.end() )
        {
            return std::map< std::string, AttribType >();
        }

        return m_attribsPerElement[ elementName ];
    }

    AttribType Schema::getAttribType( const std::string& elementName,
                                      const std::string& attribName )
    {
        if ( m_attribsPerElement.find( elementName ) == m_attribsPerElement.end() )
        {
            return TYPE_UNDEFINED;
        }

        if ( m_attribsPerElement[ elementName ].find( attribName ) == 
             m_attribsPerElement[ elementName ].end() )
        {
            return TYPE_UNDEFINED;
        }

        return m_attribsPerElement[ elementName ][ attribName ];
    }

    void Schema::print()
    {
        for ( auto const& element : m_attribsPerElement )
        {
            std::cout << "elementname: " << element.first << std::endl;
            for ( auto const& attrib : element.second )
            {
                std::cout << "\t" << "attrib> "
                          << "name: " << attrib.first << " - type: ";
                if ( attrib.second == TYPE_INT ) std::cout << "int";
                else if ( attrib.second == TYPE_FLOAT ) std::cout << "float";
                else if ( attrib.second == TYPE_ARRAY_INT ) std::cout << "array int";
                else if ( attrib.second == TYPE_ARRAY_FLOAT ) std::cout << "array float";
                else if ( attrib.second == TYPE_STRING ) std::cout << "string";
                else std::cout << "UNDEFINED";

                std::cout << std::endl;
            }
        }
    }

}}
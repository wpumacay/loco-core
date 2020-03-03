
#include <utils/parsers/parsers_xml.h>

namespace tysoc {
namespace xml {

    std::string safeParseString( tinyxml2::XMLElement* xmlElement,
                                 const std::string& attribName,
                                 const std::string& opt )
    {
        auto _attrib = xmlElement->Attribute( attribName.c_str() );

        if ( _attrib )
        {
            return std::string( _attrib );
        }

        return opt;
    }

    TVec3 safeParseVec3( tinyxml2::XMLElement* xmlElement, 
                         const std::string& attribName, 
                         const TVec3& opt )
    {
        auto _attrib = xmlElement->Attribute( attribName.c_str() );

        if ( _attrib )
        {
            return parsing::_parseVec3( _attrib, opt );
        }
        return opt;
    }

    TVec4 safeParseVec4( tinyxml2::XMLElement* xmlElement, 
                         const std::string& attribName, 
                         const TVec4& opt )
    {
        auto _attrib = xmlElement->Attribute( attribName.c_str() );

        if ( _attrib )
        {
            return parsing::_parseVec4( _attrib, opt );
        }
        return opt;
    }

    int safeParseInt( tinyxml2::XMLElement* xmlElement,
                      const std::string& attribName,
                      int opt )
    {
        auto _attrib = xmlElement->Attribute( attribName.c_str() );

        if ( _attrib )
        {
            return std::stoi( _attrib );
        }
        return opt;
    }

    float safeParseFloat( tinyxml2::XMLElement* xmlElement,
                          const std::string& attribName,
                          float opt )
    {
        auto _attrib = xmlElement->Attribute( attribName.c_str() );

        if ( _attrib )
        {
            return std::stof( _attrib );
        }
        return opt;
    }

    TSizei safeParseSizei( tinyxml2::XMLElement* xmlElement,
                           const std::string& attribName,
                           const TSizei& opt )
    {
        auto _attrib = xmlElement->Attribute( attribName.c_str() );

        if ( _attrib )
        {
            return parsing::_parseSizei( _attrib );
        }
        return opt;
    }

    TSizef safeParseSizef( tinyxml2::XMLElement* xmlElement,
                           const std::string& attribName,
                           const TSizef& opt )
    {
        auto _attrib = xmlElement->Attribute( attribName.c_str() );

        if ( _attrib )
        {
            return parsing::_parseSizef( _attrib );
        }
        return opt;
    }

}}
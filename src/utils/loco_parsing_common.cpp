
#include <utils/loco_parsing_common.h>

namespace loco {
namespace parsing {

    std::vector<std::string> Split( const std::string& str, char delimiter )
    {
        std::vector<std::string> fields;
                    
        int pos = str.find( delimiter );
        if ( pos == std::string::npos )
        {
            fields.push_back( str );
            return fields;
        }

        int initpos = 0;
        while ( pos != std::string::npos )
        {
            fields.push_back( str.substr( initpos, pos - initpos ) );
            initpos = pos + 1;
            pos = str.find( delimiter, initpos );
        }
        fields.push_back( str.substr( initpos, std::min( pos, (int) str.size() ) - initpos ) );

        CleanUserSpaces( fields );
        return fields;
    }

    void CleanUserSpaces( std::vector<std::string>& fields )
    {
        for ( size_t i = 0; i < fields.size(); i++ )
            if ( fields[i] == "" )
                fields.erase( fields.begin() + i-- );
    }

    TVec3 StringToVec3( const std::string& strvec, const TVec3& def_vec, char delimiter )
    {
        auto fields = Split( strvec, delimiter );
        if ( fields.size() != 3 )
        {
            LOCO_CORE_WARN( "StringToVec3 >>> expected 3 floats in parsed string, but got {0}", fields.size() );
            return def_vec;
        }
        return TVec3( std::stof( fields[0] ), std::stof( fields[1] ), std::stof( fields[2] ) );
    }

    TVec4 StringToVec4( const std::string& strvec, const TVec4& def_vec, char delimiter )
    {
        auto fields = Split( strvec, delimiter );
        if ( fields.size() != 4 )
        {
            LOCO_CORE_WARN( "StringToVec4 >>> expected 4 floats in parsed string, but got {0}", fields.size() );
            return def_vec;
        }
        return TVec4( std::stof( fields[0] ), std::stof( fields[1] ), std::stof( fields[2] ), std::stof( fields[3] ) );
    }

    TSizei StringToArrayInt( const std::string& strarr, char delimiter )
    {
        auto fields = Split( strarr, delimiter );
        auto arrint = TSizei();
        arrint.ndim = fields.size();
        for ( size_t i = 0; i < fields.size(); i++ )
            arrint[i] = std::stoi( fields[i] );
        return arrint;
    }

    TSizef StringToArrayFloat( const std::string& strarr, char delimiter )
    {
        auto fields = Split( strarr, delimiter );
        auto arrfloat = TSizef();
        arrfloat.ndim = fields.size();
        for ( size_t i = 0; i < fields.size(); i++ )
            arrfloat[i] = std::stof( fields[i] );
        return arrfloat;
    }

namespace xml
{
    std::string GetAttributeString( const tinyxml2::XMLElement* element,
                                    const std::string& attribName,
                                    const std::string& def_str )
    {
        if ( auto attrib = element->Attribute( attribName.c_str() ) )
            return std::string( attrib );
        return def_str;
    }

    TVec3 GetAttributeVec3( const tinyxml2::XMLElement* element,
                            const std::string& attribName,
                            const TVec3& def_vec3 )
    {
        if ( auto attrib = element->Attribute( attribName.c_str() ) )
            return parsing::StringToVec3( attrib, def_vec3 );
        return def_vec3;
    }

    TVec4 GetAttributeVec4( const tinyxml2::XMLElement* element,
                            const std::string& attribName,
                            const TVec4& def_vec4 )
    {
        if ( auto attrib = element->Attribute( attribName.c_str() ) )
            return parsing::StringToVec4( attrib, def_vec4 );
        return def_vec4;
    }

    TSizei GetAttributeArrayInt( const tinyxml2::XMLElement* element,
                                 const std::string& attribName,
                                 const TSizei& def_arrint )
    {
        if ( auto attrib = element->Attribute( attribName.c_str() ) )
            return parsing::StringToArrayInt( attrib );
        return def_arrint;
    }

    TSizef GetAttributeArrayFloat( const tinyxml2::XMLElement* element,
                                   const std::string& attribName,
                                   const TSizef& def_arrfloat )
    {
        if ( auto attrib = element->Attribute( attribName.c_str() ) )
            return parsing::StringToArrayFloat( attrib );
        return def_arrfloat;
    }

    int32_t GetAttributeInt( const tinyxml2::XMLElement* element,
                             const std::string& attribName,
                             int32_t def_int )
    {
        if ( auto attrib = element->Attribute( attribName.c_str() ) )
            return std::stoi( attrib );
        return def_int;
    }

    TScalar GetAttributeFloat( const tinyxml2::XMLElement* element,
                               const std::string& attribName,
                               TScalar def_float )
    {
        if ( auto attrib = element->Attribute( attribName.c_str() ) )
            return std::stof( attrib );
        return def_float;
    }

    std::string SerializeToString( const TSizei& array_int )
    {
        std::string _strrep;
        for ( size_t i = 0; i < array_int.ndim; i++ )
            _strrep += std::to_string( array_int[i] ) + ( ( i != ( array_int.ndim - 1 ) ) ? " " : "" );
        return _strrep;
    }

    std::string SerializeToString( const TSizef& array_float )
    {
        std::string _strrep;
        for ( size_t i = 0; i < array_float.ndim; i++ )
            _strrep += std::to_string( array_float[i] ) + ( ( i != ( array_float.ndim - 1 ) ) ? " " : "" );
        return _strrep;
    }
}

namespace json
{
    std::string GetAttributeString( const nlohmann::json& element,
                                    const std::string& attribName,
                                    const std::string& def_str )
    {
        if ( element.find( attribName ) == element.end() )
            return def_str;
        if ( !element[attribName].is_string() )
            return def_str;

        return element[attribName];
    }

    TVec3 GetAttributeVec3( const nlohmann::json& element,
                            const std::string& attribName,
                            const TVec3& def_vec3 )
    {
        if ( element.find( attribName ) == element.end() )
            return def_vec3;
        if ( !element[attribName].is_array() )
            return def_vec3;
        if ( element[attribName].size() != 3 )
            return def_vec3;

        return TVec3( element[attribName][0],
                      element[attribName][1],
                      element[attribName][2] );
    }

    TVec4 GetAttributeVec4( const nlohmann::json& element,
                            const std::string& attribName,
                            const TVec4& def_vec4 )
    {
        if ( element.find( attribName ) == element.end() )
            return def_vec4;
        if ( !element[attribName].is_array() )
            return def_vec4;
        if ( element[attribName].size() != 4 )
            return def_vec4;

        return TVec4( element[attribName][0],
                      element[attribName][1],
                      element[attribName][2],
                      element[attribName][3] );
    }

    TSizei GetAttributeArrayInt( const nlohmann::json& element,
                                 const std::string& attribName,
                                 const TSizei& def_arrint )
    {
        if ( element.find( attribName ) == element.end() )
            return def_arrint;
        if ( !element[attribName].is_array() )
            return def_arrint;
        if ( element[attribName].size() > loco::MAX_NDIM )
            return def_arrint;

        TSizei arrint;
        arrint.ndim = element[attribName].size();
        for ( size_t i = 0; i < arrint.ndim; i++ )
            arrint[i] = (int32_t)element[attribName][i];
        return arrint;
    }

    TSizef GetAttributeArrayFloat( const nlohmann::json& element,
                                   const std::string& attribName,
                                   const TSizef& def_arrfloat )
    {
        if ( element.find( attribName ) == element.end() )
            return def_arrfloat;
        if ( !element[attribName].is_array() )
            return def_arrfloat;
        if ( element[attribName].size() > loco::MAX_NDIM )
            return def_arrfloat;

        TSizef arrfloat;
        arrfloat.ndim = element[attribName].size();
        for ( size_t i = 0; i < arrfloat.ndim; i++ )
            arrfloat[i] = (TScalar)element[attribName][i];
        return arrfloat;
    }

    int32_t GetAttributeInt( const nlohmann::json& element,
                             const std::string& attribName,
                             int32_t def_int )
    {
        if ( element.find( attribName ) == element.end() )
            return def_int;
        if ( !element[attribName].is_number_integer() )
            return def_int;

        return (int32_t) element[attribName];
    }

    TScalar GetAttributeFloat( const nlohmann::json& element,
                               const std::string& attribName,
                               TScalar def_float )
    {
        if ( element.find( attribName ) == element.end() )
            return def_float;
        if ( !element[attribName].is_number_float() )
            return def_float;

        return (TScalar) element[attribName];
    }
}

}}
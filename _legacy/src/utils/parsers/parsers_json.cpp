
#include <utils/parsers/parsers_json.h>

namespace tysoc {
namespace json {


    std::string safeParseString( const nlohmann::json& jsonElement,
                                 const std::string& key,
                                 const std::string& opt )
    {
        if ( jsonElement.find( key ) == jsonElement.end() )
            return opt;

        if ( !jsonElement[key].is_string() )
            return opt;

        return jsonElement[key];
    }

    TVec3 safeParseVec3( const nlohmann::json& jsonElement, 
                         const std::string& key,
                         const TVec3& opt )
    {
        if ( jsonElement.find( key ) == jsonElement.end() )
            return opt;

        if ( !jsonElement[key].is_array() )
            return opt;

        if ( jsonElement[key].size() != 3 )
            return opt;

        return TVec3( jsonElement[key][0],
                      jsonElement[key][1],
                      jsonElement[key][2] );
    }

    TVec4 safeParseVec4( const nlohmann::json& jsonElement, 
                         const std::string& key,
                         const TVec4& opt )
    {
        if ( jsonElement.find( key ) == jsonElement.end() )
            return opt;

        if ( !jsonElement[key].is_array() )
            return opt;

        if ( jsonElement[key].size() != 4 )
            return opt;

        return TVec4( jsonElement[key][0],
                      jsonElement[key][1],
                      jsonElement[key][2],
                      jsonElement[key][3] );
    }

    int safeParseInt( const nlohmann::json& jsonElement,
                      const std::string& key,
                      int opt )
    {
        if ( jsonElement.find( key ) == jsonElement.end() )
            return opt;

        if ( !jsonElement[key].is_number() )
            return opt;

        return (int) jsonElement[key];
    }

    float safeParseFloat( const nlohmann::json& jsonElement,
                          const std::string& key,
                          float opt )
    {
        if ( jsonElement.find( key ) == jsonElement.end() )
            return opt;

        if ( !jsonElement[key].is_number() )
            return opt;

        return (float) jsonElement[key];
    }



}}
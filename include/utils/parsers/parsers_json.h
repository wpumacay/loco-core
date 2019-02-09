
#pragma once

#include "parsers_common.h"

#include <json.hpp>

namespace tysoc {
namespace json {

    /**
    * @brief        Grabs a string attribute, returning an optional in case it doest not exist
    * @param key    Name key of the string attribute to be extracted
    * @param opt    Default string value in case the attribute is not found
    */
    std::string safeParseString( const nlohmann::json& jsonElement,
                                 const std::string& key,
                                 const std::string& opt = "" );

    /**
    * @brief        Grabs a Vec3 attribute, returning an optional in case it doest not exist
    * @param key    Name key of the vec3 attribute to be extracted
    * @param opt    Default Vec3 value in case the attribute is not found
    */
    TVec3 safeParseVec3( const nlohmann::json& jsonElement, 
                         const std::string& key,
                         const TVec3& opt = { 0.0f, 0.0f, 0.0f } );

    /**
    * @brief        Grabs a Vec4 attribute, returning an optional in case it doest not exist
    * @param key    Name key of the vec4 attribute to be extracted
    * @param opt    Default Vec4 value in case the attribute is not found
    */
    TVec4 safeParseVec4( const nlohmann::json& jsonElement, 
                         const std::string& key,
                         const TVec4& opt = { 0.0f, 0.0f, 0.0f, 1.0f } );

    /**
    * @brief        Grabs an int attribute, returning an optional in case it doest not exist
    * @param key    Name key of the int attribute to be extracted
    * @param opt    Default int value in case the attribute is not found
    */
    int safeParseInt( const nlohmann::json& jsonElement,
                      const std::string& key,
                      int opt = 0 );

    /**
    * @brief        Grabs a float attribute, returning an optional in case it doest not exist
    * @param key    Name key of the float attribute to be extracted
    * @param opt    Default float value in case the attribute is not found
    */
    float safeParseFloat( const nlohmann::json& jsonElement,
                          const std::string& key,
                          float opt = 0.0 );

}}
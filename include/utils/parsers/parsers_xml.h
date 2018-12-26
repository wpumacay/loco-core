
#pragma once

#include "parsers_common.h"

#include <tinyxml2.h>

namespace tysoc {
namespace xml {

    /**
    * @brief            Grabs a string attribute, returning an optional in case it doest not exist
    * @param attribName Name of the string attribute to be extracted
    * @param opt        Default string value in case the attribute is not found
    */
    std::string safeParseString( tinyxml2::XMLElement* xmlElement,
                                 const std::string& attribName,
                                 const std::string& opt = "" );

    /**
    * @brief            Grabs a Vec3 attribute, returning an optional in case it doest not exist
    * @param attribName Name of the vec3 attribute to be extracted
    * @param opt        Default Vec3 value in case the attribute is not found
    */
    TVec3 safeParseVec3( tinyxml2::XMLElement* xmlElement, 
                         const std::string& attribName,
                         const TVec3& opt = { 0.0f, 0.0f, 0.0f } );

    /**
    * @brief            Grabs a Vec4 attribute, returning an optional in case it doest not exist
    * @param attribName Name of the vec4 attribute to be extracted
    * @param opt        Default Vec4 value in case the attribute is not found
    */
    TVec4 safeParseVec4( tinyxml2::XMLElement* xmlElement, 
                         const std::string& attribName, 
                         const TVec4& opt = { 0.0f, 0.0f, 0.0f, 1.0f } );

    /**
    * @brief            Grabs a Sizei(arrayInt) attribute, returning an optional in case it doest not exist
    * @param attribName Name of the Sizei attribute to be extracted
    * @param opt        Default Sizei value in case the attribute is not found
    */
    TSizei safeParseSizei( tinyxml2::XMLElement* xmlElement,
                           const std::string& attribName,
                           const TSizei& opt = { 0, { 0 } } );

    /**
    * @brief            Grabs a Sizef(arrayFloat) attribute, returning an optional in case it doest not exist
    * @param attribName Name of the Sizef attribute to be extracted
    * @param opt        Default Sizef value in case the attribute is not found
    */
    TSizef safeParseSizef( tinyxml2::XMLElement* xmlElement,
                           const std::string& attribName,
                           const TSizef& opt = { 0, { 0.0 } } );

    /**
    * @brief            Grabs an int attribute, returning an optional in case it doest not exist
    * @param attribName Name of the int attribute to be extracted
    * @param opt        Default int value in case the attribute is not found
    */
    int safeParseInt( tinyxml2::XMLElement* xmlElement,
                      const std::string& attribName,
                      int opt = 0 );

    /**
    * @brief            Grabs a float attribute, returning an optional in case it doest not exist
    * @param attribName Name of the float attribute to be extracted
    * @param opt        Default float value in case the attribute is not found
    */
    float safeParseFloat( tinyxml2::XMLElement* xmlElement,
                          const std::string& attribName,
                          float opt = 0.0 );

}}
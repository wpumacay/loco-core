
#pragma once

#include "mjcf_common.h"

namespace tysoc {
namespace mjcf {

    /**
    * @brief        Split a string into a vector of substrings (delimiter = " " SPACE)
    * @param str    String to split into pieces using SPACE as delimiter
    */
    std::vector< std::string > split( const std::string& str );

    /**
    * @brief        Removes empty fields from a vector of strings
    * @param fields Vector of strings to be cleaned up
    */
    void cleanFromUserSpaces( std::vector< std::string >& fields );

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
    Vec3 safeParseVec3( tinyxml2::XMLElement* xmlElement, 
                        const std::string& attribName,
                        const Vec3& opt = { 0.0f, 0.0f, 0.0f } );

    /**
    * @brief            Grabs a Vec4 attribute, returning an optional in case it doest not exist
    * @param attribName Name of the vec4 attribute to be extracted
    * @param opt        Default Vec4 value in case the attribute is not found
    */
    Vec4 safeParseVec4( tinyxml2::XMLElement* xmlElement, 
                        const std::string& attribName, 
                        const Vec4& opt = { 0.0f, 0.0f, 0.0f, 1.0f } );

    /**
    * @brief            Grabs a Sizei(arrayInt) attribute, returning an optional in case it doest not exist
    * @param attribName Name of the Sizei attribute to be extracted
    * @param opt        Default Sizei value in case the attribute is not found
    */
    Sizei safeParseSizei( tinyxml2::XMLElement* xmlElement,
                          const std::string& attribName,
                          const Sizei& opt = { 0, { 0 } } );

    /**
    * @brief            Grabs a Sizef(arrayFloat) attribute, returning an optional in case it doest not exist
    * @param attribName Name of the Sizef attribute to be extracted
    * @param opt        Default Sizef value in case the attribute is not found
    */
    Sizef safeParseSizef( tinyxml2::XMLElement* xmlElement,
                          const std::string& attribName,
                          const Sizef& opt = { 0, { 0.0 } } );

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

    /**
    * @brief            Parses a Vec3 from a string
    * @param strvec     String representation of the Vec3
    */
    Vec3 _parseVec3( const std::string& strvec );

    /**
    * @brief            Parses a Vec4 from a string
    * @param strvec     String representation of the Vec4
    */
    Vec4 _parseVec4( const std::string& strvec );

    /**
    * @brief            Parses a Sizei(arrayInt) from a string
    * @param strsize    String representation of the Sizei(arrayInt)
    */
    Sizei _parseSizei( const std::string& strsize );

    /**
    * @brief            Parses a Sizef(arrayFloat) from a string
    * @param strsize    String representation of the Sizef(arrayFloat)
    */
    Sizef _parseSizef( const std::string& strsize );

}}
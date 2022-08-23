#pragma once

#include <loco_common.h>
#include <tinyxml2.h>
#include <json.hpp>

namespace loco {
namespace parsing {

    /// Split a string into a vector of substrings given a delimiter
    /// @param[in] str          String to split into pieces using SPACE as delimiter
    /// @param[in] delimiter    Character delimiter to be used for the splits
    /// @returns A vector of strings splitted from the given base string
    std::vector<std::string> Split( const std::string& str, char delimiter );

    /// Removes empty fields from a vector of strings
    /// @param[in,out] fields
    ///     Vector of strings to be cleaned up
    void CleanUserSpaces( std::vector<std::string>& fields );

    /// Creates a vector-3 from a given string of floats separated by a delimiter
    /// @param[in] strvec       String representing a vector-3 to be parsed
    /// @param[in] def_vec      Default vector in case string conversion is not possible
    /// @param[in] delimiter    Character delimiter that separates the float entries in the string vector
    /// @returns The vector-3 parsed from the given string and using the given delimiter
    TVec3 StringToVec3( const std::string& strvec,
                        const TVec3& def_vec = TVec3( 0.0f, 0.0f, 0.0f ),
                        char delimiter = ' ' );

    /// Creates a vector-4 from a given string of floats separated by a delimiter
    /// @param[in] strvec       String representing a vector-4 to be parsed
    /// @param[in] def_vec      Default vector in case string conversion is not possible
    /// @param[in] delimiter    Character delimiter that separates the float entries in the string vector
    /// @returns The vector-4 parsed from the given string and using the given delimiter
    TVec4 StringToVec4( const std::string& strvec,
                        const TVec4& def_vec = TVec4( 0.0f, 0.0f, 0.0f, 1.0f ),
                        char delimiter = ' ' );

    /// Creates an array-int from a given string of ints separated by a delimiter
    /// @param[in] strarr       String representing an array of integers
    /// @param[in] delimiter    Character delimiter that separates the int entries in the string array
    /// @returns The array-int parsed from the given string and using the given delimiter
    TSizei StringToArrayInt( const std::string& strarr,
                             char delimiter = ' ' );

    /// Creates an array-int from a given string of floats separated by a delimiter
    /// @param[in] strarr       String representing an array of floats
    /// @param[in] delimiter    Character delimiter that separates the float entries in the string array
    /// @returns The array-float parsed from the given string and using the given delimiter
    TSizef StringToArrayFloat( const std::string& strarr,
                               char delimiter = ' ' );

namespace xml
{
    /// Gets a string attribute from an xml-element given the attribute name
    /// @param[in] element      Xml-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the string attribute to be extracted
    /// @param[in] def_str      Default string value in case the attribute is not found
    /// @returns A string copy of the requested attribute
    std::string GetAttributeString( const tinyxml2::XMLElement* element,
                                    const std::string& attribName,
                                    const std::string& def_str = "" );

    /// Gets a vector-3 attribute from an xml-element given the attribute name
    /// @param[in] element      Xml-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the vector-3 attribute to be extracted
    /// @param[in] def_vec3     Default vector-3 in case the attribute is not found
    /// @returns A vector-3 copy of the requested attribute
    TVec3 GetAttributeVec3( const tinyxml2::XMLElement* element,
                            const std::string& attribName,
                            const TVec3& def_vec3 = TVec3( 0.0f, 0.0f, 0.0f ) );

    /// Gets a vector-4 attribute from an xml-element given the attribute name
    /// @param[in] element      Xml-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the vector-4 attribute to be extracted
    /// @param[in] def_vec4     Default vector-4 in case the attribute is not found
    /// @returns A vector-4 copy of the requested attribute
    TVec4 GetAttributeVec4( const tinyxml2::XMLElement* element,
                            const std::string& attribName,
                            const TVec4& def_vec4 = TVec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

    /// Gets an array-int attribute from an xml-element given the attribute name
    /// @param[in] element      Xml-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the array-int attribute to be extracted
    /// @param[in] def_arrint   Default array-int in case the attribute is not found
    /// @returns A array-int copy of the requested attribute
    TSizei GetAttributeArrayInt( const tinyxml2::XMLElement* element,
                                 const std::string& attribName,
                                 const TSizei& def_arrint = TSizei() );

    /// Gets an array-float attribute from an xml-element given the attribute name
    /// @param[in] element      Xml-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the array-float attribute to be extracted
    /// @param[in] def_arrfloat Default array-float in case the attribute is not found
    /// @returns A array-float copy of the requested attribute
    TSizef GetAttributeArrayFloat( const tinyxml2::XMLElement* element,
                                   const std::string& attribName,
                                   const TSizef& def_arrfloat = TSizef() );

    /// Gets an int attribute from an xml-element given the attribute name
    /// @param[in] element      Xml-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the int attribute to be extracted
    /// @param[in] def_int      Default int value in case the attribute is not found
    /// @returns An int copy of the requested attribute
    int32_t GetAttributeInt( const tinyxml2::XMLElement* element,
                             const std::string& attribName,
                             int32_t def_int = 0 );

    /// Gets a float attribute from an xml-element given the attribute name
    /// @param[in] element      Xml-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the float attribute to be extracted
    /// @param[in] def_float    Default float value in case the attribute is not found
    /// @returns A float copy of the requested attribute
    TScalar GetAttributeFloat( const tinyxml2::XMLElement* element,
                               const std::string& attribName,
                               TScalar def_float = 0.0f );

    /// Returns the string representation of an array of ints for serialization into an xml-attribute
    /// @param[in] array_int    Array of integers to be serialized into a string
    /// @returns A string corresponding of space-separated string representations of the elements of the array
    std::string SerializeToString( const TSizei& array_int );

    /// Returns the string representation of an array of floats for serialization into an xml-attribute
    /// @param[in] array_float  Array of floats to be serialized into a string
    /// @returns A string corresponding of space-separated string representations of the elements of the array
    std::string SerializeToString( const TSizef& array_float );
}

namespace json
{
    /// Gets a string attribute from a json-element given the attribute name
    /// @param[in] element      Json-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the string attribute to be extracted
    /// @param[in] def_str      Default string value in case the attribute is not found
    /// @returns A string copy of the requested attribute
    std::string GetAttributeString( const nlohmann::json& element,
                                    const std::string& attribName,
                                    const std::string& def_str = "" );

    /// Gets a vector-3 attribute from a json-element given the attribute name
    /// @param[in] element      Json-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the vector-3 attribute to be extracted
    /// @param[in] def_vec3     Default vector-3 in case the attribute is not found
    /// @returns A vector-3 copy of the requested attribute
    TVec3 GetAttributeVec3( const nlohmann::json& element,
                            const std::string& attribName,
                            const TVec3& def_vec3 = TVec3( 0.0f, 0.0f, 0.0f ) );

    /// Gets a vector-4 attribute from a json-element given the attribute name
    /// @param[in] element      Json-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the vector-4 attribute to be extracted
    /// @param[in] def_vec4     Default vector-4 in case the attribute is not found
    /// @returns A vector-4 copy of the requested attribute
    TVec4 GetAttributeVec4( const nlohmann::json& element,
                            const std::string& attribName,
                            const TVec4& def_vec4 = TVec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

    /// Gets an array-int attribute from a json-element given the attribute name
    /// @param[in] element      Json-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the array-int attribute to be extracted
    /// @param[in] def_arrint   Default array-int in case the attribute is not found
    /// @returns A array-int copy of the requested attribute
    TSizei GetAttributeArrayInt( const nlohmann::json& element,
                                 const std::string& attribName,
                                 const TSizei& def_arrint = TSizei() );

    /// Gets an array-float attribute from a json-element given the attribute name
    /// @param[in] element      Json-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the array-float attribute to be extracted
    /// @param[in] def_arrfloat Default array-float in case the attribute is not found
    /// @returns A array-float copy of the requested attribute
    TSizef GetAttributeArrayFloat( const nlohmann::json& element,
                                   const std::string& attribName,
                                   const TSizef& def_arrfloat = TSizef() );

    /// Gets an int attribute from a json-element given the attribute name
    /// @param[in] element      Json-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the int attribute to be extracted
    /// @param[in] def_int      Default int value in case the attribute is not found
    /// @returns An int copy of the requested attribute
    int32_t GetAttributeInt( const nlohmann::json& element,
                             const std::string& attribName,
                             int32_t def_int = 0 );

    /// Gets a float attribute from a json-element given the attribute name
    /// @param[in] element      Json-element from whom we'll try to grab an attribute
    /// @param[in] attribName   Name of the float attribute to be extracted
    /// @param[in] def_float    Default float value in case the attribute is not found
    /// @returns A float copy of the requested attribute
    TScalar GetAttributeFloat( const nlohmann::json& element,
                               const std::string& attribName,
                               TScalar def_float = 0.0f );
}

}}

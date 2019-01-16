
#pragma once

#include <tysoc_common.h>

namespace tysoc {
namespace parsing {

    /**
    * @brief            Split a string into a vector of substrings (default delimiter = " " SPACE)
    * @param str        String to split into pieces using SPACE as delimiter
    * @param delimiter  Character delimiter to be used for the splits
    */
    std::vector< std::string > split( const std::string& str,
                                      char delimiter = ' ' );

    /**
    * @brief        Removes empty fields from a vector of strings
    * @param fields Vector of strings to be cleaned up
    */
    void cleanFromUserSpaces( std::vector< std::string >& fields );

    /**
    * @brief            Parses a Vec3 from a string
    * @param strvec     String representation of the Vec3
    */
    TVec3 _parseVec3( const std::string& strvec, const TVec3& opt = { 0.0f, 0.0f, 0.0f } );

    /**
    * @brief            Parses a Vec4 from a string
    * @param strvec     String representation of the Vec4
    */
    TVec4 _parseVec4( const std::string& strvec, const TVec4& opt = { 0.0f, 0.0f, 0.0f, 1.0f } );

    /**
    * @brief            Parses a Sizei(arrayInt) from a string
    * @param strsize    String representation of the Sizei(arrayInt)
    */
    TSizei _parseSizei( const std::string& strsize );

    /**
    * @brief            Parses a Sizef(arrayFloat) from a string
    * @param strsize    String representation of the Sizef(arrayFloat)
    */
    TSizef _parseSizef( const std::string& strsize );

    /**
    * @brief            Sends an error message to the standard output
    * @param errorMsg   String representing the error message
    */
    void logError( const std::string& errorMsg );
    
    /**
    * @brief            Sends an warning message to the standard output
    * @param warningMsg String representing the warning message
    */
    void logWarning( const std::string& warningMsg );

}}
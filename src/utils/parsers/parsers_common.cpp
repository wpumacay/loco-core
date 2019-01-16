
#include <utils/parsers/parsers_common.h>

namespace tysoc {
namespace parsing {

    std::vector< std::string > split( const std::string& str,
                                      char delimiter )
    {
        std::vector< std::string > _res;
                    
        int pos = str.find( delimiter );
        if ( pos == std::string::npos )
        {
            _res.push_back( str );
            return _res;
        }

        int initpos = 0;

        while ( pos != std::string::npos )
        {
            _res.push_back( str.substr( initpos, pos - initpos ) );
            initpos = pos + 1;

            pos = str.find( delimiter, initpos );
        }

        _res.push_back( str.substr( initpos, std::min( pos, (int) str.size() ) - initpos ) );

        cleanFromUserSpaces( _res );

        return _res;
    }

    void cleanFromUserSpaces( std::vector< std::string >& fields )
    {
        // remove unused spaces due to user using ...
        // 2 or more spaces in string
        for ( size_t i = 0; i < fields.size(); i++ )
        {
            if ( fields[i] == "" )
            {
                fields.erase( fields.begin() + i );
                i--;
            }
        }
    }

    TVec3 _parseVec3( const std::string& strvec, const TVec3& opt )
    {
        TVec3 _res;
        
        auto _fields = split( strvec );
        if ( _fields.size() != 3 )
        {
            std::cout << "WARNING> not enough fields for vec3: " << strvec << std::endl;
            return opt;
        }

        _res.x = std::stof( _fields[0] );
        _res.y = std::stof( _fields[1] );
        _res.z = std::stof( _fields[2] );

        return _res;
    }

    TVec4 _parseVec4( const std::string& strvec, const TVec4& opt )
    {
        TVec4 _res;
        
        auto _fields = split( strvec );
        if ( _fields.size() != 4 )
        {
            std::cout << "WARNING> not enough fields for vec4: " << strvec << std::endl;
            return opt;
        }

        _res.x = std::stof( _fields[0] );
        _res.y = std::stof( _fields[1] );
        _res.z = std::stof( _fields[2] );
        _res.w = std::stof( _fields[3] );

        return _res;
    }

    TSizei _parseSizei( const std::string& strsize )
    {
        TSizei _res;

        auto _fields = split( strsize );
        
        // @TOQO: Add asserts
        _res.ndim = _fields.size();
        for ( size_t i = 0; i < _fields.size(); i++ )
        {
            _res.buff[i] = std::stoi( _fields[i] );
        }

        return _res;
    }

    TSizef _parseSizef( const std::string& strsize )
    {
        TSizef _res;

        auto _fields = split( strsize );

        // @TOQO: Add asserts
        _res.ndim = _fields.size();
        for ( size_t i = 0; i < _fields.size(); i++ )
        {
            _res.buff[i] = std::stof( _fields[i] );
        }

        return _res;
    }

    void logError( const std::string& errorMsg )
    {
        std::cout << "ERROR> " << errorMsg << std::endl;
    }

    void logWarning( const std::string& warningMsg )
    {
        std::cout << "WARNING> " << warningMsg << std::endl;
    }

}}
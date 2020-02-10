
#include <loco_common.h>

namespace loco
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                      Helpers impl.                                     ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector< std::string > Split( const std::string &txt, char separator )
    {
        std::vector< std::string > _res;

        int pos = txt.find( separator );
        if ( pos == std::string::npos )
        {
            _res.push_back( txt );
            return _res;
        }

        int initpos = 0;

        while ( pos != std::string::npos )
        {
            _res.push_back( txt.substr( initpos, pos - initpos ) );
            initpos = pos + 1;

            pos = txt.find( separator, initpos );
        }

        _res.push_back( txt.substr( initpos, std::min( pos, (int) txt.size() ) - initpos ) );

        return _res;
    }

    std::string PointerToHexAddress( const void* ptr )
    {
        auto ptr_casted = reinterpret_cast<std::intptr_t>( ptr );
        std::stringstream _ss;
        _ss << "0x" << std::hex << ptr_casted;
        return _ss.str();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                  Path helpers impl.                                    ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    std::string GetFilenameFromFilePath(  const std::string& filepath )
    {
        return Split( filepath, '/' ).back();
    }

    std::string GetFoldernameFromFilePath(  const std::string& filepath )
    {
        auto _pathParts = Split( filepath, '/' );
        if ( _pathParts.size() < 2 )
            return "./";

        return _pathParts[_pathParts.size() - 2];
    }

    std::string GetFolderpathFromFilePath( const std::string& filepath )
    {
        auto _pathParts = Split( filepath, '/' );
        if ( _pathParts.size() < 2 )
            return "./";

        std::stringstream _ss;
        for ( size_t i = 0; i < _pathParts.size() - 1; i++ )
            _ss << _pathParts[i] << "/";

        return _ss.str();
    }

    std::string GetFilenameNoExtensionFromFilePath(  const std::string& filepath )
    {
        return Split( Split( filepath, '/' ).back(), '.' ).front();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                 TGenericParams impl.                                   ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    void TGenericParams::Set( const std::string& name, int val )
    {
        m_ints[name] = val;
        m_keys.insert( name );
    }

    void TGenericParams::Set( const std::string& name, float val )
    {
        m_floats[name] = val;
        m_keys.insert( name );
    }

    void TGenericParams::Set( const std::string& name, const TVec2& vec )
    {
        m_sizefs[name].ndim = 2;
        m_sizefs[name][0] = vec.x();
        m_sizefs[name][1] = vec.y();
        m_keys.insert( name );
    }

    void TGenericParams::Set( const std::string& name, const TVec3& vec )
    {
        m_sizefs[name].ndim = 3;
        m_sizefs[name][0] = vec.x();
        m_sizefs[name][1] = vec.y();
        m_sizefs[name][2] = vec.z();
        m_keys.insert( name );
    }

    void TGenericParams::Set( const std::string& name, const TVec4& vec )
    {
        m_sizefs[name].ndim = 4;
        m_sizefs[name][0] = vec.x();
        m_sizefs[name][1] = vec.y();
        m_sizefs[name][2] = vec.z();
        m_sizefs[name][3] = vec.w();
        m_keys.insert( name );
    }

    void TGenericParams::Set( const std::string& name, const TSizei& sizei )
    {
        m_sizeis[name] = sizei;
        m_keys.insert( name );
    }

    void TGenericParams::Set( const std::string& name, const TSizef& sizef )
    {
        m_sizefs[name] = sizef;
        m_keys.insert( name );
    }

    void TGenericParams::Set( const std::string& name, const std::string& str )
    {
        m_strings[name] = str;
        m_keys.insert( name );
    }

    bool TGenericParams::HasParam( const std::string& name ) const
    {
        return m_keys.find( name ) != m_keys.end();
    }

    int TGenericParams::GetInt( const std::string& name, int def ) const
    {
        if ( m_ints.find( name ) != m_ints.end() )
            return m_ints.at( name );

        return def;
    }

    float TGenericParams::GetFloat( const std::string& name, float def ) const
    {
        if ( m_floats.find( name ) != m_floats.end() )
            return m_floats.at( name );

        return def;
    }

    TVec2 TGenericParams::GetVec2( const std::string& name, const TVec2& def ) const
    {
        TVec2 _res = def;

        if ( m_sizefs.find( name ) != m_sizefs.end() )
        {
            TSizef _vec2 = m_sizefs.at( name );

            _res.x() = _vec2[0];
            _res.y() = _vec2[1];
        }

        return _res;
    }

    TVec3 TGenericParams::GetVec3( const std::string& name, const TVec3& def ) const
    {
        TVec3 _res = def;

        if ( m_sizefs.find( name ) != m_sizefs.end() )
        {
            TSizef _vec3 = m_sizefs.at( name );

            _res.x() = _vec3[0];
            _res.y() = _vec3[1];
            _res.z() = _vec3[2];
        }

        return _res;
    }

    TVec4 TGenericParams::GetVec4( const std::string& name, const TVec4& def ) const
    {
        TVec4 _res = def;

        if ( m_sizefs.find( name ) != m_sizefs.end() )
        {
            TSizef _vec4 = m_sizefs.at( name );

            _res.x() = _vec4[0];
            _res.y() = _vec4[1];
            _res.z() = _vec4[2];
            _res.w() = _vec4[3];
        }

        return _res;
    }

    TSizei TGenericParams::GetSizei( const std::string& name, const TSizei& def ) const
    {
        if ( m_sizeis.find( name ) != m_sizeis.end() )
            return m_sizeis.at( name );

        return def;
    }

    TSizef TGenericParams::GetSizef( const std::string& name, const TSizef& def ) const
    {
        if ( m_sizefs.find( name ) != m_sizefs.end() )
            return m_sizefs.at( name );

        return def;
    }

    std::string TGenericParams::GetString( const std::string& name, const std::string& def ) const
    {
        if ( m_strings.find( name ) != m_strings.end() )
            return m_strings.at( name );

        return def;
    }

    std::unordered_map< std::string, float > TGenericParams::GetFloats() const
    {
        return m_floats;
    }

    std::unordered_map< std::string, TVec3 > TGenericParams::GetVec3s() const
    {
        std::unordered_map< std::string, TVec3 > _vec3s;

        for ( auto& kv : m_sizefs )
        {
            if ( kv.second.ndim != 3 )
                continue;

            _vec3s[kv.first] = { kv.second[0], kv.second[1], kv.second[2] };
        }

        return _vec3s;
    }
}

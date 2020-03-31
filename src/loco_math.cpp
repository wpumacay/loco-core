
#include <loco_math.h>

namespace loco
{
    TScalar Rad2degrees( const TScalar& rads )
    {
        return rads * 180.0 / loco::PI;
    }

    TScalar Degrees2rad( const TScalar& degrees )
    {
        return degrees * loco::PI / 180.0;
    }

    std::string ToString( const TVec2& vec )
    {
        std::string _strrep;

        _strrep += "( ";  _strrep += std::to_string( vec.x() );
        _strrep += " , "; _strrep += std::to_string( vec.y() );
        _strrep += " )";

        return _strrep;
    }

    std::string ToString( const TVec3& vec )
    {
        std::string _strrep;

        _strrep += "( ";  _strrep += std::to_string( vec.x() );
        _strrep += " , "; _strrep += std::to_string( vec.y() );
        _strrep += " , "; _strrep += std::to_string( vec.z() );
        _strrep += " )";

        return _strrep;
    }

    std::string ToString( const TVec4& vec )
    {
        std::string _strrep;

        _strrep += "( ";  _strrep += std::to_string( vec.x() );
        _strrep += " , "; _strrep += std::to_string( vec.y() );
        _strrep += " , "; _strrep += std::to_string( vec.z() );
        _strrep += " , "; _strrep += std::to_string( vec.w() );
        _strrep += " )";

        return _strrep;
    }

    std::string ToString( const TMat2& mat )
    {
        std::string _strrep;

        _strrep += "[ ";
        for ( int i = 0; i < 2; i++ )
        {
            _strrep += "[ ";
            _strrep += std::to_string( mat( i, 0 ) ); _strrep += ", ";
            _strrep += std::to_string( mat( i, 1 ) );
            _strrep += ( i != 1 ) ? " ]\n\r  " : " ]";
        }
        _strrep += " ]";

        return _strrep;
    }

    std::string ToString( const TMat3& mat )
    {
        std::string _strrep;

        _strrep += "[ ";
        for ( int i = 0; i < 3; i++ )
        {
            _strrep += "[ ";
            _strrep += std::to_string( mat( i, 0 ) ); _strrep += ", ";
            _strrep += std::to_string( mat( i, 1 ) ); _strrep += ", ";
            _strrep += std::to_string( mat( i, 2 ) );
            _strrep += ( i != 2 ) ? " ]\n\r  " : " ]";
        }
        _strrep += " ]";

        return _strrep;
    }

    std::string ToString( const TMat4& mat )
    {
        std::string _strrep;

        _strrep += "[ ";
        for ( int i = 0; i < 4; i++ )
        {
            _strrep += "[ ";
            _strrep += std::to_string( mat( i, 0 ) ); _strrep += ", ";
            _strrep += std::to_string( mat( i, 1 ) ); _strrep += ", ";
            _strrep += std::to_string( mat( i, 2 ) ); _strrep += ", ";
            _strrep += std::to_string( mat( i, 3 ) );
            _strrep += ( i != 3 ) ? " ]\n\r  " : " ]";
        }
        _strrep += " ]";

        return _strrep;
    }

    // Extracted from bullet btQuaternion implementation (where they use the gameprogramminggems (v1?) 2.10 impl.)
    TVec4 ShortestArcQuat( const TVec3& v, const TVec3& vTarget )
    {
        TVec4 _quatres;

        auto _cross = tinymath::cross( v, vTarget );
        auto _dot = v.dot( vTarget );

        if ( _dot < -0.99999 ) // if vector are close to antiparallel
        {
            // just pick an arbitrary perpendicular (same trick as in bullet-btvector3)
            TVec3 _n; // perpendicular to pick
            if ( std::abs( v.z() ) > std::sqrt( 0.5 ) )
            {
                // choose perpendicular in y-z plane
                TScalar _a = v.y() * v.y() + v.z() * v.z();
                TScalar _k = 1.0 / _a;

                _n.x() = 0.0;
                _n.y() = -v.z() * _k;
                _n.z() = v.y() * _k;
            }
            else
            {
                // choose perpendicular in x-y plane
                TScalar _a = v.x() * v.x() + v.y() * v.y();
                TScalar _k = 1.0 / _a;

                _n.x() = -v.z() * _k;
                _n.y() = v.y() * _k;
                _n.z() = 0.0;
            }

            _quatres.x() = _n.x();
            _quatres.y() = _n.y();
            _quatres.z() = _n.z();
            _quatres.w() = 0.0;
        }
        else
        {
            // computation from gpgems
            TScalar _s = std::sqrt( 2.0 * ( 1 + _dot ) );
            _quatres.x() = _cross.x() / _s;
            _quatres.y() = _cross.y() / _s;
            _quatres.z() = _cross.z() / _s;
            _quatres.w() = _s * 0.5;
        }

        return _quatres;
    }

    TVec3 Rearrange( const TVec3& vec, const std::string& worldUp )
    {
        if ( worldUp == "y" )
            return TVec3( vec.z(), vec.x(), vec.y() );
        else if ( worldUp == "x" )
            return TVec3( vec.y(), vec.z(), vec.x() );
        return vec;
    }

    /***********************************************************************************************
    *                                     Perlin-noise generator                                   *
    ***********************************************************************************************/

    std::unique_ptr<TNoiseGenerator> TNoiseGenerator::s_Instance = nullptr;

    void TNoiseGenerator::Init()
    {
        if ( TNoiseGenerator::s_Instance )
            return;

        TNoiseGenerator::s_Instance = std::unique_ptr<TNoiseGenerator>( new TNoiseGenerator() );
    }

    void TNoiseGenerator::Release()
    {
        TNoiseGenerator::s_Instance = nullptr;
    }

    TNoiseGenerator::TNoiseGenerator()
    {
        m_Octaves = 4;
        m_Persistance = 0.5f;
        m_Lacunarity = 2.0f;
        m_NoiseScale = 10.0f;

        for ( int o = 0; o < m_Octaves; o++ )
            m_OctaveOffsets.push_back( { LOCO_RANDOM_UNIFORM( -10000.0f, 10000.0f ), 
                                         LOCO_RANDOM_UNIFORM( -10000.0f, 10000.0f ) } );

        // Initialize the permutation vector with ken perlin's reference permutation and double it to avoid overflow
        m_Permutations = { 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
                           8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
                           35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
                           134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
                           55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
                           18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
                           250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
                           189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 
                           43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
                           97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
                           107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                           138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

        m_Permutations.insert( m_Permutations.end(), m_Permutations.begin(), m_Permutations.end() );
    }

    void TNoiseGenerator::Config( int octaves, float persistance, float lacunarity, float noise_scale )
    {
        if ( !TNoiseGenerator::s_Instance )
            return;
        TNoiseGenerator::s_Instance->_Config( octaves, persistance, lacunarity, noise_scale );
    }

    float TNoiseGenerator::GetNoise1d( float x )
    {
        if ( !TNoiseGenerator::s_Instance )
            return 0.0f;
        return TNoiseGenerator::s_Instance->_GetNoise1d( x );
    }

    float TNoiseGenerator::GetNoise2d( float x, float y )
    {
        if ( !TNoiseGenerator::s_Instance )
            return 0.0f;
        return TNoiseGenerator::s_Instance->_GetNoise2d( x, y );
    }

    void TNoiseGenerator::_Config( int octaves, float persistance, float lacunarity, float noise_scale )
    {
        m_Octaves       = octaves;
        m_Persistance   = persistance;
        m_Lacunarity    = lacunarity;
        m_NoiseScale    = noise_scale;

        // recompute the octave offsets
        m_OctaveOffsets.clear();
        for ( int o = 0; o < m_Octaves; o++ )
            m_OctaveOffsets.push_back( { LOCO_RANDOM_UNIFORM( -10000.0f, 10000.0f ),
                                         LOCO_RANDOM_UNIFORM( -10000.0f, 10000.0f ) } );
    }

    float TNoiseGenerator::_GetNoise1d( float x )
    {
        // for now, just take a fixed y=0 (slice of a noise surface)
        return _GetNoise2d( x, 0.0f );
    }

    float TNoiseGenerator::_GetNoise2d( float x, float y )
    {
        float ampl = 1.0f;
        float freq = 1.0f;
        float noise_value = 0.0f;

        for ( int i = 0; i < m_Octaves; i++ )
        {
            float sample_x = freq * ( x / m_NoiseScale ) + m_OctaveOffsets[i].x();
            float sample_y = freq * ( y / m_NoiseScale ) + m_OctaveOffsets[i].y();

            noise_value += _Perlin2d( sample_x, sample_y ) * ampl;

            ampl *= m_Persistance;
            freq *= m_Lacunarity;
        }

        return noise_value;
    }

    float TNoiseGenerator::_Perlin2d( float x, float y )
    {
        // Calculate unit square position in grid ( wrap around by 255 )
        int _X = ( int ) floor( x ) & 255;
        int _Y = ( int ) floor( y ) & 255;

        // Calculate relative position [0-1] inside unit square
        float _xf = x - floor( x );
        float _yf = y - floor( y );

        // Smooth using ease function for smooth imterpolation
        float _u = _Fade( _xf );
        float _v = _Fade( _yf );

        int _hash00 = m_Permutations[ m_Permutations[ _X ] + _Y ];
        int _hash01 = m_Permutations[ m_Permutations[ _X ] + _Y + 1 ];
        int _hash10 = m_Permutations[ m_Permutations[ _X + 1 ] + _Y ];
        int _hash11 = m_Permutations[ m_Permutations[ _X + 1 ] + _Y + 1 ];

        float _d00 = _DotGrad( _hash00, _xf, _yf );
        float _d01 = _DotGrad( _hash01, _xf, _yf - 1 );
        float _d10 = _DotGrad( _hash10, _xf - 1, _yf );
        float _d11 = _DotGrad( _hash11, _xf - 1, _yf - 1 );

        return _Lerp( _Lerp( _d00, _d10, _u ), _Lerp( _d01, _d11, _u ), _v );
    }

    float TNoiseGenerator::_Lerp( float a, float b, float t )
    {
        return ( 1 - t ) * a + t * b;
    }

    float TNoiseGenerator::_Fade( float t )
    {
        return t * t * t * ( t * ( t * 6.0f - 15.0f ) + 10.0f );
    }

    float TNoiseGenerator::_DotGrad( int hash, float xf, float yf )
    {
        // Because we are in 2d, there are just 4 options for the gradient vectors ...
        // and so, just 4 options for the dot product

        switch ( hash & 0xf )
        {
            case 0x0 : return xf + yf;
            case 0x1 : return -xf + yf;
            case 0x2 : return xf - yf;
            case 0x3 : return -xf - yf;
            case 0x4 : return xf + yf;
            case 0x5 : return -xf + yf;
            case 0x6 : return xf - yf;
            case 0x7 : return -xf - yf;
            case 0x8 : return xf + yf;
            case 0x9 : return -xf + yf;
            case 0xa : return xf - yf;
            case 0xb : return -xf - yf;
            case 0xc : return xf + yf;
            case 0xd : return -xf + yf;
            case 0xe : return xf - yf;
            case 0xf : return -xf - yf;
        }
        return 0.0f;
    }
}
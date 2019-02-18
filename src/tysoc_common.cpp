
#include <tysoc_common.h>


namespace tysoc
{

    TVec2::TVec2()
    {
        x = 0;
        y = 0;
    }

    TVec2::TVec2( TScalar px, TScalar py )
    {
        x = px;
        y = py;
    }

    std::string TVec2::toString( const TVec2& v )
    {
        std::string _res;

        _res += std::string( "x: " ) + std::to_string( v.x ) + std::string( " - " );
        _res += std::string( "y: " ) + std::to_string( v.y );

        return _res;
    }

    TVec3::TVec3()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    TVec3::TVec3( TScalar x, TScalar y, TScalar z )
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    TVec3 TVec3::operator* ( TScalar factor )
    {
        return TVec3( x * factor, y * factor, z * factor );
    }

    TVec3 TVec3::operator+ ( const TVec3& other )
    {
        return TVec3( x + other.x, y + other.y, z + other.z );
    }

    TVec3 TVec3::operator- ( const TVec3& other )
    {
        return TVec3( x - other.x, y - other.y, z - other.z );
    }

    TScalar TVec3::dot( const TVec3& v1, const TVec3& v2 )
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    TVec3 TVec3::cross( const TVec3& v1, const TVec3& v2 )
    {
        TVec3 _res;

        _res.x = v1.y * v2.z - v2.y * v1.z;
        _res.y = -v1.x * v2.z + v2.x * v1.z;
        _res.z = v1.x * v2.y - v2.x * v1.y;

        return _res;
    }

    TScalar TVec3::length( const TVec3& v )
    {
        return std::sqrt( v.x * v.x + v.y * v.y + v.z * v.z );
    }

    std::string TVec3::toString( const TVec3& v )
    {
        std::string _res = "";

        _res += std::string( "x: " ) + std::to_string( v.x ) + std::string( " - " );
        _res += std::string( "y: " ) + std::to_string( v.y ) + std::string( " - " );
        _res += std::string( "z: " ) + std::to_string( v.z );

        return _res;
    }

    TVec4::TVec4()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
        w = 1.0;
    }

    TVec4::TVec4( TScalar x, TScalar y, TScalar z, TScalar w )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    std::string TVec4::toString( const TVec4& v )
    {
        std::string _res = "";

        _res += std::string( "x: " ) + std::to_string( v.x ) + std::string( " - " );
        _res += std::string( "y: " ) + std::to_string( v.y ) + std::string( " - " );
        _res += std::string( "z: " ) + std::to_string( v.z ) + std::string( " - " );
        _res += std::string( "w: " ) + std::to_string( v.w );
            
        return _res;
    }

    TMat3::TMat3()
    {
        buff[0] = 1; buff[3] = 0; buff[6] = 0;
        buff[1] = 0; buff[4] = 1; buff[7] = 0;
        buff[2] = 0; buff[5] = 0; buff[8] = 1;
    }

    TMat3::TMat3( TScalar m00, TScalar m01, TScalar m02,
                  TScalar m10, TScalar m11, TScalar m12,
                  TScalar m20, TScalar m21, TScalar m22 )
    {
        buff[0] = m00; buff[3] = m01; buff[6] = m02;
        buff[1] = m10; buff[4] = m11; buff[7] = m12;
        buff[2] = m20; buff[5] = m21; buff[8] = m22;
    }

    TMat3 TMat3::fromQuaternion( const TVec4& quat )
    {
        TScalar _qi = quat.x;
        TScalar _qj = quat.y;
        TScalar _qk = quat.z;
        TScalar _qr = quat.w;

        TScalar _n = sqrt( _qr * _qr + _qi * _qi + 
                           _qj * _qj + _qk * _qk );

        TScalar _s = 1.0 / ( _n * _n );

        TMat3 _res;

        _res.buff[0] = 1.0 - 2.0 * _s * ( _qj * _qj + _qk * _qk );
        _res.buff[1] = 2.0 * _s * ( _qi * _qj + _qk * _qr );
        _res.buff[2] = 2.0 * _s * ( _qi * _qk - _qj * _qr );

        _res.buff[3] = 2.0 * _s * ( _qi * _qj - _qk * _qr );
        _res.buff[4] = 1.0 - 2.0 * _s * ( _qi * _qi + _qk * _qk );
        _res.buff[5] = 2.0 * _s * ( _qj * _qk + _qi * _qr );

        _res.buff[6] = 2.0 * _s * ( _qi * _qk + _qj * _qr );
        _res.buff[7] = 2.0 * _s * ( _qj * _qk - _qi * _qr );
        _res.buff[8] = 1.0 - 2.0 * _s * ( _qi * _qi + _qj * _qj );

        return _res;
    }

    TMat3 TMat3::fromEuler( const TVec3& euler )
    {
        TMat3 _res;

        float _c1 = std::cos( euler.z );
        float _s1 = std::sin( euler.z );
        float _c2 = std::cos( euler.y );
        float _s2 = std::sin( euler.y );
        float _c3 = std::cos( euler.x );
        float _s3 = std::sin( euler.x );

        _res.buff[0] = _c1 * _c2;
        _res.buff[1] = _s1 * _c2;
        _res.buff[2] = -_s2;

        _res.buff[3] = _c1 * _s2 * _s3 - _s1 * _c3;
        _res.buff[4] = _c1 * _c3 + _s1 * _s2 * _s3;
        _res.buff[5] = _c2 * _s3;

        _res.buff[6] = _s1 * _s3 + _c1 * _s2 * _c3;
        _res.buff[7] = _s1 * _s2 * _c3 - _c1 * _s3;
        _res.buff[8] = _c2 * _c3;

        return _res;
    }

    // Adapted from bullet quaternion conversion (btMatrix3x3.h) ...
    // and this post : http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
    // @RANDOM: just a random thought> perhaps the finite precision could play a role when learning a policy as ...
    // it might sometimes work and some others not. If it generalizes, then should also be able to handle small ...
    // variations, even in small inaccurate computations.
    TVec4 TMat3::toQuaternion( const TMat3& mat )
    {
        TVec4 _res;

        auto _m00 = mat.buff[0];
        auto _m11 = mat.buff[4];
        auto _m22 = mat.buff[8];

        auto _trace = _m00 + _m11 + _m22;

        if ( _trace > 0 )
        {
            _res.w = std::sqrt( _trace + 1 ) / 2.0;

            _res.x = ( mat.buff[5] - mat.buff[7] ) / ( 4 * _res.w );
            _res.y = ( mat.buff[6] - mat.buff[2] ) / ( 4 * _res.w );
            _res.z = ( mat.buff[1] - mat.buff[3] ) / ( 4 * _res.w );
        }
        else if ( ( _m00 > _m11 ) && ( _m00 > _m22 ) )
        {
            auto _s = std::sqrt( 1 + _m00 - _m11 - _m22 ) * 2;

            _res.x = 0.25 * _s;
            _res.y = ( mat.buff[3] + mat.buff[1] ) / _s;
            _res.z = ( mat.buff[6] + mat.buff[2] ) / _s;
            _res.w = ( mat.buff[5] - mat.buff[7] ) / _s;
        }
        else if ( _m11 > _m22 )
        {
            auto _s = std::sqrt( 1 + _m11 - _m00 - _m22 ) * 2;

            _res.x = ( mat.buff[3] + mat.buff[1] ) / _s;
            _res.y = 0.25 * _s;
            _res.z = ( mat.buff[7] + mat.buff[5] ) / _s;
            _res.w = ( mat.buff[6] - mat.buff[2] ) / _s;
        }
        else
        {
            auto _s = std::sqrt( 1 + _m22 - _m00 - _m11 ) * 2;

            _res.x = ( mat.buff[6] + mat.buff[2] ) / _s;
            _res.y = ( mat.buff[7] + mat.buff[5] ) / _s;
            _res.z = 0.25 * _s;
            _res.w = ( mat.buff[1] - mat.buff[3] ) / _s;
        }

        return _res;
    }

    TVec3 TMat3::toEuler( const TMat3& mat )
    {
        TVec3 _res;

        auto _r11 = mat.buff[0];
        auto _r21 = mat.buff[1];
        auto _r31 = mat.buff[2];
        auto _r32 = mat.buff[5];
        auto _r33 = mat.buff[8];

        _res.x = std::atan2( _r32, _r33 );
        _res.y = std::atan2( -_r31, std::sqrt( _r11 * _r11 + _r21 * _r21 ) );
        _res.z = std::atan2( _r21, _r11 );

        return _res;
    }

    std::string TMat3::toString( const TMat3& mat )
    {
        std::string _res;

        _res += "[ ";

        for ( size_t i = 0; i < 3; i++ )
        {
            _res += "\t";

            for ( size_t j = 0; j < 3; j++ )
            {
                size_t _indx = i + j * 3;
                _res += std::to_string( mat.buff[ _indx ] ) + "\t";
            }

            _res += "\n";
        }

        _res += " ]";

        return _res;
    }

    TMat4::TMat4()
    {
        buff[0] = 1; buff[4] = 0; buff[8]  = 0; buff[12] = 0;
        buff[1] = 0; buff[5] = 1; buff[9]  = 0; buff[13] = 0;
        buff[2] = 0; buff[6] = 0; buff[10] = 1; buff[14] = 0;
        buff[3] = 0; buff[7] = 0; buff[11] = 0; buff[15] = 1;
    }

    TMat4::TMat4( const TMat4& other )
    {
        buff[0] = other.buff[0];
        buff[1] = other.buff[1];
        buff[2] = other.buff[2];
        buff[3] = other.buff[3];

        buff[4] = other.buff[4];
        buff[5] = other.buff[5];
        buff[6] = other.buff[6];
        buff[7] = other.buff[7];

        buff[8]  = other.buff[8];
        buff[9]  = other.buff[9];
        buff[10] = other.buff[10];
        buff[11] = other.buff[11];

        buff[12] = other.buff[12];
        buff[13] = other.buff[13];
        buff[14] = other.buff[14];
        buff[15] = other.buff[15];
    }

    TMat4::TMat4( TScalar m00, TScalar m01, TScalar m02, TScalar m03,
                  TScalar m10, TScalar m11, TScalar m12, TScalar m13,
                  TScalar m20, TScalar m21, TScalar m22, TScalar m23,
                  TScalar m30, TScalar m31, TScalar m32, TScalar m33 )
    {
        buff[0] = m00; buff[4] = m01; buff[8]  = m02; buff[12] = m03;
        buff[1] = m10; buff[5] = m11; buff[9]  = m12; buff[13] = m13;
        buff[2] = m20; buff[6] = m21; buff[10] = m22; buff[14] = m23;
        buff[3] = m30; buff[7] = m31; buff[11] = m32; buff[15] = m33;
    }

    TMat4& TMat4::operator= ( const TMat4& other )
    {
        buff[0] = other.buff[0];
        buff[1] = other.buff[1];
        buff[2] = other.buff[2];
        buff[3] = other.buff[3];

        buff[4] = other.buff[4];
        buff[5] = other.buff[5];
        buff[6] = other.buff[6];
        buff[7] = other.buff[7];

        buff[8]  = other.buff[8];
        buff[9]  = other.buff[9];
        buff[10] = other.buff[10];
        buff[11] = other.buff[11];
            
        buff[12] = other.buff[12];
        buff[13] = other.buff[13];
        buff[14] = other.buff[14];
        buff[15] = other.buff[15];

        return *this;
    }

    void TMat4::setPosition( const TVec3& position )
    {
        buff[12] = position.x;
        buff[13] = position.y;
        buff[14] = position.z;
    }

    void TMat4::setRotation( const TMat3& rotation )
    {
        buff[0] = rotation.buff[0];
        buff[1] = rotation.buff[1];
        buff[2] = rotation.buff[2];

        buff[4] = rotation.buff[3];
        buff[5] = rotation.buff[4];
        buff[6] = rotation.buff[5];

        buff[8]  = rotation.buff[6];
        buff[9]  = rotation.buff[7];
        buff[10] = rotation.buff[8];
    }

    void TMat4::setRotation( const TVec3& rotEuler )
    {
        setRotation( TMat3::fromEuler( rotEuler ) );
    }

    void TMat4::setRotation( const TVec4& rotQuaternion )
    {
        setRotation( TMat3::fromQuaternion( rotQuaternion ) );
    }

    TVec3 TMat4::getPosition() const
    {
        return TVec3( buff[12], buff[13], buff[14] );
    }

    TMat3 TMat4::getRotation() const
    {
        return TMat3( buff[0], buff[4], buff[8],
                      buff[1], buff[5], buff[9],
                      buff[2], buff[6], buff[10] );
    }

    TMat4 TMat4::fromPositionAndRotation( const TVec3& pos, const TMat3& rot )
    {
        TMat4 _res;

        _res.buff[0] = rot.buff[0];
        _res.buff[1] = rot.buff[1];
        _res.buff[2] = rot.buff[2];
        _res.buff[3] = 0;

        _res.buff[4] = rot.buff[3];
        _res.buff[5] = rot.buff[4];
        _res.buff[6] = rot.buff[5];
        _res.buff[7] = 0;

        _res.buff[8]  = rot.buff[6];
        _res.buff[9]  = rot.buff[7];
        _res.buff[10] = rot.buff[8];
        _res.buff[11] = 0;

        _res.buff[12] = pos.x;
        _res.buff[13] = pos.y;
        _res.buff[14] = pos.z;
        _res.buff[15] = 1;

        return _res;
    }

    TVec4 shortestArcQuat( TVec3 v, TVec3 vTarget )
    {
        TVec4 _res;

        TVec3 _cross = TVec3::cross( v, vTarget );
        TScalar _dot = TVec3::dot( v, vTarget );

        if ( _dot < -0.99999 ) // if vector are close to antiparallel
        {
            // just pick an arbitrary perpendicular (same trick as in bullet-btvector3)
            TVec3 _n; // perpendicular to pick
            if ( std::abs( v.z ) > std::sqrt( 0.5 ) )
            {
                // choose perpendicular in y-z plane
                TScalar _a = v.y * v.y + v.z * v.z;
                TScalar _k = 1.0 / _a;

                _n.x = 0.0;
                _n.y = -v.z * _k;
                _n.z = v.y * _k;
            }
            else
            {
                // choose perpendicular in x-y plane
                TScalar _a = v.x * v.x + v.y * v.y;
                TScalar _k = 1.0 / _a;

                _n.x = -v.z * _k;
                _n.y = v.y * _k;
                _n.z = 0.0;
            }

            _res.x = _n.x;
            _res.y = _n.y;
            _res.z = _n.z;
            _res.w = 0.0;
        }
        else
        {
            // computation from gpgems
            TScalar _s = std::sqrt( 2.0 * ( 1 + _dot ) );
            _res.x = _cross.x / _s;
            _res.y = _cross.y / _s;
            _res.z = _cross.z / _s;
            _res.w = _s * 0.5;
        }

        return _res;
    }

    std::string TMat4::toString( const TMat4& mat )
    {
        std::string _res;

        _res += "[ ";

        for ( size_t i = 0; i < 4; i++ )
        {
            _res += "\t";
            
            for ( size_t j = 0; j < 4; j++ )
            {
                size_t _indx = i + j * 4;
                _res += std::to_string( mat.buff[ _indx ] ) + "\t";
            }

            _res += "\n";
        }

        _res += " ]";

        return _res;
    }

    TVec3 operator* ( const TMat3& mat, const TVec3& vec )
    {
        TVec3 _res;

        _res.x = mat.buff[0] * vec.x + mat.buff[3] * vec.y + mat.buff[6] * vec.z;
        _res.y = mat.buff[1] * vec.x + mat.buff[4] * vec.y + mat.buff[7] * vec.z;
        _res.z = mat.buff[2] * vec.x + mat.buff[5] * vec.y + mat.buff[8] * vec.z;

        return _res;
    }

    bool operator== ( const TVec3& vec1, const TVec3& vec2 )
    {
        if ( std::fabs( vec1.x - vec2.x ) > TYSOC_FLOAT_EPSILON )
            return false;

        if ( std::fabs( vec1.y - vec2.y ) > TYSOC_FLOAT_EPSILON )
            return false;

        if ( std::fabs( vec1.z - vec2.z ) > TYSOC_FLOAT_EPSILON )
            return false;

        return true;
    }

    bool operator== ( const TVec4& vec1, const TVec4& vec2 )
    {
        if ( std::fabs( vec1.x - vec2.x ) > TYSOC_FLOAT_EPSILON )
            return false;

        if ( std::fabs( vec1.y - vec2.y ) > TYSOC_FLOAT_EPSILON )
            return false;

        if ( std::fabs( vec1.z - vec2.z ) > TYSOC_FLOAT_EPSILON )
            return false;

        if ( std::fabs( vec1.w - vec2.w ) > TYSOC_FLOAT_EPSILON )
            return false;

        return true;
    }

    TScalar rad2degrees( const TScalar& rads )
    {
        return rads * 180.0 / TYSOC_PI;
    }

    std::vector< TScalar > generateRandomArray( int size, TScalar min, TScalar max )
    {
        std::vector< TScalar > _res;
        for ( size_t i = 0; i < size; i++ )
        {
            _res.push_back( TRANDOM( min, max ) );
        }

        return _res;
    }

    void TGenericParams::set( const std::string& name, int val )
    {
        m_ints[ name ] = val;
    }

    void TGenericParams::set( const std::string& name, float val )
    {
        m_floats[ name ] = val;
    }

    void TGenericParams::set( const std::string& name, const TVec3& vec )
    {
        m_vec3s[ name ] = vec;
    }

    void TGenericParams::set( const std::string& name, const TVec4& vec )
    {
        m_vec4s[ name ] = vec;
    }

    void TGenericParams::set( const std::string& name, const TSizei& sizei )
    {
        m_sizeis[ name ] = sizei;
    }

    void TGenericParams::set( const std::string& name, const TSizef& sizef )
    {
        m_sizefs[ name ] = sizef;
    }

    void TGenericParams::set( const std::string& name, const std::string& str )
    {
        m_strings[ name ] = str;
    }

    int TGenericParams::getInt( const std::string& name, int def ) const
    {
        if ( m_ints.find( name ) != m_ints.end() )
        {
            return m_ints.at( name );
        }
        return def;
    }

    float TGenericParams::getFloat( const std::string& name, float def ) const
    {
        if ( m_floats.find( name ) != m_floats.end() )
        {
            return m_floats.at( name );
        }
        return def;
    }

    TVec3 TGenericParams::getVec3( const std::string& name, const TVec3& def ) const
    {
        if ( m_vec3s.find( name ) != m_vec3s.end() )
        {
            return m_vec3s.at( name );
        }
        return def;
    }

    TVec4 TGenericParams::getVec4( const std::string& name, const TVec4& def ) const
    {
        if ( m_vec4s.find( name ) != m_vec4s.end() )
        {
            return m_vec4s.at( name );
        }
        return def;
    }

    TSizei TGenericParams::getSizei( const std::string& name, const TSizei& def ) const
    {
        if ( m_sizeis.find( name ) != m_sizeis.end() )
        {
            return m_sizeis.at( name );
        }
        return def;
    }

    TSizef TGenericParams::getSizef( const std::string& name, const TSizef& def ) const
    {
        if ( m_sizefs.find( name ) != m_sizefs.end() )
        {
            return m_sizefs.at( name );
        }
        return def;
    }

    std::string TGenericParams::getString( const std::string& name, const std::string& def ) const
    {
        if ( m_strings.find( name ) != m_strings.end() )
        {
            return m_strings.at( name );
        }
        return def;
    }


}

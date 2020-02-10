
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
}
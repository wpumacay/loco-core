#pragma once

#include <string>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <memory>

// tinymath-library
#include <vector_t.h>
#include <matrix_t.h>
#include <transforms.h>

#define LOCO_RANDOM_UNIFORM( a, b ) ( a + ( b - a ) * ( rand() / ( float )RAND_MAX ) )

namespace loco
{
    /// Main scalar type used in the library (single-precision floating point)
    typedef float TScalar;

    /// Pi value (same precision as in tinymath library)
    const TScalar PI = TINYMATH_PI;
    /// Eps value (same precision as in tinymath library)
    const TScalar EPS = TINYMATH_EPS;
    /// Max. number of entries in array-ints and array-floats
    const size_t MAX_NDIM = 10;

    TScalar Rad2degrees( const TScalar& rads );
    TScalar Degrees2rad( const TScalar& degrees );

    /// Vector2f alias in the library
    typedef tinymath::Vector<TScalar, 2 > TVec2;
    /// Vector3f alias in the library
    typedef tinymath::Vector<TScalar, 3 > TVec3;
    /// Vector4f alias in the library
    typedef tinymath::Vector<TScalar, 4 > TVec4;

    /// Matrix2f alias in the library
    typedef tinymath::Matrix<TScalar, 2 > TMat2;
    /// Matrix3f alias in the library
    typedef tinymath::Matrix<TScalar, 3 > TMat3;
    /// Matrix4f alias in the library
    typedef tinymath::Matrix<TScalar, 4 > TMat4;

    // redefine VectorXY toString methods (just for easier migration)
    std::string ToString( const TVec2& vec );
    std::string ToString( const TVec3& vec );
    std::string ToString( const TVec4& vec );

    // redefine MatrixXY toString methods (just for easier migration)
    std::string ToString( const TMat2& mat );
    std::string ToString( const TMat3& mat );
    std::string ToString( const TMat4& mat );

    // extend define vector[max-buff-size] as TSize
    template< typename T >
    struct TSize : public tinymath::Vector<T, MAX_NDIM>
    {
        int ndim;

        TSize() : tinymath::Vector<T, MAX_NDIM>(), ndim( 0 ) {}
        TSize( std::initializer_list<T> values ) : ndim( values.size() )
        {
            size_t i = 0;
            for ( auto val : values )
                this->m_buff[i++] = val;
        }
    };

    template< typename T >
    std::string ToString( const TSize<T>& tsize )
    {
        std::string _strrep = "(";
        for ( ssize_t i = 0; i < tsize.ndim; i++ )
            _strrep += " " + std::to_string( tsize[i] );
        _strrep += " )";
        return _strrep;
    }

    typedef TSize<float> TSizef;
    typedef TSize<int32_t> TSizei;

    /// Returns the "shortest" quaternion that transforms the given vector to the given target
    TVec4 ShortestArcQuat( const TVec3& v, const TVec3& vTarget );

    /// Rearranges the elements of a TVec3 from a format where the z vector is given by "worldUp"
    /// into the standard format where the world z is Up. @todo: replace string for enum
    TVec3 Rearrange( const TVec3& vec, const std::string& worldUp );
}
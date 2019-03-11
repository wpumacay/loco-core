
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <set>

#include <tysoc_config.h>

typedef float TScalar;

#define TRANDOM( a, b ) ( a + ( b - a ) * ( rand() / ( float )RAND_MAX ) )

#define BUFF_MAX_SIZE 10

// Prefixes to be added to all format elements (urdf, mjcf, rlsim-json, etc.)
#define TYSOC_PREFIX_BODY       "body_"     
#define TYSOC_PREFIX_JOINT      "joint_"    
#define TYSOC_PREFIX_GEOM       "geom_"     
#define TYSOC_PREFIX_SITE       "site_"     
#define TYSOC_PREFIX_SENSOR     "sensor_"   
#define TYSOC_PREFIX_ACTUATOR   "actuator_" 
#define TYSOC_PREFIX_CAMERA     "camera_"   
#define TYSOC_PREFIX_MATERIAL   "material_" 

#define TYSOC_DEFAULT_DIFFUSE_COLOR     { 0.7f, 0.5f, 0.3f }
#define TYSOC_DEFAULT_SPECULAR_COLOR    { 0.7f, 0.5f, 0.3f }
#define TYSOC_DEFAULT_RGBA_COLOR        { 0.7f, 0.5f, 0.3f, 1.0f }

#define TYSOC_FLOAT_EPSILON 0.000001
#define TYSOC_PI 3.141592653589793

#ifndef TYSOC_PATH_RESOURCES
    #define TYSOC_PATH_RESOURCES "../../res/"
#endif

#ifndef TYSOC_PATH_MJCF_TEMPLATES
    #define TYSOC_PATH_MJCF_TEMPLATES "../../res/templates/mjcf/"
#endif

#ifndef TYSOC_PATH_URDF_TEMPLATES
    #define TYSOC_PATH_URDF_TEMPLATES "../../res/templates/urdf/"
#endif

#ifndef TYSOC_PATH_RLSIM_TEMPLATES
    #define TYSOC_PATH_RLSIM_TEMPLATES "../../res/templates/rlsim/"
#endif

#ifndef TYSOC_PATH_WORKING_DIR
    #define TYSOC_PATH_WORKING_DIR "../../res/xml/"
#endif

namespace tysoc
{

    struct TVec2
    {
        TScalar x;
        TScalar y;

        TVec2();
        TVec2( TScalar px, TScalar py );

        static std::string toString( const TVec2& v );
    };

    struct TVec3
    {
        TScalar x; 
        TScalar y; 
        TScalar z;

        TVec3();
        TVec3( TScalar x, TScalar y, TScalar z );

        TVec3 operator* ( TScalar factor );
        TVec3 operator+ ( const TVec3& other );
        TVec3 operator- ( const TVec3& other );

        static TScalar dot( const TVec3& v1, const TVec3& v2 );
        static TVec3 cross( const TVec3& v1, const TVec3& v2 );
        static TScalar length( const TVec3& v );

        static std::string toString( const TVec3& v );
    };

    struct TVec4
    {
        TScalar x;
        TScalar y;
        TScalar z;
        TScalar w;

        TVec4();
        TVec4( TScalar x, TScalar y, TScalar z, TScalar w );

        static std::string toString( const TVec4& v );
    };

    struct TMat3
    {
        TScalar buff[9];

        TMat3();
        TMat3( TScalar m00, TScalar m01, TScalar m02,
               TScalar m10, TScalar m11, TScalar m12,
               TScalar m20, TScalar m21, TScalar m22 );

        static TMat3 fromQuaternion( const TVec4& quat );
        static TMat3 fromEuler( const TVec3& euler );

        static TVec4 toQuaternion( const TMat3& mat );
        static TVec3 toEuler( const TMat3& mat );

        static std::string toString( const TMat3& v );
    };

    struct TMat4
    {
        TScalar buff[16];

        TMat4();
        TMat4( const TMat4& other );
        TMat4( TScalar m00, TScalar m01, TScalar m02, TScalar m03,
               TScalar m10, TScalar m11, TScalar m12, TScalar m13,
               TScalar m20, TScalar m21, TScalar m22, TScalar m23,
               TScalar m30, TScalar m31, TScalar m32, TScalar m33 );

        TMat4& operator= ( const TMat4& other );

        void setPosition( const TVec3& position );
        void setRotation( const TMat3& rotation );
        void setRotation( const TVec3& rotEuler );
        void setRotation( const TVec4& rotQuaternion );

        TVec3 getPosition() const;
        TMat3 getRotation() const;

        TMat4 operator* ( const TMat4& other )
        {
            TMat4 _res;

            for ( int i = 0; i < 4; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                {
                    _res.buff[ j + i * 4 ] = 0;

                    for ( int k = 0; k < 4; k++ )
                    {
                        // Matrices are stored in column major form, so ...
                        // we use this indexing for the multiplication
                        // k + 4 * (fixed) -> over column
                        // (fixed) + 4 * k -> over row
                        _res.buff[ j + i * 4 ] += this->buff[ j + k * 4 ] * 
                                                  other.buff[ k + i * 4 ];
                    }
                }
            }

            return _res;
        }

        static TMat4 fromPositionAndRotation( const TVec3& pos, const TMat3& rot );

        static std::string toString( const TMat4& v );
    };

    TVec3 operator* ( const TMat3& mat, const TVec3& vec );
    bool operator== ( const TVec3& vec1, const TVec3& vec2 );
    bool operator== ( const TVec4& vec1, const TVec4& vec2 );

    template< class T >
    struct TSize
    {
        int ndim;
        T buff[BUFF_MAX_SIZE];
    };

    template< class T >
    std::string toString( const TSize<T>& size )
    {
        std::string _res;

        for ( size_t i = 0; i < size.ndim; i++ )
        {
            _res += std::to_string( size.buff[i] );
            if ( i != ( size.ndim - 1 ) )
            {
                _res += " ";
            }
        }

        return _res;
    }

    typedef TSize< float >    TSizef;
    typedef TSize< int >      TSizei;

    /**
    * Extracted from bullet btQuaternion implementation (where they use the gameprogramminggems (v1?) 2.10 impl.)
    */
    TVec4 shortestArcQuat( TVec3 v, TVec3 vTarget );

    struct TGeometry
    {
        std::string     type;           // type of geometry (box,sphere,mesh,etc.)
        std::string     meshId;         // id in case of a mesh
        std::string     filename;       // file name in case of a mesh
        TVec3           size;           // size params for primitives, scale for mesh
        TMat4           worldTransform; // position part of the world-transform
        bool            usesFromto;     // flag for fromto checking
    };

    TScalar rad2degrees( const TScalar& rads );

    std::vector< TScalar > generateRandomArray( int size, TScalar min, TScalar max );



    class TGenericParams
    {
        private :

        std::map< std::string, int > m_ints;
        std::map< std::string, float > m_floats;
        std::map< std::string, TVec3 > m_vec3s;
        std::map< std::string, TVec4 > m_vec4s;
        std::map< std::string, TSizei > m_sizeis;
        std::map< std::string, TSizef > m_sizefs;
        std::map< std::string, std::string > m_strings;

        public :

        void set( const std::string& name, int val );
        void set( const std::string& name, float val );
        void set( const std::string& name, const TVec3& vec );
        void set( const std::string& name, const TVec4& vec );
        void set( const std::string& name, const TSizei& sizei );
        void set( const std::string& name, const TSizef& sizef );
        void set( const std::string& name, const std::string& str );

        int getInt( const std::string& name, int def = 0 ) const;
        float getFloat( const std::string& name, float def = 0.0f ) const;
        TVec3 getVec3( const std::string& name, const TVec3& def = { 0.0f, 0.0f, 0.0f } ) const;
        TVec4 getVec4( const std::string& name, const TVec4& def = { 0.0f, 0.0f, 0.0f, 1.0f } ) const;
        TSizei getSizei( const std::string& name, const TSizei& def = { 0, { 0 } } ) const;
        TSizef getSizef( const std::string& name, const TSizef& def = { 0, { 0.0f } } ) const;
        std::string getString( const std::string& name, const std::string& def = "undefined" ) const;

    };

}
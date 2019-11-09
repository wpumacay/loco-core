
#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <deque>
#include <queue>
#include <cassert>
#include <cmath>
#include <map>
#include <set>

#include <tysoc_config.h>

typedef float TScalar;

#define TYSOC_DEMO // used to activate some functionality used for demos

#define TRANDOM( a, b ) ( a + ( b - a ) * ( rand() / ( float )RAND_MAX ) )

#define BUFF_MAX_SIZE 10
#define TRAIL_MAX_SIZE 50

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
#define TYSOC_DEFAULT_SHININESS         32.0f

/* Number for generalized coordinates nq for each type of joint */
#define TYSOC_MAX_NUM_QPOS 7              // maximum number of generalized coordinates possible (free-joint case)
#define TYSOC_MAX_NUM_QVEL 6              // maximum number of degrees of freedom possible (free joint case)
#define TYSOC_NUM_QPOS_JOINT_PRISMATIC 1  // prismatic|slide joints provide only 1q and 1dof
#define TYSOC_NUM_QPOS_JOINT_REVOLUTE  1  // revolute|hinge joints provide only 1q-and 1dof
#define TYSOC_NUM_QPOS_JOINT_SPHERICAL 4  // spherical|ball joints provide 4q (quaternion) and 3dof
#define TYSOC_NUM_QPOS_JOINT_FREE      7  // free(none) joints provide 7q (pos-xyz + rot-quat-xyzw) and 6dof

#define TYSOC_FLOAT_EPSILON 0.000001
#define TYSOC_PI 3.141592653589793

#define TYSOC_DEFAULT_DENSITY 1000.0

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

#ifndef TYSOC_PATH_MESHES_DIR
    #define TYSOC_PATH_MESHES_DIR "../../res/meshes/"
#endif

// Log levels consist of the following:
//  * level 0: nothing at all
//  * level 1: only errors
//  * level 2: errors and warns
//  * level 3: errors, warns and infos
//  * level 4: everything
#define TYSOC_LOG_LEVEL 4

#if TYSOC_LOG_LEVEL > 3
    void TYSOC_LOG( const std::string& msg );
#else
    #define TYSOC_LOG( msg ) {}
#endif

#if TYSOC_LOG_LEVEL > 2
    void TYSOC_INFO( const std::string& msg );
#else
    #define TYSOC_INFO( msg ) {}
#endif

#if TYSOC_LOG_LEVEL > 1
    void TYSOC_WARN( const std::string& msg );
#else
    #define TYSOC_WARN( msg ) {}
#endif

#if TYSOC_LOG_LEVEL > 0
    void TYSOC_ERROR( const std::string& msg );
#else
    #define TYSOC_ERROR( msg ) {}
#endif

#define BIT(x) (1 << x)

namespace tysoc
{

    struct TVec2
    {
        TScalar x;
        TScalar y;

        TVec2();
        TVec2( TScalar px, TScalar py );

        static TScalar length( const TVec2& v );

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
        static TVec3 normalize( const TVec3& v );

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

        void setIdentity();

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
        TMat4( const TVec3& pos, const TMat3& rotMat );
        TMat4( const TVec3& pos, const TVec3& rotEuler );
        TMat4( const TVec3& pos, const TVec4& rotQuat );

        TMat4& operator= ( const TMat4& other );

        void set( int row, int col, TScalar value );
        TScalar get( int row, int col ) const;

        void setPosition( const TVec3& position );
        void setRotation( const TMat3& rotation );
        void setRotation( const TVec3& rotEuler );
        void setRotation( const TVec4& rotQuaternion );

        TVec3 getPosition() const;
        TMat3 getRotation() const;
        TVec3 getRotEuler() const;
        TVec4 getRotQuaternion() const;

        void setIdentity();

        TMat4 inverse() const;

        TMat4 operator* ( const TMat4& other ) const;

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

    TSizef vec3ToSizef( const TVec3& vec );

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
        TMat4           worldTransform; // world-transform
        bool            usesFromto;     // flag for fromto checking
    };

    TScalar rad2degrees( const TScalar& rads );
    TScalar degrees2rad( const TScalar& degrees );

    std::vector< TScalar > generateRandomArray( int size, TScalar min, TScalar max );

    std::vector< std::string > split( const std::string &txt, char separator = '/' );

    std::string getFilenameFromFilePath( const std::string& filepath );
    std::string getFoldernameFromFilePath( const std::string& filepath );
    std::string getFolderpathFromFilePath( const std::string& filepath );
    std::string getFilenameNoExtensionFromFilePath( const std::string& filepath );

    class TGenericParams
    {
        private :

        std::set< std::string > m_keys; 
        std::map< std::string, int > m_ints;
        std::map< std::string, float > m_floats;
        std::map< std::string, TSizei > m_sizeis;
        std::map< std::string, TSizef > m_sizefs;
        std::map< std::string, std::string > m_strings;

        public :

        void set( const std::string& name, int val );
        void set( const std::string& name, float val );
        void set( const std::string& name, const TVec2& vec );
        void set( const std::string& name, const TVec3& vec );
        void set( const std::string& name, const TVec4& vec );
        void set( const std::string& name, const TSizei& sizei );
        void set( const std::string& name, const TSizef& sizef );
        void set( const std::string& name, const std::string& str );

        bool hasParam( const std::string& name );

        int getInt( const std::string& name, int def = 0 );
        float getFloat( const std::string& name, float def = 0.0f );
        TVec2 getVec2( const std::string& name, const TVec2& def = { 0.0f, 0.0f } );
        TVec3 getVec3( const std::string& name, const TVec3& def = { 0.0f, 0.0f, 0.0f } );
        TVec4 getVec4( const std::string& name, const TVec4& def = { 0.0f, 0.0f, 0.0f, 1.0f } );
        TSizei getSizei( const std::string& name, const TSizei& def = { 0, { 0 } } );
        TSizef getSizef( const std::string& name, const TSizef& def = { 0, { 0.0f } } );
        std::string getString( const std::string& name, const std::string& def = "undefined" );

        std::map< std::string, float > floats();
        std::map< std::string, TVec3 > vec3s();
    };

    template< class T >
    class TTrail
    {
        private :

        size_t m_maxSize;
        std::deque< T > m_dqpoints;

        public :

        TTrail()
        {
            m_maxSize = TRAIL_MAX_SIZE;
        }

        ~TTrail()
        {
            m_dqpoints.clear();
        }

        void append( const T& point )
        {
            if ( m_dqpoints.size() < m_maxSize )
            {
                m_dqpoints.push_back( point );
            }
            else
            {
                m_dqpoints.pop_front();
                m_dqpoints.push_back( point );
            }
        }

        std::vector< T > points()
        {
            std::vector< T > _vecpoints;

            for ( auto _it = m_dqpoints.begin(); _it != m_dqpoints.end(); _it++ )
            {
                _vecpoints.push_back( *_it );
            }

            return _vecpoints;
        }
    };

    /**
    *   Rearranges the elements of a TVec3 from a format where the z vector ...
    *   is given by "worldUp", into the standard format where the world z is Up.
    */
    TVec3 rearrange( const TVec3& vec, const std::string& worldUp );
}
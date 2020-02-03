#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <deque>
#include <stack>
#include <cassert>
#include <cmath>
#include <set>
#include <map>
#include <unordered_map>

#include <loco_math.h>
#include <loco_logger.h>
#include <loco_config.h>

// Assimp helper functionality
#include <assimp/config.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef LOCO_PATH_RESOURCES
    #define LOCO_PATH_RESOURCES "../res/"
#endif

namespace loco
{
    const std::string PATH_RESOURCES = LOCO_PATH_RESOURCES;
    const std::string PATH_TEMPLATES_MJCF = PATH_RESOURCES + "templates/mjcf";
    const std::string PATH_TEMPLATES_URDF = PATH_RESOURCES + "templates/urdf";
    const std::string PATH_TEMPLATES_RLSIM = PATH_RESOURCES + "templates/rlsim";

    const std::string PREFIX_BODY     = "body_";
    const std::string PREFIX_JOINT    = "joint_";
    const std::string PREFIX_GEOM     = "geom_";
    const std::string PREFIX_SITE     = "site_";
    const std::string PREFIX_SENSOR   = "sensor_";
    const std::string PREFIX_ACTUATOR = "actuator_";
    const std::string PREFIX_CAMERA   = "camera_";
    const std::string PREFIX_MATERIAL = "material_";

    const TVec3 DEFAULT_AMBIENT_COLOR   = { 0.7f, 0.5f, 0.3f };
    const TVec3 DEFAULT_DIFFUSE_COLOR   = { 0.7f, 0.5f, 0.3f };
    const TVec3 DEFAULT_SPECULAR_COLOR  = { 0.7f, 0.5f, 0.3f };
    const TVec4 DEFAULT_RGBA_COLOR      = { 0.7f, 0.5f, 0.3f, 1.0f };
    const float DEFAULT_SHININESS       = 32.0f;

    /// Maximum number of generalized coordinates possible (free-joint case)
    const size_t MAX_NUM_QPOS = 7;
    /// Maximum number of degrees of freedom possible (free joint case)
    const size_t MAX_NUM_QVEL = 6;
    /// Number of generalized coordinates for prismatic|slide joints : 1qpos, 1qvel
    const size_t NUM_QPOS_JOINT_PRISMATIC = 1;
    /// Number of generalized coordinates for revolute|hinge joints : 1pos, 1qvel
    const size_t NUM_QPOS_JOINT_REVOLUTE = 1;
    /// Number of generalized coordinates for spherical|ball joints : 4qpos (quaternion), 3qvel (angular speed)
    const size_t NUM_QPOS_JOINT_SPHERICAL = 4;
    /// Number of generalized coordinates for free|none joints : 7qpo (pos-xyz, rot-quat), 6qvel (speed-xyz, angular-speed)
    const size_t NUM_QPOS_JOINT_FREE = 7;
    /// Default density value (1000kg/m3 - water density) used for mass calculation (if no mass given)
    const TScalar DEFAULT_DENSITY = 1000.0f;

    std::vector< std::string > split( const std::string &txt, char separator = '/' );

    std::string pointerToHexAddress( const void* ptr );

    std::string getFilenameFromFilePath( const std::string& filepath );
    std::string getFoldernameFromFilePath( const std::string& filepath );
    std::string getFolderpathFromFilePath( const std::string& filepath );
    std::string getFilenameNoExtensionFromFilePath( const std::string& filepath );

    class TGenericParams
    {
    public :

        void set( const std::string& name, int val );
        void set( const std::string& name, float val );
        void set( const std::string& name, const TVec2& vec );
        void set( const std::string& name, const TVec3& vec );
        void set( const std::string& name, const TVec4& vec );
        void set( const std::string& name, const TSizei& sizei );
        void set( const std::string& name, const TSizef& sizef );
        void set( const std::string& name, const std::string& str );

        bool hasParam( const std::string& name ) const;

        int getInt( const std::string& name, int def = 0 ) const;
        float getFloat( const std::string& name, float def = 0.0f ) const;
        TVec2 getVec2( const std::string& name, const TVec2& def = TVec2( 0.0f, 0.0f ) ) const;
        TVec3 getVec3( const std::string& name, const TVec3& def = TVec3( 0.0f, 0.0f, 0.0f ) ) const;
        TVec4 getVec4( const std::string& name, const TVec4& def = TVec4( 0.0f, 0.0f, 0.0f, 1.0f ) ) const;
        TSizei getSizei( const std::string& name, const TSizei& def = TSizei() ) const;
        TSizef getSizef( const std::string& name, const TSizef& def = TSizef() ) const;
        std::string getString( const std::string& name, const std::string& def = "undefined" ) const;

        std::unordered_map< std::string, float > floats() const;
        std::unordered_map< std::string, TVec3 > vec3s() const;

    private :

        std::set< std::string > m_keys; 
        std::unordered_map< std::string, int > m_ints;
        std::unordered_map< std::string, float > m_floats;
        std::unordered_map< std::string, TSizei > m_sizeis;
        std::unordered_map< std::string, TSizef > m_sizefs;
        std::unordered_map< std::string, std::string > m_strings;
    };
}
#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>
#include <memory>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <loco_math.h>
#include <loco_object.h>
#include <loco_logger.h>
#include <loco_config.h>

// Assimp helper functionality
#include <assimp/config.h>
#include <assimp/cimport.h>
#include <assimp/cexport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef LOCO_PATH_RESOURCES
    #define LOCO_PATH_RESOURCES "../res/"
#endif

namespace loco
{
    /// Path to where the framework's resources are located
    const std::string PATH_RESOURCES = LOCO_PATH_RESOURCES;
    /// Path to where the framework's mjcf-files (xml) are located
    const std::string PATH_TEMPLATES_MJCF = PATH_RESOURCES + "templates/mjcf/";
    /// Path to where the framework's urdf-files (xml) are located
    const std::string PATH_TEMPLATES_URDF = PATH_RESOURCES + "templates/urdf/";
    /// Path to where the framework's rlsim-files (json) are located
    const std::string PATH_TEMPLATES_RLSIM = PATH_RESOURCES + "templates/rlsim/";

    /// Prefix-string used as a namespace for body-resources
    const std::string PREFIX_BODY = "body_";
    /// Prefix-string used as a namespace for joint-resources
    const std::string PREFIX_JOINT = "joint_";
    /// Prefix-string used as a namespace for geom|collider-resources
    const std::string PREFIX_GEOM = "geom_";
    /// Prefix-string used as a namespace for sensor-resources
    const std::string PREFIX_SENSOR = "sensor_";
    /// Prefix-string used as a namespace for actuator-resources
    const std::string PREFIX_ACTUATOR = "actuator_";
    /// Prefix-string used as a namespace for camera-resources
    const std::string PREFIX_CAMERA = "camera_";
    /// Prefix-string used as a namespace for light-resources
    const std::string PREFIX_LIGHT = "light_";

    /// Suffix-string used for collider-resources
    const std::string SUFFIX_COLLIDER = "_col";
    /// Suffix-string used for drawable-resources
    const std::string SUFFIX_DRAWABLE = "_vis";

    /// Default rgb ambient color used for drawables
    const TVec3 DEFAULT_AMBIENT_COLOR = { 0.7f, 0.5f, 0.3f };
    /// Default rgb diffuse color used for drawables
    const TVec3 DEFAULT_DIFFUSE_COLOR = { 0.7f, 0.5f, 0.3f };
    /// Default rgb specular color used for drawables
    const TVec3 DEFAULT_SPECULAR_COLOR = { 0.7f, 0.5f, 0.3f };
    /// Default rgba color used for drawables
    const TVec4 DEFAULT_RGBA_COLOR = { 0.7f, 0.5f, 0.3f, 1.0f };
    /// Default shininess factor used for drawables
    const float DEFAULT_SHININESS = 32.0f;

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
    /// Default density value (1000 kg/m3 - water density) used for mass calculation (if no mass given)
    const TScalar DEFAULT_DENSITY = 1000.0f;

    /// Divides a given string into substrings using the given character separator
    ///
    /// @param txt          String to be splitted into substrings
    /// @param separator    Character separator used for the splitting process
    /// @return Vector of strings that form the initial string, after splitting using the separator
    std::vector< std::string > Split( const std::string &txt, char separator = '/' );

    /// Gets the string representation of the hex-address of a given pointer
    ///
    /// @param ptr  Pointer from whom to get the hex-address representation
    /// @return String representation of the hex-address of the pointer
    std::string PointerToHexAddress( const void* ptr );

    /// Gets the stripped filename from an absolute full-path
    ///
    /// @param filepath     Absolute path from which to strip the filename
    /// @return String representing the filename stripped from the given fullpath
    std::string GetFilenameFromFilePath( const std::string& filepath );

    /// Gets the stripped foldername from an absolute full-path
    ///
    /// @param filepath     Absolute path from which to strip the foldername
    /// @return String representing the foldername stripped from the given fullpath
    std::string GetFoldernameFromFilePath( const std::string& filepath );

    /// Gets the folderpath (path with removed filename) from an absolute full-path
    ///
    /// @param filepath     Absolute path from which to strip the folderpath
    /// @return String representing the folderpath stripped from the given fullpath
    std::string GetFolderpathFromFilePath( const std::string& filepath );

    /// Gets the stripped filename from an absolute full-path, and removes the file extension
    ///
    /// @param filepath     Absolute path from which to strip the filename
    /// @return String representing the filename (without extension) stripped from the given fullpath
    std::string GetFilenameNoExtensionFromFilePath( const std::string& filepath );

    /// Hashed storage for various data types
    ///
    /// @details
    /// This container acts like a dictionary where to store key-value pairs for
    /// various data types. Keys are unique string identifiers, and duplicates are
    /// not considered while adding elements to this container.
    class TGenericParams
    {
    public :

        TGenericParams() = default;
        TGenericParams( const TGenericParams& other ) = default;
        TGenericParams& operator= ( const TGenericParams& other ) = default;
        ~TGenericParams() = default;

        /// Stores a given integer for a given key
        ///
        /// @param name     Key of the given integer to be stored
        /// @param val      Integer value to be stored
        void Set( const std::string& name, int val );

        /// Stores a given float for a given key
        ///
        /// @param name     Key of the given float to be stored
        /// @param val      Float value to be stored
        void Set( const std::string& name, float val );

        /// Stores a given Vector-2 for a given key
        ///
        /// @param name     Key of the given vector-2 to be stored
        /// @param vec      Vector-2 value to be stored
        void Set( const std::string& name, const TVec2& vec );

        /// Stores a given Vector-3 for a given key
        ///
        /// @param name     Key of the given vector-3 to be stored
        /// @param vec      Vector-3 value to be stored
        void Set( const std::string& name, const TVec3& vec );

        /// Stores a given Vector-4 for a given key
        ///
        /// @param name     Key of the given vector-4 to be stored
        /// @param vec      Vector-4 value to be stored
        void Set( const std::string& name, const TVec4& vec );

        /// Stores a given Array-int for a given key
        ///
        /// @param name         Key of the given Array-int to be stored
        /// @param array_int    Array-int value to be stored
        void Set( const std::string& name, const TSizei& array_int );

        /// Stores a given Array-float for a given key
        ///
        /// @param name         Key of the given Array-float to be stored
        /// @param array_float  Array-float value to be stored
        void Set( const std::string& name, const TSizef& array_float );

        /// Stores a given string for a given key
        ///
        /// @param name     Key of the given string to be stored
        /// @param str      String value to be stored
        void Set( const std::string& name, const std::string& str );

        /// Gets whether or not the given key is being stored in the container
        ///
        /// @param name     Key value to look for in the container
        /// @return True if the given key is present, false otherwise
        bool HasParam( const std::string& name ) const;

        int GetInt( const std::string& name, int def = 0 ) const;
        float GetFloat( const std::string& name, float def = 0.0f ) const;
        TVec2 GetVec2( const std::string& name, const TVec2& def = TVec2( 0.0f, 0.0f ) ) const;
        TVec3 GetVec3( const std::string& name, const TVec3& def = TVec3( 0.0f, 0.0f, 0.0f ) ) const;
        TVec4 GetVec4( const std::string& name, const TVec4& def = TVec4( 0.0f, 0.0f, 0.0f, 1.0f ) ) const;
        TSizei GetSizei( const std::string& name, const TSizei& def = TSizei() ) const;
        TSizef GetSizef( const std::string& name, const TSizef& def = TSizef() ) const;
        std::string GetString( const std::string& name, const std::string& def = "undefined" ) const;

        std::unordered_map< std::string, float > GetFloats() const;
        std::unordered_map< std::string, TVec3 > GetVec3s() const;

    private :

        std::set< std::string > m_keys; 
        std::unordered_map< std::string, int > m_ints;
        std::unordered_map< std::string, float > m_floats;
        std::unordered_map< std::string, TSizei > m_sizeis;
        std::unordered_map< std::string, TSizef > m_sizefs;
        std::unordered_map< std::string, std::string > m_strings;
    };
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef UNIQUE_PTR_EXTENSION
#define UNIQUE_PTR_EXTENSION
namespace std
{
    // make_unique implementation from Herb Sutter's blog
    // url: https://herbsutter.com/gotw/_102/
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&& ...args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }
}
#endif /* UNIQUE_PTR_EXTENSION */

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
#pragma once

#include <loco_common.h>

/// Loco core functionality
namespace loco
{
    /// Type of shape, used for collider-related objects in the simulation
    enum class eShapeType : uint8_t
    {
        NONE = 0,       ///< Dummy collision shape
        BOX,            ///< Box collision shape, defined by its width, depth, and height
        PLANE,          ///< Plane collision shape, defined by its width and depth
        SPHERE,         ///< Sphere collision shape, defined by its radius
        CYLINDER,       ///< Cylinder collision shape, defined by its radius and height
        CAPSULE,        ///< Capsule collision shape, defined by its radius and cylindrical-part height
        ELLIPSOID,      ///< Ellipsoid collision shape, defined by its three radii (x,y,z)
        CONVEX_MESH,    ///< Convex-mesh collision shape, defined by a file-resource, or user-defined vertex-data
        TRIANGULAR_MESH,///< Triangular-mesh collision shape, defined by a file-resources, or user defined vertex-data
        HEIGHTFIELD     ///< Heightfield collision-shape, defined by user-defined elevation data
    };

    /// Type of joint, used for joint-related objects in the simulation
    enum class eJointType : uint8_t
    {
        FREE = 0,
        REVOLUTE,
        PRISMATIC,
        SPHERICAL,
        PLANAR,
        FIXED
    };

    /// Simulation dynamics-behaviour
    enum class eDynamicsType : uint8_t
    {
        DYNAMIC = 0,
        STATIC
    };

    /// Sensor type, used for sensor-related objects in the simulation
    enum class eSensorType : uint8_t
    {
        NONE = 0,
        PROP_JOINT,
        PROP_BODY,
        EXT_HEIGHTFIELD_1D,
        EXT_HEIGHTFIELD_2D,
        EXT_CAMERA_RGB,
        EXT_CAMERA_DEPTH,
        EXT_CAMERA_SEGMENTATION
    };

    /// Actuator type, used for actuator-related objects in the simulation
    enum class eActuatorType : uint8_t
    {
        TORQUE = 0,
        POSITION,
        VELOCITY,
        PD_CONTROLLER
    };

    std::string ToString( const eShapeType& type );
    std::string ToString( const eJointType& type );
    std::string ToString( const eDynamicsType& type );
    std::string ToString( const eSensorType& type );
    std::string ToString( const eActuatorType& type );

    eShapeType ToEnumShape( const std::string& shape );
    eJointType ToEnumJoint( const std::string& type );
    eActuatorType ToEnumActuator( const std::string& type );

    struct TMeshData
    {
        // Absolute path to the mesh-resource (create from-file)
        std::string filename;
        // Buffer of user vertex-data for mesh-resource (create from user data)
        std::vector<float> vertices;
        // Buffer of user faces data for mesh resource (create from user data)
        std::vector<int> faces;
    };

    struct THeightFieldData
    {
        // Number of samples of the grid in the x-dimension (extent is given by size.x)
        int nWidthSamples;
        // Number of samples of the grid on the y-dimension (extent is given by size.y)
        int nDepthSamples;
        // Height values (row-major format) in [0-1], scaled to actual height by size.z component
        std::vector< float > heights;
    };

    struct TShapeData
    {
        eShapeType          type;               // type of collision shape (see enum above)
        TVec3               size;               // size of the collision shape (e.g. x->radius for sphere shapes)
        TMeshData           mesh_data;          // struct with required data for mesh-type objects
        THeightFieldData    hfield_data;        // struct with required data for hfield-type objects
        TMat4               localTransform;     // relative transform of this shape (visual|collision) w.r.t. owner (body)
    };

    std::string ToString( const TShapeData& shapeData );

    // @todo: Add contructor that expects type, to construct an initialized joint (nqpos, nqvel)
    struct TJointData
    {
        eJointType  type;           // type of joint (see enum above)
        TVec3       axis;           // axis of joint to be constructed
        TVec2       limits;         // motion range (lo==hi: fixed, lo>hi: continuous, lo<hi: limited)
        TScalar     stiffness;      // Stiffness (spring like behaviour)
        TScalar     armature;       // Armature (extra diag inertia)
        TScalar     damping;        // Damping applied to the joint
        TScalar     ref;            // Ref. for the joint
        size_t      nqpos;          // Number of generalized coordinates
        size_t      nqvel;          // Number of degrees of freedom
        TMat4       localTransform; // relative transform of this joint w.r.t. owner (body)

        TJointData() 
            : limits( { 1.0f, -1.0f } ), stiffness( 0.0f ), 
              armature( 0.0f ), damping( 0.0f ), ref( 0.0f ), 
              nqpos( 0 ), nqvel( 0 )
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TJointData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TJointData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TJointData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TJointData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TJointData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }
    };

    struct TCollisionData : public TShapeData
    {
        int     collisionGroup;
        int     collisionMask;
        TVec3   friction;
        TScalar density;

        TCollisionData()
            : collisionGroup( 1 ), collisionMask( 1 ),
              friction( { 1., 0.005, 0.0001 } ), density( loco::DEFAULT_DENSITY )
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TCollisionData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TCollisionData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TCollisionData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TCollisionData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TCollisionData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }
    };

    struct TVisualData : public TShapeData
    {
        TVec3       ambient;    // ambient-color (rgb) of the renderable associated with this shape
        TVec3       diffuse;    // diffuse-color (rgb) of the renderable associated with this shape
        TVec3       specular;   // specular-color (rgb) of the renderable associated with this shape
        TScalar     shininess;  // shininess-value for the specular-component of the color material
        std::string texture;    // texture id (or filename if not cached)
        bool        usesMaterialFromMesh; // mesh-loading takes care of material properties

        TVisualData()
            : ambient( loco::DEFAULT_AMBIENT_COLOR ),
              diffuse( loco::DEFAULT_DIFFUSE_COLOR ),
              specular( loco::DEFAULT_SPECULAR_COLOR ),
              shininess( loco::DEFAULT_SHININESS ),
              usesMaterialFromMesh( false )
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TVisualData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TVisualData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TVisualData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TVisualData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TVisualData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }
    };

    struct TActuatorData
    {
        eActuatorType   type;               // type of actuator (see enum above)
        TVec2           limits;             // range for control limits. If no limits, then lower(x) > high(y)
        TSizef          gear;               // Scaling factors for actuator's transmissions
        TScalar         kp;                 // Position feedback gain (in case controller-typ used)
        TScalar         kv;                 // Velocity feedback gain (in case controller-typ used)
        TMat4           localTransform;     // relative transform of this actuator w.r.t. owner (body)

        TActuatorData()
            : type( eActuatorType::TORQUE ), limits( { -1.0f, 1.0f } ),
              gear( TSizef() ), kp( 0.0f ), kv( 0.0f )
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TActuatorData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TActuatorData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TActuatorData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TActuatorData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TActuatorData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }
    };

    struct TSensorData
    {
        eSensorType type;               // type of sensor (see enum above)
        TMat4       localTransform;     // relative transform of this sensor w.r.t. owner (either body or joint)

        TSensorData()
            : type( eSensorType::PROP_JOINT )
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TSensorData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TSensorData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TSensorData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TSensorData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TSensorData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }
    };

    /// Object representation of the contact associated with a specific collider (resolved by the collision detector)
    struct TContactData
    {
        /// Contact-point in world-space
        TVec3 position = { 0.0, 0.0, 0.0 };
        /// Contact-normal in world-space (from this collider to the collider with whom the contact was detected)
        TVec3 normal = { 0.0, 0.0, 0.0 };
        /// Unique identifier of the collider with whom the contact was detected
        std::string name = "";
    };

    struct TInertialData
    {
        TScalar     mass;               // mass of the body (if negative then inertial props come from colliders)
        TScalar     ixx;                // elements of the inertia matrix
        TScalar     ixy;
        TScalar     ixz;
        TScalar     iyy;
        TScalar     iyz;
        TScalar     izz;
        TMat4       localTransform;     // relative transform w.r.t. owner (body) of the inertial frame

        TInertialData()
            : mass( -1.0f ), ixx( -1.0f ), ixy( -1.0f ), ixz( -1.0f ),
              iyy( -1.0f ), iyz( -1.0f ), izz( -1.0f )
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TInertialData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TInertialData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TInertialData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TInertialData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TInertialData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }
    };

    struct TBodyData
    {
        eDynamicsType   dyntype;            // dynamics type used to instantiate either static, kinematic or dynamic bodies
        TInertialData   inertia;            // inertial properties of this body
        TMat4           localTransformZero; // relative transform at zero-configuration of this body w.r.t. parent body
        TCollisionData  collision;          // collisions owned by this body
        TVisualData     visual;             // visuals owned by this body

        TBodyData() : dyntype( eDynamicsType::STATIC )
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TBodyData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TBodyData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TBodyData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TBodyData @ {0}", tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TBodyData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
        }
    };

    TScalar ComputeVolumeFromShape( const TShapeData& shapeData );
    std::pair<TVec3, TVec3> ComputeMeshAABB( const std::string& filename );
    std::pair<TVec3, TVec3> ComputeMeshAABB( const std::vector<float>& vertices );
}
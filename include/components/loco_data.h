#pragma once

#include <loco_common.h>

namespace loco
{
    enum class eShapeType
    {
        NONE = 0,
        BOX, 
        PLANE, 
        SPHERE, 
        CYLINDER, 
        CAPSULE, 
        ELLIPSOID, 
        MESH, 
        HFIELD
    };

    enum class eJointType
    {
        FREE = 0,
        REVOLUTE,
        PRISMATIC,
        SPHERICAL,
        PLANAR,
        FIXED
    };

    enum class eDynamicsType
    {
        DYNAMIC = 0,
        STATIC,
        KINEMATIC
    };

    enum class eSensorType
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

    enum class eActuatorType
    {
        TORQUE = 0,
        POSITION,
        VELOCITY,
        PD_CONTROLLER
    };

    enum class eBodyClassType
    {
        NONE = 0,
        SINGLE_BODY,
        COMPOUND_BODY,
        KINEMATIC_TREE_BODY
    };

    std::string ToString( const eShapeType& type );
    std::string ToString( const eJointType& type );
    std::string ToString( const eDynamicsType& type );
    std::string ToString( const eSensorType& type );
    std::string ToString( const eActuatorType& type );
    std::string ToString( const eBodyClassType& type );

    eShapeType ToEnumShape( const std::string& shape );
    eJointType ToEnumJoint( const std::string& type );
    eActuatorType ToEnumActuator( const std::string& type );

    // @todo: add mesh-data field in shape-data
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
        std::string         filename;           // in case of a mesh shape, abs-path for the resource
        THeightFieldData    hdata;              // heightfield struct with required data for hfield-type objects
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
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TJointData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TJointData @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TJointData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TJointData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TJointData @ " << loco::PointerToHexAddress( this ) << std::endl;
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
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TCollisionData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TCollisionData @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TCollisionData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TCollisionData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TCollisionData @ " << loco::PointerToHexAddress( this ) << std::endl;
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
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TVisualData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TVisualData @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TVisualData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TVisualData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TVisualData @ " << loco::PointerToHexAddress( this ) << std::endl;
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
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TActuatorData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TActuatorData @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TActuatorData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TActuatorData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TActuatorData @ " << loco::PointerToHexAddress( this ) << std::endl;
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
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TSensorData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TSensorData @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TSensorData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TSensorData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TSensorData @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
        }
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
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TInertialData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TInertialData @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TInertialData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TInertialData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TInertialData @ " << loco::PointerToHexAddress( this ) << std::endl;
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
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TBodyData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TBodyData @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
        }

        ~TBodyData()
        {
        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TBodyData @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TBodyData @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
        }
    };

    TScalar ComputeVolumeFromShape( const TShapeData& shapeData );
    std::pair<TVec3, TVec3> ComputeMeshAABB( const std::string& filename );
}
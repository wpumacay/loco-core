
#pragma once

#include <agent/agent_base.h>


namespace tysoc{
namespace agent{

    // The approach is to define as much functionality supported ...
    // as possible (like a variant/generic). This is a bit wasteful if ...
    // some schemas (urdf, mjcf, ...) dont have certain functionality, but ...
    // in case they try to add this, we can accomodate without losses.
    // Some structures that might or might not be in some representations are ...
    // sensors and actuators. Still, in case they don't support them, the ...
    // concrete representation class will handle initializing by default this structures.

    // forward declaration to allow composition
    struct TKinTreeBody;
    struct TKinTreeJoint;
    struct TKinTreeVisual;
    struct TKinTreeSensor;
    struct TKinTreeActuator;

    struct TKinTreeMeshAsset
    {
        std::string     name;
        std::string     file;
    };

    struct TMaterial
    {
        std::string     name;
        TVec3           diffuse;
        TVec3           specular;
        std::string     texture;
    };

    struct TKinTreeCollision
    {
        std::string     name;
        TGeometry       geometry;       // Geometry information
        TKinTreeBody*   parentBodyPtr;  // Parent body
        TMat4           relTransform;   // Relative transform to parent body
        // @CHECK|@WIP : Add a variant here, for special params given model data
        int             contype;        // (mujoco) Contype bitmask for collision detection
        int             conaffinity;    // (mujoco) Conaffinity bitmask for collision detection
        int             condim;         // (mujoco) Contact dimensionality
        int             group;          // (mujoco) Group this object belongs (for compiler calcs.)

        TKinTreeCollision()
        {
            parentBodyPtr   = NULL;
            // @CHECK|@WIP : Initialize variant here
            contype         = -1; // -1 indicates not available
            conaffinity     = -1; // -1 indicates not available
            condim          = -1; // -1 indicates not available
            group           = -1; // -1 indicates not available
        }
    };

    struct TKinTreeVisual
    {
        std::string     name;
        TMaterial       material;
        TGeometry       geometry;       // Geometry information
        TKinTreeBody*   parentBodyPtr;  // Parent body
        TMat4           relTransform;   // Relative transform to parent body
        // @CHECK|@WIP : Add a variant here, for special params given model data
        int             contype;        // (mujoco) Contype bitmask for collision detection
        int             conaffinity;    // (mujoco) Conaffinity bitmask for collision detection
        int             condim;         // (mujoco) Contact dimensionality
        int             group;          // (mujoco) Group this object belongs (for compiler calcs.)
        std::string     materialName;   // (mujoco) Name of the material from includes (materials.xml) (@CHECK: should use TMaterial)
        TVec4           rgba;           // (mujoco) RGBA color (@CHECK: should use TMaterial)

        TKinTreeVisual()
        {
            parentBodyPtr   = NULL;
            // @CHECK|@WIP : Initialize variant here
            contype         = -1; // -1 indicates not available
            conaffinity     = -1; // -1 indicates not available
            condim          = -1; // -1 indicates not available
            group           = -1; // -1 indicates not available
            materialName    = "";
            rgba            = { 0.3, 0.4, 0.5, 1.0 };
        }
    };

    /**
    * @brief: Sensor node attached to a component of the kintree
    */
    struct TKinTreeSensor
    {
        std::string         name;               // name of this sensor
        std::string         type;               // type of sensor
        TMat4               worldTransform;     // world transform
        TKinTreeBody*       bodyPtr;            // parent body pointer (if linked to a body|or site(mujoco))
        TKinTreeJoint*      jointPtr;           // parent joint pointer (if linked to a joint)

        TKinTreeSensor()
        {
            bodyPtr = NULL;
            jointPtr = NULL;
        }
    };

    struct TKinTreeJointSensor : public TKinTreeSensor
    {
        std::string     jointName;
        TScalar         theta;
        TScalar         thetadot;

        TKinTreeJointSensor()
        {
            type        = "joint";
            jointName   = "";
            theta       = 0;
            thetadot    = 0;
        }
    };

    struct TKinTreeBodySensor : public TKinTreeSensor
    {
        std::string     bodyName;
        TVec3           linVelocity;
        TVec3           linAcceleration;

        TKinTreeBodySensor()
        {
            type        = "body";
            bodyName    = "";
        }
    };

    struct TKinTreeInertia
    {
        TScalar     mass;
        TScalar     ixx;
        TScalar     ixy;
        TScalar     ixz;
        TScalar     iyy;
        TScalar     iyz;
        TScalar     izz;
        TMat4       relTransform;   // Relative transform to parent body
    };

    /**
    * @brief: Body node for the kinematic tree
    */
    struct TKinTreeBody
    {
        std::string                         name;
        TMat4                               worldTransform;     // World transform
        TMat4                               relTransform;       // Relative transform to parent body (mjcf)
        TKinTreeBody*                       parentBodyPtr;      // Parent body (mjcf)
        TKinTreeJoint*                      parentJointPtr;     // Parent joint (urdf)
        TKinTreeInertia*                    inertiaPtr;         // Inertia (if defined. if not, calculated by engine) (urdf)
        std::vector< TKinTreeCollision* >   childCollisions;    // Collisions attached to the body
        std::vector< TKinTreeVisual* >      childVisuals;       // Visuals attached to the body
        std::vector< TKinTreeJoint* >       childJoints;        // Connections to other bodies
        std::vector< TKinTreeBody* >        childBodies;        // Child bodies (mjcf)

        TKinTreeBody()
        {
            parentBodyPtr   = NULL;
            parentJointPtr  = NULL;
            inertiaPtr      = NULL;
        }
    };

    /**
    * @brief: Joint node for the kinematic tree
    */
    struct TKinTreeJoint
    {
        std::string         name;
        std::string         type;           // Joint type
        TVec3               axis;           // Joint axis
        TMat4               worldTransform; // World transform
        TMat4               relTransform;   // Relative transform
        TKinTreeBody*       parentBodyPtr;  // Parent body (urdf,mjcf)
        TKinTreeBody*       childBodyPtr;   // Child body (urdf)
        TScalar             lowerLimit;     // Lower range limit
        TScalar             upperLimit;     // Upper range limit
        bool                limited;        // Flag for joint value clamping
        // @CHECK|@WIP : Add a variant here, for special params given model data
        TScalar             stiffness;      // (mujoco) Stiffness (spring like behaviour like)
        TScalar             armature;       // (mujoco) Armature (extra diag inertia)
        TScalar             damping;        // (mujoco) Damping applied to the joint

        TKinTreeJoint()
        {
            parentBodyPtr   = NULL;
            childBodyPtr    = NULL;
            lowerLimit      = 0;
            upperLimit      = 0;
            limited         = false;
            // @CHECK|@WIP : Initialize variant here
            stiffness       = 0.0;
            armature        = 0.0;
            damping         = 0.0;
        }
    };

    struct TKinTreeActuator
    {
        std::string         name;
        std::string         type;           // Actuator type (generic, depends of the backend)
        TMat4               worldTransform; // World transform
        TMat4               relTransform;   // Relative transform
        TKinTreeJoint*      jointPtr;       // Attached joint (to whom the actuator is applied)
        TScalar             ctrlValue;      // Control signal to be applied to this actuator
        TScalar             minCtrl;        // Control min limit
        TScalar             maxCtrl;        // Control max limit
        // @CHECK|@WIP : Add a variant here, for special params given model data
        TSizef              gear;           // (mujoco) Length scaling (6 dim)
        TScalar             kp;             // (mujoco) Position feedback gain
        TScalar             kv;             // (mujoco) Velocity feedback gain
        bool                clampCtrl;      // (mujoco) Flag for ctrl value clamping

        TKinTreeActuator()
        {
            jointPtr    = NULL;
            minCtrl     = -1.0;
            maxCtrl     = 1.0;
            ctrlValue   = 0.0;
            // @CHECK|@WIP : Initialize variant here
            gear        = { 1, { 1.0 } };
            kp          = 1.0;
            kv          = 1.0;
            clampCtrl   = true;
        }
    };

    const std::string MODEL_TEMPLATE_TYPE_MJCF  = "mjcf";
    const std::string MODEL_TEMPLATE_TYPE_URDF  = "urdf";
    const std::string MODEL_TEMPLATE_TYPE_RLSIM = "rlsim";

    // @TODO|@CHECK: should be able to also create a kintree programmatically, ...
    // as well as changing some properties of the agent as desired. So far it seems that ...
    // just some methods for easy manipulation should suffice, but it also needs some extra ...
    // functionality to allow the constraints be in check (like if you change a leg size, the other ...
    // children should have its properties modified as well to take that into account ).
    // This is a kintree itself, so this support should be possible (as I think about it, it seems ...
    // it will not be that hard to implement this functionality). I'll try that out definitely in the ...
    // next commit. So far, I'm fixing some issues with the new agent API and making the sensors work with ...
    // this new API (some maintenance)
    // Check the functionality I implemented here: https://wpumacay.github.io/leoJS/playground.html
    // Something like this should be maintained. As I create more frames, these are kept together in the tree ...
    // and changes are propagated properly down the chain as needed.
    
    class TAgentKinTree : public TIAgent
    {

        protected :

        TKinTreeBody*                       m_rootBodyPtr;          // root body

        std::vector< TKinTreeBody* >        m_kinTreeBodies;        // kinTree bodies
        std::vector< TKinTreeJoint* >       m_kinTreeJoints;        // kinTree joints
        std::vector< TKinTreeVisual* >      m_kinTreeVisuals;       // kinTree visuals
        std::vector< TKinTreeCollision* >   m_kinTreeCollisions;    // kinTree collisions
        std::vector< TKinTreeActuator* >    m_kinTreeActuators;     // kinTree actuators
        std::vector< TKinTreeSensor* >      m_kinTreeSensors;       // kinTree actuators
        std::vector< TKinTreeMeshAsset* >   m_kinTreeMeshAssets;    // kinTree mesh assets

        std::map< std::string, TKinTreeBody* >      m_mapKinTreeBodies;     // map of bodies (easy access)
        std::map< std::string, TKinTreeJoint* >     m_mapKinTreeJoints;     // map of joints (...)
        std::map< std::string, TKinTreeVisual* >    m_mapKinTreeVisuals;    // map of visuals (...)
        std::map< std::string, TKinTreeCollision* > m_mapKinTreeCollisions; // map of collisions (...)
        std::map< std::string, TKinTreeActuator* >  m_mapKinTreeActuators;  // map of actuators (...)
        std::map< std::string, TKinTreeSensor* >    m_mapKinTreeSensors;    // map of sensors (...)
        std::map< std::string, TKinTreeMeshAsset* > m_mapKinTreeMeshAssets; // map of assets (...)

        std::string m_modelTemplateType;

        void _updateAgentInternal( float dt ) override;

        // update components
        void _updateBody( TKinTreeBody* kinTreeBodyPtr );
        void _updateJoint( TKinTreeJoint* kinTreeJointPtr );
        void _updateActuator( TKinTreeActuator* kinTreeActuatorPtr );
        void _updateSensor( TKinTreeSensor* kinTreeSensorPtr );
        void _updateVisual( TKinTreeVisual* kinTreeVisualPtr );
        void _updateCollision( TKinTreeCollision* kinTreeCollisionPtr );

        void _initializeKinTree();
        void _constructDefaultSensors();
        virtual void _constructKinTree() = 0;
        virtual void _initializeWorldTransforms() = 0;

        public :

        TAgentKinTree( const std::string& name,
                       const TVec3& position );
        ~TAgentKinTree() override;

        void setActions( const std::vector< TScalar >& actions );
        int getActionDim();
        void getActuatorLimits( size_t actuatorIndx, TScalar& minCtrl, TScalar& maxCtrl );

        std::vector< TKinTreeBody* >        getKinTreeBodies();
        std::vector< TKinTreeJoint* >       getKinTreeJoints();
        std::vector< TKinTreeVisual* >      getKinTreeVisuals();
        std::vector< TKinTreeCollision* >   getKinTreeCollisions();
        std::vector< TKinTreeActuator* >    getKinTreeActuators();
        std::vector< TKinTreeSensor* >      getKinTreeSensors();
        std::vector< TKinTreeMeshAsset* >   getKinTreeMeshAssets();

        TKinTreeBody* getRootBody();
        std::string getModelTemplateType();
    };


}}
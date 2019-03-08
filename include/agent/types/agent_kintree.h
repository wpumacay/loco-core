
#pragma once

#include <agent/agent_base.h>


namespace tysoc {
namespace agent {

    // The approach is to define as much functionality supported ...
    // as possible (like a variant/generic). This is a bit wasteful if ...
    // some schemas (urdf, mjcf, ...) dont have certain functionality, but ...
    // in case they try to add this, we can accomodate without losses.
    // Some structures that might or might not be in some representations are ...
    // sensors and actuators. Still, in case they don't support them, the ...
    // concrete representation class will handle initializing by default this structures.

    /**
    *   Kinematic Tree definition : the basic structure of our generic kintree is
    *                               show in the following "figure" (where the ...
    *                               letters b,j,v,c stand for body, joint, ...
    *                               visual and collision respectively, and the ...
    *                               "[x]" stand for an array of elements "x", e.g ...
    *                               [j] an array of joints)
    *
    *                        b______________     (root of kintree)
    *                       / \____  \____  \____
    *                      /       \      \      \
    *                    [b]______  [j]    [v]    [c]
    *                    / \_  \_ \__
    *                   /    \   \   \
    *                  [b]  [j]  [v] [c]
    *                 /
    *                .
    *               .
    *              .
    *
    *
    *   > Each body consists of a collections of joints, visuals, collisions and ...
    *     some other potential elements. Basically the body obj. is the node of the tree.
    *   > Joints are a bit tricky, as it means different things in different formats, ...
    *     like in urdf and mjcf. For example, in mjcf, they represent connections of ...
    *     the current body to a parent body, whereas in urdf they represent connections ...
    *     of the current body to its child bodies.
    *   > The approach to make this generic is quite simple. Just parse the given ...
    *     format into this representation, and ensure the left portion of the tree ...
    *     is the same, which means that bodies are connected to bodies.
    *   > The relative transforms between bodies are not that necessary as the world ...
    *     transforms are calculated and extracted from the specific backend, in any ...
    *     way the compute this internally.
    *   > Once we have the world transforms for each body, we then just update the ...
    *     components of the body accordingly (visuals, joints, collisions). By this ...
    *     I mean updating its world transforms given the relative transforms.
    *   > For a mimic kintree in which we want to give the pose and let it play, ...
    *     e.g from keyframes, we have to traverse the tree in the format that it ...
    *     was parsed in.
    *   > By the previous point I mean traverse it using the connections used ...
    *     by the format it was given (urdf, mjcf, etc.). The possible connections ...
    *     are represented by pointers between components. In some representations ...
    *     these are NULL, and in some they are not (see the parentJointPtr field in ...
    *     the TKinTreeBody structure).
    *   > This animation-kintree should be implemented in a different file with ...
    *     this functionality in mind (@WIP). If it hasn't just shout out in the issues ...
    *     to remind me :D.
    *   > If you have any other issues that might potentially arise from this representation ...
    *     just let me know. I'd be happy to change the design into something more robust ...
    *     that can handle as many representations and backends as possible. So far this ...
    *     is what I've come up with.
    *   > joints in the body are the actual dof, so they are the joints that connect
    *     that body to its parents
    */




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

        TMaterial()
        {
            name        = "";
            diffuse     = TYSOC_DEFAULT_DIFFUSE_COLOR;
            specular    = TYSOC_DEFAULT_SPECULAR_COLOR;
            texture     = "";
        }
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
        TSizef          friction;       // (mujoco) Friction values shape
        TScalar         density;        // (mujoco)  

        TKinTreeVisual()
        {
            parentBodyPtr   = NULL;
            // @CHECK|@WIP : Initialize variant here
            contype         = -1; // -1 indicates not available
            conaffinity     = -1; // -1 indicates not available
            condim          = -1; // -1 indicates not available
            group           = -1; // -1 indicates not available
            materialName    = "";
            density         = -1.0; // (-) indicates not available
            friction.ndim   = 0; // 0 indicates not available
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
        TVec3           comForces;
        TVec3           comTorques;

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
        TMat4                               relTransform;       // Relative transform to parent body (all)
        TKinTreeBody*                       parentBodyPtr;      // Parent body (mjcf)
        TKinTreeJoint*                      parentJointPtr;     // Parent joint (urdf)
        TKinTreeInertia*                    inertiaPtr;         // Inertia (if defined. if not, calculated by engine) (all)
        std::vector< TKinTreeCollision* >   childCollisions;    // Collisions attached to the body
        std::vector< TKinTreeVisual* >      childVisuals;       // Visuals attached to the body
        std::vector< TKinTreeJoint* >       childJoints;        // Connections to other bodies
        std::vector< TKinTreeBody* >        childBodies;        // Child bodies (all)

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
        void _resetAgentInternal() override;

        // update components
        void _updateBodyComponents( TKinTreeBody* kinTreeBodyPtr );
        void _updateJoint( TKinTreeJoint* kinTreeJointPtr );
        void _updateActuator( TKinTreeActuator* kinTreeActuatorPtr );
        void _updateSensor( TKinTreeSensor* kinTreeSensorPtr );
        void _updateVisual( TKinTreeVisual* kinTreeVisualPtr );
        void _updateCollision( TKinTreeCollision* kinTreeCollisionPtr );

        void _initializeKinTree();
        virtual void _constructKinTree() = 0;
        void _constructDefaultSensors();
        void _initializeWorldTransforms();// current transforms should represent zero/rest position
        void _initializeBody( TKinTreeBody* kinTreeBodyPtr );
        
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

        std::string toString();
        std::string _strTraverse( TKinTreeBody* kinTreeBodyPtr, size_t depth );
    };


}}
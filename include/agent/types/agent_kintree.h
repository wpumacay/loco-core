
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

        TKinTreeCollision()
        {
            parentBodyPtr = NULL;
        }
    };

    struct TKinTreeVisual
    {
        std::string     name;
        TMaterial       material;
        TGeometry       geometry;       // Geometry information
        TKinTreeBody*   parentBodyPtr;  // Parent body
        TMat4           relTransform;   // Relative transform to parent body

        TKinTreeVisual()
        {
            parentBodyPtr = NULL;
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

        TKinTreeJoint()
        {
            parentBodyPtr   = NULL;
            childBodyPtr    = NULL;
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

        TKinTreeActuator()
        {
            jointPtr = NULL;
        }
    };

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

        std::map< std::string, TKinTreeBody* >      m_mapKinTreeBodies;     // map of bodies (easy access)
        std::map< std::string, TKinTreeJoint* >     m_mapKinTreeJoints;     // map of joints (...)
        std::map< std::string, TKinTreeVisual* >    m_mapKinTreeVisuals;    // map of visuals (...)
        std::map< std::string, TKinTreeCollision* > m_mapKinTreeCollisions; // map of collisions (...)
        std::map< std::string, TKinTreeActuator* >  m_mapKinTreeActuators;  // map of actuators (...)
        std::map< std::string, TKinTreeSensor* >    m_mapKinTreeSensors;    // map of sensors (...)

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
    };


}}
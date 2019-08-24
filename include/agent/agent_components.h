
#pragma once

#include <tysoc_common.h>

namespace tysoc {
namespace agent {

    /* Keep track of how many misconfigured elements we have */
    extern size_t NUM_UNKNOWN_COLLISIONS;   // Counter for unknown collision
    extern size_t NUM_UNKNOWN_VISUALS;      // Counter for unknown visuals
    extern size_t NUM_UNKNOWN_JOINTS;       // Counter for unknown joints
    extern size_t NUM_UNKNOWN_BODIES;       // Counter for unknown bodies

    struct TKinTreeBody;
    struct TKinTreeJoint;
    struct TKinTreeVisual;
    struct TKinTreeSensor;
    struct TKinTreeActuator;

    /**
    *   Mesh asset
    *   ----------
    *
    *   Helper object used store a (name,file) pair
    */
    struct TKinTreeMeshAsset
    {
        std::string     name;
        std::string     file;
        TVec3           scale;

        TKinTreeMeshAsset();
    };

    /**
    *   Material properties
    *   -------------------
    *
    *   Defines the visual properties of a visual component.
    */
    struct TMaterial
    {
        std::string     name;       // name of the material
        std::string     texture;    // texture Id associated with the material
        TVec3           diffuse;    // diffuse color component
        TVec3           specular;   // specular color component

        TMaterial();
    };

    /**
    *   Collision component
    *   -------------------
    *
    *   Defines a collision component that can ...
    *   be added as a child of a body in a kinematic tree.
    */
    struct TKinTreeCollision
    {
        std::string     name;           // Unique name of this collision
        TKinTreeBody*   parentBodyPtr;  // Parent body of this collision
        TGeometry       geometry;       // Geometry information
        TMat4           relTransform;   // Relative transform to parent body
        TSizef          friction;       // Friction of the collider
        TScalar         density;        // Density of the collider
        int             contype;        // Contype bitmask for collision detection
        int             conaffinity;    // Conaffinity bitmask for collision detection
        int             condim;         // Contact dimensionality

        TKinTreeCollision();
    };

    /**
    *   Visual component
    *   ----------------
    *
    *   Defines a renderable component that can ...
    *   be added as a child of a body in a kinematic tree.
    */
    struct TKinTreeVisual
    {
        std::string     name;           // Unique name of this visual
        TKinTreeBody*   parentBodyPtr;  // Parent body of this visual
        TMaterial       material;       // Material properties
        TGeometry       geometry;       // Geometry information
        TMat4           relTransform;   // Relative transform to parent body

        TKinTreeVisual();
    };

    /**
    *   Sensor component
    *   ----------------
    *
    *   Defines a sensor component that stores ...
    *   information from the simulation. This can ...
    *   either be attached to a body of a joint.
    */
    struct TKinTreeSensor
    {
        std::string         name;               // Unique name of this sensor
        std::string         type;               // Type of this sensor (see sensor_base.h)
        TMat4               worldTransform;     // Reference frame in world-space
        TKinTreeBody*       bodyPtr;            // Reference to parent body (if linked to a body)
        TKinTreeJoint*      jointPtr;           // Reference to parent joint (if linked to a joint)

        TKinTreeSensor();
    };

    /**
    *   JointSensor component
    *   ---------------------
    *
    *   Defines a sensor component that attaches ...
    *   to a joint and gathers information from it.
    */
    struct TKinTreeJointSensor : public TKinTreeSensor
    {
        std::string     jointName;  // Name of the parent joint
        TScalar         theta;      // Value of the joint angle
        TScalar         thetadot;   // Value of the joint speed

        TKinTreeJointSensor();
    };

    /**
    *   BodySensor component
    *   --------------------
    *
    *   Defines a sensor component that attaches ...
    *   to a body and gathers information from it.
    */
    struct TKinTreeBodySensor : public TKinTreeSensor
    {
        std::string     bodyName;           // Name of the parent body
        TVec3           linVelocity;        // Linear velocity of the body
        TVec3           linAcceleration;    // Linear acceleration of the body
        TVec3           comForces;          // Forces at the COM of the body
        TVec3           comTorques;         // Torques at the COM of the body

        TKinTreeBodySensor();
    };

    /**
    *   Inertial properties
    *   -------------------
    *
    *   Storage for the inertial properties of a body
    */
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
    *   Body Node
    *   ---------
    *
    *   Defines a body in the kinematic tree. These serve as ...
    *   nodes in the tree which are connected appropriately, ...
    *   and the data of the whole kintree is stored in this node' ...
    *   components (colliders, joints, visuals, etc.).
    */
    struct TKinTreeBody
    {
        std::string                         name;               // Unique name of this body
        TMat4                               worldTransform;     // Reference frame in world-space
        TMat4                               relTransform;       // Relative transform to parent body
        TKinTreeBody*                       parentBodyPtr;      // Reference to the parent body
        TKinTreeInertia*                    inertiaPtr;         // Inertia (if defined. if not, calculated by engine)
        std::vector< TKinTreeCollision* >   childCollisions;    // Collisions attached to the body
        std::vector< TKinTreeVisual* >      childVisuals;       // Visuals attached to the body
        std::vector< TKinTreeJoint* >       childJoints;        // Joints attached to the body (dof)
        std::vector< TKinTreeBody* >        childBodies;        // Child bodies

        TKinTreeBody();
    };

    /**
    *   Joint component
    *   ---------------
    *
    *   Defines a joint component, which is used to create ...
    *   degrees of freedom (DOF) for its parent body.
    */
    struct TKinTreeJoint
    {
        std::string                               name;           // Unique name of this joint
        std::string                               type;           // Type of this joint (revolute,prismatic,etc.)
        TVec3                                     axis;           // Axis of this joint
        TMat4                                     worldTransform; // Reference frame in world-space
        TMat4                                     relTransform;   // Relative transform to parent body
        TKinTreeBody*                             parentBodyPtr;  // Parent body
        TScalar                                   lowerLimit;     // Lower range limit
        TScalar                                   upperLimit;     // Upper range limit
        bool                                      limited;        // Flag for joint value clamping
        TScalar                                   stiffness;      // Stiffness (spring like behaviour)
        TScalar                                   armature;       // Armature (extra diag inertia)
        TScalar                                   damping;        // Damping applied to the joint
        TScalar                                   ref;            // Ref. for the joint
        int                                       nqpos;          // Number of generalized coordinates
        int                                       nqvel;          // Number of degrees of freedom
        std::array< TScalar, TYSOC_MAX_NUM_QPOS > qpos;           // Values of the generalized coordinates
        std::array< TScalar, TYSOC_MAX_NUM_QVEL > qvel;           // Generalized speeds or the "degrees of freedom"
        std::array< TScalar, TYSOC_MAX_NUM_QPOS > qpos0;          // Initial value for the generalized coordinates
        std::array< TScalar, TYSOC_MAX_NUM_QVEL > qvel0;          // Initial value for the generalized speeds or the "degrees of freedom"

        TKinTreeJoint();

        /* configures this joint in some type-specific way */
        void configure();
    };

    /**
    *   Actuator component
    *   ---------------
    *
    *   Defines an actuator for the kinematic tree, which ...
    *   can be attached to degrees of freedom controlled by joints.
    */
    struct TKinTreeActuator
    {
        std::string         name;           // Unique name of this actuator
        std::string         type;           // Actuator type
        TMat4               worldTransform; // Reference frame in world-space
        TMat4               relTransform;   // Relative transform
        TKinTreeJoint*      jointPtr;       // Attached joint (to whom the actuator is applied)
        TScalar             ctrlValue;      // Control signal to be applied to this actuator
        TScalar             minCtrl;        // Control min limit
        TScalar             maxCtrl;        // Control max limit
        TSizef              gear;           // Scaling factor for torques (6 dim)
        TScalar             kp;             // Position feedback gain (if PD controller)
        TScalar             kv;             // Velocity feedback gain (if PD controller)
        bool                clampCtrl;      // Flag for ctrl value clamping

        TKinTreeActuator();
    };

    /* Some helper function to print these resources */
    std::string toString( TKinTreeBody* kinTreeBodyPtr );
    std::string toString( TKinTreeJoint* kinTreeJointPtr );
    std::string toString( TKinTreeCollision* kinTreeCollisionPtr );
    std::string toString( TKinTreeVisual* kinTreeVisualPtr );
    std::string toString( TKinTreeSensor* kinTreeSensorPtr );
    std::string toString( TKinTreeActuator* kinTreeActuatorPtr );

}}
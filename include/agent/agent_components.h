
#pragma once

#include <tysoc_common.h>
#include <components/data.h>

namespace tysoc {

    const TVec3 DEFAULT_AMBIENT_COLOR = { 0.7f, 0.5f, 0.3f };
    const TVec3 DEFAULT_DIFFUSE_COLOR = { 0.7f, 0.5f, 0.3f };
    const TVec3 DEFAULT_SPECULAR_COLOR = { 0.7f, 0.5f, 0.3f };
    const float DEFAULT_SHININESS = 32.0f;

    /* Keep track of how many misconfigured elements we have */
    extern size_t NUM_UNNAMED_COLLISIONS;   // Counter for unnamed collision
    extern size_t NUM_UNNAMED_VISUALS;      // Counter for unnamed visuals
    extern size_t NUM_UNNAMED_JOINTS;       // Counter for unnamed joints
    extern size_t NUM_UNNAMED_BODIES;       // Counter for unnamed bodies

    struct TKinTreeBody;
    struct TKinTreeJoint;
    struct TKinTreeVisual;
    struct TKinTreeSensor;
    struct TKinTreeActuator;

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
        TCollisionData  data;           // Information|properties of this collider
        TKinTreeBody*   parentBodyPtr;  // Parent body of this collision
        TMat4           worldTransform; // Transform in world-space

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
        TVisualData     data;           // Information|properties of this visual
        TKinTreeBody*   parentBodyPtr;  // Parent body of this visual
        TMat4           worldTransform; // Transform in world-space

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
        TSensorData         data;               // Information|properties of this sensor
        std::string         bodyName;           // unique name of the parent body (used during construction only)
        std::string         jointName;          // unique name of the parent joint (used during construction only)
        TKinTreeBody*       bodyPtr;            // Reference to parent body (if linked to a body)
        TKinTreeJoint*      jointPtr;           // Reference to parent joint (if linked to a joint)
        TMat4               worldTransform;     // Transform in world-space

        TKinTreeSensor();
        virtual ~TKinTreeSensor();
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
        TVec3           linVelocity;        // Linear velocity of the body
        TVec3           linAcceleration;    // Linear acceleration of the body
        TVec3           comForce;           // Total force at the COM of the body
        TVec3           comTorque;          // Total torque at the COM of the body

        TKinTreeBodySensor();
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
        TInertialData                       inertialData;       // Inertial data (if valid mass set. If not, calculated by engine)
        TKinTreeBody*                       parentBodyPtr;      // Reference to the parent body
        std::vector< TKinTreeCollision* >   collisions;         // Collisions attached to the body
        std::vector< TKinTreeVisual* >      visuals;            // Visuals attached to the body
        std::vector< TKinTreeJoint* >       joints;             // Joints attached to the body (dof)
        std::vector< TKinTreeBody* >        children;           // Child bodies
        TMat4                               localTransformZero;     // Relative transform (at zero-configuration) of this body w.r.t. parent body
        TMat4                               worldTransform;     // Reference frame in world-space

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
        std::string                                 name;           // Unique name of this joint
        TJointData                                  data;           // Information|properties of this joint
        std::array< TScalar, TYSOC_MAX_NUM_QPOS >   qpos;           // Values of the generalized coordinates
        std::array< TScalar, TYSOC_MAX_NUM_QVEL >   qvel;           // Generalized speeds or the "degrees of freedom"
        std::array< TScalar, TYSOC_MAX_NUM_QPOS >   qpos0;          // Initial value for the generalized coordinates
        std::array< TScalar, TYSOC_MAX_NUM_QVEL >   qvel0;          // Initial value for the generalized speeds or the "degrees of freedom"
        TKinTreeBody*                               parentBodyPtr;  // Parent body
        TMat4                                       worldTransform; // Transform in world-space

        TKinTreeJoint( const eJointType& type );
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
        TActuatorData       data;           // Information|properties of this actuator
        TScalar             ctrlValue;      // Control signal to be applied to this actuator
        TKinTreeJoint*      jointPtr;       // Attached joint (to whom the actuator is applied)
        TMat4               worldTransform; // Transform in world-space

        TKinTreeActuator();
    };

    /* Some helper function to print these resources */
    std::string toString( TKinTreeBody* kinTreeBodyPtr );
    std::string toString( TKinTreeJoint* kinTreeJointPtr );
    std::string toString( TKinTreeCollision* kinTreeCollisionPtr );
    std::string toString( TKinTreeVisual* kinTreeVisualPtr );
    std::string toString( TKinTreeSensor* kinTreeSensorPtr );
    std::string toString( TKinTreeActuator* kinTreeActuatorPtr );

}
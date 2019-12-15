#pragma once

#include <compounds/compound_body.h>
#include <adapters/compound_adapter.h>

namespace tysoc 
{
    class TCompoundBody;

    class TCompound
    {

    public :

        TCompound( const std::string& name,
                   const TVec3& position,
                   const TMat3& rotation,
                   const eDynamicsType& dyntype );

        ~TCompound();

        /**
        *   @brief Creates the root-body of this compound given user configuration
        *
        *   @details
        *    Creates a TCompoundBody as the root of the compound, given the data provided
        *    by the user. This root-body is placed according to the local transform given,
        *    w.r.t. the origin of the compound, which in turn can move through world-space.
        *
        *   @param name             Unique identifier of this body in the simulation
        *   @param bodyData         Body build information (collision, visual, inertia, etc.)
        *   @param localTransform   Relative transform of this body w.r.t. origin of the compound
        */
        TCompoundBody* createRootBody( const std::string& name,
                                       const TBodyData& bodyData,
                                       const TMat4& localTransform );

        /**
        *   @brief Creates the root-body of this compound, given simpler user configuration
        *
        *   @details
        *    Creates a TCompoundBody, similar to the method above, but without requiring the
        *    build-information structures.
        *
        *   @see tysoc::TCompound::createRootBody
        *
        *   @param name             Unique identifier of this body in the simulation
        *   @param shape            Shape of the collider to use for this body
        *   @param size             Size of the collider to use for this body
        *   @param color            Color of the drawable used for visualization
        *   @param localTransform   Relative transform of this body w.r.t. origin of the compound
        */
        TCompoundBody* createRootBody( const std::string& name,
                                       const eShapeType& shape,
                                       const TVec3& size,
                                       const TVec3& color,
                                       const TMat4& localTransform );

        /**
        *   @brief Creates a (root-body,root-joint) pair for this compound, given user configuration
        *
        *   @details
        *    Creates a TCompoundBody as the root of the compound, constraint by a given joint. Construction 
        *    is performed using the data provided by the user. This root-body is placed according to 
        *    the local transform given, w.r.t. the origin of the compound, which in turn can move 
        *    through world-space. This builder comes in handy when the root-body is required to
        *    have a constraint in the world, like in sliding doors.s
        *
        *   @param bodyName             Unique identifier of this body in the simulation
        *   @param bodyData             Body build-information (collision, visual, inertia, etc.)
        *   @param bodyLocalTransform   Relative transform of this body w.r.t. origin of the compound
        *   @param jointData            Joint build-information (type, axis, etc.)
        */
        std::pair< TCompoundBody*, TJoint* > createRootBodyJointPair( const std::string& bodyName,
                                                                      const TBodyData& bodyData,
                                                                      const TJointData& jointData,
                                                                      const TMat4& localTransform );

        /**
        *   @brief Creates a (root-body,root-joint) pair for this compound, given simpler user configuration
        *
        *   @details
        *    Similar to the previous builder-method, but it allows to pass some simpler parameters
        *    instead of building build-structs for both the body and joint.
        *
        *   @see tysoc::TCompound::createRootBody
        *
        *   @param bodyName             Unique identifier of this body in the simulation
        *   @param bodyShape            Shape of the collider to use for this body
        *   @param bodySize             Size of the collider to use for this body
        *   @param bodyColor            Color of the drawable used for visualization
        *   @param bodyLocalTransform   Relative transform of this body w.r.t. origin of the compound
        *   @param jointType            Type of joint used to constrain the root body
        *   @param jointAxis            Axis of the joint (not used if fixed or free joint)
        *   @param jointLimits          (Min,Max) limits of motion allowed by the joint constraint
        *   @param jointLocalTransform  Relative transform of this joint w.r.t its related body
        */
        std::pair< TCompoundBody*, TJoint* > createRootBodyJointPair( const std::string& bodyName,
                                                                      const eShapeType& bodyShape,
                                                                      const TVec3& bodySize,
                                                                      const TVec3& bodyColor,
                                                                      const TMat4& bodyLocalTransform,
                                                                      const eJointType& jointType,
                                                                      const TVec3& jointAxis,
                                                                      const TVec2& jointLimits,
                                                                      const TMat4& jointLocalTransform );

        /**
        *   @brief Stores body in this container (takes ownership of the body)
        *
        *   @details
        *    Stores and transfers ownership of the given body to this compound (lifetime
        *    of the body is linked to the lifetime of this compound). Returns a reference
        *    raw pointer to use.
        *
        *   @param body     Body to be stored as part of this compound
        */
        TCompoundBody* addCompoundBody( std::unique_ptr< TCompoundBody > body );

        /**
        *   @brief Recursively configures the zero|rest configuration of the compound
        *
        *   @details
        *    Configures the rest configuration of the compound by computing the world-transform
        *    recursively for each compound and each component of the compound. Note that during 
        *    simulation the position|orientation of the bodies and components of the bodies are 
        *    updated using the backend-specific adapters, so we avoid computing some recursive 
        *    DH-like traversal using the generalized coordinates and joints frames.
        */
        void initializeToRestConfiguration();

        /**
        *   @brief Sets the position of the whole compound in a given point in world-space
        *
        *   @details
        *    Changes the position in world-space of the compound by translating all bodies
        *    by the same distance. The compound adapter must handle moving the compound
        *    according to the implementation available. Some examples of possible implementations
        *    are:
        *       - For Raisim, uses setBasePos to move the base of the articulated system
        *       - For MuJoCo, uses generalized coordinates (qpos) directly
        *       - For Bullet, uses setWorldTransform directly (if using maximal coordinates), or
        *                     setBasePos (if using minimal coordinates)
        *
        *   @param position     Position in world-space to place the whole compound
        */
        void setPosition( const TVec3& position );

        /**
        *   @brief Sets the orientation of the whole compound in world-space
        *
        *   @details
        *    Changes the orientation in world-space of the compound by rotating all bodies
        *    by the same amount. The compound adapter must handle moving the compound
        *    according to the implementation available. Some examples of possible implementations
        *    are:
        *       - For Raisim, uses setBaseOrientation to rotate the base of the articulated system
        *       - For MuJoCo, uses generalized coordinates (qpos) directly
        *       - For Bullet, uses setWorldTransform directly (if using maximal coordinates), or
        *                     setBasePos (if using minimal coordinates)
        *
        *   @param rotation     Orientation given by a rotation matrix in world-space
        */
        void setRotation( const TMat3& rotation );

        /**
        *   @brief Sets the world transform of the whole compound in world-space
        *
        *   @details
        *    Changes the transform in world-space of the compound by placing the compound (base) 
        *    frame in the given world-transform. The compound adapter must handle moving the compound
        *    according to the implementation available. The implementation combines the previous
        *    methods above to set both the position and orientation required by the given transform.
        *
        *   @param transform    Pose of the compound frame given by a transformation matrix in world-space
        */
        void setTransform( const TMat4& transform );

        /**
        *   @brief Gets a reference to a body in the compound, given the name of the body
        *
        *   @details
        *    Grabs and returns a reference to a body requested by the user. If not found, it
        *    returns nullptr instead. Ownership is maintained by the compound, so the references
        *    obtained should not be deleted by the user.
        *
        *   @return     Compound body with given name (nullptr if not found)
        */
        TCompoundBody* getBodyByName( const std::string& name );

        /**
        *   @brief Gets all body-references in the compound.
        *
        *   @details
        *    Grabs and returns all references to all bodies contained in this compound. Ownership is
        *    still maintained by the compound, so the references ontained should not be deleted by
        *    the user.
        *
        *   @return     Vector of body references
        */
        std::vector< TCompoundBody* > bodies();

        TVec3 position() const { return m_tf.getPosition(); }

        TMat3 rotation() const { return m_tf.getRotation(); }

        TMat4 transform() const { return m_tf; }

        std::string name() const { return m_name; }

        eDynamicsType dyntype() const { return m_dyntype; }

        TCompoundBody* root() const { return m_rootBodyRef; }

    private :

        /**
        *   @brief Updates the current configuration of the components of the compound in a recursive fashion
        *
        *   @details
        *    Recursively update the current world-transform of bodies in the compound, as well as their
        *    components, by using the current local-transforms and parent world-transforms appropriately.
        *    Use this method to compute zero|rest configurations, as well as configurations of the
        *    compound when the body is moved via setPosition, setRotation, etc. when the simulation
        *    has no control over the compound (no adapters yet, or adapters are inactive due to paused
        *    simulation)
        *
        *   @see tysoc::TCompound::computeRestConfiguration
        *   @see tysoc::TCompound::setPosition
        *   @see tysoc::TCompound::setRotation
        *   @see tysoc::TCompound::setTransform
        *
        *   @param body             Compound body to be updated using its current local-transform
        *   @param parentWorldTf    World-transform of the parent of this compound (if root, tf of the whole compound)
        */
        void _updateCurrentConfigurationRecursive( TCompoundBody* body, const TMat4& parentWorldTf );

    private :

        /* unique identifier for this compound in the simulation */
        std::string m_name;

        /* current world-transform of the whole compound */
        TMat4 m_tf;

        /* initial world-transform of the whole compound */
        TMat4 m_tf0;

        /* indicator of whether the compound is free to move, or fixed w.r.t. the world */
        eDynamicsType m_dyntype;

        /* reference to the root-body of this compound */
        TCompoundBody* m_rootBodyRef;

        /* Adapter reference that gives access to the low-level API for a specific backend (reference only, as simulation owns it)  */
        TICompoundAdapter* m_compoundImplRef;

        /* Container for all bodies that belong to this compound */
        std::vector< std::unique_ptr< TCompoundBody > > m_bodies;

        /* Dictionary with reference to all bodies in simulation */
        std::unordered_map< std::string, TCompoundBody* > m_bodiesMap;
    };

}
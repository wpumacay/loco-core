#pragma once

#include <tysoc_common.h>

#include <components/data.h>
#include <components/collision.h>
#include <components/visual.h>

#include <adapters/body_adapter.h>

namespace tysoc {

    class TCollision;
    class TVisual;
    class TIBodyAdapter;

    /**
    *   Body object interface, defining base functionality for any body-type
    *
    *   This class provides an interface|specification of the minimum functionality
    *   and resources required by bodies in the framework, namely the following three 
    *   types (which implement this interface):
    *
    *       > Single-body: defines bodies that for primitives that can lay around in
    *                      the scenario. These are used as obstacles, simple parts of
    *                      terrain, and primitives for terrain generation.
    *       > Compound-body: defines bodies that can be connected to form chains (compounds) by
    *                        using joints to link them, whose degrees of freedom can be set 
    *                        according to the user. Use these to create doors, conveyors, etc.
    *       > Kintree-body: defines bodies that are used to create agents (kinematic trees) used
    *                       for locomotion tasks. This comes in handy to separate some internal
    *                       backend APIs that provide both Minimal Coordinates (usually through a 
    *                       Featherstone implementation) and Maximal Coordinates (usually using
    *                       recursive Newton-Euler).
    *
    *   Notes:
    *       - We're still unsure about the way we're handling this hierarchy is the correct way
    *         to do the job. So far, all functionality is baked into this single interface, and
    *         disabled|extended according to the class-type used.
    *       - We're open to suggestions, this seems to work, but it's not elegant to just place
    *         everything under the same interface, even though some objects don't actually fully
    *         use nor expose this functionality
    */
    class TIBody
    {

    public :
        
        TIBody( const std::string& name,
                const TBodyData& data );

        virtual ~TIBody();

        void setAdapter( TIBodyAdapter* bodyImplRef );

        void setCollision( std::unique_ptr< TCollision > collisionObj );

        void setVisual( std::unique_ptr< TVisual > visualObj );

        virtual void update();

        virtual void reset();

        /*******************************************************************************
        * World-space setters: use these to set the world-space pose of the body in 
        * question. Implementation varies among body types in the following way:
        *
        *   > single-body: works normally (sets world pose)
        *   > compound-body: works normally if root-body, and does nothing otherwise
        *   > kintree-body: disabled, to allow access only through kintree-api
        *******************************************************************************/

        virtual void setPosition( const TVec3& position );

        virtual void setRotation( const TMat3& rotation );

        virtual void setEuler( const TVec3& euler );

        virtual void setQuaternion( const TVec4& quat );

        virtual void setTransform( const TMat4& transform );

        /*******************************************************************************
        * Local-space setters: use these to set the relative pose of the body in question 
        * w.r.t. a parent body. Implementation varies among body types in the following way:
        *
        *   > single-body: disabled, as single primitives have no parent
        *   > compound-body: works normally (sets local pose w.r.t. parent body)
        *   > kintree-body: disabled, to allow access only through kintree-api
        *******************************************************************************/

        virtual void setLocalPosition( const TVec3& localPosition );

        virtual void setLocalRotation( const TMat3& localRotation );

        virtual void setLocalEuler( const TVec3& localEuler );

        virtual void setLocalQuaternion( const TVec4& localQuat );

        virtual void setLocalTransform( const TMat4& localTransform );

        /*************************************************************
        *                      World-space getters                   *
        *************************************************************/

        TVec3 pos() const { return m_pos; }

        TMat3 rot() const { return m_rot; }

        TVec4 quat() const { return m_tf.getRotQuaternion(); }

        TVec3 euler() const { return m_tf.getRotEuler(); }

        TMat4 tf() const { return m_tf; }

        TVec3 pos0() const { return m_pos0; }

        TMat3 rot0() const { return m_rot0; }

        TVec4 quat0() const { return m_tf0.getRotQuaternion(); }

        TVec3 euler0() const { return m_tf0.getRotEuler(); }

        TMat4 tf0() const { return m_tf0; }

        /*************************************************************
        *                      Local-space getters                   *
        *************************************************************/

        TVec3 localPos() const { return m_localPos; }

        TMat3 localRot() const { return m_localRot; }

        TVec4 localQuat() const { return m_localTf.getRotQuaternion(); }

        TVec3 localEuler() const { return m_localTf.getRotEuler(); }

        TMat4 localTf() const { return m_localTf; }

        TVec3 localPos0() const { return m_localPos0; }

        TMat3 localRot0() const { return m_localRot0; }

        TVec4 localQuat0() const { return m_localTf0.getRotQuaternion(); }

        TVec3 localEuler0() const { return m_localTf0.getRotEuler(); }

        TMat4 localTf0() const { return m_localTf0; }

        std::string name() { return m_name; }

        eBodyClassType classType() { return m_classType; }

        TVisual* visual() { return m_visual.get(); }

        TCollision* collision() { return m_collision.get(); }

        TIBodyAdapter* adapter() { return m_bodyImplRef; }

        TBodyData data() const { return m_data; }

        TBodyData& dataRef() { return m_data; }

        eDynamicsType dyntype() const { return m_data.dyntype; }

    protected :

        /* unique name identifier */
        std::string m_name;

        /* type of body (single|compound|kintree) */
        eBodyClassType m_classType;

        /* initial and current position in world-space */
        TVec3 m_pos0;
        TVec3 m_pos;
        /* initial and current orientation in world-space */
        TMat3 m_rot0;
        TMat3 m_rot; 
        /* initial and current transform in world-space */
        TMat4 m_tf0;
        TMat4 m_tf;

        /* initial and current position in local-space */
        TVec3 m_localPos0;
        TVec3 m_localPos;
        /* initial and current orientation in local-space */
        TMat3 m_localRot0;
        TMat3 m_localRot; 
        /* initial and current transform in local-space */
        TMat4 m_localTf0;
        TMat4 m_localTf;

        /* properties of this object */
        TBodyData m_data;

        /* Adapter-object that gives access to the low-level API for a specific backend (reference only, as simulation owns it) */
        TIBodyAdapter* m_bodyImplRef;

        /* Single collider (collision object) of this body */
        std::unique_ptr< TCollision > m_collision;

        /* Single visual (viewer object) of this body */
        std::unique_ptr< TVisual > m_visual;
    };

}

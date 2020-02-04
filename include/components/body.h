#pragma once

#include <loco_common.h>

#include <components/data.h>
#include <components/collision.h>
#include <components/visual.h>

#include <adapters/body_adapter.h>

namespace loco
{
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

        void preStep();

        void postStep();

        void reset();

        //------------------------------------------------------------------------------------------
        // World-space setters: use these to set the world-space pose of the body in 
        // question. Implementation varies among body types in the following way:
        //
        //   > single-body: works normally (sets world pose)
        //   > compound-body: works normally if root-body, and does nothing otherwise
        //   > kintree-body: disabled, to allow access only through kintree-api
        //------------------------------------------------------------------------------------------

        void setPosition( const TVec3& position );

        void setRotation( const TMat3& rotation );

        void setEuler( const TVec3& euler );

        void setQuaternion( const TVec4& quat );

        void setTransform( const TMat4& transform );

        //------------------------------------------------------------------------------------------
        // Local-space setters: use these to set the relative pose of the body in question 
        // w.r.t. a parent body. Implementation varies among body types in the following way:
        //
        //   > single-body: disabled, as single primitives have no parent
        //   > compound-body: works normally (sets local pose w.r.t. parent body)
        //   > kintree-body: disabled, to allow access only through kintree-api
        //------------------------------------------------------------------------------------------

        void setLocalPosition( const TVec3& localPosition );

        void setLocalRotation( const TMat3& localRotation );

        void setLocalEuler( const TVec3& localEuler );

        void setLocalQuaternion( const TVec4& localQuat );

        void setLocalTransform( const TMat4& localTransform );

        //------------------------------------------------------------------------------------------
        //                                 World-space getters
        //------------------------------------------------------------------------------------------

        TVec3 pos() const { return TVec3( m_tf.col( 3 ) ); }

        TMat3 rot() const { return TMat3( m_tf ); }

        TVec4 quat() const { return tinymath::quaternion( m_tf ); }

        TVec3 euler() const { return tinymath::euler( m_tf ); }

        TMat4 tf() const { return m_tf; }

        TVec3 pos0() const { return TVec3( m_tf0.col( 3 ) ); }

        TMat3 rot0() const { return TMat3( m_tf0 ); }

        TVec4 quat0() const { return tinymath::quaternion( m_tf0 ); }

        TVec3 euler0() const { return tinymath::euler( m_tf0 ); }

        TMat4 tf0() const { return m_tf0; }

        //------------------------------------------------------------------------------------------
        //                                Local-space getters
        //------------------------------------------------------------------------------------------

        TVec3 localPos() const { return TVec3( m_localTf.col( 3 ) ); }

        TMat3 localRot() const { return TMat3( m_localTf ); }

        TVec4 localQuat() const { return tinymath::quaternion( m_localTf ); }

        TVec3 localEuler() const { return tinymath::euler( m_localTf ); }

        TMat4 localTf() const { return m_localTf; }

        TVec3 localPos0() const { return TVec3( m_localTf0.col( 3 ) ); }

        TMat3 localRot0() const { return TMat3( m_localTf0 ); }

        TVec4 localQuat0() const { return tinymath::quaternion( m_localTf0 ); }

        TVec3 localEuler0() const { return tinymath::euler( m_localTf0 ); }

        TMat4 localTf0() const { return m_localTf0; }

        //------------------------------------------------------------------------------------------
        //                               Read-only properties
        //------------------------------------------------------------------------------------------

        std::string name() const { return m_name; }

        eBodyClassType classType() const { return m_classType; }

        eDynamicsType dyntype() const { return m_data.dyntype; }

        TVisual* visual() { return m_visual.get(); }

        const TVisual* visual() const { return m_visual.get(); }

        TCollision* collision() { return m_collision.get(); }

        const TCollision* collision() const { return m_collision.get(); }

        TIBodyAdapter* adapter() { return m_bodyImplRef; }

        TBodyData data() const { return m_data; }

        TBodyData& dataRef() { return m_data; }

        const TBodyData& dataRef() const { return m_data; }

    protected :

        virtual void _preStepInternal() = 0;

        virtual void _postStepInternal() = 0;

        virtual void _resetInternal() = 0;

        virtual void _setPositionInternal( const TVec3& position ) = 0;

        virtual void _setRotationInternal( const TMat3& rotation ) = 0;

        virtual void _setEulerInternal( const TVec3& euler ) = 0;

        virtual void _setQuaternionInternal( const TVec4& quat ) = 0;

        virtual void _setTransformInternal( const TMat4& transform ) = 0;

        virtual void _setLocalPositionInternal( const TVec3& localPosition ) = 0;

        virtual void _setLocalRotationInternal( const TMat3& localRotation ) = 0;

        virtual void _setLocalEulerInternal( const TVec3& localEuler ) = 0;

        virtual void _setLocalQuaternionInternal( const TVec4& localQuat ) = 0;

        virtual void _setLocalTransformInternal( const TMat4& localTransform ) = 0;

    protected :

        /// Unique name identifier
        std::string m_name;
        /// Type of body (single|compound|kintree)
        eBodyClassType m_classType;
        /// Transform in world-space
        TMat4 m_tf;
        /// Initial transform in world-space
        TMat4 m_tf0;
        /// Relative transform w.r.t. parent (if applicable)
        TMat4 m_localTf;
        /// Initial relative transform w.r.t. parent (if applicable)
        TMat4 m_localTf0;
        /// Properties(struct) of this body
        TBodyData m_data;
        /// Adapter-object that gives access to the low-level API for a specific backend
        TIBodyAdapter* m_bodyImplRef;
        /// Single collider (collision object) associated with this body
        std::unique_ptr< TCollision > m_collision;
        /// Single visual (viewer object) associated with this body
        std::unique_ptr< TVisual > m_visual;
    };

}

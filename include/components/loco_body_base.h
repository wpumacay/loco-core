#pragma once

#include <loco_common.h>

#include <components/loco_data.h>
#include <components/loco_collision.h>
#include <components/loco_visual.h>
#include <adapters/loco_body_adapter.h>

namespace loco
{
    class TCollision;
    class TVisual;
    class TIBodyAdapter;

    /// @brief Body object interface, defining the common API for all body-types
    ///
    /// @details
    /// This class provides an interface|specification of the minimum functionality
    /// and resources required by bodies in the framework, namely the following three 
    /// types (which implement this interface):
    ///
    ///    > Single-body: defines bodies that for primitives that can lay around in
    ///                   the scenario. These are used as obstacles, simple parts of
    ///                   terrain, and primitives for terrain generation.
    ///    > Compound-body: defines bodies that can be connected to form chains (compounds) by
    ///                     using joints to link them, whose degrees of freedom can be set 
    ///                     according to the user. Use these to create doors, conveyors, etc.
    ///    > Kintree-body: defines bodies that are used to create agents (kinematic trees) used
    ///                    for locomotion tasks. This comes in handy to separate some internal
    ///                    backend APIs that provide both Minimal Coordinates (usually through a 
    ///                    Featherstone implementation) and Maximal Coordinates (usually using
    ///                    recursive Newton-Euler).
    class TIBody
    {

    public :
        
        TIBody( const std::string& name,
                const TBodyData& data );

        virtual ~TIBody();

        void SetAdapter( TIBodyAdapter* bodyImplRef );

        void SetCollision( std::unique_ptr< TCollision > collisionObj );

        void SetVisual( std::unique_ptr< TVisual > visualObj );

        void Initialize();

        void PreStep();

        void PostStep();

        void Reset();

        //------------------------------------------------------------------------------------------
        // World-space setters: use these to set the world-space pose of the body in 
        // question. Implementation varies among body types in the following way:
        //
        //   > single-body: works normally (sets world pose)
        //   > compound-body: works normally if root-body, and does nothing otherwise
        //   > kintree-body: disabled, to allow access only through kintree-api
        //------------------------------------------------------------------------------------------

        void SetPosition( const TVec3& position );

        void SetRotation( const TMat3& rotation );

        void SetEuler( const TVec3& euler );

        void SetQuaternion( const TVec4& quat );

        void SetTransform( const TMat4& transform );

        //------------------------------------------------------------------------------------------
        // Local-space setters: use these to set the relative pose of the body in question 
        // w.r.t. a parent body. Implementation varies among body types in the following way:
        //
        //   > single-body: disabled, as single primitives have no parent
        //   > compound-body: works normally (sets local pose w.r.t. parent body)
        //   > kintree-body: disabled, to allow access only through kintree-api
        //------------------------------------------------------------------------------------------

        void SetLocalPosition( const TVec3& localPosition );

        void SetLocalRotation( const TMat3& localRotation );

        void SetLocalEuler( const TVec3& localEuler );

        void SetLocalQuaternion( const TVec4& localQuat );

        void SetLocalTransform( const TMat4& localTransform );

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

        virtual void _InitializeInternal() = 0;

        virtual void _PreStepInternal() = 0;

        virtual void _PostStepInternal() = 0;

        virtual void _ResetInternal() = 0;

        virtual void _SetPositionInternal( const TVec3& position ) = 0;

        virtual void _SetRotationInternal( const TMat3& rotation ) = 0;

        virtual void _SetEulerInternal( const TVec3& euler ) = 0;

        virtual void _SetQuaternionInternal( const TVec4& quat ) = 0;

        virtual void _SetTransformInternal( const TMat4& transform ) = 0;

        virtual void _SetLocalPositionInternal( const TVec3& localPosition ) = 0;

        virtual void _SetLocalRotationInternal( const TMat3& localRotation ) = 0;

        virtual void _SetLocalEulerInternal( const TVec3& localEuler ) = 0;

        virtual void _SetLocalQuaternionInternal( const TVec4& localQuat ) = 0;

        virtual void _SetLocalTransformInternal( const TMat4& localTransform ) = 0;

    protected :

        // Unique name identifier
        std::string m_name;
        // Type of body (single|compound|kintree)
        eBodyClassType m_classType;
        // Transform in world-space
        TMat4 m_tf;
        // Initial transform in world-space
        TMat4 m_tf0;
        // Relative transform w.r.t. parent (if applicable)
        TMat4 m_localTf;
        // Initial relative transform w.r.t. parent (if applicable)
        TMat4 m_localTf0;
        // Properties(struct) of this body
        TBodyData m_data;
        // Adapter-object that gives access to the low-level API for a specific backend
        TIBodyAdapter* m_bodyImplRef;
        // Single collider (collision object) associated with this body
        std::unique_ptr< TCollision > m_collision;
        // Single visual (viewer object) associated with this body
        std::unique_ptr< TVisual > m_visual;
    };
}

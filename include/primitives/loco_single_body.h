#pragma once

#include <loco_data.h>
#include <visualizer/loco_visualizer_drawable.h>
#include <primitives/loco_single_body_collider.h>
#include <primitives/loco_single_body_adapter.h>

namespace loco
{
    class TDrawable;
    class TSingleBodyCollider;
    class TISingleBodyAdapter;

    class TSingleBody : public TObject
    {
    public :

        TSingleBody( const std::string& name,
                     const TBodyData& data,
                     const TVec3& position,
                     const TMat3& rotation );

        ~TSingleBody();

        void SetBodyAdapter( TISingleBodyAdapter* body_adapter_ref );

        void SetCollider( std::unique_ptr<TSingleBodyCollider> collider );

        void SetDrawable( std::unique_ptr<TDrawable> drawable );

        void SetLinearVelocity( const TVec3& linear_vel );

        void SetAngularVelocity( const TVec3& angular_vel );

        void AddForceCOM( const TVec3& force );

        void AddTorqueCOM( const TVec3& torque );

        void SetInitialPosition( const TVec3& position0 ) { m_tf0.set( position0, 3 ); }

        void SetInitialRotation( const TMat3& rotation0 ) { m_tf0.set( rotation0 ); }

        void SetInitialEuler( const TVec3& euler0 ) { m_tf0.set( tinymath::rotation( euler0 ) ); }

        void SetInitialQuaternion( const TVec4& quaternion0 ) { m_tf0.set( tinymath::rotation( quaternion0 ) ); }

        void SetInitialLinearVelocity( const TVec3& linear_vel0 ) { m_LinearVelocity0 = linear_vel0; }

        void SetInitialAngularVelocity( const TVec3& angular_vel0 ) { m_AngularVelocity0 = angular_vel0; }

        TVec3 linear_vel() const { return m_LinearVelocity; }

        TVec3 angular_vel() const { return m_AngularVelocity; }

        TVec3 linear_vel0() const { return m_LinearVelocity0; }

        TVec3 angular_vel0() const { return m_AngularVelocity0; }

        TVec3 total_force_com() const { return m_TotalForce; }

        TVec3 total_torque_com() const { return m_TotalTorque; }

        TVec3 pos0() const { return TVec3( m_tf0.col( 3 ) ); }

        TMat3 rot0() const { return TMat3( m_tf0 ); }

        TVec3 euler0() const { return tinymath::euler( m_tf0 ); }

        TVec4 quat0() const { return tinymath::quaternion( m_tf0 ); }

        TMat4 tf0() const { return m_tf0; }

        TBodyData& data() { return m_Data; }

        const TBodyData& data() const { return m_Data; }

        eDynamicsType dyntype() const { return m_Data.dyntype; }

        eObjectType GetObjectType() const override { return eObjectType::SINGLE_BODY; }

        static eObjectType GetStaticType() { return eObjectType::SINGLE_BODY; }

        TSingleBodyCollider* collider() { return m_Collider.get(); }

        const TSingleBodyCollider* collider() const { return m_Collider.get(); }

        TDrawable* drawable() { return m_Drawable.get(); }

        const TDrawable* drawable() const { return m_Drawable.get(); }

        TISingleBodyAdapter* adapter() { return m_BodyAdapter; }

        const TISingleBodyAdapter* adapter() const { return m_BodyAdapter; }

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;

        void _DetachSimInternal() override;

        void _DetachVizInternal() override;

        void _SetTransformInternal( const TMat4& transform ) override;

    private :

        // Body properties (construction info)
        TBodyData m_Data;
        // Adapter-object that gives access to the low-level API for a specific backend
        TISingleBodyAdapter* m_BodyAdapter;
        // Single collider (collider object) associated with this body
        std::unique_ptr< TSingleBodyCollider > m_Collider;
        // Single drawable (viewer object) associated with this body
        std::unique_ptr< TDrawable > m_Drawable;
        // Initial world-transform of the single-body on the scenario
        TMat4 m_tf0;
        // Total force to be applied to the COM of this body
        TVec3 m_TotalForce;
        // Total torque to be applied to the COM of this body
        TVec3 m_TotalTorque;
        // Current linear velocity
        TVec3 m_LinearVelocity;
        // Current angular velocity
        TVec3 m_AngularVelocity;
        // Initial linear velocity
        TVec3 m_LinearVelocity0;
        // Initial angular velocity
        TVec3 m_AngularVelocity0;
    };
}
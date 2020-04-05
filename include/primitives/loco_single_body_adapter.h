#pragma once

#include <primitives/loco_single_body.h>
#include <primitives/loco_single_body_collider_adapter.h>
#include <primitives/loco_single_body_constraint_adapter.h>

namespace loco
{
    class TSingleBody;
    class TISingleBodyColliderAdapter;
    class TISingleBodyConstraintAdapter;

    class TISingleBodyAdapter
    {
    public :

        TISingleBodyAdapter( TSingleBody* body_ref )
            : m_BodyRef( body_ref ), m_Detached( false ), m_ColliderAdapter( nullptr ) {}

        virtual ~TISingleBodyAdapter() { m_BodyRef = nullptr; }

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void Reset() = 0;

        virtual void OnDetach() = 0;

        virtual void SetTransform( const TMat4& transform ) = 0;

        virtual void SetLinearVelocity( const TVec3& linear_vel ) = 0;

        virtual void SetAngularVelocity( const TVec3& angular_vel ) = 0;

        virtual void SetForceCOM( const TVec3& force ) = 0;

        virtual void SetTorqueCOM( const TVec3& torque ) = 0;

        virtual void GetTransform( TMat4& dst_transform ) = 0;

        virtual void GetLinearVelocity( TVec3& dst_linear_vel ) = 0;

        virtual void GetAngularVelocity( TVec3& dst_angular_vel ) = 0;

        TSingleBody* single_body() { return m_BodyRef; }

        const TSingleBody* single_body() const { return m_BodyRef; }

        TISingleBodyColliderAdapter* collider_adapter() { return m_ColliderAdapter.get(); }

        const TISingleBodyColliderAdapter* collider_adapter() const { return m_ColliderAdapter.get(); }

        TISingleBodyConstraintAdapter* constraint_adapter() { return m_ConstraintAdapter.get(); }

        const TISingleBodyConstraintAdapter* constraint_adapter() const { return m_ConstraintAdapter.get(); }

        bool detached() const { return m_Detached; }

    protected :

        // Reference to owner body-obj
        TSingleBody* m_BodyRef;
        // Flag used to check resource state
        bool m_Detached;
        // Adapter for related collider object
        std::unique_ptr<TISingleBodyColliderAdapter> m_ColliderAdapter;
        // Adapter for related constraint object
        std::unique_ptr<TISingleBodyConstraintAdapter> m_ConstraintAdapter;
    };
}
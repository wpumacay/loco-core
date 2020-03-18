#pragma once

#include <primitives/loco_single_body.h>

namespace loco
{
    class TSingleBody;

    class TISingleBodyAdapter
    {
    public :

        TISingleBodyAdapter( TSingleBody* body_ref )
            : m_BodyRef( body_ref ) {}

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

    protected :

        // Reference to ownerr body-obj
        TSingleBody* m_BodyRef;
    };
}
#pragma once

#include <primitives/loco_single_body_collider.h>

namespace loco
{
    class TSingleBodyCollider;

    class TISingleBodyColliderAdapter
    {
    public :

        TISingleBodyColliderAdapter( TSingleBodyCollider* collider_ref );

        virtual ~TISingleBodyColliderAdapter();

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void OnDetach();

        virtual void ChangeSize( const TVec3& new_size ) = 0;

        virtual void ChangeElevationData( const std::vector<float>& heights ) = 0;

        virtual void ChangeCollisionGroup( int collision_group ) = 0;

        virtual void ChangeCollisionMask( int collision_mask ) = 0;

        TSingleBodyCollider* collider() { return m_ColliderRef; }

        const TSingleBodyCollider* collider() const { return m_ColliderRef; }

        bool detached() const { return m_Detached; }

    protected :

        // Reference to owner collider
        TSingleBodyCollider* m_ColliderRef;
        // Flag used to check resource state
        bool m_Detached;
    };
}
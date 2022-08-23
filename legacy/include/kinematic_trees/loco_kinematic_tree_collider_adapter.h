#pragma once

#include <kinematic_trees/loco_kinematic_tree_collider.h>

namespace loco {
namespace kintree {

    class TKinematicTreeCollider;

    class TIKinematicTreeColliderAdapter
    {
    public :

        TIKinematicTreeColliderAdapter( TKinematicTreeCollider* collider_ref );

        virtual ~TIKinematicTreeColliderAdapter();

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void OnDetach();

        virtual void SetLocalTransform( const TMat4& local_tf ) = 0;

        virtual void ChangeSize( const TVec3& new_size ) = 0;

        virtual void ChangeCollisionGroup( int collision_group ) = 0;

        virtual void ChangeCollisionMask( int collision_mask ) = 0;

        virtual void ChangeFriction( const TScalar& friction ) = 0;

        TKinematicTreeCollider* collider() { return m_ColliderRef; }

        const TKinematicTreeCollider* collider() const { return m_ColliderRef; }

        bool detached() const { return m_Detached; }

    protected :

        /// Reference to the kintree-collider that handles this adapter
        TKinematicTreeCollider* m_ColliderRef = nullptr;
        /// Flags used to check resource state (either linked to simulation or not)
        bool m_Detached = false;
    };

}}
#pragma once

#include <kinematic_trees/loco_kinematic_tree_body.h>
#include <kinematic_trees/loco_kinematic_tree_collider_adapter.h>
#include <kinematic_trees/loco_kinematic_tree_joint_adapter.h>

namespace loco {
namespace kintree {

    class TKinematicTreeBody;
    class TIKinematicTreeColliderAdapter;
    class TIKinematicTreeJointAdapter;

    class TIKinematicTreeBodyAdapter
    {
    public :

        TIKinematicTreeBodyAdapter( TKinematicTreeBody* body_ref );

        virtual ~TIKinematicTreeBodyAdapter();

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void Reset() = 0;

        virtual void OnDetach();

        virtual void SetForceCOM( const TVec3& force ) = 0;

        virtual void SetTorqueCOM( const TVec3& torque ) = 0;

        virtual void GetTransform( TMat4& dst_transform ) = 0;

        TKinematicTreeBody* body() { return m_BodyRef; }

        const TKinematicTreeBody* body() const { return m_BodyRef; }

        std::vector<TIKinematicTreeColliderAdapter*> colliders_adapters();

        std::vector<const TIKinematicTreeColliderAdapter*> colliders_adapters() const;

        std::vector<TIKinematicTreeJointAdapter*> joints_adapters();

        std::vector<const TIKinematicTreeJointAdapter*> joints_adapters() const;

        bool detached() const { return m_Detached; }

    protected :

        /// Reference to the kintree-body that handles this adapter
        TKinematicTreeBody* m_BodyRef = nullptr;
        /// Flags used to check resource state (either linked to simulation or not)
        bool m_Detached = false;
        /// Adapter for related collider object
        std::vector<std::unique_ptr<TIKinematicTreeColliderAdapter>> m_CollidersAdapters;
        /// Adapter for related joint object
        std::vector<std::unique_ptr<TIKinematicTreeJointAdapter>> m_JointsAdapters;
    };
}}
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

        TIKinematicTreeColliderAdapter* collider_adapter() { return m_ColliderAdapter.get(); }

        const TIKinematicTreeColliderAdapter* collider_adapter() const { return m_ColliderAdapter.get(); }

        TIKinematicTreeJointAdapter* joint_adapter() { return m_JointAdapter.get(); }

        const TIKinematicTreeJointAdapter* joint_adapter() const { return m_JointAdapter.get(); };

        bool detached() const { return m_Detached; }

    protected :

        /// Reference to the kintree-body that handles this adapter
        TKinematicTreeBody* m_BodyRef = nullptr;
        /// Flags used to check resource state (either linked to simulation or not)
        bool m_Detached = false;
        /// Adapter for related collider object
        std::unique_ptr<TIKinematicTreeColliderAdapter> m_ColliderAdapter = nullptr;
        /// Adapter for related joint object
        std::unique_ptr<TIKinematicTreeJointAdapter> m_JointAdapter = nullptr;
    };
}}
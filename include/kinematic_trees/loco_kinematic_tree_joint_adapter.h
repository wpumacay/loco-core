#pragma once

#include <kinematic_trees/loco_kinematic_tree_joint.h>

namespace loco {
namespace kintree {

    class TKinematicTreeJoint;

    class TIKinematicTreeJointAdapter
    {
    public :

        TIKinematicTreeJointAdapter( TKinematicTreeJoint* joint_ref );

        virtual ~TIKinematicTreeJointAdapter();

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void OnDetach();

        virtual void Reset() = 0;

        virtual void SetQpos( const std::vector<TScalar>& qpos ) = 0;

        virtual void SetQvel( const std::vector<TScalar>& qvel ) = 0;

        virtual void SetLocalTransform( const TMat4& local_tf ) = 0;

        virtual void ChangeStiffness( const TScalar& stiffness ) = 0;

        virtual void ChangeArmature( const TScalar& armature ) = 0;

        virtual void ChangeAxis( const TVec3& axis ) = 0;

        virtual void ChangeLimits( const TVec2& limits ) = 0;

        virtual void GetQpos( std::vector<TScalar>& dst_qpos ) = 0;

        virtual void GetQvel( std::vector<TScalar>& dst_qvel ) = 0;

        TKinematicTreeJoint* joint() { return m_JointRef; }

        const TKinematicTreeJoint* joint() const { return m_JointRef; }

        bool detached() const { return m_Detached; }

    protected :

        /// Reference to the kintree-joint that handles this adapter
        TKinematicTreeJoint* m_JointRef = nullptr;
        /// Flags used to check resource state (either linked to simulation or not)
        bool m_Detached = false;
    };
}}

#include <kinematic_trees/loco_kinematic_tree_joint_adapter.h>

namespace loco {
namespace kintree {

    TIKinematicTreeJointAdapter::TIKinematicTreeJointAdapter( TKinematicTreeJoint* joint_ref )
    {
        m_JointRef = joint_ref;
    }

    TIKinematicTreeJointAdapter::~TIKinematicTreeJointAdapter()
    {
        if ( m_JointRef )
            m_JointRef->DetachSim();
        m_JointRef = nullptr;
    }

    void TIKinematicTreeJointAdapter::OnDetach()
    {
        if ( m_JointRef )
            m_JointRef->DetachSim();
        m_JointRef = nullptr;
    }
}}
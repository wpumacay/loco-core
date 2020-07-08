
#include <kinematic_trees/loco_kinematic_tree_body_adapter.h>

namespace loco {
namespace kintree {

    TIKinematicTreeBodyAdapter::TIKinematicTreeBodyAdapter( TKinematicTreeBody* body_ref )
    {
        m_BodyRef = body_ref;
    }

    TIKinematicTreeBodyAdapter::~TIKinematicTreeBodyAdapter()
    {
        if ( m_BodyRef )
            m_BodyRef->DetachSim();
        m_BodyRef = nullptr;

        m_CollidersAdapters.clear();
        m_JointsAdapters.clear();
    }

    void TIKinematicTreeBodyAdapter::OnDetach()
    {
        m_Detached = true;
        m_BodyRef = nullptr;
    }

    std::vector<TIKinematicTreeColliderAdapter*> TIKinematicTreeBodyAdapter::colliders_adapters()
    {
        std::vector<TIKinematicTreeColliderAdapter*> vec_colliders_adapters;
        for ( auto& collider_adapter : m_CollidersAdapters )
            vec_colliders_adapters.push_back( collider_adapter.get() );
        return vec_colliders_adapters;
    }

    std::vector<const TIKinematicTreeColliderAdapter*> TIKinematicTreeBodyAdapter::colliders_adapters() const
    {
        std::vector<const TIKinematicTreeColliderAdapter*> vec_colliders_adapters;
        for ( auto& collider_adapter : m_CollidersAdapters )
            vec_colliders_adapters.push_back( collider_adapter.get() );
        return vec_colliders_adapters;
    }

    std::vector<TIKinematicTreeJointAdapter*> TIKinematicTreeBodyAdapter::joints_adapters()
    {
        std::vector<TIKinematicTreeJointAdapter*> vec_joints_adapters;
        for ( auto& joint_adapter : m_JointsAdapters )
            vec_joints_adapters.push_back( joint_adapter.get() );
        return vec_joints_adapters;
    }

    std::vector<const TIKinematicTreeJointAdapter*> TIKinematicTreeBodyAdapter::joints_adapters() const
    {
        std::vector<const TIKinematicTreeJointAdapter*> vec_joints_adapters;
        for ( auto& joint_adapter : m_JointsAdapters )
            vec_joints_adapters.push_back( joint_adapter.get() );
        return vec_joints_adapters;
    }
}}
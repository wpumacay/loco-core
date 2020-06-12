
#include <kinematic_trees/loco_kinematic_tree_collider_adapter.h>

namespace loco {
namespace kintree {

    TIKinematicTreeColliderAdapter::TIKinematicTreeColliderAdapter( TKinematicTreeCollider* collider_ref )
    {
        m_ColliderRef = collider_ref;
    }

    TIKinematicTreeColliderAdapter::~TIKinematicTreeColliderAdapter()
    {
        if ( m_ColliderRef )
            m_ColliderRef->DetachSim();
        m_ColliderRef = nullptr;
    }

    void TIKinematicTreeColliderAdapter::OnDetach()
    {
        m_Detached = true;
        m_ColliderRef = nullptr;
    }
}}
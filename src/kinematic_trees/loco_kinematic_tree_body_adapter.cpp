
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
        m_ColliderAdapter = nullptr;
        m_JointAdapter = nullptr;
    }

    void TIKinematicTreeBodyAdapter::OnDetach()
    {
        m_Detached = true;
        m_BodyRef = nullptr;
    }
}}
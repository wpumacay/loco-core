
#include <primitives/loco_single_body_collider_adapter.h>

namespace loco {
namespace primitives {

    TISingleBodyColliderAdapter::TISingleBodyColliderAdapter( TSingleBodyCollider* collider_ref )
    {
        m_ColliderRef = collider_ref;
        m_Detached = false;
    }

    TISingleBodyColliderAdapter::~TISingleBodyColliderAdapter()
    {
        if ( m_ColliderRef )
            m_ColliderRef->DetachSim();
        m_ColliderRef = nullptr;
    }

    void TISingleBodyColliderAdapter::OnDetach()
    {
        m_Detached = true;
        m_ColliderRef = nullptr;
    }
}}
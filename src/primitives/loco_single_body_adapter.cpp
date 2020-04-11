
#include <primitives/loco_single_body_adapter.h>

namespace loco
{
    TISingleBodyAdapter::TISingleBodyAdapter( TSingleBody* body_ref )
        : m_BodyRef( body_ref )
    {
        m_Detached = false;
        m_ColliderAdapter = nullptr;
    }

    TISingleBodyAdapter::~TISingleBodyAdapter()
    {
        if ( m_BodyRef )
            m_BodyRef->DetachSim();
        m_BodyRef = nullptr;
    }

    void TISingleBodyAdapter::OnDetach()
    {
        m_Detached = true;
        m_BodyRef = nullptr;
    }
}
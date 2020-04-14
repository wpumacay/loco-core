
#include <primitives/loco_single_body_adapter.h>

namespace loco
{
    TISingleBodyAdapter::TISingleBodyAdapter( TSingleBody* body_ref )
        : m_BodyRef( body_ref )
    {
        m_Detached = false;
        m_ColliderAdapter = nullptr;
        m_ConstraintAdapter = nullptr;
    }

    TISingleBodyAdapter::~TISingleBodyAdapter()
    {
        if ( m_BodyRef )
            m_BodyRef->DetachSim();

        m_BodyRef = nullptr;
        m_ColliderAdapter = nullptr;
        m_ConstraintAdapter = nullptr;
    }

    void TISingleBodyAdapter::OnDetach()
    {
        m_Detached = true;
        m_BodyRef = nullptr;
    }
}
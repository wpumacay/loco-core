
#include <primitives/loco_single_body_constraint_adapter.h>

namespace loco
{
    TISingleBodyConstraintAdapter::TISingleBodyConstraintAdapter( TISingleBodyConstraint* constraint_ref )
    {
        m_ConstraintRef = constraint_ref;
        m_Detached = false;
    }

    TISingleBodyConstraintAdapter::~TISingleBodyConstraintAdapter()
    {
        if ( m_ConstraintRef )
            m_ConstraintRef->DetachSim();
        m_ConstraintRef = nullptr;
    }

    void TISingleBodyConstraintAdapter::OnDetach()
    {
        m_Detached = true;
        m_ConstraintRef = nullptr;
    }
}
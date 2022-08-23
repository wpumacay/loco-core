
#include <kinematic_trees/loco_kinematic_tree_adapter.h>

namespace loco {
namespace kintree {

    TIKinematicTreeAdapter::TIKinematicTreeAdapter( TKinematicTree* kintree_ref )
    {
        m_KintreeRef = kintree_ref;
    }

    TIKinematicTreeAdapter::~TIKinematicTreeAdapter()
    {
        if ( m_KintreeRef )
            m_KintreeRef->DetachSim();
        m_KintreeRef = nullptr;
        m_BodyAdapters.clear();
    }

    void TIKinematicTreeAdapter::OnDetach()
    {
        m_Detached = true;
        m_KintreeRef = nullptr;
    }

    std::vector<TIKinematicTreeBodyAdapter* > TIKinematicTreeAdapter::body_adapters()
    {
        std::vector<TIKinematicTreeBodyAdapter*> body_adapters_list;
        for ( auto& body_adapter : m_BodyAdapters )
            body_adapters_list.push_back( body_adapter.get() );
        return body_adapters_list;
    }

    std::vector<const TIKinematicTreeBodyAdapter*> TIKinematicTreeAdapter::body_adapters() const
    {
        std::vector<const TIKinematicTreeBodyAdapter*> body_adapters_list;
        for ( auto& body_adapter : m_BodyAdapters )
            body_adapters_list.push_back( body_adapter.get() );
        return body_adapters_list;
    }
}}
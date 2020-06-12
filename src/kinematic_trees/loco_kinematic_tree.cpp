
#include <kinematic_trees/loco_kinematic_tree.h>

namespace loco {
namespace kintree {

    TKinematicTree::TKinematicTree( const std::string& name,
                                    const TVec3& position,
                                    const TMat3& rotation )
        : TObject( name )
    {
        m_tf.set( position, 3 );
        m_tf.set( rotation );
        m_tf0.set( position, 3 );
        m_tf0.set( rotation );

    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TKinematicTree {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TKinematicTree " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TKinematicTree::~TKinematicTree()
    {
        DetachSim();
        DetachViz();

        m_Root = nullptr;
        m_KintreeAdapterRef = nullptr;
        m_MjcfModel = nullptr;
        m_UrdfModel = nullptr;
        m_RlsimModel = nullptr;

        m_Bodies.clear();
        m_BodiesMap.clear();

    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TKinematicTree {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TKinematicTree " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TKinematicTree::SetKintreeAdapter( TIKinematicTreeAdapter* kintree_adapter_ref )
    {
        LOCO_CORE_ASSERT( kintree_adapter_ref, "TKinematicTree:.SetKintreeAdapter >>> tried adding nullptr to kintree {0}", m_name );
        m_KintreeAdapterRef = kintree_adapter_ref;
    }

    void TKinematicTree::AddBody( std::unique_ptr<TKinematicTreeBody> kintree_body, bool is_root )
    {
        m_Bodies.push_back( std::move( kintree_body ) );
        m_Bodies.back()->SetKintree( this );
        m_BodiesMap[m_Bodies.back()->name()] = m_Bodies.size() - 1;

        if ( is_root )
        {
            LOCO_CORE_ASSERT( !m_Root, "TKinematicTree::AddBody >>> tried adding root of the tree more than once, for kintree {0}", m_name );
            m_Root = m_Bodies.back().get();
        }
    }

    void TKinematicTree::AddBodyJointPair( std::unique_ptr<TKinematicTreeBody> kintree_body,
                                           std::unique_ptr<TKinematicTreeJoint> kintree_joint,
                                           const TMat4& tf_joint_wrt_body,
                                           bool is_root )
    {
        kintree_body->AddJoint( std::move( kintree_joint ), tf_joint_wrt_body );
        m_Bodies.push_back( std::move( kintree_body ) );
        m_Bodies.back()->SetKintree( this );
        m_BodiesMap[m_Bodies.back()->name()] = m_Bodies.size() - 1;

        if ( is_root )
        {
            LOCO_CORE_ASSERT( !m_Root, "TKinematicTree::AddBodyJointPair >>> tried adding root of the tree more than once, for kintree {0}", m_name );
            m_Root = m_Bodies.back().get();
        }
    }

    bool TKinematicTree::HasBody( const std::string& name ) const
    {
        return m_BodiesMap.find( name ) != m_BodiesMap.end();
    }

    void TKinematicTree::RemoveBodyByName( const std::string& name )
    {
        if ( !HasBody( name ) )
        {
            LOCO_CORE_ERROR( "TKinematicTree::RemoveBodyByName >>> kintree-body {0}  not found on the kinematic tree {1}", name, m_name );
            return;
        }
        const ssize_t idx_body = m_BodiesMap[name];
        RemoveBodyByIndex( idx_body );
        for ( auto kv : m_BodiesMap )
            if ( kv.second > idx_body )
                m_BodiesMap[kv.first]--;
        m_BodiesMap.erase( name );
    }

    void TKinematicTree::RemoveBodyByIndex( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_Bodies.size() ), "TKinematicTree::RemoveBodyByIndex >>> \
                          index {0} out of range (0,{1}) while working with kintree {2}", index, m_Bodies.size() - 1, m_name );
        m_Bodies.erase( m_Bodies.begin() + index );
    }

    TKinematicTreeBody* TKinematicTree::GetBodyByName( const std::string& name )
    {
        if ( !HasBody( name ) )
        {
            LOCO_CORE_ERROR( "TKinematicTree::GetBodyByName >>> kintree-body {0} not found on the kinematic tree {1}", name, m_name );
            return nullptr;
        }
        return GetBodyByIndex( m_BodiesMap.at( name ) );
    }

    const TKinematicTreeBody* TKinematicTree::GetBodyByName( const std::string& name ) const
    {
        if ( !HasBody( name ) )
        {
            LOCO_CORE_ERROR( "TKinematicTree::GetBodyByName >>> kintree-body {0} not found on the kinematic tree {1}", name, m_name );
            return nullptr;
        }
        return GetBodyByIndex( m_BodiesMap.at( name ) );
    }

    TKinematicTreeBody* TKinematicTree::GetBodyByIndex( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_Bodies.size() ), "TKinematicTree::GetBodyByIndex >>> \
                          index {0} out of range (0,{1}) while working with kintree {2}", index, m_Bodies.size() - 1, m_name );
        return m_Bodies[index].get();
    }

    const TKinematicTreeBody* TKinematicTree::GetBodyByIndex( ssize_t index ) const
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_Bodies.size() ), "TKinematicTree::GetBodyByIndex >>> \
                          index {0} out of range (0,{1}) while working with kintree {2}", index, m_Bodies.size() - 1, m_name );
        return m_Bodies[index].get();
    }

    std::vector<TKinematicTreeBody*> TKinematicTree::GetBodiesList()
    {
        std::vector<TKinematicTreeBody*> bodies_list;
        for ( auto& kintree_body : m_Bodies )
            bodies_list.push_back( kintree_body.get() );
        return bodies_list;
    }

    std::vector<const TKinematicTreeBody*> TKinematicTree::GetBodiesList() const
    {
        std::vector<const TKinematicTreeBody*> bodies_list;
        for ( auto& kintree_body : m_Bodies )
            bodies_list.push_back( kintree_body.get() );
        return bodies_list;
    }

    void TKinematicTree::_InitializeInternal()
    {
        // Let the backend initialize any required resources
        if ( m_KintreeAdapterRef )
            m_KintreeAdapterRef->Initialize();
        // Call initialize recursively on the tree
        if ( m_Root )
            m_Root->Initialize();
    }

    void TKinematicTree::_PreStepInternal()
    {
        if ( m_Root )
            m_Root->PreStep();
    }

    void TKinematicTree::_PostStepInternal()
    {
        if ( m_KintreeAdapterRef ) // If kintree is being simulated, grab information from backend
        {
            m_KintreeAdapterRef->GetTransform( m_tf );
            m_KintreeAdapterRef->GetLinearVelocity( m_LinearVel );
            m_KintreeAdapterRef->GetAngularVelocity( m_AngularVel );
        }
        else if ( m_Root ) // If there's no current simulation, then update the pose of the kintree using forward kinematics
        {
            m_Root->Forward();
        }

        if ( m_Root )
            m_Root->PostStep();
    }

    void TKinematicTree::_ResetInternal()
    {
        m_tf = m_tf0;
        m_LinearVel = m_LinearVel0;
        m_AngularVel = m_AngularVel0;
        if ( m_KintreeAdapterRef )
            m_KintreeAdapterRef->Reset();

        if ( m_Root )
            m_Root->Reset();
    }

    void TKinematicTree::_DetachSimInternal()
    {
        if ( m_KintreeAdapterRef )
            m_KintreeAdapterRef->OnDetach();
        m_KintreeAdapterRef = nullptr;

        // Call detach-sim recursively on the tree
        if ( m_Root )
            m_Root->DetachSim();
    }

    void TKinematicTree::_DetachVizInternal()
    {
        // Call detach-viz recursively on the tree
        if ( m_Root )
            m_Root->DetachViz();
    }

    void TKinematicTree::_SetTransformInternal( const TMat4& transform )
    {
        m_tf = transform;
        if ( m_KintreeAdapterRef ) // Set only the transform of the base|root
            m_KintreeAdapterRef->SetTransform( transform );
        else if ( m_Root ) // If there's no simulation that will propagate the changes, use forward-kinematics instead
            m_Root->Forward();
    }
}}
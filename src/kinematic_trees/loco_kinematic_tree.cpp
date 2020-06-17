
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

    void TKinematicTree::SetRoot( std::unique_ptr<TKinematicTreeBody> body )
    {
        LOCO_CORE_ASSERT( body, "TKinematicTree::SetRoot >>> tried setting nullptr as root to the kintree {0}", m_name );
        m_Root = std::move( body );
        m_Root->SetKintree( this );
        RegisterBody( m_Root.get() );
    }

    void TKinematicTree::RegisterBody( TKinematicTreeBody* body )
    {
        LOCO_CORE_ASSERT( body, "TKinematicTreeBody::RegisterBody >>> tried registering nullptr to the kintree {0}", m_name );
        if ( HasBody( body->name() ) )
        {
            LOCO_CORE_WARN( "TKinematicTreeBody::RegisterBody >>> tried registering body {0} more than once, for kintree {1}", body->name(), m_name );
            return;
        }

        std::stack<TKinematicTreeBody*> bodies_to_register;
        while ( bodies_to_register.size() > 0 )
        {
            auto body_to_register = bodies_to_register.top();
            bodies_to_register.pop();
            if ( !body_to_register )
                continue;

            m_BodiesMap[body_to_register->name()] = body_to_register;
            m_NumKintreeBodies++;
            auto children = body_to_register->children();
            for ( auto child_body : children )
                bodies_to_register.push( child_body );
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

        if ( m_Root->name() == name )
            m_Root = nullptr;
        else
            m_BodiesMap[name]->parent()->RemoveChild( name );
        m_BodiesMap.erase( name );
    }

    TKinematicTreeBody* TKinematicTree::GetBodyByName( const std::string& name )
    {
        if ( !HasBody( name ) )
        {
            LOCO_CORE_ERROR( "TKinematicTree::GetBodyByName >>> kintree-body {0} not found on the kinematic tree {1}", name, m_name );
            return nullptr;
        }
        return m_BodiesMap.at( name );
    }

    const TKinematicTreeBody* TKinematicTree::GetBodyByName( const std::string& name ) const
    {
        if ( !HasBody( name ) )
        {
            LOCO_CORE_ERROR( "TKinematicTree::GetBodyByName >>> kintree-body {0} not found on the kinematic tree {1}", name, m_name );
            return nullptr;
        }
        return m_BodiesMap.at( name );
    }

    std::vector<TKinematicTreeBody*> TKinematicTree::GetBodiesList()
    {
        std::vector<TKinematicTreeBody*> bodies_list;
        std::stack<TKinematicTreeBody*> dfs_bodies;
        dfs_bodies.push( m_Root.get() );
        while ( dfs_bodies.size() > 0 )
        {
            auto kintree_body = dfs_bodies.top();
            dfs_bodies.pop();
            if ( !kintree_body )
                continue;

            bodies_list.push_back( kintree_body );
            auto children = kintree_body->children();
            for ( auto child_body : children )
                dfs_bodies.push( child_body );
        }
        return bodies_list;
    }

    std::vector<const TKinematicTreeBody*> TKinematicTree::GetBodiesList() const
    {
        std::vector<const TKinematicTreeBody*> bodies_list;
        std::stack<const TKinematicTreeBody*> dfs_bodies;
        dfs_bodies.push( m_Root.get() );
        while ( dfs_bodies.size() > 0 )
        {
            auto kintree_body = dfs_bodies.top();
            dfs_bodies.pop();
            if ( !kintree_body )
                continue;

            bodies_list.push_back( kintree_body );
            auto children = kintree_body->children();
            for ( auto child_body : children )
                dfs_bodies.push( child_body );
        }
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
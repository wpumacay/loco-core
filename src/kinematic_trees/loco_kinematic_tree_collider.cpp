
#include <kinematic_trees/loco_kinematic_tree_collider.h>

namespace loco {
namespace kintree {

    TKinematicTreeCollider::TKinematicTreeCollider( const std::string& name,
                                                    const TCollisionData& collision_data )
        : TObject( name )
    {
        m_Data = collision_data;
        m_Visible = true;
        m_Wireframe = true;

        m_ParentBodyRef = nullptr;
        m_ColliderAdapterRef = nullptr;
        m_DrawableAdapterRef = nullptr;

    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TKinematicTreeCollider {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TKinematicTreeCollider " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TKinematicTreeCollider::~TKinematicTreeCollider()
    {
        DetachSim();
        DetachViz();

        m_ParentBodyRef = nullptr;
        m_DrawableAdapterRef = nullptr;
        m_ColliderAdapterRef = nullptr;

    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TKinematicTreeCollider {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TKinematicTreeCollider " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TKinematicTreeCollider::SetParentBody( TKinematicTreeBody* parent_body_ref, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( parent_body_ref, "TKinematicTreeCollider::SetParentBody >>> tried adding nullptr as parent to collider {0}", m_name );
        m_ParentBodyRef = parent_body_ref;
        m_LocalTf = local_tf;
        m_tf = m_ParentBodyRef->tf() * m_LocalTf;
        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->SetTransform( m_tf );
    }

    void TKinematicTreeCollider::SetColliderAdapter( TIKinematicTreeColliderAdapter* collider_adapter_ref )
    {
        LOCO_CORE_ASSERT( collider_adapter_ref, "TKinematicTreeCollider::SetAdapter >>> tried adding nullptr as collider-adapter to collider {0}", m_name );
        m_ColliderAdapterRef = collider_adapter_ref;
    }

    void TKinematicTreeCollider::SetDrawableAdapter( TIDrawableAdapter* drawable_adapter_ref )
    {
        LOCO_CORE_ASSERT( drawable_adapter_ref, "TKinematicTreeCollider::SetDrawable >>> tried adding nullptr as drawable-adapter to collider {0}", m_name );
        m_DrawableAdapterRef = drawable_adapter_ref;
        m_DrawableAdapterRef->SetTransform( m_tf );
    }

    void TKinematicTreeCollider::SetVisible( bool visible )
    {
        m_Visible = visible;
        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->SetVisible( visible );
    }

    void TKinematicTreeCollider::SetWireframe( bool wireframe )
    {
        m_Wireframe = wireframe;
        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->SetWireframe( wireframe );
    }

    void TKinematicTreeCollider::SetLocalTransform( const TMat4& local_transform )
    {
        m_LocalTf = local_transform;
        m_Data.localTransform = local_transform;
        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->SetLocalTransform( local_transform );
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->SetTransform( m_tf );
    }

    void TKinematicTreeCollider::ChangeSize( const TVec3& new_size )
    {
        m_Data.size = new_size;
        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->ChangeSize( new_size );
        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->ChangeSize( new_size );
    }

    void TKinematicTreeCollider::ChangeCollisionGroup( int collision_group )
    {
        m_Data.collisionGroup = collision_group;
        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->ChangeCollisionGroup( collision_group );
    }

    void TKinematicTreeCollider::ChangeCollisionMask( int collision_mask )
    {
        m_Data.collisionMask = collision_mask;
        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->ChangeCollisionMask( collision_mask );
    }

    void TKinematicTreeCollider::ChangeFriction( const TScalar& friction )
    {
        m_Data.friction.x() = friction;
        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->ChangeFriction( friction );
    }

    void TKinematicTreeCollider::_InitializeInternal()
    {
        // Nothing to setup (adapter is handled by parent body)
    }

    void TKinematicTreeCollider::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TKinematicTreeCollider::_PostStepInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->SetTransform( m_tf );
    }

    void TKinematicTreeCollider::_ResetInternal()
    {
        // Nothing to setup for a reset request
    }

    void TKinematicTreeCollider::_DetachSimInternal()
    {
        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->OnDetach();
        m_ColliderAdapterRef = nullptr;
    }

    void TKinematicTreeCollider::_DetachVizInternal()
    {
        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->OnDetach();
        m_DrawableAdapterRef = nullptr;
    }

    void TKinematicTreeCollider::_SetTransformInternal( const TMat4& transform )
    {
        LOCO_CORE_WARN( "TKinematicTreeCollider::_SetTransformInternal >>> collider objects don't have \
                        this functionality. Change the transform of the parent body instead. (collider={0})", m_name );
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
    }
}}

#include <primitives/loco_single_body_collider.h>

namespace loco
{
    TSingleBodyCollider::TSingleBodyCollider( const std::string& name,
                                              const TCollisionData& collisionData )
        : TObject( name )
    {
        m_Data = collisionData;
        m_Visible = true;
        m_Wireframe = false;

        m_ParentBodyRef = nullptr;
        m_ColliderAdapterRef = nullptr;
        m_DrawableAdapterRef = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBodyCollider {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBodyCollider " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBodyCollider::~TSingleBodyCollider()
    {
        DetachSim();
        DetachViz();

        m_ParentBodyRef = nullptr;
        m_DrawableAdapterRef = nullptr;
        m_ColliderAdapterRef = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TSingleBodyCollider {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TSingleBodyCollider " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TSingleBodyCollider::SetParentBody( TSingleBody* parent_body_ref )
    {
        LOCO_CORE_ASSERT( parent_body_ref, "TSingleBodyCollider::SetParentBody >>> tried adding nullptr" );

        m_ParentBodyRef = parent_body_ref;
        m_tf = m_ParentBodyRef->tf();

        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->SetTransform( m_tf );
    }

    void TSingleBodyCollider::SetColliderAdapter( TISingleBodyColliderAdapter* collider_adapter_ref )
    {
        LOCO_CORE_ASSERT( collider_adapter_ref, "TSingleBodyCollider::SetAdapter >>> tried adding nullptr" );

        m_ColliderAdapterRef = collider_adapter_ref;
    }

    void TSingleBodyCollider::SetDrawableAdapter( TIDrawableAdapter* drawable_adapter_ref )
    {
        LOCO_CORE_ASSERT( drawable_adapter_ref, "TSingleBodyCollider::SetDrawable >>> tried adding nullptr" );

        m_DrawableAdapterRef = drawable_adapter_ref;
        m_DrawableAdapterRef->SetTransform( m_tf );
    }

    void TSingleBodyCollider::SetVisible( bool visible )
    {
        m_Visible = visible;

        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->SetVisible( visible );
    }

    void TSingleBodyCollider::SetWireframe( bool wireframe )
    {
        m_Wireframe = wireframe;

        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->SetWireframe( wireframe );
    }

    void TSingleBodyCollider::ChangeSize( const TVec3& new_size )
    {
        m_Data.size = new_size;

        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->ChangeSize( new_size );

        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->ChangeSize( new_size );
    }

    void TSingleBodyCollider::ChangeElevationData( const std::vector< float >& heights )
    {
        if ( m_Data.type != eShapeType::HFIELD )
        {
            LOCO_CORE_WARN( "TSingleBodyCollider::ChangeElevationData >>> collision shape {0} is not a hfield", m_name );
            return;
        }
        auto& hfield_data = m_Data.hfield_data;
        // Sanity check: make sure that both internal and given buffers have same num-elements
        const ssize_t num_samples = hfield_data.nWidthSamples * hfield_data.nDepthSamples;
        if( num_samples != heights.size() )
        {
            LOCO_CORE_WARN( "TSingleBodyCollider::ChangeElevationData >>> given buffer doesn't match expected size of collision shape {0}", m_name );
            LOCO_CORE_WARN( "\tnx-samples    : {0}", hfield_data.nWidthSamples );
            LOCO_CORE_WARN( "\tny-samples    : {0}", hfield_data.nDepthSamples );
            LOCO_CORE_WARN( "\tinternal-size : {0}", num_samples );
            LOCO_CORE_WARN( "\tgiven-size    : {0}", heights.size() );
            return;
        }

        // Change the internal elevation data (memcpy to avoid destroying and creating internal buffer)
        memcpy( hfield_data.heights.data(), heights.data(), sizeof(float) * num_samples );

        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->ChangeElevationData( heights );

        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->ChangeElevationData( heights );
    }

    void TSingleBodyCollider::ChangeCollisionGroup( int collisionGroup )
    {
        m_Data.collisionGroup = collisionGroup;

        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->ChangeCollisionGroup( collisionGroup );
    }

    void TSingleBodyCollider::ChangeCollisionMask( int collisionMask )
    {
        m_Data.collisionMask = collisionMask;

        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->ChangeCollisionMask( collisionMask );
    }

    void TSingleBodyCollider::ChangeFriction( const TScalar& friction )
    {
        m_Data.friction.x() = friction;

        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->ChangeFriction( m_Data.friction.x() );
    }

    void TSingleBodyCollider::_InitializeInternal()
    {
        // Nothing to setup (adapter is handled by parent body)
    }

    void TSingleBodyCollider::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TSingleBodyCollider::_PostStepInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf();

        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->SetTransform( m_tf );
    }

    void TSingleBodyCollider::_ResetInternal()
    {
        // Nothing to setup for a reset request
    }

    void TSingleBodyCollider::_DetachSimInternal()
    {
        if ( m_ColliderAdapterRef )
            m_ColliderAdapterRef->OnDetach();
        m_ColliderAdapterRef = nullptr;
    }

    void TSingleBodyCollider::_DetachVizInternal()
    {
        if ( m_DrawableAdapterRef )
            m_DrawableAdapterRef->OnDetach();
        m_DrawableAdapterRef = nullptr;
    }

    void TSingleBodyCollider::_SetTransformInternal( const TMat4& transform )
    {
        LOCO_CORE_WARN( "TSingleBodyCollider::_SetTransformInternal >>> collider objects don't have \
                        this functionality. Change the transform of the parent body instead. (collider={0})", m_name );

        // Recover transform from parent
        m_tf = m_ParentBodyRef->tf();
    }
}
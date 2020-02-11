
#include <components/loco_collision.h>

namespace loco
{
    TCollision::TCollision( const std::string& name,
                            const TCollisionData& collisionData )
    {
        m_name = name;
        m_data = collisionData;
        m_localTf = m_data.localTransform;
        m_visible = true;
        m_wireframe = false;

        m_parentBodyRef = nullptr;
        m_collisionImplRef = nullptr;
        m_drawableImplRef = nullptr;

        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TCollision {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TCollision " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
    }

    TCollision::~TCollision()
    {
        m_parentBodyRef = nullptr;
        m_drawableImplRef = nullptr;
        m_collisionImplRef = nullptr;

        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TCollision {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TCollision " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
    }

    void TCollision::SetParentBody( TIBody* parentBodyRef )
    {
        LOCO_CORE_ASSERT( parentBodyRef, "TCollision::SetParentBody >>> tried adding nullptr" );

        m_parentBodyRef = parentBodyRef;
        m_tf = m_parentBodyRef->tf() * m_localTf;

        if ( m_collisionImplRef )
            m_collisionImplRef->PostStep();

        if ( m_drawableImplRef )
            m_drawableImplRef->SetWorldTransform( m_tf );
    }

    void TCollision::SetAdapter( TICollisionAdapter* collisionImplRef )
    {
        LOCO_CORE_ASSERT( collisionImplRef, "TCollision::SetAdapter >>> tried adding nullptr" );

        m_collisionImplRef = collisionImplRef;
    }

    void TCollision::SetDrawable( TIDrawable* drawableRef )
    {
        LOCO_CORE_ASSERT( drawableRef, "TCollision::SetDrawable >>> tried adding nullptr" );

        m_drawableImplRef = drawableRef;
        m_drawableImplRef->SetWorldTransform( m_tf );
    }

    void TCollision::SetVisible( bool visible )
    {
        m_visible = visible;

        if ( m_drawableImplRef )
            m_drawableImplRef->SetVisible( visible );
    }

    void TCollision::SetWireframe( bool wireframe )
    {
        m_wireframe = wireframe;

        if ( m_drawableImplRef )
            m_drawableImplRef->SetWireframe( wireframe );
    }

    void TCollision::Initialize()
    {
        // Tell the backend to initialize any extra required resources for the simulation
        if ( m_collisionImplRef )
            m_collisionImplRef->Initialize();
    }

    void TCollision::PreStep()
    {
        // Tell the backend to update any required information prior to taking a simulation step
        if ( m_collisionImplRef )
            m_collisionImplRef->PreStep();
    }

    void TCollision::PostStep()
    {
        // Tell the backend to update any required information after a simulation step has been taken
        if ( m_collisionImplRef )
            m_collisionImplRef->PostStep();

        // Update our own transform using the world-transform from the parent
        if ( m_parentBodyRef )
            m_tf = m_parentBodyRef->tf() * m_localTf;

        // Set the transform of the renderable to be our own world-transform
        if ( m_drawableImplRef )
            m_drawableImplRef->SetWorldTransform( m_tf );
    }

    void TCollision::Reset()
    {
        // Tell the backend to reset any required information before restarting the simulation
        if ( m_collisionImplRef )
            m_collisionImplRef->Reset();
    }

    void TCollision::SetLocalPosition( const TVec3& localPosition )
    {
        m_localTf.set( localPosition, 3 );

        if ( m_collisionImplRef )
            m_collisionImplRef->SetLocalPosition( localPosition );
    }

    void TCollision::SetLocalRotation( const TMat3& localRotation )
    {
        m_localTf.set( localRotation );

        if ( m_collisionImplRef )
            m_collisionImplRef->SetLocalRotation( localRotation );
    }

    void TCollision::SetLocalQuat( const TVec4& localQuaternion )
    {
        auto localRotation = tinymath::rotation( localQuaternion );
        m_localTf.set( localRotation );

        if ( m_collisionImplRef )
            m_collisionImplRef->SetLocalRotation( localRotation );
    }

    void TCollision::SetLocalEuler( const TVec3& localEuler )
    {
        auto localRotation = tinymath::rotation( localEuler );
        m_localTf.set( localRotation );

        if ( m_collisionImplRef )
            m_collisionImplRef->SetLocalRotation( localRotation );
    }

    void TCollision::SetLocalTransform( const TMat4& localTransform )
    {
        m_localTf = localTransform;

        if ( m_collisionImplRef )
            m_collisionImplRef->SetLocalTransform( m_localTf );
    }

    void TCollision::ChangeSize( const TVec3& newSize )
    {
        if ( m_data.type == eShapeType::MESH )
        {
            LOCO_CORE_WARN( "TCollision::ChangeSize >>> for collision shape {0} of type mesh, changing \
                             mesh sizes at runtime is not supported, as it requires recomputing the \
                             vertex positions for this new size in various backend in some specific \
                             way. Please set the initial scale of the mesh first, which does the \
                             job during construction.", m_name );
            return;
        }

        if ( m_data.type == eShapeType::HFIELD )
        {
            LOCO_CORE_WARN( "TCollision::ChangeSize >>> for collision shape {0} of type hfield, changing \
                             hfield sizes at runtime is not supported (yet), as it requires recomputing \
                             the elevation data every time. For now, set the size properties of the hfield \
                             first (which will do the job, during construction), or use ChangeElevationData \
                             instead to replace the internal data for a new buffer", m_name );
            return;
        }

        // Change the collision-data size property
        m_data.size = newSize;

        // Tell the backend resource to change the size internally
        if ( m_collisionImplRef )
            m_collisionImplRef->ChangeSize( newSize );

        // Tell the rendering resource to change the size internally
        if ( m_drawableImplRef )
            m_drawableImplRef->ChangeSize( newSize );
    }

    void TCollision::ChangeElevationData( const std::vector< float >& heightData )
    {
        if ( m_data.type != eShapeType::HFIELD )
        {
            LOCO_CORE_WARN( "TCollision::ChangeElevationData >>> collision shape {0} is not a hfield", m_name );
            return;
        }

        // sanity check: make sure that both internal and given buffers have same num-elements
        if( ( m_data.hdata.nWidthSamples * m_data.hdata.nDepthSamples ) != heightData.size() )
        {
            LOCO_CORE_WARN( "TCollision::ChangeElevationData >>> given buffer doesn't match expected size of collision shape {0}", m_name );
            LOCO_CORE_WARN( "\tnx-samples    : {0}", m_data.hdata.nWidthSamples );
            LOCO_CORE_WARN( "\tny-samples    : {0}", m_data.hdata.nDepthSamples );
            LOCO_CORE_WARN( "\tinternal-size : {0}", ( m_data.hdata.nWidthSamples * m_data.hdata.nDepthSamples ) );
            LOCO_CORE_WARN( "\tgiven-size    : {0}", heightData.size() );
            return;
        }

        // Change the internal elevation data
        m_data.hdata.heights = heightData;

        // Tell the backend resource to change the elevation data internally
        if ( m_collisionImplRef )
            m_collisionImplRef->ChangeElevationData( heightData );

        // Tell the rendering resource to change the elevation data internally
        if ( m_drawableImplRef )
            m_drawableImplRef->ChangeElevationData( heightData );
    }

    void TCollision::ChangeCollisionGroup( int collisionGroup )
    {
        m_data.collisionGroup = collisionGroup;

        // Tell the backend resource to change the collisionGroup internally
        if ( m_collisionImplRef )
            m_collisionImplRef->ChangeCollisionGroup( collisionGroup );
    }

    void TCollision::ChangeCollisionMask( int collisionMask )
    {
        m_data.collisionMask = collisionMask;

        // Tell the backend resource to change the collisionMask internally
        if ( m_collisionImplRef )
            m_collisionImplRef->ChangeCollisionMask( collisionMask );
    }
}
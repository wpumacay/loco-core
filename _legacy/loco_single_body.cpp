
#include <primitives/loco_single_body.h>

namespace loco
{
    TSingleBody::TSingleBody( const std::string& name,
                              const TBodyData& data,
                              const TVec3& position,
                              const TMat3& rotation )
        : TIBody( name, data )
    {
        // Set world-transform directly (world-space)
        m_tf.set( position, 3 );
        m_tf.set( rotation );

        // Set initial world-transform directly (world-space)
        m_tf0.set( position, 3 );
        m_tf0.set( rotation );

        // Set local pose (and friends) as Identity
        m_localTf.setIdentity();
        m_localTf0.setIdentity();

        // Make sure our type is single-body
        m_classType = eBodyClassType::SINGLE_BODY;

        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBody {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TSingleBody " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
    }

    TSingleBody::~TSingleBody()
    {
        // nothing extra to delete for now

        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( tinyutils::Logger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TSingleBody {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TSingleBody " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
        #endif
    }

    void TSingleBody::_InitializeInternal()
    {
        // Tell the backend to initialize any required resources for simulation
        if ( m_bodyImplRef )
            m_bodyImplRef->Initialize();
    }

    void TSingleBody::_PreStepInternal()
    {
        // Update any internal backend resources prior to a simulation step
        if ( m_bodyImplRef )
            m_bodyImplRef->PreStep();

        // Notify the collider to update any required resources prior to take a simulation step
        if ( m_collision )
            m_collision->PreStep();

        // Notify the visual to update any required resources prior to take a simulation step
        if ( m_visual )
            m_visual->PreStep();
    }

    void TSingleBody::_PostStepInternal()
    {
        // Update any internal backend resources (grab sim-state) after a sim. step has been taken
        if ( m_bodyImplRef )
        {
            m_bodyImplRef->PostStep();

            // Grab the latest world-transform from the backend
            m_bodyImplRef->GetTransform( m_tf );
        }

        // Notify the collider to update any required resources after a simulation step was taken
        if ( m_collision )
            m_collision->PostStep();

        // Notify the visual to update any required resources after a simulation step was taken
        if ( m_visual )
            m_visual->PostStep();
    }

    void TSingleBody::_ResetInternal()
    {
        if ( m_bodyImplRef )
        {
            // Reset the low-level resources through the adapter
            m_bodyImplRef->Reset();
            // Grab the world position|rotation information as well
            m_bodyImplRef->GetTransform( m_tf );
        }
        else
        {
            m_tf = m_tf0;
        }

        if ( m_collision )
        {
            m_collision->Reset();
            m_collision->PostStep();
        }

        if ( m_visual )
        {
            m_visual->Reset();
            m_visual->PostStep();
        }
    }

    void TSingleBody::_DetachInternal()
    {
        // Nothing extra to detach from
    }

    void TSingleBody::_SetPositionInternal( const TVec3& position )
    {
        m_tf.set( position, 3 );

        if ( m_bodyImplRef )
            m_bodyImplRef->SetPosition( position );

        if ( m_collision )
            m_collision->PostStep();

        if ( m_visual )
            m_visual->PostStep();
    }

    void TSingleBody::_SetRotationInternal( const TMat3& rotation )
    {
        m_tf.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->SetRotation( rotation );

        if ( m_collision )
            m_collision->PostStep();

        if ( m_visual )
            m_visual->PostStep();
    }

    void TSingleBody::_SetEulerInternal( const TVec3& euler )
    {
        auto rotation = tinymath::rotation( euler );
        m_tf.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->SetRotation( rotation );

        if ( m_collision )
            m_collision->PostStep();

        if ( m_visual )
            m_visual->PostStep();
    }

    void TSingleBody::_SetQuaternionInternal( const TVec4& quat )
    {
        auto rotation = tinymath::rotation( quat );
        m_tf.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->SetRotation( rotation );

        if ( m_collision )
            m_collision->PostStep();

        if ( m_visual )
            m_visual->PostStep();
    }

    void TSingleBody::_SetTransformInternal( const TMat4& transform )
    {
        m_tf = transform;

        if ( m_bodyImplRef )
            m_bodyImplRef->SetTransform( m_tf );

        if ( m_collision )
            m_collision->PostStep();

        if ( m_visual )
            m_visual->PostStep();
    }

    void TSingleBody::_SetInitialPositionInternal( const TVec3& position )
    {
        m_tf0.set( position, 3 );

        if ( m_bodyImplRef )
            m_bodyImplRef->SetInitialPosition( position );
    }

    void TSingleBody::_SetInitialRotationInternal( const TMat3& rotation )
    {
        m_tf0.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->SetInitialRotation( rotation );
    }

    void TSingleBody::_SetInitialEulerInternal( const TVec3& euler )
    {
        auto rotation = tinymath::rotation( euler );
        m_tf0.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->SetInitialRotation( rotation );
    }

    void TSingleBody::_SetInitialQuaternionInternal( const TVec4& quat )
    {
        auto rotation = tinymath::rotation( quat );
        m_tf0.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->SetInitialRotation( rotation );
    }

    void TSingleBody::_SetInitialTransformInternal( const TMat4& transform )
    {
        m_tf0 = transform;

        if ( m_bodyImplRef )
            m_bodyImplRef->SetTransform( m_tf0 );
    }

    void TSingleBody::_SetLocalPositionInternal( const TVec3& localPosition )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_SetLocalRotationInternal( const TMat3& localRotation )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_SetLocalEulerInternal( const TVec3& localEuler )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_SetLocalQuaternionInternal( const TVec4& localQuat )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_SetLocalTransformInternal( const TMat4& localTransform )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_SetInitialLocalPositionInternal( const TVec3& localPosition )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_SetInitialLocalRotationInternal( const TMat3& localRotation )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_SetInitialLocalEulerInternal( const TVec3& localEuler )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_SetInitialLocalQuaternionInternal( const TVec4& localQuat )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_SetInitialLocalTransformInternal( const TMat4& localTransform )
    {
        // Do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }
}
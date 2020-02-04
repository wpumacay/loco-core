
#include <primitives/single_body.h>

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
    }

    TSingleBody::~TSingleBody()
    {
        // nothing extra to delete for now
    }

    void TSingleBody::_preStepInternal()
    {
        // Update any internal backend resources prior to a simulation step
        if ( m_bodyImplRef )
            m_bodyImplRef->preStep();

        // Notify the collider to update any required resources prior to take a simulation step
        if ( m_collision )
            m_collision->preStep();

        // Notify the visual to update any required resources prior to take a simulation step
        if ( m_visual )
            m_visual->preStep();
    }

    void TSingleBody::_postStepInternal()
    {
        // Update any internal backend resources (grab sim-state) after a sim. step has been taken
        if ( m_bodyImplRef )
        {
            m_bodyImplRef->postStep();

            // Grab the latest world-transform from the backend
            m_bodyImplRef->getTransform( m_tf );
        }

        // Notify the collider to update any required resources after a simulation step was taken
        if ( m_collision )
            m_collision->postStep();

        // Notify the visual to update any required resources after a simulation step was taken
        if ( m_visual )
            m_visual->postStep();
    }

    void TSingleBody::_resetInternal()
    {
        if ( m_bodyImplRef )
        {
            // Reset the low-level resources through the adapter
            m_bodyImplRef->reset();

            // Grab the world position|rotation information as well
            m_bodyImplRef->getTransform( m_tf );
        }

        if ( m_collision )
        {
            m_collision->reset();
            m_collision->postStep();
        }

        if ( m_visual )
        {
            m_visual->reset();
            m_visual->postStep();
        }
    }

    void TSingleBody::_setPositionInternal( const TVec3& position )
    {
        m_tf.set( position, 3 );

        if ( m_bodyImplRef )
            m_bodyImplRef->setPosition( position );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TSingleBody::_setRotationInternal( const TMat3& rotation )
    {
        m_tf.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( rotation );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TSingleBody::_setEulerInternal( const TVec3& euler )
    {
        auto rotation = tinymath::rotation( euler );
        m_tf.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( rotation );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TSingleBody::_setQuaternionInternal( const TVec4& quat )
    {
        auto rotation = tinymath::rotation( quat );
        m_tf.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( rotation );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TSingleBody::_setTransformInternal( const TMat4& transform )
    {
        m_tf = transform;

        if ( m_bodyImplRef )
            m_bodyImplRef->setTransform( m_tf );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TSingleBody::_setLocalPositionInternal( const TVec3& localPosition )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_setLocalRotationInternal( const TMat3& localRotation )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_setLocalEulerInternal( const TVec3& localEuler )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_setLocalQuaternionInternal( const TVec4& localQuat )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::_setLocalTransformInternal( const TMat4& localTransform )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }
}
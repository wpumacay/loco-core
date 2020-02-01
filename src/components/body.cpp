
#include <components/body.h>

namespace tysoc {

    TIBody::TIBody( const std::string& name,
                    const TBodyData& data )
    {
        m_name = name;
        m_data = data;
        m_bodyImplRef = nullptr;
        m_classType = eBodyClassType::NONE;

        m_collision = std::unique_ptr< TCollision >( new TCollision( m_name + "_col", m_data.collision ) );
        m_collision->setParentBody( this );

        m_visual = std::unique_ptr< TVisual >( new TVisual( m_name + "_viz", m_data.visual ) );
        m_visual->setParentBody( this );
    }

    TIBody::~TIBody()
    {
        m_bodyImplRef = nullptr;
        m_collision = nullptr;
        m_visual = nullptr;
    }

    void TIBody::setAdapter( TIBodyAdapter* bodyImplRef )
    {
        m_bodyImplRef = bodyImplRef;
    }

    void TIBody::setCollision( std::unique_ptr< TCollision > collisionObj )
    {
        if ( !collisionObj )
            return;

        // Keep ownership of this collider, update collision-body references, and
        // update the body data (note: previous collider is deleted)
        m_collision = std::move( collisionObj );
        m_collision->setParentBody( this );
        m_data.collision = m_collision->data();
    }

    void TIBody::setVisual( std::unique_ptr< TVisual > visualObj )
    {
        if ( !visualObj )
            return;

        // Keep ownership of this visual, update collision-body references, and
        // update the body data (note: previous visual is deleted)
        m_visual = std::move( visualObj );
        m_visual->setParentBody( this );
        m_data.visual = m_visual->data();
    }

    void TIBody::preStep()
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

    void TIBody::postStep()
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

    void TIBody::reset()
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

    ////////////////////////////////////////////////////////////
    //                   World-space setters                  //
    ////////////////////////////////////////////////////////////

    void TIBody::setPosition( const TVec3& position )
    {
        m_tf.set( position, 3 );

        if ( m_bodyImplRef )
            m_bodyImplRef->setPosition( position );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TIBody::setRotation( const TMat3& rotation )
    {
        m_tf.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( rotation );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TIBody::setEuler( const TVec3& euler )
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

    void TIBody::setQuaternion( const TVec4& quat )
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

    void TIBody::setTransform( const TMat4& transform )
    {
        m_tf = transform;

        if ( m_bodyImplRef )
            m_bodyImplRef->setTransform( m_tf );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    ////////////////////////////////////////////////////////////
    //                   Local-space setters                  //
    ////////////////////////////////////////////////////////////

    void TIBody::setLocalPosition( const TVec3& localPosition )
    {
        m_localTf.set( localPosition, 3 );

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalPosition( localPosition );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TIBody::setLocalRotation( const TMat3& localRotation )
    {
        m_localTf.set( localRotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalRotation( localRotation );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TIBody::setLocalEuler( const TVec3& localEuler )
    {
        auto rotation = tinymath::rotation( localEuler );
        m_localTf.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalRotation( rotation );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TIBody::setLocalQuaternion( const TVec4& localQuat )
    {
        auto rotation = tinymath::rotation( localQuat );
        m_localTf.set( rotation );

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalRotation( rotation );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }

    void TIBody::setLocalTransform( const TMat4& localTransform )
    {
        m_localTf = localTransform;

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalTransform( m_localTf );

        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();
    }
}
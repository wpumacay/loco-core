
#include <components/body.h>

namespace tysoc {

    TIBody::TIBody( const std::string& name,
                    const TBodyData& data )
    {
        m_name = name;
        m_data = data;
        m_bodyImplRef = nullptr;
        m_classType = eBodyClassType::NONE;

        /* create collider-obj from collision-data */
        m_collision = std::unique_ptr< TCollision >( new TCollision( m_name + "_col", m_data.collision ) );
        m_collision->setParentBody( this );

        /* create visual-obj from visual-data */
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
        /* keep a reference to the adapter */
        m_bodyImplRef = bodyImplRef;
    }

    void TIBody::setCollision( std::unique_ptr< TCollision > collisionObj )
    {
        if ( !collisionObj )
            return;

        /* keep ownership of this collider, update collision-body references, and 
           update the body data (note: previous collider is deleted)*/
        m_collision = std::move( collisionObj );
        m_collision->setParentBody( this );
        m_data.collision = m_collision->data();
    }

    void TIBody::setVisual( std::unique_ptr< TVisual > visualObj )
    {
        if ( !visualObj )
            return;

        /* keep ownership of this visual, update collision-body references, and 
           update the body data (note: previous visual is deleted)*/
        m_visual = std::move( visualObj );
        m_visual->setParentBody( this );
        m_data.visual = m_visual->data();
    }

    void TIBody::update()
    {
        if ( m_bodyImplRef )
        {
            // update the adapter to handle internal stuff
            m_bodyImplRef->update();

            // grab the latest world-transform from the backend
            m_bodyImplRef->getPosition( m_pos );
            m_bodyImplRef->getRotation( m_rot );
            m_bodyImplRef->getTransform( m_tf );

            // grab the latest local-transfrom from the backend
            m_bodyImplRef->getLocalPosition( m_localPos );
            m_bodyImplRef->getLocalRotation( m_localRot );
            m_bodyImplRef->getLocalTransform( m_localTf );
        }

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TIBody::reset()
    {
        if ( m_bodyImplRef )
        {
            // reset the low-level resources through the adapter
            m_bodyImplRef->reset();

            // grab the world position|rotation information as well
            m_bodyImplRef->getPosition( m_pos );
            m_bodyImplRef->getRotation( m_rot );
            m_bodyImplRef->getTransform( m_tf );

            // grab the local position|rotation information as well
            m_bodyImplRef->getLocalPosition( m_localPos );
            m_bodyImplRef->getLocalRotation( m_localRot );
            m_bodyImplRef->getLocalTransform( m_localTf );
        }

        if ( m_collision )
        {
            m_collision->reset();
            m_collision->update();
        }

        if ( m_visual )
        {
            m_visual->reset();
            m_visual->update();
        }
    }

    ////////////////////////////////////////////////////////////
    //                   World-space setters                  //
    ////////////////////////////////////////////////////////////

    void TIBody::setPosition( const TVec3& position )
    {
        m_pos = position;
        m_tf.setPosition( m_pos );

        if ( m_bodyImplRef )
            m_bodyImplRef->setPosition( m_pos );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TIBody::setRotation( const TMat3& rotation )
    {
        m_rot = rotation;
        m_tf.setRotation( m_rot );

        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( m_rot );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TIBody::setEuler( const TVec3& euler )
    {
        m_rot = TMat3::fromEuler( euler );
        m_tf.setRotation( m_rot );

        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( m_rot );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TIBody::setQuaternion( const TVec4& quat )
    {
        m_rot = TMat3::fromQuaternion( quat );
        m_tf.setRotation( m_rot );

        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( m_rot );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TIBody::setTransform( const TMat4& transform )
    {
        m_tf = transform;
        m_pos = m_tf.getPosition();
        m_rot = m_tf.getRotation();

        if ( m_bodyImplRef )
            m_bodyImplRef->setTransform( m_tf );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    ////////////////////////////////////////////////////////////
    //                   Local-space setters                  //
    ////////////////////////////////////////////////////////////

    void TIBody::setLocalPosition( const TVec3& localPosition )
    {
        m_localPos = localPosition;
        m_localTf.setPosition( m_localPos );

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalPosition( m_localPos );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TIBody::setLocalRotation( const TMat3& localRotation )
    {
        m_localRot = localRotation;
        m_localTf.setRotation( m_localRot );

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalRotation( m_localRot );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TIBody::setLocalEuler( const TVec3& localEuler )
    {
        m_localRot = TMat3::fromEuler( localEuler );
        m_localTf.setRotation( m_localRot );

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalRotation( m_localRot );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TIBody::setLocalQuaternion( const TVec4& localQuat )
    {
        m_localRot = TMat3::fromQuaternion( localQuat );
        m_localTf.setRotation( m_localRot );

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalRotation( m_localRot );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TIBody::setLocalTransform( const TMat4& localTransform )
    {
        m_localTf = localTransform;
        m_localPos = m_localTf.getPosition();
        m_localPot = m_localTf.getRotation();

        if ( m_bodyImplRef )
            m_bodyImplRef->setLocalTransform( m_localTf );

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }
}
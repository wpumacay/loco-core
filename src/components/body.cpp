
#include <components/body.h>

namespace tysoc {

    TBody::TBody( const std::string& name,
                  const TBodyData& data,
                  const TVec3& position,
                  const TMat3& rotation )
    {
        m_name = name;
        m_data = data;
        m_bodyImplRef = nullptr;

        m_pos = m_pos0 = position;
        m_rot = m_rot0 = rotation;

        m_tf.setPosition( m_pos );
        m_tf.setRotation( m_rot );

        m_tf0.setPosition( m_pos0 );
        m_tf0.setRotation( m_rot0 );

        /* create collider-obj from collision-data */
        m_collision = std::unique_ptr< TCollision >( new TCollision( m_name + "_col", m_data.collision ) );
        m_collision->setParentBody( this );

        /* create visual-obj from visual-data */
        m_visual = std::unique_ptr< TVisual >( new TVisual( m_name + "_viz", m_data.visual ) );
        m_visual->setParentBody( this );
        
    }

    TBody::~TBody()
    {
        m_bodyImplRef = nullptr;
        m_collision = nullptr;
        m_visual = nullptr;
    }

    void TBody::setAdapter( TIBodyAdapter* bodyImpl )
    {
        // keep the reference to the adapter
        m_bodyImplRef = bodyImpl;
    }

    void TBody::setCollision( std::unique_ptr< TCollision > collisionObj )
    {
        if ( !collisionObj )
            return;

        m_collision = std::move( collisionObj );
        m_collision->setParentBody( this );
        m_data.collision = m_collision->data();
    }

    void TBody::setVisual( std::unique_ptr< TVisual > visualObj )
    {
        if ( !visualObj )
            return;

        m_visual = std::move( visualObj );
        m_visual->setParentBody( this );
        m_data.visual = m_visual->data();
    }

    void TBody::update()
    {
        if ( m_bodyImplRef )
        {
            // update the adapter to handle internal stuff
            m_bodyImplRef->update();

            // grab the latest world-transform from the backend
            m_bodyImplRef->getPosition( m_pos );
            m_bodyImplRef->getRotation( m_rot );
            m_bodyImplRef->getTransform( m_tf );
        }

        if ( m_collision )
            m_collision->update();

        if ( m_visual )
            m_visual->update();
    }

    void TBody::reset()
    {
        if ( m_bodyImplRef )
        {
            // reset the low-level resources through the adapter
            m_bodyImplRef->reset();

            // and grab the position|rotation information as well
            m_bodyImplRef->getPosition( m_pos );
            m_bodyImplRef->getRotation( m_rot );
            m_bodyImplRef->getTransform( m_tf );
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

    void TBody::setPosition( const TVec3& position )
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

    void TBody::setRotation( const TMat3& rotation )
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

    void TBody::setEuler( const TVec3& euler )
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

    void TBody::setQuaternion( const TVec4& quat )
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

    void TBody::setTransform( const TMat4& transform )
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

}
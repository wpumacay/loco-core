
#include <components/bodies.h>


namespace tysoc {

    TBody::TBody( const std::string& name,
                  const TBodyData& data,
                  const TVec3& position,
                  const TMat3& rotation )
    {
        m_name = name;
        m_data = data;
        m_bodyImpl = NULL;

        m_pos = m_pos0 = position;
        m_rot = m_rot0 = rotation;

        m_tf.setPosition( m_pos );
        m_tf.setRotation( m_rot );

        m_tf0.setPosition( m_pos0 );
        m_tf0.setRotation( m_rot0 );

        // Create collisions and visuals from the given data
        auto _collisionsData = m_data.collisions;
        for ( size_t i = 0; i < _collisionsData.size(); i++ )
        {
            auto _collisionName = m_name + "_col_" + std::to_string( i );
            auto _collisionPtr = new TCollision( _collisionName, _collisionsData[i] );
            addCollision( _collisionPtr );
        }

        auto _visualsData = m_data.visuals;
        for ( size_t i = 0; i < _visualsData.size(); i++ )
        {
            auto _visualName = m_name + "_viz_" + std::to_string( i );
            auto _visualPtr = new TVisual( _visualName, _visualsData[i] );
            addVisual( _visualPtr );
        }
    }

    TBody::~TBody()
    {
        if ( m_bodyImpl )
            delete m_bodyImpl;

        m_bodyImpl = NULL;

        for ( size_t i = 0; i < m_collisions.size(); i++ )
            delete m_collisions[i];
        m_collisions.clear();
        m_collisionsMap.clear();

        for ( size_t i = 0; i < m_visuals.size(); i++ )
            delete m_visuals[i];
        m_visuals.clear();
        m_visualsMap.clear();
    }

    void TBody::setAdapter( TIBodyAdapter* bodyImpl )
    {
        // in case there's a danling adapter from before, release its resources
        if ( m_bodyImpl )
            delete m_bodyImpl;

        // and assign the new adapter to be used
        m_bodyImpl = bodyImpl;
    }

    void TBody::addCollision( TCollision* collisionPtr )
    {
        if ( !collisionPtr )
            return;

        m_collisions.push_back( collisionPtr );
        m_collisionsMap[ collisionPtr->name() ] = collisionPtr;

        m_data.collisions.push_back( collisionPtr->data() );
        collisionPtr->setParentBody( this );
    }

    void TBody::addVisual( TVisual* visualPtr )
    {
        if ( !visualPtr )
            return;

        m_visuals.push_back( visualPtr );
        m_visualsMap[ visualPtr->name() ] = visualPtr;

        m_data.visuals.push_back( visualPtr->data() );
        visualPtr->setParentBody( this );
    }

    void TBody::update()
    {
        if ( m_bodyImpl )
        {
            // update the adapter to handle internal stuff
            m_bodyImpl->update();

            // grab the latest world-transform from the backend
            m_bodyImpl->getPosition( m_pos );
            m_bodyImpl->getRotation( m_rot );
            m_bodyImpl->getTransform( m_tf );
        }

        for ( size_t i = 0; i < m_collisions.size(); i++ )
            m_collisions[i]->update();

        for ( size_t i = 0; i < m_visuals.size(); i++ )
            m_visuals[i]->update();
    }

    void TBody::reset()
    {
        if ( m_bodyImpl )
        {
            // reset the low-level resources through the adapter
            m_bodyImpl->reset();

            // and grab the position|rotation information as well
            m_bodyImpl->getPosition( m_pos );
            m_bodyImpl->getRotation( m_rot );
            m_bodyImpl->getTransform( m_tf );
        }

        for ( size_t i = 0; i < m_collisions.size(); i++ )
        {
            m_collisions[i]->reset();
            m_collisions[i]->update();
        }

        for ( size_t i = 0; i < m_visuals.size(); i++ )
        {
            m_visuals[i]->reset();
            m_visuals[i]->update();
        }
    }

    void TBody::setPosition( const TVec3& position )
    {
        m_pos = position;
        m_tf.setPosition( m_pos );

        if ( m_bodyImpl )
            m_bodyImpl->setPosition( m_pos );

        for ( size_t i = 0; i < m_collisions.size(); i++ )
            m_collisions[i]->update();

        for ( size_t i = 0; i < m_visuals.size(); i++ )
            m_visuals[i]->update();
    }

    void TBody::setRotation( const TMat3& rotation )
    {
        m_rot = rotation;
        m_tf.setRotation( m_rot );

        if ( m_bodyImpl )
            m_bodyImpl->setRotation( m_rot );

        for ( size_t i = 0; i < m_collisions.size(); i++ )
            m_collisions[i]->update();

        for ( size_t i = 0; i < m_visuals.size(); i++ )
            m_visuals[i]->update();
    }

    void TBody::setEuler( const TVec3& euler )
    {
        m_rot = TMat3::fromEuler( euler );
        m_tf.setRotation( m_rot );

        if ( m_bodyImpl )
            m_bodyImpl->setRotation( m_rot );

        for ( size_t i = 0; i < m_collisions.size(); i++ )
            m_collisions[i]->update();

        for ( size_t i = 0; i < m_visuals.size(); i++ )
            m_visuals[i]->update();
    }

    void TBody::setQuaternion( const TVec4& quat )
    {
        m_rot = TMat3::fromQuaternion( quat );
        m_tf.setRotation( m_rot );

        if ( m_bodyImpl )
            m_bodyImpl->setRotation( m_rot );

        for ( size_t i = 0; i < m_collisions.size(); i++ )
            m_collisions[i]->update();

        for ( size_t i = 0; i < m_visuals.size(); i++ )
            m_visuals[i]->update();
    }

    void TBody::setTransform( const TMat4& transform )
    {
        m_tf = transform;
        m_pos = m_tf.getPosition();
        m_rot = m_tf.getRotation();

        if ( m_bodyImpl )
            m_bodyImpl->setTransform( m_tf );

        for ( size_t i = 0; i < m_collisions.size(); i++ )
            m_collisions[i]->update();

        for ( size_t i = 0; i < m_visuals.size(); i++ )
            m_visuals[i]->update();
    }

}
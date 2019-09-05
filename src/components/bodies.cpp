
#include <components/bodies.h>


namespace tysoc {

    TBody::TBody( const std::string& name,
                  const TBodyData& data )
    {
        m_name = name;
        m_data = data;
        m_bodyImpl = NULL;
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

    void TBody::setAdapter( adapters::TIBodyAdapter* bodyImpl )
    {
        // in case there's a danling adapter from before, release its resources
        if ( m_bodyImpl )
            delete m_bodyImpl;

        // and assign the new adapter to be used
        m_bodyImpl = bodyImpl;
    }

    void TBody::addCollision( TCollision* collisionPtr )
    {
        m_collisions.push_back( collisionPtr );
        m_collisionsMap[ collisionPtr->name() ] = collisionPtr;

        m_data.collisions.push_back( collisionPtr->data() );
    }

    void TBody::addVisual( TVisual* visualPtr )
    {
        m_visuals.push_back( visualPtr );
        m_visualsMap[ visualPtr->name() ] = visualPtr;

        m_data.visuals.push_back( visualPtr->data() );
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
            m_collisions[i]->reset();

        for ( size_t i = 0; i < m_visuals.size(); i++ )
            m_visuals[i]->reset();
    }

    void TBody::setPosition( const TVec3& position )
    {
        m_pos = position;

        if ( m_bodyImpl )
            m_bodyImpl->setPosition( m_pos );
    }

    void TBody::setRotation( const TMat3& rotation )
    {
        m_rot = rotation;

        if ( m_bodyImpl )
            m_bodyImpl->setRotation( m_rot );
    }

    void TBody::setTransform( const TMat4& transform )
    {
        m_tf = transform;

        if ( m_bodyImpl )
            m_bodyImpl->setTransform( m_tf );
    }

}
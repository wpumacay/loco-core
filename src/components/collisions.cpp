
#include <components/collisions.h>

namespace tysoc {

    TCollision::TCollision( const std::string& name,
                            const TCollisionData& collisionData )
    {
        m_name = name;
        m_data = collisionData;

        m_parentBodyPtr = NULL;
        m_collisionImplPtr = NULL;
        m_drawableImplPtr = NULL;
    }

    TCollision::~TCollision()
    {
        if ( m_drawableImplPtr )
            delete m_drawableImplPtr;

        if ( m_collisionImplPtr )
            delete m_collisionImplPtr;

        m_drawableImplPtr = NULL;
        m_collisionImplPtr = NULL;
    }

    void TCollision::setParentBody( TBody* parentBodyPtr )
    {
        m_parentBodyPtr = parentBodyPtr;
    }

    void TCollision::setAdapter( adapters::TICollisionAdapter* collisionImplPtr )
    {
        // if a previous adapter is present, release its resources
        if ( m_collisionImplPtr )
            delete m_collisionImplPtr;

        m_collisionImplPtr = collisionImplPtr;
    }

    void TCollision::setDrawable( viz::TIDrawable* drawablePtr )
    {
        // be nice and tell the drawable that it can be recycled for later usage (if applicable)
        if ( m_drawableImplPtr )
            m_drawableImplPtr->recycle();

        // change the reference to our new shiny drawable
        m_drawableImplPtr = drawablePtr;
    }

    void TCollision::show( bool visible )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->show( visible );
    }

    void TCollision::update()
    {
        // update internal stuff that might be required in the backend
        if ( m_collisionImplPtr )
            m_collisionImplPtr->update();

        // update our own transform using the world-transform from the parent
        assert( m_parentBodyPtr != NULL );
        m_tf = m_parentBodyPtr->tf() * m_localTf;
        m_pos = m_tf.getPosition();
        m_rot = m_tf.getRotation();

        // set the transform of the renderable to be our own world-transform
        if ( m_drawableImplPtr )
            m_drawableImplPtr->setWorldTransform( m_tf );
    }

    void TCollision::reset()
    {
        if ( m_collisionImplPtr )
            m_collisionImplPtr->reset();
    }

    void TCollision::setLocalPosition( const TVec3& localPosition )
    {
        m_localPos = localPosition;
        m_localTf.setPosition( m_localPos );

        if ( m_collisionImplPtr )
            m_collisionImplPtr->setLocalPosition( m_localPos );
    }

    void TCollision::setLocalRotation( const TMat3& localRotation )
    {
        m_localRot = localRotation;
        m_localTf.setRotation( m_localRot );

        if ( m_collisionImplPtr )
            m_collisionImplPtr->setLocalRotation( m_localRot );
    }

    void TCollision::setLocalTransform( const TMat4& localTransform )
    {
        m_localTf = localTransform;
        m_localPos = m_localTf.getPosition();
        m_localRot = m_localTf.getRotation();

        if ( m_collisionImplPtr )
            m_collisionImplPtr->setLocalTransform( m_localTf );
    }

    void TCollision::changeSize( const TVec3& newSize )
    {
        if ( m_collisionImplPtr )
            m_collisionImplPtr->changeSize( newSize );

        // change the collision-data size property
        m_data.size = newSize;

        // set the new size of the drawable resource
        if ( m_drawableImplPtr )
            m_drawableImplPtr->changeSize( newSize );
    }
}
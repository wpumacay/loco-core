
#include <components/collisions.h>


namespace tysoc {


    TCollision::TCollision( const std::string& name,
                            const TCollisionData& collisionData )
    {
        m_name = name;
        m_data = collisionData;
    }

    TCollision::~TCollision()
    {
        m_drawablePtr = NULL;
    }

    void TCollision::setDrawable( viz::TIDrawable* drawablePtr )
    {
        // be nice and tell the drawable that it can be recycled for later usage (if applicable)
        if ( m_drawablePtr )
            m_drawablePtr->recycle();

        // change the reference to our new shiny drawable
        m_drawablePtr = drawablePtr;
    }

    void TCollision::show( bool visible )
    {
        if ( m_drawablePtr )
            m_drawablePtr->show( visible );
    }

    void TCollision::build()
    {
        // @OVERRIDE: here we should create low-level resources from the backend
    }

    void TCollision::update()
    {
        // @OVERRIDE|EXTEND: override to make extra update functionality (don't forget to call base)

        // set the transform of the renderable to be our own world-transform
        if ( m_drawablePtr )
            m_drawablePtr->setWorldTransform( m_tf );
    }

    void TCollision::reset()
    {
        // @OVERRIDE: add extra reset-specific functionality if required
    }

    void TCollision::changeSize( const TVec3& newSize )
    {
        // @OVERRIDE|EXTEND: override to call low-level resize functionality (don't forget to call base)

        // change the collision-data size property
        m_data.size = newSize;

        // set the new size of the drawable resource
        if ( m_drawablePtr )
            m_drawablePtr->changeSize( newSize );
    }
}
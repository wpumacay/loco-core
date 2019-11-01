
#include <components/collisions.h>

namespace tysoc {

    TCollision::TCollision( const std::string& name,
                            const TCollisionData& collisionData )
    {
        m_name = name;
        m_data = collisionData;

        m_parentBodyPtr = nullptr;
        m_collisionImplPtr = nullptr;
        m_drawableImplPtr = nullptr;
    }

    TCollision::~TCollision()
    {
        if ( m_drawableImplPtr )
            delete m_drawableImplPtr;

        if ( m_collisionImplPtr )
            delete m_collisionImplPtr;

        m_drawableImplPtr = nullptr;
        m_collisionImplPtr = nullptr;
    }

    void TCollision::setParentBody( TBody* parentBodyPtr )
    {
        m_parentBodyPtr = parentBodyPtr;
    }

    void TCollision::setAdapter( TICollisionAdapter* collisionImplPtr )
    {
        // if a previous adapter is present, release its resources
        if ( m_collisionImplPtr )
            delete m_collisionImplPtr;

        m_collisionImplPtr = collisionImplPtr;
    }

    void TCollision::setDrawable( TIDrawable* drawablePtr )
    {
        // change the reference to our new shiny drawable
        m_drawableImplPtr = drawablePtr;
    }

    void TCollision::show( bool visible )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->show( visible );
    }

    void TCollision::wireframe( bool wireframe )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->wireframe( wireframe );
    }

    bool TCollision::isVisible()
    {
        if ( !m_drawableImplPtr )
            return false;

        return m_drawableImplPtr->isVisible();
    }

    bool TCollision::isWireframe()
    {
        if ( !m_drawableImplPtr )
            return false;

        return m_drawableImplPtr->isWireframe();
    }

    void TCollision::update()
    {
        // update internal stuff that might be required in the backend
        if ( m_collisionImplPtr )
            m_collisionImplPtr->update();

        // update our own transform using the world-transform from the parent
        assert( m_parentBodyPtr != nullptr );
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

    void TCollision::setLocalQuat( const TVec4& localQuaternion )
    {
        m_localRot = TMat3::fromQuaternion( localQuaternion );
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
        if ( m_data.type == eShapeType::MESH )
        {
            std::cout << "WARNING> changing mesh sizes at runtime is not supported, "
                      << "as it requires recomputing the vertex positions for this new size "
                      << "in various backend in some specific way. Please set the initial "
                      << "scale of the mesh first, which does the job during construction." << std::endl;
            return;
        }

        if ( m_data.type == eShapeType::HFIELD )
        {
            std::cout << "WARNING> changing hfield sizes at runtime is not supported (yet), "
                      << "as it requires recomputing the elevation data every time. For now, "
                      << "set the size properties of the hfield first, which will do the job, "
                      << "during construction" << std::endl;
            return;
        }

        // change the collision-data size property
        m_data.size = newSize;

        // tell the backend resource to change the size internally
        if ( m_collisionImplPtr )
            m_collisionImplPtr->changeSize( newSize );

        // tell the rendering resource to change the size internally
        if ( m_drawableImplPtr )
            m_drawableImplPtr->changeSize( newSize );
    }

    void TCollision::changeElevationData( const std::vector< float >& heightData )
    {
        if ( m_data.type != eShapeType::HFIELD )
        {
            std::cout << "WARNING> tried changing elevation data of a non-hfield collider" << std::endl;
            return;
        }

        // sanity check: make sure that both internal and given buffers have same num-elements
        if( ( m_data.hdata.nWidthSamples * m_data.hdata.nDepthSamples ) != heightData.size() )
        {
            std::cout << "WARNING> number of elements in internal and given elevation buffers does not match" << std::endl;
            std::cout << "nx-samples    : " << m_data.hdata.nWidthSamples << std::endl;
            std::cout << "ny-samples    : " << m_data.hdata.nDepthSamples << std::endl;
            std::cout << "hdata.size()  : " << heightData.size() << std::endl;
            return;
        }

        // change the internal elevation data
        m_data.hdata.heightData = heightData;

        // tell the backend resource to change the elevation data internally
        if ( m_collisionImplPtr )
            m_collisionImplPtr->changeElevationData( heightData );

        // tell the rendering resource to change the elevation data internally
        if ( m_drawableImplPtr )
            m_drawableImplPtr->changeElevationData( heightData );
    }
}
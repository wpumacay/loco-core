
#include <components/collisions.h>

namespace tysoc {

    TVisual::TVisual( const std::string& name,
                      const TVisualData& visualData )
    {
        m_name = name;
        m_data = visualData;

        m_parentBodyPtr = NULL;
        m_drawableImplPtr = NULL;
    }

    TVisual::~TVisual()
    {
        if ( m_drawableImplPtr )
            delete m_drawableImplPtr;

        m_drawableImplPtr = NULL;
    }

    void TVisual::setParentBody( TBody* parentBodyPtr )
    {
        m_parentBodyPtr = parentBodyPtr;
    }

    void TVisual::setDrawable( TIDrawable* drawablePtr )
    {
        // be nice and tell the drawable that it can be recycled for later usage (if applicable)
        if ( m_drawableImplPtr )
            m_drawableImplPtr->recycle();

        // change the reference to our new shiny drawable
        m_drawableImplPtr = drawablePtr;
    }

    void TVisual::show( bool visible )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->show( visible );
    }

    void TVisual::wireframe( bool wireframe )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->wireframe( wireframe );
    }

    bool TVisual::isVisible()
    {
        if ( !m_drawableImplPtr )
            return false;

        return m_drawableImplPtr->isVisible();
    }

    bool TVisual::isWireframe()
    {
        if ( !m_drawableImplPtr )
            return false;

        return m_drawableImplPtr->isWireframe();
    }

    void TVisual::update()
    {
        // update our own transform using the world-transform from the parent
        assert( m_parentBodyPtr != NULL );
        m_tf = m_parentBodyPtr->tf() * m_localTf;
        m_pos = m_tf.getPosition();
        m_rot = m_tf.getRotation();

        // set the transform of the renderable to be our own world-transform
        if ( m_drawableImplPtr )
            m_drawableImplPtr->setWorldTransform( m_tf );
    }

    void TVisual::reset()
    {
        // do nothing for now
    }

    void TVisual::setLocalPosition( const TVec3& localPosition )
    {
        m_localPos = localPosition;
        m_localTf.setPosition( m_localPos );
    }

    void TVisual::setLocalRotation( const TMat3& localRotation )
    {
        m_localRot = localRotation;
        m_localTf.setRotation( m_localRot );
    }

    void TVisual::setLocalQuat( const TVec4& localQuaternion )
    {
        m_localRot = TMat3::fromQuaternion( localQuaternion );
        m_localTf.setRotation( m_localRot );
    }

    void TVisual::setLocalTransform( const TMat4& localTransform )
    {
        m_localTf = localTransform;
        m_localPos = m_localTf.getPosition();
        m_localRot = m_localTf.getRotation();
    }

    void TVisual::changeSize( const TVec3& newSize )
    {
        if ( m_data.type == eShapeType::MESH )
        {
            std::cout << "WARNING> changing mesh sizes at runtime is not supported, "
                      << "as it requires recomputing the vertex positions for this new size "
                      << "in various backend in some specific way. Please set the initial "
                      << "scale of the mesh first, which does the job during construction." << std::endl;
            return;
        }

        // change the visual-data size property
        m_data.size = newSize;

        // set the new size of the drawable resource
        if ( m_drawableImplPtr )
            m_drawableImplPtr->changeSize( newSize );
    }

    void TVisual::changeColor( const TVec3& newFullColor )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->setColor( newFullColor );
    }

    void TVisual::changeAmbientColor( const TVec3& newAmbientColor )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->setAmbientColor( newAmbientColor );
    }

    void TVisual::changeDiffuseColor( const TVec3& newDiffuseColor )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->setDiffuseColor( newDiffuseColor );
    }

    void TVisual::changeSpecularColor( const TVec3& newSpecularColor )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->setSpecularColor( newSpecularColor );
    }

    void TVisual::changeShininess( const TScalar& shininess )
    {
        if ( m_drawableImplPtr )
            m_drawableImplPtr->setShininess( shininess );
    }

}
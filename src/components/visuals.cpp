
#include <components/collisions.h>

namespace tysoc {

    TVisual::TVisual( const std::string& name,
                      const TVisualData& visualData )
    {
        m_name = name;
        m_data = visualData;

        m_parentBodyPtr = nullptr;
        m_drawableImplPtr = nullptr;
    }

    TVisual::~TVisual()
    {
        if ( m_drawableImplPtr )
            delete m_drawableImplPtr;

        m_drawableImplPtr = nullptr;
    }

    void TVisual::setParentBody( TBody* parentBodyPtr )
    {
        m_parentBodyPtr = parentBodyPtr;
    }

    void TVisual::setDrawable( TIDrawable* drawablePtr )
    {
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
        assert( m_parentBodyPtr != nullptr );
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

        if ( m_data.type == eShapeType::HFIELD )
        {
            std::cout << "WARNING> changing hfield sizes at runtime is not supported (yet), "
                      << "as it requires recomputing the elevation data every time. For now, "
                      << "set the size properties of the hfield first, which will do the job, "
                      << "during construction" << std::endl;
            return;
        }

        // change the visual-data size property
        m_data.size = newSize;

        // set the new size of the drawable resource
        if ( m_drawableImplPtr )
            m_drawableImplPtr->changeSize( newSize );
    }

    void TVisual::changeElevationData( const std::vector< float >& heightData )
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

        // set the new elevation data of the drawable resource
        if ( m_drawableImplPtr )
            m_drawableImplPtr->changeElevationData( heightData );
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
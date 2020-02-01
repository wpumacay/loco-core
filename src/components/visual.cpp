
#include <components/collision.h>

namespace tysoc
{
    TVisual::TVisual( const std::string& name,
                      const TVisualData& visualData )
    {
        m_name = name;
        m_data = visualData;

        m_parentBodyRef = nullptr;
        m_drawableImplRef = nullptr;
    }

    TVisual::~TVisual()
    {
        // Clear references (owners are in charge or releasing them)
        m_parentBodyRef = nullptr;
        m_drawableImplRef = nullptr;
    }

    void TVisual::setParentBody( TIBody* parentBodyRef )
    {
        if ( !parentBodyRef )
            return;

        m_parentBodyRef = parentBodyRef;
        m_tf = m_parentBodyRef->tf() * m_localTf;

        if ( m_drawableImplRef )
            m_drawableImplRef->setWorldTransform( m_tf );
    }

    void TVisual::setDrawable( TIDrawable* drawableImplRef )
    {
        if ( !drawableImplRef )
            return;

        m_drawableImplRef = drawableImplRef;
        m_drawableImplRef->setWorldTransform( m_tf );
    }

    void TVisual::show( bool visible )
    {
        if ( m_drawableImplRef )
            m_drawableImplRef->show( visible );
    }

    void TVisual::wireframe( bool wireframe )
    {
        if ( m_drawableImplRef )
            m_drawableImplRef->wireframe( wireframe );
    }

    bool TVisual::isVisible() const
    {
        if ( !m_drawableImplRef )
            return false;

        return m_drawableImplRef->isVisible();
    }

    bool TVisual::isWireframe() const
    {
        if ( !m_drawableImplRef )
            return false;

        return m_drawableImplRef->isWireframe();
    }

    void TVisual::preStep()
    {
        // not much to do before a simulation step
    }

    void TVisual::postStep()
    {
        // update our own transform using the world-transform from the parent
        assert( m_parentBodyRef != nullptr );
        m_tf = m_parentBodyRef->tf() * m_localTf;

        // set the transform of the renderable to be our own world-transform
        if ( m_drawableImplRef )
            m_drawableImplRef->setWorldTransform( m_tf );
    }

    void TVisual::reset()
    {
        // nothing required for now
    }

    void TVisual::setLocalPosition( const TVec3& localPosition )
    {
        m_localTf.set( localPosition, 3 );
    }

    void TVisual::setLocalRotation( const TMat3& localRotation )
    {
        m_localTf.set( localRotation );
    }

    void TVisual::setLocalQuat( const TVec4& localQuaternion )
    {
        auto localRotation = tinymath::rotation( localQuaternion );
        m_localTf.set( localRotation );
    }

    void TVisual::setLocalEuler( const TVec3& localEuler )
    {
        auto localRotation = tinymath::rotation( localEuler );
        m_localTf.set( localRotation );
    }

    void TVisual::setLocalTransform( const TMat4& localTransform )
    {
        m_localTf = localTransform;
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
        if ( m_drawableImplRef )
            m_drawableImplRef->changeSize( newSize );
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
        if ( m_drawableImplRef )
            m_drawableImplRef->changeElevationData( heightData );
    }

    void TVisual::changeColor( const TVec3& newFullColor )
    {
        if ( m_drawableImplRef )
            m_drawableImplRef->setColor( newFullColor );
    }

    void TVisual::changeAmbientColor( const TVec3& newAmbientColor )
    {
        if ( m_drawableImplRef )
            m_drawableImplRef->setAmbientColor( newAmbientColor );
    }

    void TVisual::changeDiffuseColor( const TVec3& newDiffuseColor )
    {
        if ( m_drawableImplRef )
            m_drawableImplRef->setDiffuseColor( newDiffuseColor );
    }

    void TVisual::changeSpecularColor( const TVec3& newSpecularColor )
    {
        if ( m_drawableImplRef )
            m_drawableImplRef->setSpecularColor( newSpecularColor );
    }

    void TVisual::changeShininess( const TScalar& shininess )
    {
        if ( m_drawableImplRef )
            m_drawableImplRef->setShininess( shininess );
    }
}
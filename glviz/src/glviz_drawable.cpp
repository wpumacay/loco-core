
#include <glviz_drawable.h>

namespace tysoc {

    /***************************************************************************
    *                                                                          *
    *                           TGLDRAWABLE-IMPL                               *
    *                                                                          *
    ***************************************************************************/

    TGLDrawable::TGLDrawable( const TShapeData& shapeData )
    {
        m_scale         = { 1.0, 1.0, 1.0 };
        m_size          = shapeData.size;
        m_size0         = shapeData.size;
        m_type          = shapeData.type;
        m_data          = shapeData;
        m_renderablePtr = createRenderable( shapeData );
    }

    TGLDrawable::~TGLDrawable()
    {
        // @TODO: Implement free in our rendering backend
        // // Notify the owner that this resource can be freed now
        // if ( m_renderablePtr )
        //     m_renderablePtr->free();

        m_renderablePtr = nullptr;
    }

    void TGLDrawable::setWorldTransform( const TMat4& transform )
    {
        if ( !m_renderablePtr )
            return;

        auto _position = transform.getPosition();
        auto _rotation = transform.getRotation();

        auto _ePosition = fromTVec3( _position );
        auto _eRotation = fromTMat3( _rotation );

        m_renderablePtr->position = _ePosition;
        m_renderablePtr->rotation = _eRotation;
    }

    void TGLDrawable::setColor( const TVec3& fullColor )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->type() == engine::eRenderableType::MESH )
        {
            m_renderablePtr->material()->ambient = { fullColor.x, fullColor.y, fullColor.z };
            m_renderablePtr->material()->diffuse = { fullColor.x, fullColor.y, fullColor.z };
            m_renderablePtr->material()->specular = { fullColor.x, fullColor.y, fullColor.z };
        }
        else if ( m_renderablePtr->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( m_renderablePtr )->meshes();
            for ( auto _submesh : _submeshes )
            {
                _submesh->material()->ambient = { fullColor.x, fullColor.y, fullColor.z };
                _submesh->material()->diffuse = { fullColor.x, fullColor.y, fullColor.z };
                _submesh->material()->specular = { fullColor.x, fullColor.y, fullColor.z };
            }
        }
    }

    void TGLDrawable::setAmbientColor( const TVec3& ambientColor )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->type() == engine::eRenderableType::MESH )
        {
            m_renderablePtr->material()->ambient = fromTVec3( ambientColor );
        }
        else if ( m_renderablePtr->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( m_renderablePtr )->meshes();
            for ( auto _submesh : _submeshes )
                _submesh->material()->ambient = { ambientColor.x, ambientColor.y, ambientColor.z };
        }
    }

    void TGLDrawable::setDiffuseColor( const TVec3& diffuseColor )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->type() == engine::eRenderableType::MESH )
        {
            m_renderablePtr->material()->diffuse = fromTVec3( diffuseColor );
        }
        else if ( m_renderablePtr->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( m_renderablePtr )->meshes();
            for ( auto _submesh : _submeshes )
                _submesh->material()->diffuse = { diffuseColor.x, diffuseColor.y, diffuseColor.z };
        }
    }

    void TGLDrawable::setSpecularColor( const TVec3& specularColor )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->type() == engine::eRenderableType::MESH )
        {
            m_renderablePtr->material()->specular = fromTVec3( specularColor );
        }
        else if ( m_renderablePtr->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( m_renderablePtr )->meshes();
            for ( auto _submesh : _submeshes )
                _submesh->material()->specular = { specularColor.x, specularColor.y, specularColor.z };
        }
    }

    void TGLDrawable::setShininess( const TScalar& shininess )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->type() == engine::eRenderableType::MESH )
        {
            m_renderablePtr->material()->shininess = shininess;
        }
        else if ( m_renderablePtr->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( m_renderablePtr )->meshes();
            for ( auto _submesh : _submeshes )
                _submesh->material()->shininess = shininess;
        }
    }

    void TGLDrawable::setTexture( const std::string& name )
    {
        if ( !engine::CTextureManager::HasCachedTexture( name ) )
            return;

        auto _texture = engine::CTextureManager::GetCachedTexture( name );

        if ( m_renderablePtr->type() == engine::eRenderableType::MESH )
        {
            m_renderablePtr->material()->setAlbedoMap( _texture );
        }
        else if ( m_renderablePtr->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( m_renderablePtr )->meshes();
            for ( auto _submesh : _submeshes )
                _submesh->material()->setAlbedoMap( _texture );
        }
    }

    void TGLDrawable::changeSize( const tysoc::TVec3& newSize )
    {
        m_size = newSize;
        if ( m_type == eShapeType::PLANE )
        {
            m_scale.x = m_size.x / m_size0.x;
            m_scale.y = m_size.y / m_size0.y;
        }
        else if ( m_type == eShapeType::BOX )
        {
            m_scale = { m_size.x / m_size0.x,
                        m_size.y / m_size0.y,
                        m_size.z / m_size0.z };
        }
        else if ( m_type == eShapeType::SPHERE )
        {
            // scale according to radius
            m_scale.x = m_scale.y = m_scale.z = m_size.x / m_size0.x;
        }
        else if ( m_type == eShapeType::CYLINDER )
        {
            // scale according to radius
            m_scale.x = m_size.x / m_size0.x;
            m_scale.y = m_size.x / m_size0.x;
            // scale according to height
            m_scale.z = m_size.y / m_size0.y;
        }
        else if ( m_type == eShapeType::CAPSULE )
        {
            // scale according to radius
            m_scale.x = m_size.x / m_size0.x;
            m_scale.y = m_size.x / m_size0.x;
            // scale according to height
            m_scale.z = m_size.y / m_size0.y;
        }
        else if ( m_type == eShapeType::MESH )
        {
            // scale every dimension
            m_scale.x = m_size.x / m_size0.x;
            m_scale.y = m_size.y / m_size0.y;
            m_scale.z = m_size.z / m_size0.z;
        }
        else if ( m_type == eShapeType::HFIELD )
        {
            // not allowed, use changeElevationData instead
            std::cout << "WARNING> tried changing the scale of the drawable for a hfield. Should "
                      << "change the elevation data instead" << std::endl;
        }

        if ( !m_renderablePtr )
            return;

        m_renderablePtr->scale = fromTVec3( m_scale );
    }

    void TGLDrawable::changeElevationData( const std::vector< float >& heightData )
    {
        if ( m_data.type != eShapeType::HFIELD )
        {
            std::cout << "WARNING> tried changing gldrawable's elevation data, even though "
                      << "its not a hfield shape" << std::endl;
            return;
        }

        if( ( m_data.hdata.nWidthSamples * m_data.hdata.nDepthSamples ) != heightData.size() )
        {
            std::cout << "WARNING> number of elements in internal and given elevation buffers does not match" << std::endl;
            std::cout << "nx-samples    : " << m_data.hdata.nWidthSamples << std::endl;
            std::cout << "ny-samples    : " << m_data.hdata.nDepthSamples << std::endl;
            std::cout << "hdata.size()  : " << heightData.size() << std::endl;
            return;
        }

        // @TODO: Should add support for dynamic buffers, so for now just destroy the 
        //        renderable and create a new one :(
        delete m_renderablePtr;

        m_data.hdata.heightData = heightData;
        m_renderablePtr = createRenderable( m_data );
    }

    void TGLDrawable::show( bool visible )
    {
        if ( !m_renderablePtr )
            return;

        m_renderablePtr->setVisibility( visible );
        if ( m_renderablePtr->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( m_renderablePtr )->meshes();
            for ( auto _submesh : _submeshes )
                _submesh->setVisibility( visible );
        }
    }

    void TGLDrawable::wireframe( bool wireframe )
    {
        if ( !m_renderablePtr )
            return;

        m_renderablePtr->setWireframe( wireframe );
        if ( m_renderablePtr->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( m_renderablePtr )->meshes();
            for ( auto _submesh : _submeshes )
                _submesh->setWireframe( wireframe );
        }

    }

    bool TGLDrawable::isVisible()
    {
        if ( !m_renderablePtr )
            return false;

        return m_renderablePtr->visible();
    }

    bool TGLDrawable::isWireframe()
    {
        if ( !m_renderablePtr )
            return false;

        return m_renderablePtr->wireframe();
    }

    extern "C" TIDrawable* visualizer_createDrawable( const TShapeData& data )
    {
        return new TGLDrawable( data );
    }

}
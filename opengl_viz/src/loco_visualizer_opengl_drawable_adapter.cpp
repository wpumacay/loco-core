
#include <loco_visualizer_opengl_drawable_adapter.h>

namespace loco
{
    TOpenGLDrawableAdapter::TOpenGLDrawableAdapter( TObject* owner_ref,
                                                    const TShapeData& shapeData,
                                                    engine::CIRenderable* gl_renderableRef )
        : TIDrawableAdapter( owner_ref )
    {
        m_Scale = { 1.0, 1.0, 1.0 };
        m_Size  = shapeData.size;
        m_Size0 = shapeData.size;
        m_data  = shapeData;

        m_AwaitingDeletion = false;
        m_glRenderableRef = gl_renderableRef;
    }

    TOpenGLDrawableAdapter::~TOpenGLDrawableAdapter()
    {
        if ( m_OwnerRef )
            m_OwnerRef->DetachViz();

        m_OwnerRef = nullptr;
        m_glRenderableRef = nullptr;
    }

    void TOpenGLDrawableAdapter::SetTransform( const TMat4& transform )
    {
        if ( !m_glRenderableRef )
            return;

        m_glRenderableRef->position = TVec3( transform.col( 3 ) );
        m_glRenderableRef->rotation = TMat3( transform );
    }

    void TOpenGLDrawableAdapter::SetColor( const TVec3& fullColor )
    {
        if ( !m_glRenderableRef )
            return;

        if ( m_glRenderableRef->type() == engine::eRenderableType::MESH )
        {
            m_glRenderableRef->material()->ambient = fullColor;
            m_glRenderableRef->material()->diffuse = fullColor;
            m_glRenderableRef->material()->specular = fullColor;
        }
        else if ( m_glRenderableRef->type() == engine::eRenderableType::MODEL )
        {
            auto submeshes = dynamic_cast<engine::CModel*>( m_glRenderableRef )->meshes();
            for ( auto submesh : submeshes )
            {
                submesh->material()->ambient = fullColor;
                submesh->material()->diffuse = fullColor;
                submesh->material()->specular = fullColor;
            }
        }
    }

    void TOpenGLDrawableAdapter::SetAmbientColor( const TVec3& ambientColor )
    {
        if ( !m_glRenderableRef )
            return;

        if ( m_glRenderableRef->type() == engine::eRenderableType::MESH )
        {
            m_glRenderableRef->material()->ambient = ambientColor;
        }
        else if ( m_glRenderableRef->type() == engine::eRenderableType::MODEL )
        {
            auto submeshes = dynamic_cast<engine::CModel*>( m_glRenderableRef )->meshes();
            for ( auto submesh : submeshes )
                submesh->material()->ambient = ambientColor;
        }
    }

    void TOpenGLDrawableAdapter::SetDiffuseColor( const TVec3& diffuseColor )
    {
        if ( !m_glRenderableRef )
            return;

        if ( m_glRenderableRef->type() == engine::eRenderableType::MESH )
        {
            m_glRenderableRef->material()->diffuse = diffuseColor;
        }
        else if ( m_glRenderableRef->type() == engine::eRenderableType::MODEL )
        {
            auto submeshes = dynamic_cast<engine::CModel*>( m_glRenderableRef )->meshes();
            for ( auto submesh : submeshes )
                submesh->material()->diffuse = diffuseColor;
        }
    }

    void TOpenGLDrawableAdapter::SetSpecularColor( const TVec3& specularColor )
    {
        if ( !m_glRenderableRef )
            return;

        if ( m_glRenderableRef->type() == engine::eRenderableType::MESH )
        {
            m_glRenderableRef->material()->specular = specularColor;
        }
        else if ( m_glRenderableRef->type() == engine::eRenderableType::MODEL )
        {
            auto submeshes = dynamic_cast<engine::CModel*>( m_glRenderableRef )->meshes();
            for ( auto submesh : submeshes )
                submesh->material()->specular = specularColor;
        }
    }

    void TOpenGLDrawableAdapter::SetShininess( float shininess )
    {
        if ( !m_glRenderableRef )
            return;

        if ( m_glRenderableRef->type() == engine::eRenderableType::MESH )
        {
            m_glRenderableRef->material()->shininess = shininess;
        }
        else if ( m_glRenderableRef->type() == engine::eRenderableType::MODEL )
        {
            auto submeshes = dynamic_cast<engine::CModel*>( m_glRenderableRef )->meshes();
            for ( auto submesh : submeshes )
                submesh->material()->shininess = shininess;
        }
    }

    void TOpenGLDrawableAdapter::SetTexture( const std::string& textureName )
    {
        if ( !m_glRenderableRef )
            return;

        engine::CTexture* textureRef = nullptr;
        if ( engine::CTextureManager::HasCachedTexture( textureName ) )
        {
            textureRef = engine::CTextureManager::GetCachedTexture( textureName );
        }
        else if ( ( textureName.find( ".png" ) != std::string::npos ) ||
                  ( textureName.find( ".jpg" ) != std::string::npos ) ||
                  ( textureName.find( ".jpeg" ) != std::string::npos ) )
        {
            // Try to load the texture if has an extension (user might have wanted to load a texture)
            engine::CTextureOptions texOptions;
            texOptions.filterMin        = engine::eTextureFilter::LINEAR;
            texOptions.filterMag        = engine::eTextureFilter::LINEAR;
            texOptions.wrapU            = engine::eTextureWrap::REPEAT;
            texOptions.wrapV            = engine::eTextureWrap::REPEAT;
            texOptions.borderColorU     = { 0.0f, 0.0f, 0.0f, 1.0f };
            texOptions.borderColorV     = { 0.0f, 0.0f, 0.0f, 1.0f };
            texOptions.dtype            = engine::ePixelDataType::UINT_8;
            const bool flipVertically   = true;

            textureRef = engine::CTextureManager::LoadTexture( textureName, texOptions, flipVertically );
        }
        
        if ( !textureRef )
            LOCO_CORE_ERROR( "TOpenGLDrawableAdapter::SetTexture >>> couldn't load texture with name {0}", textureName );

        if ( m_glRenderableRef->type() == engine::eRenderableType::MESH )
        {
            m_glRenderableRef->material()->setAlbedoMap( textureRef );
        }
        else if ( m_glRenderableRef->type() == engine::eRenderableType::MODEL )
        {
            auto submeshes = dynamic_cast<engine::CModel*>( m_glRenderableRef )->meshes();
            for ( auto submesh : submeshes )
                submesh->material()->setAlbedoMap( textureRef );
        }
    }

    void TOpenGLDrawableAdapter::ChangeSize( const TVec3& new_size )
    {
        if ( !m_glRenderableRef )
            return;

        m_Size = new_size;
        switch ( m_data.type )
        {
            case eShapeType::PLANE :
            {
                m_Scale.x() = m_Size.x() / m_Size0.x();
                m_Scale.y() = m_Size.y() / m_Size0.y();
                break;
            }
            case eShapeType::BOX :
            {
                m_Scale = { m_Size.x() / m_Size0.x(),
                            m_Size.y() / m_Size0.y(),
                            m_Size.z() / m_Size0.z() };
                break;
            }
            case eShapeType::SPHERE :
            {
                // scale according to radius
                m_Scale.x() = m_Scale.y() = m_Scale.z() = m_Size.x() / m_Size0.x();
                break;
            }
            case eShapeType::CYLINDER :
            {
                // scale according to radius
                m_Scale.x() = m_Size.x() / m_Size0.x();
                m_Scale.y() = m_Size.x() / m_Size0.x();
                // scale according to height
                m_Scale.z() = m_Size.y() / m_Size0.y();
                break;
            }
            case eShapeType::CAPSULE :
            {
                // scale according to radius
                m_Scale.x() = m_Size.x() / m_Size0.x();
                m_Scale.y() = m_Size.x() / m_Size0.x();
                // scale according to height
                m_Scale.z() = m_Size.y() / m_Size0.y();
                break;
            }
            case eShapeType::ELLIPSOID :
            {
                // scale every dimension
                m_Scale.x() = m_Size.x() / m_Size0.x();
                m_Scale.y() = m_Size.y() / m_Size0.y();
                m_Scale.z() = m_Size.z() / m_Size0.z();
                break;
            }
            case eShapeType::MESH :
            {
                // scale every dimension
                m_Scale.x() = m_Size.x() / m_Size0.x();
                m_Scale.y() = m_Size.y() / m_Size0.y();
                m_Scale.z() = m_Size.z() / m_Size0.z();
                break;
            }
            case eShapeType::HFIELD :
            {
                LOCO_CORE_WARN( "TOpenGLDrawableAdapter::ChangeSize >>> Hfield shapes don't support changing the scale. Change the elevation data instead" );
                break;
            }
        }

        m_glRenderableRef->scale = m_Scale;
    }

    void TOpenGLDrawableAdapter::ChangeElevationData( const std::vector< float >& heightData )
    {
        if ( !m_glRenderableRef )
            return;

        LOCO_CORE_WARN( "TOpenGLDrawableAdapter::ChangeElevationData >>> not supported yet. Requires to construct a new renderable" );
    }

    void TOpenGLDrawableAdapter::SetVisible( bool visible )
    {
        if ( !m_glRenderableRef )
            return;

        m_glRenderableRef->setVisibility( visible );
    }

    void TOpenGLDrawableAdapter::SetWireframe( bool wireframe )
    {
        if ( !m_glRenderableRef )
            return;

        m_glRenderableRef->setWireframe( wireframe );
    }

    void TOpenGLDrawableAdapter::OnDetach()
    {
        m_OwnerRef = nullptr;
        m_AwaitingDeletion = true;

        if ( m_glRenderableRef )
            m_glRenderableRef->setVisibility( false );
    }
}
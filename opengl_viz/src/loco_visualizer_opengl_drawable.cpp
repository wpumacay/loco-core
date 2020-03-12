
#include <loco_visualizer_opengl_drawable.h>

namespace loco
{
    TOpenGLDrawableAdapter::TOpenGLDrawableAdapter( const TShapeData& shapeData,
                                                    engine::CIRenderable* gl_renderableRef )
        : TIDrawable()
    {
        m_scale = { 1.0, 1.0, 1.0 };
        m_size  = shapeData.size;
        m_size0 = shapeData.size;
        m_type  = shapeData.type;
        m_data  = shapeData;

        m_awaitingDeletion = false;
        m_glRenderableRef = gl_renderableRef;
    }

    TOpenGLDrawableAdapter::~TOpenGLDrawableAdapter()
    {
        m_glRenderableRef = nullptr;
    }

    void TOpenGLDrawableAdapter::SetWorldTransform( const TMat4& transform )
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

    void TOpenGLDrawableAdapter::ChangeSize( const TVec3& newSize )
    {
        if ( !m_glRenderableRef )
            return;

        m_size = newSize;
        if ( m_type == eShapeType::PLANE )
        {
            m_scale.x() = m_size.x() / m_size0.x();
            m_scale.y() = m_size.y() / m_size0.y();
        }
        else if ( m_type == eShapeType::BOX )
        {
            m_scale = { m_size.x() / m_size0.x(),
                        m_size.y() / m_size0.y(),
                        m_size.z() / m_size0.z() };
        }
        else if ( m_type == eShapeType::SPHERE )
        {
            // scale according to radius
            m_scale.x() = m_scale.y() = m_scale.z() = m_size.x() / m_size0.x();
        }
        else if ( m_type == eShapeType::CYLINDER )
        {
            // scale according to radius
            m_scale.x() = m_size.x() / m_size0.x();
            m_scale.y() = m_size.x() / m_size0.x();
            // scale according to height
            m_scale.z() = m_size.y() / m_size0.y();
        }
        else if ( m_type == eShapeType::CAPSULE )
        {
            // scale according to radius
            m_scale.x() = m_size.x() / m_size0.x();
            m_scale.y() = m_size.x() / m_size0.x();
            // scale according to height
            m_scale.z() = m_size.y() / m_size0.y();
        }
        else if ( m_type == eShapeType::ELLIPSOID )
        {
            // scale every dimension
            m_scale.x() = m_size.x() / m_size0.x();
            m_scale.y() = m_size.y() / m_size0.y();
            m_scale.z() = m_size.z() / m_size0.z();
        }
        else if ( m_type == eShapeType::MESH )
        {
            // scale every dimension
            m_scale.x() = m_size.x() / m_size0.x();
            m_scale.y() = m_size.y() / m_size0.y();
            m_scale.z() = m_size.z() / m_size0.z();
        }
        else if ( m_type == eShapeType::HFIELD )
        {
            LOCO_CORE_WARN( "TOpenGLDrawableAdapter::ChangeSize >>> Hfield shapes don't support changing the scale. Change the elevation data instead" );
        }

        m_glRenderableRef->scale = m_scale;
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
        m_awaitingDeletion = true;

        if ( m_glRenderableRef )
            m_glRenderableRef->setVisibility( false );
    }
}

#include <loco_visualizer_opengl_drawable_adapter.h>

namespace loco {
namespace visualizer {

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
                LOCO_CORE_ASSERT( m_glRenderableRef->type() == engine::CModel::GetStaticType(),
                                  "TOpenGLDrawableAdapter::ChangeSize >>> gl-renderable should be of type MODEL" );
                auto capsule_model = static_cast<engine::CModel*>( m_glRenderableRef );
                auto capsule_meshes = capsule_model->meshes();
                // First submesh is the cylinder, so just scale it appropriately as above
                auto cylinder_mesh = capsule_meshes[0];
                cylinder_mesh->scale.x() = m_Size.x() / m_Size0.x();
                cylinder_mesh->scale.y() = m_Size.x() / m_Size0.x();
                cylinder_mesh->scale.z() = m_Size.y() / m_Size0.y();
                // Second mesh is the top-sphere, so just update its radius and relative-transform appropriately
                auto top_sphere = capsule_meshes[1];
                top_sphere->scale.x() = m_Size.x() / m_Size0.x();
                top_sphere->scale.y() = m_Size.x() / m_Size0.x();
                top_sphere->scale.z() = m_Size.x() / m_Size0.x();
                capsule_model->localTransforms()[1].set( engine::CVec3( 0.0f, 0.0f, 0.5f * m_Size.y() ), 3 );
                // Thrid mesh is the bottom sphere, so just update its radius and relative-transform appropriately
                auto bottom_sphere = capsule_meshes[2];
                bottom_sphere->scale.x() = m_Size.x() / m_Size0.x();
                bottom_sphere->scale.y() = m_Size.x() / m_Size0.x();
                bottom_sphere->scale.z() = m_Size.x() / m_Size0.x();
                capsule_model->localTransforms()[2].set( engine::CVec3( 0.0f, 0.0f, -0.5f * m_Size.y() ), 3 );
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
            case eShapeType::CONVEX_MESH :
            {
                // scale every dimension
                m_Scale.x() = m_Size.x();
                m_Scale.y() = m_Size.y();
                m_Scale.z() = m_Size.z();
                // update old size(scale) to new size
                m_Size0 = m_Size;
                break;
            }
            case eShapeType::HEIGHTFIELD :
            {
                LOCO_CORE_WARN( "TOpenGLDrawableAdapter::ChangeSize >>> Hfield shapes don't support changing the scale. Change the elevation data instead" );
                break;
            }
        }

        m_glRenderableRef->scale = m_Scale;
    }

    void TOpenGLDrawableAdapter::ChangeVertexData( const std::vector<float>& vertices, const std::vector<int>& faces )
    {
        if ( !m_glRenderableRef )
            return;

        if ( vertices.size() % 3 != 0 )
            LOCO_CORE_ERROR( "TOpenGLDrawableAdapter::ChangeVertexData >>> there must be 3 elements per vertex \
                              (while trying to change vertex-data for drawable {0})", m_OwnerRef->name() );
        if ( faces.size() % 3 != 0 )
            LOCO_CORE_ERROR( "TOpenGLDrawableAdapter::ChangeVertexData >>> there must be 3 indices per face \
                              (while trying to change vertex-data for drawable {0})", m_OwnerRef->name() );

        const ssize_t num_faces = faces.size() / 3;
        const ssize_t num_vertices = num_faces * 3;
        const ssize_t num_vertices_unique = vertices.size() / 3;
        std::vector<engine::CVec3> new_vertices_unique( num_vertices_unique );
        std::vector<engine::CVec3> new_vertices( num_vertices );
        std::vector<engine::CVec3> new_normals( num_vertices );
        std::vector<engine::CVec2> new_texCoords( num_vertices );
        std::vector<engine::CInd3> new_faces( num_faces );

        loco::visualizer::CreateMeshVertexDataFromUserData( vertices, faces,
                                                            new_vertices_unique,
                                                            new_vertices,
                                                            new_normals,
                                                            new_texCoords,
                                                            new_faces );

        if ( auto gl_mesh = dynamic_cast<engine::CMesh*>( m_glRenderableRef ) )
        {
            gl_mesh->vertices() = new_vertices;
            gl_mesh->normals() = new_normals;
            gl_mesh->uvs() = new_texCoords;
            gl_mesh->indices() = new_faces;
            auto& gl_mesh_vao = gl_mesh->vertexArray();
            // Resize the buffers, as the amount of data might be different
            gl_mesh_vao->vertexBuffers()[0]->resize( sizeof( engine::CVec3 ) * new_vertices.size() );
            gl_mesh_vao->vertexBuffers()[1]->resize( sizeof( engine::CVec3 ) * new_normals.size() );
            gl_mesh_vao->vertexBuffers()[2]->resize( sizeof( engine::CVec2 ) * new_texCoords.size() );
            gl_mesh_vao->indexBuffer()->resize( 3 * new_faces.size() );
            // Update the buffers with the new vertex data
            gl_mesh_vao->vertexBuffers()[0]->updateData( sizeof( engine::CVec3 ) * new_vertices.size(), 
                                                         (engine::float32*)new_vertices.data() );
            gl_mesh_vao->vertexBuffers()[1]->updateData( sizeof( engine::CVec3 ) * new_normals.size(),
                                                         (engine::float32*)new_normals.data() );
            gl_mesh_vao->vertexBuffers()[2]->updateData( sizeof( engine::CVec2 ) * new_texCoords.size(),
                                                         (engine::float32*)new_texCoords.data() );
            gl_mesh_vao->indexBuffer()->updateData( 3 * new_faces.size(),
                                                    (engine::uint32*)new_faces.data() );
        }
    }

    void TOpenGLDrawableAdapter::ChangeElevationData( const std::vector< float >& heightData )
    {
        if ( !m_glRenderableRef )
            return;

        // Unnormalize the heights data (use data.size.z() as scaler)
        auto heightsScaled = heightData;
        for ( size_t i = 0; i < heightsScaled.size(); i++ )
            heightsScaled[i] *= m_data.size.z();

        std::vector<engine::CVec3> new_vertices;
        std::vector<engine::CVec3> new_normals;
        std::vector<engine::CVec2> new_texCoords;
        std::vector<engine::CInd3> new_indices;
        float max_height = -1e6f; float min_height = 1e6f;
        engine::CMeshBuilder::hfieldCreateVertexData( m_data.hfield_data.nWidthSamples,
                                                      m_data.hfield_data.nDepthSamples,
                                                      m_data.size.x(), m_data.size.y(),
                                                      m_data.size.x() / 2.0f,
                                                      m_data.size.y() / 2.0f,
                                                      heightsScaled,
                                                      loco::visualizer::HFIELD_HEIGHT_BASE,
                                                      engine::eAxis::Z,
                                                      new_vertices, new_normals, new_texCoords, new_indices,
                                                      max_height, min_height );

        if ( auto gl_hfield_mesh = dynamic_cast<engine::CMesh*>( m_glRenderableRef ) )
        {
            gl_hfield_mesh->vertices() = new_vertices;
            gl_hfield_mesh->normals() = new_normals;
            gl_hfield_mesh->uvs() = new_texCoords;
            gl_hfield_mesh->indices() = new_indices;
            auto& gl_hfield_vao = gl_hfield_mesh->vertexArray();
            gl_hfield_vao->vertexBuffers()[0]->updateData( sizeof( engine::CVec3 ) * new_vertices.size(), 
                                                           (engine::float32*)new_vertices.data() );
            gl_hfield_vao->vertexBuffers()[1]->updateData( sizeof( engine::CVec3 ) * new_normals.size(),
                                                           (engine::float32*)new_normals.data() );
            gl_hfield_vao->vertexBuffers()[2]->updateData( sizeof( engine::CVec2 ) * new_texCoords.size(),
                                                           (engine::float32*)new_texCoords.data() );
            gl_hfield_vao->indexBuffer()->updateData( 3 * new_indices.size(),
                                                      (engine::uint32*)new_indices.data() );
        }
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
}}
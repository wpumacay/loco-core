#pragma once

#include <visualizer/loco_visualizer_drawable_adapter.h>
#include <loco_visualizer_opengl_common.h>

namespace loco
{
    class TOpenGLDrawableAdapter : public TIDrawableAdapter
    {
    public :

        TOpenGLDrawableAdapter( TObject* owner_ref,
                                const TShapeData& data,
                                engine::CIRenderable* gl_renderableRef );

        ~TOpenGLDrawableAdapter();

        void SetTransform( const TMat4& transform ) override;

        void SetColor( const TVec3& fullColor ) override;

        void SetAmbientColor( const TVec3& ambientColor ) override;

        void SetDiffuseColor( const TVec3& diffuseColor ) override;

        void SetSpecularColor( const TVec3& specularColor ) override;

        void SetShininess( float shininess ) override;

        void SetTexture( const std::string& textureName ) override;

        void ChangeSize( const TVec3& newSize ) override;

        void ChangeVertexData( const std::vector<float>& vertices, const std::vector<int>& faces ) override;

        void ChangeElevationData( const std::vector< float >& heights ) override;

        void SetVisible( bool visible ) override;

        void SetWireframe( bool wireframe ) override;

        void OnDetach() override;

    private :

        engine::CIRenderable* m_glRenderableRef;

        TShapeData m_data;
    };
}
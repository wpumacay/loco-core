#pragma once

#include <visualizer/loco_drawable.h>
#include <loco_visualizer_opengl_common.h>

namespace loco
{
    class TOpenGLDrawableAdapter : public TIDrawable
    {
    public :

        TOpenGLDrawableAdapter( const TShapeData& data,
                                engine::CIRenderable* gl_renderableRef );

        ~TOpenGLDrawableAdapter();

        void SetWorldTransform( const TMat4& transform ) override;

        void SetColor( const TVec3& fullColor ) override;

        void SetAmbientColor( const TVec3& ambientColor ) override;

        void SetDiffuseColor( const TVec3& diffuseColor ) override;

        void SetSpecularColor( const TVec3& specularColor ) override;

        void SetShininess( float shininess ) override;

        void SetTexture( const std::string& textureName ) override;

        void ChangeSize( const TVec3& newSize ) override;

        void ChangeElevationData( const std::vector< float >& heightData ) override;

        void SetVisible( bool visible ) override;

        void SetWireframe( bool wireframe ) override;

        void Detach() override;

    private :

        engine::CIRenderable* m_glRenderableRef;

        TShapeData m_data;
    };
}
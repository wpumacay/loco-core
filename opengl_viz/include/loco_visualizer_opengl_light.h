#pragma once

#include <visualizer/loco_visualizer_light.h>
#include <loco_visualizer_opengl_common.h>

namespace loco {
namespace visualizer {

    class TOpenGLLightAdapter : public TVizLightAdapter
    {
    public :

        TOpenGLLightAdapter( TVizLight* lightRef, engine::CILight* gl_lightRef );

        ~TOpenGLLightAdapter();

        void Build() override;
        void Detach() override;
        void Reset() override;

        void ChangeAmbientColor( const TVec3& ambientColor ) override;
        void ChangeDiffuseColor( const TVec3& diffuseColor ) override;
        void ChangeSpecularColor( const TVec3& specularColor ) override;
        void ChangeIntensity( float intensity ) override;

        void SetPosition( const TVec3& position ) override;
        void SetDirection( const TVec3& direction ) override;
        void SetCastShadows( bool castShadows ) override;

        void ChangeAttenuationK0( float attnk0 ) override;
        void ChangeAttenuationK1( float attnk1 ) override;
        void ChangeAttenuationK2( float attnk2 ) override;
        void ChangeInnerCutoff( float innerCutoff ) override;
        void ChangeOuterCutoff( float outerCutoff ) override;

        engine::CILight* gl_light() { return m_glLightRef; }
        const engine::CILight* gl_light() const { return m_glLightRef; }

    private :

        engine::CILight* m_glLightRef;
    };
}}
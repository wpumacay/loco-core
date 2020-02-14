
#include <loco_visualizer_opengl_light.h>

namespace loco
{
    TOpenGLLightAdapter::TOpenGLLightAdapter( TVizLight* lightRef, engine::CILight* gl_lightRef )
        : TVizLightAdapter( lightRef )
    {
        m_glLightRef = gl_lightRef;
    }

    TOpenGLLightAdapter::~TOpenGLLightAdapter()
    {
        m_glLightRef = nullptr;
    }

    void TOpenGLLightAdapter::Build()
    {
        // Nothing extra to build (resources created by visualizer and given as reference)
    }

    void TOpenGLLightAdapter::Detach()
    {
        m_awaitingDeletion = true;
    }

    void TOpenGLLightAdapter::Reset()
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->position = m_lightRef->position();
        m_glLightRef->direction = m_lightRef->direction();
    }

    void TOpenGLLightAdapter::ChangeAmbientColor( const TVec3& ambientColor )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->ambient = ambientColor;
    }

    void TOpenGLLightAdapter::ChangeDiffuseColor( const TVec3& diffuseColor )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->diffuse = diffuseColor;
    }

    void TOpenGLLightAdapter::ChangeSpecularColor( const TVec3& specularColor )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->specular = specularColor;
    }

    void TOpenGLLightAdapter::ChangeIntensity( float intensity )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->intensity = intensity;
    }


    void TOpenGLLightAdapter::SetPosition( const TVec3& position )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->position = position;
    }

    void TOpenGLLightAdapter::SetDirection( const TVec3& direction )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->direction = direction;
    }

    void TOpenGLLightAdapter::SetCastShadows( bool castShadows )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->producesShadows = castShadows;
    }

    void TOpenGLLightAdapter::ChangeAttenuationK0( float attnk0 )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->atnConstant = attnk0;
    }

    void TOpenGLLightAdapter::ChangeAttenuationK1( float attnk1 )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->atnLinear = attnk1;
    }

    void TOpenGLLightAdapter::ChangeAttenuationK2( float attnk2 )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->atnQuadratic = attnk2;
    }

    void TOpenGLLightAdapter::ChangeInnerCutoff( float innerCutoff )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->innerCutoff = innerCutoff;
    }

    void TOpenGLLightAdapter::ChangeOuterCutoff( float outerCutoff )
    {
        if ( !m_glLightRef )
            return;

        m_glLightRef->outerCutoff = outerCutoff;
    }
}
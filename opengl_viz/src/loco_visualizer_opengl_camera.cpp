
#include <loco_visualizer_opengl_camera.h>

namespace loco {
namespace visualizer {

    TOpenGLCameraAdapter::TOpenGLCameraAdapter( TVizCamera* cameraRef, engine::CICamera* gl_cameraRef )
        : TVizCameraAdapter( cameraRef )
    {
        m_glCameraRef = gl_cameraRef;
    }

    TOpenGLCameraAdapter::~TOpenGLCameraAdapter()
    {
        m_glCameraRef = nullptr;
    }

    void TOpenGLCameraAdapter::Build()
    {
        // Nothing extra to build (resources created by visualizer and given as reference)
    }

    void TOpenGLCameraAdapter::Detach()
    {
        m_awaitingDeletion = true;
        if ( m_glCameraRef )
            m_glCameraRef->SetActiveMode( false );
    }

    void TOpenGLCameraAdapter::Reset()
    {
        if ( !m_glCameraRef || !m_cameraRef )
            return;

        m_glCameraRef->SetPosition( m_cameraRef->position() );
        m_glCameraRef->SetTargetPoint( m_cameraRef->target() );
    }

    void TOpenGLCameraAdapter::SetPosition( const TVec3& position )
    {
        if ( !m_glCameraRef )
            return;

        m_glCameraRef->SetPosition( position );
    }

    void TOpenGLCameraAdapter::SetTarget( const TVec3& target )
    {
        if ( !m_glCameraRef )
            return;

        m_glCameraRef->SetTargetPoint( target );
    }

    void TOpenGLCameraAdapter::GetPosition( TVec3& dstPosition )
    {
        if ( !m_glCameraRef )
            return;

        dstPosition = m_glCameraRef->position();
    }

    void TOpenGLCameraAdapter::GetTarget( TVec3& dstTarget )
    {
        if ( !m_glCameraRef )
            return;

        dstTarget = m_glCameraRef->target_point();
    }
}}
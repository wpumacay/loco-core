#pragma once

#include <visualizer/loco_visualizer_camera.h>
#include <loco_visualizer_opengl_common.h>

namespace loco {
namespace visualizer {

    class TOpenGLCameraAdapter : public TVizCameraAdapter
    {
    public :

        TOpenGLCameraAdapter( TVizCamera* cameraRef, engine::CICamera* gl_cameraRef );

        ~TOpenGLCameraAdapter();

        void Build() override;
        void Detach() override;
        void Reset() override;

        void SetPosition( const TVec3& position ) override;
        void SetTarget( const TVec3& target ) override;

        void GetPosition( TVec3& dstPosition ) override;
        void GetTarget( TVec3& dstTarget ) override;

        engine::CICamera* gl_camera() { return m_glCameraRef; }
        const engine::CICamera* gl_camera() const { return m_glCameraRef; }

    private :

        engine::CICamera* m_glCameraRef;
    };
}}
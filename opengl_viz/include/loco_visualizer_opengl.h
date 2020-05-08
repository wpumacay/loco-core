#pragma once

#include <visualizer/loco_visualizer.h>
#include <loco_visualizer_opengl_common.h>
#include <loco_visualizer_opengl_camera.h>
#include <loco_visualizer_opengl_light.h>
#include <loco_visualizer_opengl_drawable_adapter.h>

namespace loco
{
    class TOpenGLVisualizer : public TIVisualizer
    {
    public :

        TOpenGLVisualizer( TScenario* scenarioRef,
                           size_t windowWidth,
                           size_t windowHeight,
                           bool windowResizable,
                           bool renderOffscreen );
        ~TOpenGLVisualizer();

    protected :

        void _SetRenderOffscreenInternal() override;
        void _ChangeScenarioInternal() override;
        void _InitializeInternal() override;
        void _ResetInternal() override;

        std::unique_ptr<uint8_t[]> _RenderInternal( const eRenderMode& mode );

        void _DrawLineInternal( const TVec3& start, const TVec3& end, const TVec3& color ) override;
        void _DrawAABBInternal( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color ) override;
        void _DrawSolidCylinderInternalX( float radius, float height, const TMat4& transform, const TVec4& color ) override;
        void _DrawSolidCylinderInternalY( float radius, float height, const TMat4& transform, const TVec4& color ) override;
        void _DrawSolidCylinderInternalZ( float radius, float height, const TMat4& transform, const TVec4& color ) override;
        void _DrawSolidArrowInternalX( float length, const TMat4& transform, const TVec4& color) override;
        void _DrawSolidArrowInternalY( float length, const TMat4& transform, const TVec4& color) override;
        void _DrawSolidArrowInternalZ( float length, const TMat4& transform, const TVec4& color) override;

        bool _IsActiveInternal() const override;
        bool _IsKeyDownInternal( int key ) const override;
        bool _CheckSingleKeyPressInternal( int key ) const override;
        bool _IsMouseDownInternal( int button ) const override;
        TVec2 _GetCursorPositionInternal() const override;

        void _CreateCameraInternal( TVizCamera* cameraRef ) override;
        void _CreateLightInternal( TVizLight* lightRef ) override;

        void _ChangeCurrentCameraInternal( TVizCamera* cameraRef ) override;
        void _ChangeCurrentLightInternal( TVizLight* lightRef ) override;

    protected :

        void _SetupRenderingEngine( size_t windowWidth,
                                    size_t windowHeight,
                                    bool windowResizable,
                                    bool renderOffscreen );

        void _CollectDrawables();
        void _CollectSingleBodies();
        //// void _CollectCompounds();
        //// void _CollectKintreeAgents();
        //// void _CollectKintreeSensors();
        //// void _CollectTerrainGenerators();

        void _CollectDetached();

    protected :

        // Rendering application object
        std::unique_ptr<engine::CApplication> m_glApplication;
    };
#ifndef LOCO_OPENGL_VISUALIZER_EDITOR
    extern "C" TIVisualizer* visualizer_create( TScenario* scenarioRef,
                                                size_t windowWidth, size_t windowHeight,
                                                bool windowResizable, bool renderOffscreen );
#endif /* LOCO_OPENGL_VISUALIZER_EDITOR */
}

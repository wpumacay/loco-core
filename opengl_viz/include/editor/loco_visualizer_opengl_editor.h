#pragma once

#include <loco_visualizer_opengl.h>

namespace loco
{
    class TOpenGLEditor;

    class TOpenGLEditorLayer : public engine::CImGuiLayer
    {
    public :

        TOpenGLEditorLayer( TOpenGLEditor* editorRef,
                            TScenario* scenarioRef );

        ~TOpenGLEditorLayer();

        void update() override;

        void render() override;

        bool onEvent( const engine::CInputEvent& event ) override;

    private :

        void _WindowObjects();

        void _WindowScenarioView();

        void _WindowInspector();

        void _WindowTools();

    private :

        // Reference to the scenario (to grab resources to analyze)
        TScenario* m_scenarioRef;
        // Reference to the editor
        TOpenGLEditor* m_editorRef;
        // Checks if events are consumed by this layer
        bool m_wantsToCaptureMouse;
    };

    class TOpenGLEditor : public TOpenGLVisualizer
    {
    public :

        TOpenGLEditor( TScenario* scenarioRef,
                       size_t windowWidth,
                       size_t windowHeight,
                       bool windowResizable,
                       bool renderOffscreen );
        ~TOpenGLEditor();

    private :

        // Reference to the editor's imgui-layer
        TOpenGLEditorLayer* m_guiEditorLayerRef;
    };

#if defined( LOCO_OPENGL_VISUALIZER_EDITOR )
    extern "C" TIVisualizer* visualizer_create( TScenario* scenarioRef,
                                                size_t windowWidth, size_t windowHeight,
                                                bool windowResizable, bool renderOffscreen );
#endif /* LOCO_OPENGL_VISUALIZER_EDITOR */
}
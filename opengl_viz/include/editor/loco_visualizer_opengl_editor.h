#pragma once

#include <loco_visualizer_opengl.h>

namespace loco
{
    class TOpenGLEditor;

    class TOpenGLEditorLayer : public engine::CImGuiLayer
    {
    public :

        TOpenGLEditorLayer( TOpenGLEditor* editorRef,
                            TScenario* scenarioRef,
                            engine::CApplication* glApplicationRef );

        ~TOpenGLEditorLayer();

        void update() override;

        void render() override;

        bool onEvent( const engine::CInputEvent& event ) override;

    private :

        void _WindowObjects();

        void _WindowInspector();
        void _WindowInspectorSingleBody( TSingleBody* single_body_ref );
        //// void _WindowInspectorCompound( TCompound* compound_ref );
        //// void _WindowInspectorKinematicTree( TKinematicTree* kinematic_tree_ref );
        //// void _WindowInspectorTerrainGenerator( TTerrainGenerator* terrain_generator_ref );

        void _WindowScenario();

        void _WindowTools();

    private :

        // Reference to the scenario (to grab resources to analyze)
        TScenario* m_scenarioRef;
        // Reference to the editor
        TOpenGLEditor* m_editorRef;
        // Reference to the gl-application (to grab render-target)
        engine::CApplication* m_glApplicationRef;
        // Checks if events are consumed by this layer
        bool m_wantsToCaptureMouse;
        // Selection State
        enum class eSelectionType : uint8_t
        {
            NONE = 0,
            SINGLE_BODY,
            COMPOUND,
            KINEMATIC_TREE,
            TERRAIN_GENERATOR
        };
        struct TEditorSelectionState
        {
            eSelectionType selection_type;
            std::string name_single_body;
            std::string name_compound;
            std::string name_kinematic_tree;
            std::string name_terrain_generator;
        } m_selectionState;
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

#include <editor/loco_visualizer_opengl_editor.h>

namespace loco
{
    TOpenGLEditorLayer::TOpenGLEditorLayer( TOpenGLEditor* editorRef,
                                            TScenario* scenarioRef )
        : engine::CImGuiLayer( "editor_layer" )
    {
        m_editorRef = editorRef;
        m_scenarioRef = scenarioRef;
        m_wantsToCaptureMouse = false;
    }

    TOpenGLEditorLayer::~TOpenGLEditorLayer()
    {
        m_editorRef = nullptr;
        m_scenarioRef = nullptr;
    }

    void TOpenGLEditorLayer::update()
    {
        // Nothing to pool|check|update
    }

    void TOpenGLEditorLayer::render()
    {
        m_wantsToCaptureMouse = false;

        ImGui::Begin( "test-window" );
        ImGui::Text( "hello-world!!! from editor" );
        ImGui::End();

        _WindowObjects();
        _WindowScenarioView();
        _WindowInspector();
        _WindowTools();

        ImGuiIO& io = ImGui::GetIO();
        m_wantsToCaptureMouse = io.WantCaptureMouse;
    }

    bool TOpenGLEditorLayer::onEvent( const engine::CInputEvent& event )
    {
        if ( event.type() == engine::eEventType::MOUSE_PRESSED )
            return m_wantsToCaptureMouse;
        return false;
    }

    void TOpenGLEditorLayer::_WindowObjects()
    {

    }

    void TOpenGLEditorLayer::_WindowScenarioView()
    {

    }

    void TOpenGLEditorLayer::_WindowInspector()
    {

    }

    void TOpenGLEditorLayer::_WindowTools()
    {

    }

    TOpenGLEditor::TOpenGLEditor( TScenario* scenarioRef,
                                  size_t windowWidth,
                                  size_t windowHeight,
                                  bool windowResizable,
                                  bool renderOffscreen )
        : TOpenGLVisualizer( scenarioRef, windowWidth, windowHeight, windowResizable, renderOffscreen )
    {
        auto editor_gui_layer = std::make_unique<TOpenGLEditorLayer>( this, m_scenarioRef );
        m_guiEditorLayerRef = dynamic_cast<TOpenGLEditorLayer*>( m_glApplication->addGuiLayer( std::move( editor_gui_layer ) ) );
    }

    TOpenGLEditor::~TOpenGLEditor()
    {
        m_guiEditorLayerRef = nullptr;
    }

#if defined( LOCO_OPENGL_VISUALIZER_EDITOR )
    extern "C" TIVisualizer* visualizer_create( TScenario* scenarioRef,
                                                size_t windowWidth, size_t windowHeight,
                                                bool windowResizable, bool renderOffscreen )
    {
        return new TOpenGLEditor( scenarioRef,
                                  windowWidth, windowHeight,
                                  windowResizable, renderOffscreen );
    }
#endif /* LOCO_OPENGL_VISUALIZER_EDITOR */
}
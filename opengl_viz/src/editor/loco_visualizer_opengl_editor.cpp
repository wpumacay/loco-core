
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

        m_selectionState.selection_type = eSelectionType::NONE;
        m_selectionState.name_single_body = "";
        m_selectionState.name_compound = "";
        m_selectionState.name_kinematic_tree = "";
        m_selectionState.name_terrain_generator = "";
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

        _WindowObjects();
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
        ImGui::Begin( "Objects" );
        if ( ImGui::CollapsingHeader( "Single-bodies" ) )
        {
            auto single_bodies = m_scenarioRef->GetSingleBodiesList();
            for ( ssize_t i = 0; i < single_bodies.size(); i++ )
            {
                ImGui::Bullet();
                ImGui::SameLine();
                if ( ImGui::Selectable( single_bodies[i]->name().c_str(),
                                        m_selectionState.name_single_body == single_bodies[i]->name() ) )
                {
                    m_selectionState.name_single_body = single_bodies[i]->name();
                    m_selectionState.selection_type = eSelectionType::SINGLE_BODY;
                }
            }
        }
////         if ( ImGui::CollapsingHeader( "Compounds" ) )
////         {
////             auto compounds = m_scenarioRef->GetCompoundsList();
////             for ( ssize_t i = 0; i < compounds.size(); i++ )
////             {
////                 ImGui::Bullet();
////                 ImGui::SameLine();
////                 if ( ImGui::Selectable( compounds[i]->name().c_str(),
////                                         m_selectionState.name_compound == compounds[i]->name() ) )
////                 {
////                     m_selectionState.name_compound = compounds[i]->name;
////                     m_selectionState.selection_type = eSelectionType::COMPOUND;
////                 }
////             }
////         }
////         if ( ImGui::CollapsingHeader( "Kinematic-trees" ) )
////         {
////             auto kinematic_trees = m_scenarioRef->GetKinematicTreesList();
////             for ( ssize_t i = 0; i < kinematic_trees.size(); i++ )
////             {
////                 ImGui::Bullet();
////                 ImGui::SameLine();
////                 if ( ImGui::Selectable( kinematic_trees[i]->name().c_str(),
////                                         m_selectionState.name_kinematic_tree == kinematic_trees[i]->name() ) )
////                 {
////                     m_selectionState.name_kinematic_tree = kinematic_trees[i]->name();
////                     m_selectionState.selection_type = eSelectionType::KINEMATIC_TREE;
////                 }
////             }
////         }
////         if ( ImGui::CollapsingHeader( "Terrain-generators" ) )
////         {
////             auto terrain_generators = m_scenarioRef->GetTerrainGeneratorsList();
////             for ( ssize_t i = 0; i - terrain_generators.size(); i++ )
////             {
////                 ImGui::Bullet();
////                 ImGui::SameLine();
////                 if ( ImGui::Selectable( terrain_generators[i]->name().c_str(),
////                                         m_selectionState.name_terrain_generator == terrain_generators[i]->name() ) )
////                 {
////                     m_selectionState.name_terrain_generator = terrain_generators[i]->name();
////                     m_selectionState.selection_type = eSelectionType::TERRAIN_GENERATOR;
////                 }
////             }
////         }

        ImGui::End();
    }

    void TOpenGLEditorLayer::_WindowInspector()
    {
        ImGui::Begin( "Inspector" );

        if ( ( m_selectionState.selection_type == eSelectionType::SINGLE_BODY ) &&
             ( m_scenarioRef->HasSingleBodyNamed( m_selectionState.name_single_body ) ) )
                _WindowInspectorSingleBody( m_scenarioRef->GetSingleBodyByName( m_selectionState.name_single_body ) );

        ImGui::End();
    }

    void TOpenGLEditorLayer::_WindowInspectorSingleBody( TSingleBody* single_body_ref )
    {
        if ( !ImGui::CollapsingHeader( "Transform" ) )
            return;

        // Transform.Position
        ImGui::Text( "Position" );
        auto position = single_body_ref->pos();
        const float change_rate_position = 0.01f;
        ImGui::SetNextItemWidth( 100 ); ImGui::DragFloat( "x", &position.x(), change_rate_position ); ImGui::SameLine();
        ImGui::SetNextItemWidth( 100 ); ImGui::DragFloat( "y", &position.y(), change_rate_position ); ImGui::SameLine();
        ImGui::SetNextItemWidth( 100 ); ImGui::DragFloat( "z", &position.z(), change_rate_position );
        single_body_ref->SetPosition( position );
        // Transform.Euler
        ImGui::Text( "Euler" );
        auto euler = single_body_ref->euler();
        const float change_rate_euler = 0.01f;
        const float min_ex = -loco::PI + 1e-2,       max_ex = loco::PI - 1e-2;
        const float min_ey = -loco::PI / 2.0 + 1e-2, max_ey = loco::PI / 2.0 - 1e-2;
        const float min_ez = -loco::PI + 1e-2,       max_ez = loco::PI - 1e-2;
        ImGui::SetNextItemWidth( 100 ); ImGui::DragFloat( "e.x", &euler.x(), change_rate_euler, min_ex, max_ex ); ImGui::SameLine();
        ImGui::SetNextItemWidth( 100 ); ImGui::DragFloat( "e.y", &euler.y(), change_rate_euler, min_ey, max_ey ); ImGui::SameLine();
        ImGui::SetNextItemWidth( 100 ); ImGui::DragFloat( "e.z", &euler.z(), change_rate_euler, min_ez, max_ez );
        single_body_ref->SetEuler( euler );
    }

    void TOpenGLEditorLayer::_WindowTools()
    {
        ImGui::Begin( "Tools" );

        ImGui::End();
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
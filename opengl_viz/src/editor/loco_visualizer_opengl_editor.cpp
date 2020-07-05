
#include <editor/loco_visualizer_opengl_editor.h>

namespace loco {
namespace visualizer {

    /***********************************************************************************************
    *                                     Editor-GUI Impl.                                         *
    ***********************************************************************************************/

    TOpenGLEditorLayer::TOpenGLEditorLayer( TOpenGLEditor* editorRef,
                                            TScenario* scenarioRef,
                                            engine::CApplication* glApplicationRef )
        : engine::CImGuiLayer( "editor_layer" )
    {
        m_editorRef = editorRef;
        m_scenarioRef = scenarioRef;
        m_runtimeRef = nullptr;
        m_glApplicationRef = glApplicationRef;
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
        m_runtimeRef = nullptr;
        m_glApplicationRef = nullptr;
    }

    void TOpenGLEditorLayer::SetRuntimeRef( TRuntime* runtimeRef )
    {
        m_runtimeRef = runtimeRef;
    }

    void TOpenGLEditorLayer::update()
    {
        // Nothing to pool|check|update
    }

    void TOpenGLEditorLayer::render()
    {
        m_wantsToCaptureMouse = false;

        _CheckUserKeyActions();
        _WindowObjects();
        _WindowScenario();
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

    void TOpenGLEditorLayer::_CheckUserKeyActions()
    {
        if ( engine::CInputManager::IsKeyDown( engine::Keys::KEY_ESCAPE ) )
        {
            m_selectionState.selection_type = eSelectionType::NONE;
            m_selectionState.name_single_body = "";
            m_selectionState.name_compound = "";
            m_selectionState.name_kinematic_tree = "";
            m_selectionState.name_terrain_generator = "";
        }
        else if ( engine::CInputManager::IsKeyDown( engine::Keys::KEY_DELETE ) )
        {
            if ( m_selectionState.selection_type == eSelectionType::SINGLE_BODY &&
                 m_scenarioRef->HasSingleBodyNamed( m_selectionState.name_single_body ) )
            {
                m_scenarioRef->RemoveSingleBodyByName( m_selectionState.name_single_body );
                m_selectionState.selection_type = eSelectionType::NONE;
                m_selectionState.name_single_body = "";
            }
        }
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

    void TOpenGLEditorLayer::_WindowInspectorSingleBody( primitives::TSingleBody* single_body_ref )
    {
        if ( m_runtimeRef->GetCurrentSimulation() )
            return;

        if ( ImGui::CollapsingHeader( "Transform" ) )
        {
            // Transform.Position
            ImGui::Text( "Position" );
            auto position = single_body_ref->pos();
            const float change_rate_position = 0.01f;
            ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "x##position", &position.x(), change_rate_position ); ImGui::SameLine();
            ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "y##position", &position.y(), change_rate_position ); ImGui::SameLine();
            ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "z##position", &position.z(), change_rate_position );
            single_body_ref->SetPosition( position );
            single_body_ref->SetInitialPosition( position );
            // Transform.Euler
            ImGui::Text( "Euler" );
            auto euler = single_body_ref->euler();
            const float change_rate_euler = 0.01f;
            const float min_ex = -loco::PI + 1e-2,       max_ex = loco::PI - 1e-2;
            const float min_ey = -loco::PI / 2.0 + 1e-2, max_ey = loco::PI / 2.0 - 1e-2;
            const float min_ez = -loco::PI + 1e-2,       max_ez = loco::PI - 1e-2;
            ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "x##euler", &euler.x(), change_rate_euler, min_ex, max_ex ); ImGui::SameLine();
            ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "y##euler", &euler.y(), change_rate_euler, min_ey, max_ey ); ImGui::SameLine();
            ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "z##euler", &euler.z(), change_rate_euler, min_ez, max_ez );
            single_body_ref->SetEuler( euler );
            single_body_ref->SetInitialEuler( euler );
        }

        if ( ImGui::CollapsingHeader( "Shape" ) )
        {
            auto collider = single_body_ref->collider();
            auto visual = single_body_ref->drawable();
            if ( collider && visual )
            {
                const bool have_same_shape = ( collider->shape() == visual->shape() );

                static bool lock_visual_to_collider = have_same_shape;
                if ( have_same_shape )
                    ImGui::Checkbox( "Lock visual-to-collider size", &lock_visual_to_collider );
                const float change_rate_size = 0.01f;
                const float min_size = 1e-2f;
                const float max_size = 1e2f;

                ImGui::Text( "Collider: %s", ToString( collider->shape() ).c_str() );
                auto collider_size = collider->size();
                switch ( collider->shape() )
                {
                    case eShapeType::PLANE :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Width##C.Plane", &collider_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Depth##C.Plane", &collider_size.y(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::BOX :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Width##C.Box", &collider_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Depth##C.Box", &collider_size.y(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Height##C.Box", &collider_size.z(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::SPHERE :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Radius##C.Sph", &collider_size.x(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::CYLINDER :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Radius##C.Cyl", &collider_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Height##C.Cyl", &collider_size.y(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::CAPSULE :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Radius##C.Cap", &collider_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Height##C.Cap", &collider_size.y(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::ELLIPSOID :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Rad-x##C.Ell", &collider_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Rad-y##C.Ell", &collider_size.y(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Rad-z##C.Ell", &collider_size.z(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::CONVEX_MESH :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Scale-x##C.Mesh", &collider_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Scale-y##C.Mesh", &collider_size.y(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Scale-z##C.Mesh", &collider_size.z(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::HEIGHTFIELD :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Width##C.Hfield", &collider_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Depth##C.Hfield", &collider_size.y(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Scale-height##C.Hfield", &collider_size.z(), change_rate_size, min_size, max_size );
                        break;
                    }
                }
                const bool change_collider_size = !tinymath::allclose( collider_size, collider->size(), 1e-4f );

                ImGui::Text( "Visual: %s", ToString( visual->shape() ).c_str() );
                auto visual_size = visual->size();
                switch ( visual->shape() )
                {
                    case eShapeType::PLANE :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Width##V.Plane", &visual_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Depth##V.Plane", &visual_size.y(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::BOX :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Width##V.Box", &visual_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Depth##V.Box", &visual_size.y(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Height##V.Box", &visual_size.z(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::SPHERE :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Radius##V.Sph", &visual_size.x(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::CYLINDER :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Radius##V.Cyl", &visual_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Height##V.Cyl", &visual_size.y(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::CAPSULE :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Radius##V.Cap", &visual_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Height##V.Cap", &visual_size.y(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::ELLIPSOID :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Rad-x##V.Ell", &visual_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Rad-y##V.Ell", &visual_size.y(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Rad-z##V.Ell", &visual_size.z(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::CONVEX_MESH :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Scale-x##V.Mesh", &visual_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Scale-y##V.Mesh", &visual_size.y(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Scale-z##V.Mesh", &visual_size.z(), change_rate_size, min_size, max_size );
                        break;
                    }
                    case eShapeType::HEIGHTFIELD :
                    {
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Width##V.Hfield", &visual_size.x(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Depth##V.Hfield", &visual_size.y(), change_rate_size, min_size, max_size ); ImGui::SameLine();
                        ImGui::SetNextItemWidth( 80 ); ImGui::DragFloat( "Scale-height##V.Hfield", &visual_size.z(), change_rate_size, min_size, max_size );
                        break;
                    }
                }
                const bool change_visual_size = !tinymath::allclose( visual_size, visual->size(), 1e-4f );

                if ( !lock_visual_to_collider )
                {
                    if ( change_collider_size )
                        collider->ChangeSize( collider_size );
                    if ( change_visual_size )
                        visual->ChangeSize( visual_size );
                }
                else
                {
                    // User couldn't have changed both at the same time
                    if ( change_collider_size )
                    {
                        collider->ChangeSize( collider_size );
                        visual->ChangeSize( collider_size );
                    }
                    else if ( change_visual_size )
                    {
                        collider->ChangeSize( visual_size );
                        visual->ChangeSize( visual_size );
                    }
                }
            }
        }
    }

    void TOpenGLEditorLayer::_WindowScenario()
    {
        auto fbo_texture = m_glApplicationRef->renderTarget()->getTextureAttachment( "color_attachment" );
        auto fbo_texture_id = fbo_texture->openglId();

        ImGui::Begin( "Scenario" );
        const bool option_play = ImGui::Button( "Play##simulation" ); ImGui::SameLine();
        const bool option_pause = ImGui::Button( "Pause##simulation" ); ImGui::SameLine();
        const bool option_stop = ImGui::Button( "Stop##simulation" );
        ImGui::Spacing();

        if ( m_runtimeRef )
        {
            if ( option_stop ) // stop simulation (destroy current simulaton)
            {
                m_scenarioRef->DetachSim();
                m_scenarioRef->Reset();
                m_runtimeRef->DestroySimulation();
            }
            else if ( option_pause ) // pause current simulation
            {
                if ( auto simulation = m_runtimeRef->GetCurrentSimulation() )
                    simulation->Pause();
            }
            else if ( option_play )
            {
                if ( auto simulation = m_runtimeRef->GetCurrentSimulation() )
                {
                    simulation->Resume();
                }
                else
                {
                    m_scenarioRef->Reset();
                    m_runtimeRef->CreateSimulation( m_scenarioRef );
                }
            }
        }

        const float curr_window_width = std::max( 10.0f, ImGui::GetWindowWidth() - 80 );
        const float curr_window_height = std::max( 10.0f, ImGui::GetWindowHeight() - 80 );
        const float window_x = ImGui::GetWindowPos().x;
        const float window_y = ImGui::GetWindowPos().y;
        const bool is_window_focused = ImGui::IsWindowFocused();
        ImGui::Image( (void*)(intptr_t)fbo_texture_id,
                      { curr_window_width, curr_window_height }, { 0.0f, 1.0f }, { 1.0f, 0.0f } );
        ImGui::End();

        static float last_width = curr_window_width;
        static float last_height = curr_window_height;
        const bool is_resizing = std::abs( last_width - curr_window_width ) > 0.0f ||
                                 std::abs( last_height - curr_window_height ) > 0.0f;
        if ( is_resizing )
        {
            m_glApplicationRef->scene()->Resize( curr_window_width, curr_window_height );
            m_glApplicationRef->renderOptions().viewportWidth = curr_window_width;
            m_glApplicationRef->renderOptions().viewportHeight = curr_window_height;
            m_glApplicationRef->renderTarget()->resize( curr_window_width, curr_window_height );
        }
        last_width = curr_window_width;
        last_height = curr_window_height;

        const float cursor_x = ImGui::GetMousePos().x;
        const float cursor_y = ImGui::GetMousePos().y;
        const float dx = cursor_x - window_x;
        const float dy = cursor_y - window_y;
        const bool inside_window = ( dx > 0 && dx < curr_window_width ) && ( dy > 0 && dy < curr_window_height );
        if ( !is_resizing && is_window_focused )
        {

            if ( inside_window && ImGui::IsMouseDown( 0 ) )
            {
                engine::CInputManager::Callback_mouse( engine::Mouse::BUTTON_LEFT, engine::MouseAction::BUTTON_PRESSED, dx, dy );
            }
            else if ( inside_window && ImGui::IsMouseDown( 1 ) )
            {
                engine::CInputManager::Callback_mouse( engine::Mouse::BUTTON_RIGHT, engine::MouseAction::BUTTON_PRESSED, dx, dy );
            }
            else
            {
                engine::CInputManager::Callback_mouse( engine::Mouse::BUTTON_LEFT, engine::MouseAction::BUTTON_RELEASED, dx, dy );
                engine::CInputManager::Callback_mouse( engine::Mouse::BUTTON_RIGHT, engine::MouseAction::BUTTON_RELEASED, dx, dy );
            }

            if ( inside_window )
                engine::CInputManager::Callback_mouseMove( dx, dy );
        }
        else
        {
                engine::CInputManager::Callback_mouse( engine::Mouse::BUTTON_LEFT, engine::MouseAction::BUTTON_RELEASED, dx, dy );
                engine::CInputManager::Callback_mouse( engine::Mouse::BUTTON_RIGHT, engine::MouseAction::BUTTON_RELEASED, dx, dy );
        }
    }

    void TOpenGLEditorLayer::_WindowTools()
    {
        ImGui::Begin( "Tools" );

        ImGui::End();
    }

    /***********************************************************************************************
    *                                       Editor Impl.                                           *
    ***********************************************************************************************/

    TOpenGLEditor::TOpenGLEditor( TScenario* scenarioRef,
                                  size_t windowWidth,
                                  size_t windowHeight,
                                  bool windowResizable,
                                  bool renderOffscreen )
        : TOpenGLVisualizer( scenarioRef, windowWidth, windowHeight, windowResizable, renderOffscreen )
    {
        // Render offscreen and show target on view-window
        m_glApplication->setOffscreenRendering( true );

        auto editor_gui_layer = std::make_unique<TOpenGLEditorLayer>( this, m_scenarioRef, m_glApplication.get() );
        m_guiEditorLayerRef = dynamic_cast<TOpenGLEditorLayer*>( m_glApplication->addGuiLayer( std::move( editor_gui_layer ) ) );
    }

    TOpenGLEditor::~TOpenGLEditor()
    {
        m_guiEditorLayerRef = nullptr;
    }

    void TOpenGLEditor::_SetRuntimeInternal( TRuntime* runtimeRef )
    {
        m_guiEditorLayerRef->SetRuntimeRef( runtimeRef );
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
}}
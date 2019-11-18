
#include <glviz_ui.h>

namespace tysoc
{

    TGLScenarioUtilsLayer::TGLScenarioUtilsLayer( const std::string& name,
                                                  TGLVisualizer* visualizer,
                                                  TScenario* scenario )
        : engine::CImGuiLayer( name )
    {
        m_scenario = scenario;
        m_visualizer = visualizer;
        m_wantsToCaptureMouse = false;
    }

    TGLScenarioUtilsLayer::~TGLScenarioUtilsLayer()
    {
        m_scenario = nullptr;
        m_visualizer = nullptr;
    }

    void TGLScenarioUtilsLayer::update()
    {
        // nothing to do for now
    }

    void TGLScenarioUtilsLayer::render()
    {
        m_wantsToCaptureMouse = false;

        _menuDemo();
        _menuGeneral();
        _menuScenario();

        ImGuiIO& io = ImGui::GetIO();
        m_wantsToCaptureMouse = io.WantCaptureMouse;
    }

    bool TGLScenarioUtilsLayer::onEvent( const engine::CInputEvent& event )
    {
        if ( event.type() == engine::eEventType::MOUSE_PRESSED )
            return m_wantsToCaptureMouse;

        return false;
    }

    void TGLScenarioUtilsLayer::_menuDemo()
    {
    #ifdef TYSOC_DEMO
        ImGui::Begin( "Sim-Demo" );

        ImGui::Checkbox( "Use camera sensors", &DEMO_OPTIONS.useDemoCameraSensors );
        ImGui::Checkbox( "Use contact manager", &DEMO_OPTIONS.useDemoContactManager );
        ImGui::Checkbox( "USe debug draws", &DEMO_OPTIONS.useDemoDebugDraws );

        engine::CDebugDrawer::SetEnabled( DEMO_OPTIONS.useDemoDebugDraws );

        ImGui::End();
    #endif
    }

    void TGLScenarioUtilsLayer::_menuGeneral()
    {
        /* @debug *********************************************/
        ImGui::Begin( "Sim-general" );

        bool _useSensorReadings = m_visualizer->useSensorReadings();
        bool _useSensorReadingRgb = m_visualizer->useSensorReadingRgb();
        bool _useSensorReadingDepth = m_visualizer->useSensorReadingDepth();
        bool _useSensorReadingSemantic = m_visualizer->useSensorReadingSemantic();
        ImGui::Checkbox( "Use sensor readings", &_useSensorReadings );
        ImGui::Checkbox( "Use sensor reading - rgb", &_useSensorReadingRgb );
        ImGui::Checkbox( "Use sensor reading - depth", &_useSensorReadingDepth );
        ImGui::Checkbox( "Use sensor reading - semantic", &_useSensorReadingSemantic );
        m_visualizer->setSensorsEnabled( _useSensorReadings );
        m_visualizer->setSensorRgbEnabled( _useSensorReadingRgb );
        m_visualizer->setSensorDepthEnabled( _useSensorReadingDepth );
        m_visualizer->setSensorSemanticEnabled( _useSensorReadingSemantic );

        ImGui::End();

        if ( _useSensorReadings && _useSensorReadingRgb && DEMO_OPTIONS.useDemoCameraSensors )
        {
            auto _fboRgb = m_visualizer->fboRgb();
            if ( _fboRgb )
            {
                auto _textureAttachment = _fboRgb->getTextureAttachment( "color_attachment" );
                ImGui::Begin( "rgb-view" );

                ImGui::Image( (void*)(intptr_t) _textureAttachment->openglId(),
                              ImVec2( _textureAttachment->width() / 2.0f, 
                                      _textureAttachment->height() / 2.0f ),
                              { 0.0f, 1.0f }, { 1.0f, 0.0f } );
                ImGui::End();
            }
        }

        if ( _useSensorReadings && _useSensorReadingDepth && DEMO_OPTIONS.useDemoCameraSensors )
        {
            auto _fboDepth = m_visualizer->fboDepth();
            if ( _fboDepth )
            {
                auto _textureAttachment = _fboDepth->getTextureAttachment( "color_attachment" );
                ImGui::Begin( "depth-view" );

                ImGui::Image( (void*)(intptr_t) _textureAttachment->openglId(),
                              ImVec2( _textureAttachment->width() / 2.0f, 
                                      _textureAttachment->height() / 2.0f ),
                              { 0.0f, 1.0f }, { 1.0f, 0.0f } );
                ImGui::End();
            }
        }

        if ( _useSensorReadings && _useSensorReadingSemantic && DEMO_OPTIONS.useDemoCameraSensors )
        {
            auto _fboSemantic = m_visualizer->fboSemantic();
            if ( _fboSemantic )
            {
                auto _textureAttachment = _fboSemantic->getTextureAttachment( "color_attachment" );
                ImGui::Begin( "semantic-view" );

                ImGui::Image( (void*)(intptr_t) _textureAttachment->openglId(),
                              ImVec2( _textureAttachment->width() / 2.0f, 
                                      _textureAttachment->height() / 2.0f ),
                              { 0.0f, 1.0f }, { 1.0f, 0.0f } );
                ImGui::End();
            }
        }

        /* @debug *********************************************/
    }

    void TGLScenarioUtilsLayer::_menuScenario()
    {
        if ( !m_scenario )
            return;

        ImGui::Begin( "Scenario" );

        _submenuScenarioPrimitives();
        _submenuScenarioAgents();
        _submenuScenarioTerrainGens();

        ImGui::End();
    }

    void TGLScenarioUtilsLayer::_submenuScenarioPrimitives()
    {
        if ( !ImGui::CollapsingHeader( "Primitives" ) )
            return;

        auto _bodies = m_scenario->getBodies();

        static std::string _lastBodyName = "";
        static std::string _currentBodyName = "";
        GLVIZ_IMGUI_COMBO_CONSTRUCT( "primitive", _currentBodyName, _bodies );
        bool _refresh = ( _currentBodyName != _lastBodyName );
        _lastBodyName = _currentBodyName;

        if ( m_scenario->hasBody( _currentBodyName ) )
            _submenuPrimitive( m_scenario->getBodyByName( _currentBodyName ), _refresh );
    }

    void TGLScenarioUtilsLayer::_submenuPrimitive( TBody* body, bool refresh )
    {
        if ( !body )
            return;


    }

    void TGLScenarioUtilsLayer::_submenuScenarioAgents()
    {
        if ( !ImGui::CollapsingHeader( "Agents" ) )
            return;

        auto _agents = m_scenario->getAgents();

        static std::string _lastAgentName = "";
        static std::string _currentAgentName = "";
        GLVIZ_IMGUI_COMBO_CONSTRUCT( "agent", _currentAgentName, _agents );
        bool _refresh = ( _currentAgentName != _lastAgentName );
        _lastAgentName = _currentAgentName;

        if ( m_scenario->hasAgent( _currentAgentName ) )
            _submenuAgent( m_scenario->getAgentByName( _currentAgentName ), _refresh );
    }

    void TGLScenarioUtilsLayer::_submenuAgent( TAgent* agent, bool refresh )
    {
        if ( !agent )
            return;

        std::string _infoSummary;
        _infoSummary += std::string( "num. bodies       : " ) + std::to_string( agent->bodies.size() ) + "\n\r";
        _infoSummary += std::string( "num. joints       : " ) + std::to_string( agent->joints.size() ) + "\n\r";
        _infoSummary += std::string( "num. collisions   : " ) + std::to_string( agent->collisions.size() ) + "\n\r";
        _infoSummary += std::string( "num. visuals      : " ) + std::to_string( agent->visuals.size() ) + "\n\r";
        _infoSummary += std::string( "num. sensors      : " ) + std::to_string( agent->sensors.size() ) + "\n\r";
        _infoSummary += std::string( "num. actuators    : " ) + std::to_string( agent->actuators.size() ) + "\n\r";

        ImGui::TextColored( { 0.2f, 0.4f, 0.8f, 1.0f }, "Summary: " );
        ImGui::Text( _infoSummary.c_str() );

        //// bool _wireframe = agent->
        //// ImGui::Checkbox( "Wireframe", agent-> )

        _submenuTreeAgentQpos( agent, refresh );
        _submenuTreeAgentQvel( agent, refresh );
        _submenuTreeAgentActuators( agent, refresh );
        _submenuTreeAgentSensors( agent, refresh );
    }

    void TGLScenarioUtilsLayer::_submenuTreeAgentQpos( TAgent* agent, bool refresh )
    {
        if ( ImGui::TreeNode( "Agent qpos" ) )
        {
            size_t _jointIndex = 0;
            for ( auto _kinJoint : agent->joints )
            {
                auto _jointNameStripped = mjcf::stripMjcPrefix( "joint", _kinJoint->name, agent->name() );
                ImGui::TextColored( { 0.2f, 0.4f, 0.8f, 1.0f }, ( tysoc::toString( _kinJoint->data.type ) + "-joint: " + _jointNameStripped ).c_str() );
                ImGui::Checkbox( ( "kj(" + std::to_string( _jointIndex ) + ").userControlled" ).c_str() , &_kinJoint->userControlled );
                if ( _kinJoint->data.type == eJointType::FREE )
                {
                    float _position[3] = { _kinJoint->qpos[0], _kinJoint->qpos[1], _kinJoint->qpos[2] };
                    float _rotation[4] = { _kinJoint->qpos[3], _kinJoint->qpos[4], _kinJoint->qpos[5], _kinJoint->qpos[6] };

                    if ( _kinJoint->userControlled )
                    {
                        ImGui::DragFloat3( ( "kj(" + std::to_string( _jointIndex ) + ").position" ).c_str(), _position, 0.1f );
                        ImGui::DragFloat4( ( "kj(" + std::to_string( _jointIndex ) + ").rotation" ).c_str(), _rotation, 0.01f );

                        _kinJoint->qpos = { _position[0], _position[1], _position[2], 
                                            _rotation[0], _rotation[1], _rotation[2], _rotation[3] };
                    }
                    else
                    {
                        ImGui::InputFloat3( ( "kj(" + std::to_string( _jointIndex ) + ").position" ).c_str(), _position, "%.3f", ImGuiInputTextFlags_ReadOnly );
                        ImGui::InputFloat4( ( "kj(" + std::to_string( _jointIndex ) + ").rotation" ).c_str(), _rotation, "%.3f", ImGuiInputTextFlags_ReadOnly );
                    }
                }
                else if ( _kinJoint->data.type == eJointType::REVOLUTE )
                {
                    float _angle = _kinJoint->qpos[0];
                    if ( _kinJoint->userControlled )
                    {
                        if ( _kinJoint->data.limits.x < _kinJoint->data.limits.y )
                            ImGui::SliderFloat( ( "kj(" + std::to_string( _jointIndex ) + ").angle" ).c_str(), &_angle, _kinJoint->data.limits.x, _kinJoint->data.limits.y );
                        else
                            ImGui::DragFloat( ( "kj(" + std::to_string( _jointIndex ) + ").angle" ).c_str(), &_angle, 0.001f );
                        _kinJoint->qpos[0] = _angle;
                    }
                    else
                    {
                        ImGui::InputFloat( ( "kj.angle" + std::to_string( _jointIndex ) ).c_str(), &_angle, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );
                    }
                }

                _jointIndex++;
            }

            ImGui::TreePop();
        }
    }

    void TGLScenarioUtilsLayer::_submenuTreeAgentQvel( TAgent* agent, bool refresh )
    {
        // @todo: add plots of qvels
    }

    void TGLScenarioUtilsLayer::_submenuTreeAgentActuators( TAgent* agent, bool refresh )
    {
        if ( ImGui::TreeNode( "Agent actuators" ) )
        {
            bool _clearAll = ImGui::Button( "Clear-All-Controls" );
            for ( auto _kinActuator : agent->actuators )
            {
                auto _actuatorNameStripped = mjcf::stripMjcPrefix( "actuator", _kinActuator->name, agent->name() );

                if ( _kinActuator->data.limits.x < _kinActuator->data.limits.y )
                    ImGui::SliderFloat( _actuatorNameStripped.c_str(), &_kinActuator->ctrlValue, _kinActuator->data.limits.x, _kinActuator->data.limits.y );
                else
                    ImGui::DragFloat( _actuatorNameStripped.c_str(), &_kinActuator->ctrlValue, 0.001f );

                if ( _clearAll )
                    _kinActuator->ctrlValue = 0.0f;
            }

            ImGui::TreePop();
        }
    }

    void TGLScenarioUtilsLayer::_submenuTreeAgentSensors( TAgent* agent, bool refresh )
    {
        if ( ImGui::TreeNode( "Agent sensors" ) )
        {
            if ( ImGui::TreeNode( "Joint-sensors" ) )
            {
                static int s_index = 0;
                if ( refresh ) s_index = 0;
                for ( auto _kinSensor : agent->sensors )
                {
                    if ( _kinSensor->data.type != eSensorType::PROP_JOINT )
                        continue;

                    auto _jointNameStripped = mjcf::stripMjcPrefix( "joint",
                                                                    mjcf::stripMjcPrefix( "sensor", _kinSensor->name, agent->name() ),
                                                                    agent->name() );
                    auto _kinJointSensor = dynamic_cast< TKinTreeJointSensor* >( _kinSensor );

                    static std::unordered_map< std::string, std::array< float, VIZ_PLOT_BUFFER_SIZE > > s_theta_vals;
                    static std::unordered_map< std::string, std::array< float, VIZ_PLOT_BUFFER_SIZE > > s_thetadot_vals;
                    if ( s_theta_vals.find( _jointNameStripped ) == s_theta_vals.end() ) s_theta_vals[_jointNameStripped] = std::array< float, VIZ_PLOT_BUFFER_SIZE >();
                    if ( s_thetadot_vals.find( _jointNameStripped ) == s_thetadot_vals.end() ) s_thetadot_vals[_jointNameStripped] = std::array< float, VIZ_PLOT_BUFFER_SIZE >();
                    if ( refresh )
                    {
                        if ( s_theta_vals.find( _jointNameStripped ) != s_theta_vals.end() ) s_theta_vals[_jointNameStripped].fill( 0.0f );
                        if ( s_thetadot_vals.find( _jointNameStripped ) != s_thetadot_vals.end() ) s_thetadot_vals[_jointNameStripped].fill( 0.0f );
                    }

                    s_theta_vals[_jointNameStripped][s_index] = _kinJointSensor->theta;
                    s_thetadot_vals[_jointNameStripped][s_index] = _kinJointSensor->thetadot;

                    ImGui::PlotLines( ( "joint-angle(" + _jointNameStripped + ")" ).c_str(),
                                      s_theta_vals[_jointNameStripped].data(),
                                      VIZ_PLOT_BUFFER_SIZE,
                                      s_index,
                                      ( "current: " + std::to_string( s_theta_vals[_jointNameStripped][s_index] ) ).c_str(),
                                      -3.1415, 3.1415, ImVec2( 0, 80 ) );

                    ImGui::PlotLines( ( "joint-wspeed(" + _jointNameStripped + ")" ).c_str(),
                                      s_thetadot_vals[_jointNameStripped].data(),
                                      VIZ_PLOT_BUFFER_SIZE,
                                      s_index,
                                      ( "current: " + std::to_string( s_thetadot_vals[_jointNameStripped][s_index] ) ).c_str(),
                                      -10.0f, 10.0f, ImVec2( 0, 80 ) );
                }

                s_index = ( s_index + 1 ) % VIZ_PLOT_BUFFER_SIZE;
                ImGui::TreePop();
            }

            if ( ImGui::TreeNode( "Body-sensors" ) )
            {
                for ( auto _kinSensor : agent->sensors )
                {
                    if ( _kinSensor->data.type != eSensorType::PROP_BODY )
                        continue;

                    auto _bodyNameStripped = mjcf::stripMjcPrefix( "body",
                                                                    mjcf::stripMjcPrefix( "sensor", _kinSensor->name, agent->name() ),
                                                                    agent->name() );
                    auto _kinBodySensor = dynamic_cast< TKinTreeBodySensor* >( _kinSensor );

                    ImGui::Text( ( "body-linvel(" + _bodyNameStripped + ")    : %s" ).c_str(), TVec3::toString( _kinBodySensor->linVelocity ).c_str() );
                    ImGui::Text( ( "body-linacc(" + _bodyNameStripped + ")    : %s" ).c_str(), TVec3::toString( _kinBodySensor->linAcceleration ).c_str() );
                    ImGui::Text( ( "body-comforce(" + _bodyNameStripped + ")  : %s" ).c_str(), TVec3::toString( _kinBodySensor->comForce ).c_str() );
                    ImGui::Text( ( "body-comtorque(" + _bodyNameStripped + ") : %s" ).c_str(), TVec3::toString( _kinBodySensor->comTorque ).c_str() );
                }
            
                ImGui::TreePop();
            }
        }
    }

    void TGLScenarioUtilsLayer::_submenuScenarioTerrainGens()
    {
        if ( !ImGui::CollapsingHeader( "Terrain-Generators" ) )
            return;

        auto _terrainGens = m_scenario->getTerrainGenerators();

        static std::string _lastTerrainGenName = "";
        static std::string _currentTerrainGenName = "";
        GLVIZ_IMGUI_COMBO_CONSTRUCT( "terrainGen", _currentTerrainGenName, _terrainGens );
        bool _refresh = ( _currentTerrainGenName != _lastTerrainGenName );
        _lastTerrainGenName = _currentTerrainGenName;

        if ( m_scenario->hasTerrainGen( _currentTerrainGenName ) )
            _submenuTerrainGen( m_scenario->getTerrainGeneratorByName( _currentTerrainGenName ), _refresh );
    }

    void TGLScenarioUtilsLayer::_submenuTerrainGen( TITerrainGenerator* terrainGen, bool refresh )
    {
        if ( !terrainGen )
            return;


    }

}
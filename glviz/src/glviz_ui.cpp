
#include <glviz_ui.h>

namespace tysoc
{

    TGLScenarioUtilsLayer::TGLScenarioUtilsLayer( const std::string& name,
                                                  TScenario* scenario )
        : engine::CImGuiLayer( name )
    {
        m_scenario = scenario;
        m_wantsToCaptureMouse = false;
    }

    TGLScenarioUtilsLayer::~TGLScenarioUtilsLayer()
    {
        m_scenario = nullptr;
    }

    void TGLScenarioUtilsLayer::update()
    {
        // nothing to do for now
    }

    void TGLScenarioUtilsLayer::render()
    {
        m_wantsToCaptureMouse = false;

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

    void TGLScenarioUtilsLayer::_menuGeneral()
    {

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

        _submenuTreeAgentQpos( agent->joints );
        _submenuTreeAgentQvel( agent->joints );
        _submenuTreeAgentActuators( agent->actuators );
    }

    void TGLScenarioUtilsLayer::_submenuTreeAgentQpos( const std::vector< TKinTreeJoint* >& joints )
    {
        if ( ImGui::TreeNode( "Agent qpos" ) )
        {
            size_t _jointIndex = 0;
            for ( auto _kinJoint : joints )
            {
                ImGui::TextColored( { 0.2f, 0.4f, 0.8f, 1.0f }, ( tysoc::toString( _kinJoint->data.type ) + "-joint: " + _kinJoint->name ).c_str() );
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

    void TGLScenarioUtilsLayer::_submenuTreeAgentQvel( const std::vector< TKinTreeJoint* >& joints )
    {
        // @todo: add plots of qvels
    }

    void TGLScenarioUtilsLayer::_submenuTreeAgentActuators( const std::vector< TKinTreeActuator* >& actuators )
    {
        if ( ImGui::TreeNode( "Agent actuators" ) )
        {
            bool _clearAll = ImGui::Button( "Clear-All-Controls" );
            for ( auto _kinActuator : actuators )
            {
                if ( _kinActuator->data.limits.x < _kinActuator->data.limits.y )
                    ImGui::SliderFloat( _kinActuator->name.c_str(), &_kinActuator->ctrlValue, _kinActuator->data.limits.x, _kinActuator->data.limits.y );
                else
                    ImGui::DragFloat( _kinActuator->name.c_str(), &_kinActuator->ctrlValue, 0.001f );

                if ( _clearAll )
                    _kinActuator->ctrlValue = 0.0f;
            }

            ImGui::TreePop();
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
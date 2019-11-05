#pragma once

#include <viz/viz.h>
#include <glviz_common.h>
#include <glviz_kintree.h>
#include <glviz_terrainGen.h>
#include <glviz_drawable.h>

#include <components/bodies.h>

#define GLVIZ_IMGUI_COMBO_CONSTRUCT( combo_name, current_name_str, vect_container ) \
        if ( ImGui::BeginCombo( combo_name, current_name_str.c_str() ) )            \
        {                                                                           \
            for ( auto _vect_elm : vect_container )                                 \
            {                                                                       \
                bool _isSelected = ( _vect_elm->name() == current_name_str );       \
                                                                                    \
                if ( ImGui::Selectable( _vect_elm->name().c_str(), _isSelected ) )  \
                    current_name_str = _vect_elm->name();                           \
                                                                                    \
                if ( _isSelected )                                                  \
                    ImGui::SetItemDefaultFocus();                                   \
            }                                                                       \
            ImGui::EndCombo();                                                      \
        }

namespace tysoc
{

    class TGLScenarioUtilsLayer : public engine::CImGuiLayer
    {

    public :

        TGLScenarioUtilsLayer( const std::string& name,
                               TScenario* scenario );
        ~TGLScenarioUtilsLayer();

        void update() override;
        void render() override;
        bool onEvent( const engine::CInputEvent& event ) override;

    private :

        void _menuGeneral();
        void _menuScenario();

        void _submenuScenarioPrimitives();
        void _submenuScenarioAgents();
        void _submenuScenarioTerrainGens();

        void _submenuPrimitive( TBody* body, bool refresh );
        void _submenuAgent( TAgent* agent, bool refresh );
        void _submenuTerrainGen( TITerrainGenerator* terrainGen, bool refresh );

        void _submenuTreeAgentQpos( const std::vector< TKinTreeJoint* >& joints );
        void _submenuTreeAgentQvel( const std::vector< TKinTreeJoint* >& joints );
        void _submenuTreeAgentActuators( const std::vector< TKinTreeActuator* >& actuators );
        void _submenuTreeAgentSensors( const std::vector< TKinTreeSensor* >& sensors );

    private :

        TScenario* m_scenario;

        bool m_wantsToCaptureMouse;
    };

}
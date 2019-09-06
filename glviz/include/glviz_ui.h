
#pragma once

// interface for our UI implementation
#include <viz/ui.h>
// cat1 rendering engine resources
#include <LApp.h>
// dear imgui resources
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// visualization wrappers
#include <glviz_kintree.h>
#include <glviz_terrainGen.h>

// separate ui panels
#include <ui/glviz_ui_bodies.h>

namespace tysoc {

    class TGLUiBodies;

    struct TGLUiContext
    {
        bool                                    isUiActive;
        bool                                    isBasicUiActive;
        GLFWwindow*                             glfwWindowPtr;
        std::vector< TGLVizKinTree* >           vizKinTreePtrs;
        std::vector< TGLVizTerrainGenerator* >  vizTerrainGenPtrs;
    };

    class TGLUi : public TIVisualizerUI
    {

    public :

        TGLUi( TScenario* scenarioPtr,
               TGLUiContext* uiContextPtr );

        ~TGLUi();

    protected :

        void _initUIInternal() override;
        void _renderUIInternal() override;

    private :

        void _renderBasicMainMenu();
        void _renderBasicKinTreeVisualsMenu( TGLVizKinTree* vizKinTreePtr );
        void _renderBasicKinTreeModelInfoMenu( agent::TAgent* agentPtr );
        void _renderBasicKinTreeActionsMenu( agent::TAgent* agentPtr );
        void _renderBasicKinTreeQvalues( agent::TAgent* agentPtr );
        void _renderBasicKinTreeSummary( agent::TAgent* agentPtr );

        void _showBodyInfo( agent::TKinTreeBody* kinTreeBodyPtr  );
        void _showJointInfo( agent::TKinTreeJoint* kinTreeJointPtr );
        void _showCollisionInfo( agent::TKinTreeCollision* kinTreeCollisionPtr );
        void _showVisualInfo( agent::TKinTreeVisual* kinTreeVisualPtr );

        GLFWwindow*     m_glfwWindowPtr;
        TGLUiContext*   m_uiContextPtr;

        int             m_basicCurrentKinTreeIndx;
        std::string     m_basicCurrentKinTreeName;

        int m_basicCurrentBodyIndx;
        int m_basicCurrentJointIndx;
        int m_basicCurrentCollisionIndx;
        int m_basicCurrentVisualIndx;

        // separate ui panels to be used
        TGLUiBodies* m_uiPanelBodies;

        enum
        {
            LAST_PICKED_BODY,
            LAST_PICKED_JOINT,
            LAST_PICKED_COLLISION,
            LAST_PICKED_VISUAL
        };

        int m_lastElementPicked;

    };

}
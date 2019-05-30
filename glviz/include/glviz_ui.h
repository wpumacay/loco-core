
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
#include <glviz_sandbox.h>

namespace tysoc {
namespace viz {

    struct TCustomContextUI
    {
        bool                                            isUiActive;
        bool                                            isBasicUiActive;
        bool                                            isDebugDrawingActive;
        bool                                            isSandboxUiActive;
        GLFWwindow*                                     glfwWindowPtr;
        viz::TGLVizSandbox*                             vizSandboxPtr;
        std::vector< viz::TCustomVizKinTree* >          vizKinTreePtrs;
        std::vector< viz::TCustomVizTerrainGenerator* > vizTerrainGenPtrs;
    };

    class TCustomUI : public TIVisualizerUI
    {

        private :

        GLFWwindow*         m_glfwWindowPtr;
        TCustomContextUI*   m_uiContextPtr;

        // basic functionality ******************************************
        int             m_basicCurrentKinTreeIndx;
        std::string     m_basicCurrentKinTreeName;

        int m_basicCurrentBodyIndx;
        int m_basicCurrentJointIndx;
        int m_basicCurrentCollisionIndx;
        int m_basicCurrentVisualIndx;

        enum
        {
            LAST_PICKED_BODY,
            LAST_PICKED_JOINT,
            LAST_PICKED_COLLISION,
            LAST_PICKED_VISUAL
        };

        int m_lastElementPicked;

        void _renderBasicMainMenu();
        void _renderBasicKinTreeVisualsMenu( TCustomVizKinTree* vizKinTreePtr );
        void _renderBasicKinTreeModelInfoMenu( agent::TAgentKinTree* agentKinTreePtr );
        void _renderBasicKinTreeActionsMenu( agent::TAgentKinTree* agentKinTreePtr );
        void _renderBasicKinTreeSummary( agent::TAgentKinTree* agentKinTreePtr );

        void _showBodyInfo( agent::TKinTreeBody* kinTreeBodyPtr  );
        void _showJointInfo( agent::TKinTreeJoint* kinTreeJointPtr );
        void _showCollisionInfo( agent::TKinTreeCollision* kinTreeCollisionPtr );
        void _showVisualInfo( agent::TKinTreeVisual* kinTreeVisualPtr );
        // **************************************************************

        protected :

        void _initUIInternal() override;
        void _renderUIInternal() override;

        public :

        TCustomUI( TScenario* scenarioPtr,
                   TCustomContextUI* uiContextPtr );
        ~TCustomUI();
    };




}}
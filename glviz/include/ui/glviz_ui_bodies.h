#pragma once

// interface for our UI implementation
#include <viz/ui.h>
// cat1 rendering engine resources
#include <LApp.h>
// dear imgui resources
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glviz_ui.h>

namespace tysoc {

    class TGLUi;

    class TGLUiBodies
    {

    public :

        TGLUiBodies( TGLUi* parentUiPtr,
                     TScenario* scenarioPtr );

        ~TGLUiBodies();

        void render();

    private :

        void _renderBodyInfo();
        void _renderCollisionInfo();
        void _renderVisualInfo();

        void _renderShapeInfo( const TShapeData& data,
                               TVec3& size,
                               bool isCollider );

        TGLUi*      m_parentUiPtr;
        TScenario*  m_scenarioPtr;

        int m_currentBodyIndex;
        int m_currentCollisionIndex;
        int m_currentVisualIndex;

        TBody*      m_currentBodyPtr;
        TCollision* m_currentCollisionPtr;
        TVisual*    m_currentVisualPtr;

        std::string m_currentBodyName;
        std::string m_currentCollisionName;
        std::string m_currentVisualName;

    };

}
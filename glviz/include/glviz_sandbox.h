
#pragma once

// Rendering functionality
#include <LScene.h>
#include <LMeshBuilder.h>
#include <debug/LDebugSystem.h>

// Definitions for elements to be rendered
#include <sandbox/sandbox.h>

// Default includes to be used for visualization
#include <glviz_common.h>

#define VIZSANDBOX_AXES_DEFAULT_SIZE 0.1f

namespace tysoc {
namespace viz {

    struct TGLVizSandboxBody
    {
        engine::LModel*         axesPtr;
        engine::LIRenderable*   renderablePtr;
        sandbox::TBody*         bodyPtr;
    };

    struct TGLVizSandboxJoint
    {
        engine::LModel*         axesPtr;
        engine::LIRenderable*   renderablePtr;
        sandbox::TJoint*        jointPtr;
    };

    struct TGLVizSandboxDrawState
    {
        bool showBodies;
        bool showJoints;
        bool wireframe;
        bool frameAxes;

        TGLVizSandboxDrawState()
        {
            showBodies = true;
            showJoints = false;
            wireframe = false;
            frameAxes = false;
        }
    };

    class TGLVizSandbox
    {

        private :

        engine::LScene*     m_scenePtr;
        std::string         m_workingDir;

        std::vector< TGLVizSandboxBody >    m_vizBodies;
        std::vector< TGLVizSandboxJoint >   m_vizJoints;

        void _updateBody( TGLVizSandboxBody& body );
        void _updateJoint( TGLVizSandboxJoint& joint );

        void _collectBodies( const std::vector< sandbox::TBody* >& bodies );

        engine::LIRenderable* _createRenderable( const std::string& type,
                                                 const TVec3& size,
                                                 const TVec3& cAmbient,
                                                 const TVec3& cDiffuse,
                                                 const TVec3& cSpecular,
                                                 const std::string& filename = "" );

        void _setRenderableColor( engine::LIRenderable* renderablePtr,
                                  const TVec3& cAmbient,
                                  const TVec3& cDiffuse,
                                  const TVec3& cSpecular );

        public :

        TGLVizSandbox( const std::vector< sandbox::TBody* >& bodies,
                       engine::LScene* scenePtr,
                       const std::string& workingDir );

        ~TGLVizSandbox();

        TGLVizSandboxDrawState drawState;

        void update();

    };

}}
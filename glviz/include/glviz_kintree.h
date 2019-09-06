
#pragma once

#include <vector>
#include <iostream>
#include <string>

#include <LMeshBuilder.h>
#include <LScene.h>

#include <agent/agent.h>

#include <glviz_common.h>

#define VIZKINTREE_AXES_DEFAULT_SIZE 0.1f
#define VIZKINTREE_COLLISION_DEFAULT_MARGIN 0.01f

#define VIZKINTREE_BODY_DEFAULT_SIZE { 0.025f, 0.0f, 0.0f }
#define VIZKINTREE_BODY_DEFAULT_COLOR { 0.7f, 0.8f, 0.2f }

#define VIZKINTREE_JOINT_DEFAULT_SIZE { 0.0125f, 0.025f, 0.0f }
#define VIZKINTREE_JOINT_DEFAULT_COLOR { 0.1f, 0.1f, 0.85f }

#define VIZKINTREE_ACTUATOR_DEFAULT_SIZE { 0.025f, 0.0125f, 0.0f }
#define VIZKINTREE_ACTUATOR_DEFAULT_COLOR { 0.1f, 0.55f, 0.85f }

#define VIZKINTREE_SENSOR_DEFAULT_SIZE { 0.01f, 0.01f, 0.01f }
#define VIZKINTREE_SENSOR_DEFAULT_COLOR { 0.0f, 0.9f, 0.9f }

#define VIZKINTREE_SENSOR_DEFAULT_SIZE { 0.01f, 0.01f, 0.01f }
#define VIZKINTREE_SENSOR_DEFAULT_COLOR { 0.0f, 0.9f, 0.9f }

#define VIZKINTREE_VISUAL_DEFAULT_COLOR { 0.25f, 0.25f, 1.0f }

#define VIZKINTREE_COLLISION_DEFAULT_COLOR { 0.25f, 1.0f, 0.25f }

namespace tysoc {

    struct TGLVizKinCollision
    {
        engine::LModel*             axesPtr;
        engine::LIRenderable*       meshPtr;
        agent::TKinTreeCollision*   collisionPtr;
    };

    struct TGLVizKinVisual
    {
        engine::LModel*         axesPtr;
        engine::LIRenderable*   meshPtr;
        agent::TKinTreeVisual*  visualPtr;
    };

    struct TGLVizKinSensor
    {
        engine::LModel*         axesPtr;
        engine::LIRenderable*   meshPtr;
        agent::TKinTreeSensor*  sensorPtr;
    };

    struct TGLVizKinActuator
    {
        engine::LModel*             axesPtr;
        engine::LIRenderable*       meshPtr;
        agent::TKinTreeActuator*    actuatorPtr;
    };

    struct TGLVizKinJoint
    {
        engine::LModel*         axesPtr;
        engine::LIRenderable*   meshPtr;
        agent::TKinTreeJoint*   jointPtr;
    };

    struct TGLVizKinBody
    {
        engine::LModel*         axesPtr;
        engine::LIRenderable*   meshPtr;
        agent::TKinTreeBody*    bodyPtr;
    };

    struct TGLVizDrawState
    {
        bool drawAsWireframe;
        bool drawFrameAxes;
        bool showBodies;
        bool showVisuals;
        bool showCollisions;
        bool showJoints;
        bool showSensors;
        bool showActuators;

        TGLVizDrawState()
        {
            drawAsWireframe     = false;
            drawFrameAxes       = false;
            showBodies          = false;
            showVisuals         = true;
            showCollisions      = false;
            showSensors         = false;
            showJoints          = false;
            showActuators       = false;
        }
    };

    /**
    * This is a wrapper on top of a kintree for our ...
    * visualizer. This will construct the engine-mesh data ...
    * to be use by the visualizer, and update these meshes ...
    * properties using the wrapped kintree (which should be ...
    * updated by the underlying physics backend.)
    */
    class TGLVizKinTree
    {

        private :

        engine::LScene* m_scenePtr;
        agent::TAgent*  m_agentPtr;
        std::string     m_workingDir;

        std::vector< TGLVizKinBody >         m_vizBodies;
        std::vector< TGLVizKinJoint >        m_vizJoints;
        std::vector< TGLVizKinSensor >       m_vizSensors;
        std::vector< TGLVizKinVisual >       m_vizVisuals;
        std::vector< TGLVizKinActuator >     m_vizActuators;
        std::vector< TGLVizKinCollision >    m_vizCollisions;

        void _updateBody( TGLVizKinBody& kinBody );
        void _updateJoint( TGLVizKinJoint& kinJoint );
        void _updateSensor( TGLVizKinSensor& kinSensor );
        void _updateVisual( TGLVizKinVisual& kinVisual );
        void _updateActuator( TGLVizKinActuator& kinActuator );
        void _updateCollision( TGLVizKinCollision& kinCollision );

        void _collectFromKinTree();
        void _collectKinBodies();
        void _collectKinJoints();
        void _collectKinActuators();
        void _collectKinSensors();
        void _collectKinVisuals();
        void _collectKinCollisions();

        engine::LIRenderable* _createMesh( const std::string& type,
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

        TGLVizKinTree( agent::TAgent* agentPtr,
                           engine::LScene* scenePtr,
                           const std::string& workingDir );
        ~TGLVizKinTree();

        TGLVizDrawState drawState;

        void update();
        agent::TAgent* getKinTreePtr();
    };


}
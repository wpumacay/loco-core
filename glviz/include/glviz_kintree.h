#pragma once

#include <agent/agent.h>
#include <glviz_common.h>

using namespace tysoc::agent;

namespace tysoc {

    /**
        This is a wrapper on top of a kintree for our visualizer. This will construct 
        the engine-mesh data to be use by the visualizer, and update these meshes properties 
        using the wrapped kintree (which should be updated by the underlying physics backend.)
    */
    class TGLVizKinTree
    {
        const float AXES_SIZE = 0.1f;
        const float COLLISION_MARGIN = 0.01f;

        // body represented by a sphere-gizmo
        const TVec3 GIZMO_BODY_SIZE         = { 0.025f, 0.0f, 0.0f };
        const TVec3 GIZMO_BODY_COLOR        = { 0.7f, 0.8f, 0.2f };

        // joint represented by a cylinder-gizmo (axis along joint axis)
        const TVec3 GIZMO_JOINT_SIZE        = { 0.0125f, 0.025f, 0.0f };
        const TVec3 GIZMO_JOINT_COLOR       = { 0.1f, 0.1f, 0.85f };

        // actuator represented by a cylinder-gizmo (axis along actuation axis)
        const TVec3 GIZMO_ACTUATOR_SIZE         = { 0.025f, 0.0125f, 0.0f };
        const TVec3 GIZMO_ACTUATOR_COLOR        = { 0.1f, 0.55f, 0.85f };

        // sensor represented by a box-gizmo
        const TVec3 GIZMO_SENSOR_SIZE       = { 0.01f, 0.01f, 0.01f };
        const TVec3 GIZMO_SENSOR_COLOR      = { 0.0f, 0.9f, 0.9f };

        const TVec3 DEFAULT_VISUAL_COLOR    = { 0.25f, 0.25f, 1.0f };
        const TVec3 DEFAULT_COLLISION_COLOR = { 0.25f, 1.0f, 0.25f };

    public :

        TGLVizKinTree( agent::TAgent* agentPtr,
                       engine::CScene* scenePtr,
                       const std::string& workingDir );
        ~TGLVizKinTree();

        void update();

    private :

        void _collectKinVisuals();
        void _collectKinCollisions();

        void _updateBodyGizmos( TKinTreeBody* kinBody );
        void _updateJointGizmos( TKinTreeJoint* kinJoint );
        void _updateActuatorGizmos( TKinTreeActuator* kinActuator );
        void _updateSensorGizmos( TKinTreeSensor* kinSensor );

        void _updateVisual( TKinTreeVisual* kinVisual, engine::CIRenderable* renderable );
        void _updateCollision( TKinTreeCollision* kinCollision, engine::CIRenderable* renderable );

    private :

        engine::CScene* m_scenePtr;
        agent::TAgent*  m_agentPtr;
        std::string     m_workingDir;

        std::vector< std::pair< TKinTreeVisual*, engine::CIRenderable* > >      m_vizVisuals;
        std::vector< std::pair< TKinTreeCollision*, engine::CIRenderable* > >   m_vizCollisions;
    };

}
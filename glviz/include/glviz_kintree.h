#pragma once

#include <agent/agent.h>
#include <glviz_common.h>

using namespace engine;

namespace tysoc {

    /**
        This is a wrapper on top of a kintree for our visualizer. This will construct 
        the engine-mesh data to be use by the visualizer, and update these meshes properties 
        using the wrapped kintree (which should be updated by the underlying physics backend.)
    */
    class TGLVizKinTree
    {
        const float AXES_SCALER = 50.0f;
        const float AXES_SIZE = 0.1f;

        // body represented by a sphere-gizmo
        const float GIZMO_BODY_SCALER       = 50.0f;
        const float GIZMO_BODY_MAX_SIZE     = 0.05f;
        const float GIZMO_BODY_SIZE         = 0.025f;
        const TVec3 GIZMO_BODY_COLOR        = { 0.7f, 0.8f, 0.2f };

        // joint represented by a cylinder-gizmo (axis along joint axis)
        const float GIZMO_JOINT_SCALER      = 50.0f;
        const float GIZMO_JOINT_MAX_SIZE    = 0.05f;
        const float GIZMO_JOINT_SIZE        = 0.0125f;
        const TVec3 GIZMO_JOINT_COLOR       = { 0.1f, 0.1f, 0.85f };

        // actuator represented by a cylinder-gizmo (axis along actuation axis)
        const float GIZMO_ACTUATOR_SCALER       = 50.0f;
        const float GIZMO_ACTUATOR_MAX_SIZE     = 0.05f;
        const float GIZMO_ACTUATOR_SIZE         = 0.025f;
        const TVec3 GIZMO_ACTUATOR_COLOR        = { 0.1f, 0.55f, 0.85f };

        // sensor represented by a box-gizmo
        const float GIZMO_SENSOR_SCALER     = 50.0f;
        const float GIZMO_SENSOR_MAX_SIZE   = 0.05f;
        const float GIZMO_SENSOR_SIZE       = 0.01f;
        const TVec3 GIZMO_SENSOR_COLOR      = { 0.0f, 0.9f, 0.9f };

        const TVec3 DEFAULT_VISUAL_COLOR    = { 0.7f, 0.5f, 0.3f };
        const TVec3 DEFAULT_COLLISION_COLOR = { 0.3f, 0.5f, 0.7f };

    public :

        TGLVizKinTree( TAgent* agentPtr,
                       CScene* scenePtr );
        ~TGLVizKinTree();

        void update();

    private :

        void _collectKinVisuals();
        void _collectKinCollisions();
        void _collectGizmosSizes();

        void _updateBodyGizmos( TKinTreeBody* kinBody );
        void _updateJointGizmos( TKinTreeJoint* kinJoint );
        void _updateActuatorGizmos( TKinTreeActuator* kinActuator );
        void _updateSensorGizmos( TKinTreeSensor* kinSensor );

        void _updateVisual( TKinTreeVisual* kinVisual, CIRenderable* renderable );
        void _updateCollision( TKinTreeCollision* kinCollision, CIRenderable* renderable );

    private :

        CScene* m_scenePtr;
        TAgent* m_agentPtr;

        std::vector< std::pair< TKinTreeVisual*, CIRenderable* > >      m_vizVisuals;
        std::vector< std::pair< TKinTreeCollision*, CIRenderable* > >   m_vizCollisions;

        std::unordered_map< std::string, float > m_vizVisualsGimoSizes;
        std::unordered_map< std::string, float > m_vizCollisionsGizmoSizes;
        std::unordered_map< std::string, float > m_vizBodiesGizmoSizes;
        std::unordered_map< std::string, float > m_vizJointsGizmoSizes;
        std::unordered_map< std::string, float > m_vizActuatorsGizmoSizes;
        std::unordered_map< std::string, float > m_vizSensorsGizmoSizes;

        bool m_drawBodyGizmos;
        bool m_drawJointGizmos;
        bool m_drawActuatorGizmos;
        bool m_drawSensorGizmos;
    };

}
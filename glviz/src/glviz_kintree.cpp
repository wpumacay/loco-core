
#include <glviz_kintree.h>

using namespace tysoc::agent;

namespace tysoc {

    /***************************************************************************
    *                                                                          *
    *                           TGLVIZKINTREE-IMPL                             *
    *                                                                          *
    ***************************************************************************/

    TGLVizKinTree::TGLVizKinTree( TAgent* agentPtr,
                                  engine::CScene* scenePtr,
                                  const std::string& workingDir )
    {
        m_scenePtr      = scenePtr;
        m_agentPtr      = agentPtr;
        m_workingDir    = workingDir;

        if ( !m_agentPtr )
        {
            ENGINE_ERROR( "WARNING> There is no kintree to extract info from" );
            return;
        }

        _collectKinVisuals();
        _collectKinCollisions();
    }

    TGLVizKinTree::~TGLVizKinTree()
    {
        m_scenePtr = nullptr;
        m_agentPtr = nullptr;

        m_vizVisuals.clear();
        m_vizCollisions.clear();
    }

    void TGLVizKinTree::_collectKinVisuals()
    {
        for ( auto _visual : m_agentPtr->visuals )
        {
            auto _renderable = createRenderable( _visual->data.type,
                                                 _visual->data.size,
                                                 _visual->data.filename );

            if ( _renderable )
            {
                setRenderableColor( _renderable, 
                                    DEFAULT_VISUAL_COLOR, 
                                    DEFAULT_VISUAL_COLOR, 
                                    DEFAULT_VISUAL_COLOR );

                m_scenePtr->addRenderable( std::unique_ptr< engine::CIRenderable >( _renderable ) );
            }

            m_vizVisuals.push_back( std::make_pair( _visual, _renderable ) );
        }
    }

    void TGLVizKinTree::_collectKinCollisions()
    {
        auto _collisions = m_agentPtr->collisions;
        for ( auto _collision : _collisions )
        {
            auto _renderable = createRenderable( _collision->data.type, 
                                                 _collision->data.size, 
                                                 _collision->data.filename );

            if ( _renderable )
            {
                setRenderableColor( _renderable, 
                                    DEFAULT_COLLISION_COLOR, 
                                    DEFAULT_COLLISION_COLOR, 
                                    DEFAULT_COLLISION_COLOR );

                m_scenePtr->addRenderable( std::unique_ptr< engine::CIRenderable >( _renderable ) );
            }

            m_vizCollisions.push_back( std::make_pair( _collision, _renderable ) );
        }
    }

    void TGLVizKinTree::update()
    {
        if ( !m_agentPtr )
            return;

        /* update gizmos for bodies, joints, sensors and actuators */
        for ( auto _body : m_agentPtr->bodies )
            _updateBodyGizmos( _body );

        for ( auto _joint : m_agentPtr->joints )
            _updateJointGizmos( _joint );

        for ( auto _actuator : m_agentPtr->actuators )
            _updateActuatorGizmos( _actuator );

        for ( auto _sensor : m_agentPtr->sensors )
            _updateSensorGizmos( _sensor );

        /* update visuals (renderable and gizmos) */
        for ( auto& _pair : m_vizVisuals )
            _updateVisual( _pair.first, _pair.second );

        /* update collision (renderable and gizmos) */
        for ( auto& _pair : m_vizCollisions )
            _updateCollision( _pair.first, _pair.second );
    }

    void TGLVizKinTree::_updateBodyGizmos( TKinTreeBody* kinBody )
    {
        auto _wTransform = fromTMat4( kinBody->worldTransform );

        engine::CDebugDrawer::DrawSolidSphere( GIZMO_BODY_SIZE.x, 
                                               _wTransform,
                                               { GIZMO_BODY_COLOR.x, 
                                                 GIZMO_BODY_COLOR.y, 
                                                 GIZMO_BODY_COLOR.z, 1.0f } );

        engine::CDebugDrawer::DrawSolidAxes( AXES_SIZE, _wTransform, 1.0f );
    }

    void TGLVizKinTree::_updateJointGizmos( TKinTreeJoint* kinJoint )
    {
        auto _wTransform = fromTMat4( kinJoint->worldTransform );

        engine::CDebugDrawer::DrawSolidCylinder( GIZMO_JOINT_SIZE.x,
                                                 GIZMO_JOINT_SIZE.y,
                                                 engine::eAxis::Z,
                                                 _wTransform,
                                                 { GIZMO_JOINT_COLOR.x,
                                                   GIZMO_JOINT_COLOR.y,
                                                   GIZMO_JOINT_COLOR.z, 1.0f } );

        engine::CDebugDrawer::DrawSolidAxes( AXES_SIZE, _wTransform, 1.0f );
    }

    void TGLVizKinTree::_updateActuatorGizmos( TKinTreeActuator* kinActuator )
    {
        auto _wTransform = fromTMat4( kinActuator->worldTransform );

        engine::CDebugDrawer::DrawSolidCylinder( GIZMO_ACTUATOR_SIZE.x,
                                                 GIZMO_ACTUATOR_SIZE.y,
                                                 engine::eAxis::Z,
                                                 _wTransform,
                                                 { GIZMO_ACTUATOR_COLOR.x,
                                                   GIZMO_ACTUATOR_COLOR.y,
                                                   GIZMO_ACTUATOR_COLOR.z, 1.0f } );

        engine::CDebugDrawer::DrawSolidAxes( AXES_SIZE, _wTransform, 1.0f );
    }

    void TGLVizKinTree::_updateSensorGizmos( TKinTreeSensor* kinSensor )
    {
        auto _wTransform = fromTMat4( kinSensor->worldTransform );

        engine::CDebugDrawer::DrawSolidBox( { GIZMO_SENSOR_SIZE.x,
                                              GIZMO_SENSOR_SIZE.y,
                                              GIZMO_SENSOR_SIZE.z },
                                            _wTransform,
                                            { GIZMO_SENSOR_COLOR.x,
                                              GIZMO_SENSOR_COLOR.y,
                                              GIZMO_SENSOR_COLOR.z, 1.0f } );

        engine::CDebugDrawer::DrawSolidAxes( AXES_SIZE, _wTransform, 1.0f );
    }

    void TGLVizKinTree::_updateVisual( TKinTreeVisual* kinVisual, engine::CIRenderable* renderable )
    {
        auto _wTransform = fromTMat4( kinVisual->worldTransform );
        engine::CDebugDrawer::DrawSolidAxes( AXES_SIZE, _wTransform, 1.0f );

        if ( !renderable )
            return;

        renderable->position = fromTVec3( kinVisual->worldTransform.getPosition() );
        renderable->rotation = fromTMat3( kinVisual->worldTransform.getRotation() );
    }

    void TGLVizKinTree::_updateCollision( TKinTreeCollision* kinCollision, engine::CIRenderable* renderable )
    {
        auto _wTransform = fromTMat4( kinCollision->worldTransform );
        engine::CDebugDrawer::DrawSolidAxes( AXES_SIZE, _wTransform, 1.0f );
        if ( !renderable )
            return;

        renderable->position = fromTVec3( kinCollision->worldTransform.getPosition() );
        renderable->rotation = fromTMat3( kinCollision->worldTransform.getRotation() );
    }

}
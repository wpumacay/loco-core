
#include <glviz_kintree.h>

using namespace engine;

namespace tysoc {

    /***************************************************************************
    *                                                                          *
    *                           TGLVIZKINTREE-IMPL                             *
    *                                                                          *
    ***************************************************************************/

    TGLVizKinTree::TGLVizKinTree( TAgent* agentPtr,
                                  CScene* scenePtr,
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
        _collectGizmosSizes();
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
                _renderable->material()->setType( eMaterialType::BLINN_PHONG );

                if ( _visual->data.type != eShapeType::MESH || !_visual->data.usesMaterialFromMesh )
                {
                    setRenderableColor( _renderable, 
                                        _visual->data.ambient, 
                                        _visual->data.diffuse, 
                                        _visual->data.specular );

                    _renderable->material()->shininess = _visual->data.shininess;
                    if ( _visual->data.texture != "" && CTextureManager::HasCachedTexture( _visual->data.texture ) )
                        _renderable->material()->setAlbedoMap( CTextureManager::GetCachedTexture( _visual->data.texture ) );
                }

                m_scenePtr->addRenderable( std::unique_ptr< CIRenderable >( _renderable ) );

                // save size for gizmos of parent as well
                if ( _visual->parentBodyPtr )
                    m_vizBodiesGizmoSizes[_visual->parentBodyPtr->name] = std::min( _renderable->bsphere().radius / GIZMO_BODY_SCALER, GIZMO_BODY_MAX_SIZE );
                else
                    m_vizJointsGizmoSizes[_visual->parentBodyPtr->name] = GIZMO_BODY_SIZE;

                // save size for visual's gizmo
                m_vizVisualsGimoSizes[_visual->name] = _renderable->bsphere().radius / GIZMO_BODY_SCALER;
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

                m_scenePtr->addRenderable( std::unique_ptr< CIRenderable >( _renderable ) );
                // keep it visible until required, and also in wireframe-mode
                _renderable->setVisibility( false );
                _renderable->setWireframe( true );

                // save size for gizmos of parent as well
                if ( _collision->parentBodyPtr )
                    m_vizBodiesGizmoSizes[_collision->parentBodyPtr->name] = std::min( _renderable->bsphere().radius / GIZMO_BODY_SCALER, GIZMO_BODY_MAX_SIZE );
                else
                    m_vizBodiesGizmoSizes[_collision->parentBodyPtr->name] = GIZMO_BODY_SIZE;

                // save size for collision's gizmo
                m_vizCollisionsGizmoSizes[_collision->name] = _renderable->bsphere().radius / GIZMO_BODY_SCALER;
            }

            m_vizCollisions.push_back( std::make_pair( _collision, _renderable ) );
        }
    }

    void TGLVizKinTree::_collectGizmosSizes()
    {
        for ( auto _joint : m_agentPtr->joints )
        {
            if ( !_joint )
                continue;

            if ( _joint->parentBodyPtr )
            {
                auto _bodyName = _joint->parentBodyPtr->name;
                if ( m_vizBodiesGizmoSizes.find( _bodyName ) != m_vizBodiesGizmoSizes.end() )
                    m_vizJointsGizmoSizes[_joint->name] = std::min( m_vizBodiesGizmoSizes[_bodyName] / GIZMO_JOINT_SCALER, GIZMO_JOINT_MAX_SIZE );
                else
                    m_vizJointsGizmoSizes[_joint->name] = GIZMO_JOINT_SIZE;
            }
            else
            {
                m_vizJointsGizmoSizes[_joint->name] = GIZMO_JOINT_SIZE;
            }
        }

        for ( auto _actuator : m_agentPtr->actuators )
        {
            if ( !_actuator )
                continue;

            if ( _actuator->jointPtr )
            {
                auto _jointName = _actuator->jointPtr->name;
                if ( m_vizJointsGizmoSizes.find( _jointName ) != m_vizJointsGizmoSizes.end() )
                    m_vizActuatorsGizmoSizes[_actuator->name] = std::min( m_vizJointsGizmoSizes[_jointName] / GIZMO_ACTUATOR_SCALER, GIZMO_ACTUATOR_MAX_SIZE );
                else
                    m_vizActuatorsGizmoSizes[_actuator->name] = GIZMO_ACTUATOR_SIZE;
            }
            else
            {
                m_vizActuatorsGizmoSizes[_actuator->name] = GIZMO_ACTUATOR_SIZE;
            }
        }

        for ( auto _sensor : m_agentPtr->sensors )
        {
            if ( !_sensor )
                continue;

            if ( _sensor->bodyPtr )
            {
                auto _bodyName = _sensor->bodyPtr->name;
                if ( m_vizBodiesGizmoSizes.find( _bodyName ) != m_vizBodiesGizmoSizes.end() )
                    m_vizSensorsGizmoSizes[_sensor->name] = std::min( m_vizBodiesGizmoSizes[_bodyName] / GIZMO_SENSOR_SCALER, GIZMO_SENSOR_MAX_SIZE );
                else
                    m_vizSensorsGizmoSizes[_sensor->name] = GIZMO_SENSOR_SIZE;
            }
            else if ( _sensor->jointPtr )
            {
                auto _jointName = _sensor->jointPtr->name;
                if ( m_vizJointsGizmoSizes.find( _jointName ) != m_vizJointsGizmoSizes.end() )
                    m_vizSensorsGizmoSizes[_sensor->name] = std::min( m_vizJointsGizmoSizes[_jointName] / GIZMO_SENSOR_SCALER, GIZMO_SENSOR_MAX_SIZE );
                else
                    m_vizSensorsGizmoSizes[_sensor->name] = GIZMO_SENSOR_SIZE;
            }
            else
            {
                m_vizSensorsGizmoSizes[_sensor->name] = GIZMO_SENSOR_SIZE;
            }
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
        float _gizmoSize = m_vizBodiesGizmoSizes[kinBody->name];

        CDebugDrawer::DrawSolidSphere( _gizmoSize, 
                                       _wTransform,
                                       { GIZMO_BODY_COLOR.x, 
                                         GIZMO_BODY_COLOR.y, 
                                         GIZMO_BODY_COLOR.z, 1.0f } );

        CDebugDrawer::DrawSolidAxes( _gizmoSize * 0.5f, _wTransform, 1.0f );
    }

    void TGLVizKinTree::_updateJointGizmos( TKinTreeJoint* kinJoint )
    {
        auto _wTransform = fromTMat4( kinJoint->worldTransform );
        float _gizmoSize = m_vizJointsGizmoSizes[kinJoint->name];

        CDebugDrawer::DrawSolidCylinder( _gizmoSize * 1.0f,
                                         _gizmoSize * 2.0f,
                                         eAxis::Z,
                                         _wTransform,
                                         { GIZMO_JOINT_COLOR.x,
                                           GIZMO_JOINT_COLOR.y,
                                           GIZMO_JOINT_COLOR.z, 1.0f } );

        CDebugDrawer::DrawSolidAxes( _gizmoSize * 0.5f, _wTransform, 1.0f );
    }

    void TGLVizKinTree::_updateActuatorGizmos( TKinTreeActuator* kinActuator )
    {
        auto _wTransform = fromTMat4( kinActuator->worldTransform );
        float _gizmoSize = m_vizActuatorsGizmoSizes[kinActuator->name];

        CDebugDrawer::DrawSolidCylinder( _gizmoSize * 2.0f,
                                         _gizmoSize * 3.0f,
                                         eAxis::Z,
                                         _wTransform,
                                         { GIZMO_ACTUATOR_COLOR.x,
                                           GIZMO_ACTUATOR_COLOR.y,
                                           GIZMO_ACTUATOR_COLOR.z, 1.0f } );

        CDebugDrawer::DrawSolidAxes( _gizmoSize * 0.5f, _wTransform, 1.0f );
    }

    void TGLVizKinTree::_updateSensorGizmos( TKinTreeSensor* kinSensor )
    {
        auto _wTransform = fromTMat4( kinSensor->worldTransform );
        auto _gizmoSize = m_vizSensorsGizmoSizes[kinSensor->name];

        CDebugDrawer::DrawSolidBox( { _gizmoSize,
                                      _gizmoSize,
                                      _gizmoSize },
                                    _wTransform,
                                    { GIZMO_SENSOR_COLOR.x,
                                      GIZMO_SENSOR_COLOR.y,
                                      GIZMO_SENSOR_COLOR.z, 1.0f } );

        CDebugDrawer::DrawSolidAxes( _gizmoSize * 0.5f, _wTransform, 1.0f );
    }

    void TGLVizKinTree::_updateVisual( TKinTreeVisual* kinVisual, CIRenderable* renderable )
    {
        auto _wTransform = fromTMat4( kinVisual->worldTransform );
        //// CDebugDrawer::DrawSolidAxes( AXES_SIZE, _wTransform, 1.0f );

        if ( !renderable )
            return;

        renderable->position = fromTVec3( kinVisual->worldTransform.getPosition() );
        renderable->rotation = fromTMat3( kinVisual->worldTransform.getRotation() );
    }

    void TGLVizKinTree::_updateCollision( TKinTreeCollision* kinCollision, CIRenderable* renderable )
    {
        auto _wTransform = fromTMat4( kinCollision->worldTransform );
        //// CDebugDrawer::DrawSolidAxes( AXES_SIZE, _wTransform, 1.0f );

        if ( !renderable )
            return;

        renderable->position = fromTVec3( kinCollision->worldTransform.getPosition() );
        renderable->rotation = fromTMat3( kinCollision->worldTransform.getRotation() );
    }

}
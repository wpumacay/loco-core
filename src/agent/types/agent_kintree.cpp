
#include <agent/types/agent_kintree.h>

namespace tysoc {
namespace agent {


    TAgentKinTree::TAgentKinTree( const std::string& name,
                                  const TVec3& position )
        : TIAgent( name, position )
    {
        m_type  = "kintree";

        m_rootBodyPtr = NULL;
    }

    TAgentKinTree::~TAgentKinTree()
    {
        m_rootBodyPtr = NULL;

        for ( size_t i = 0; i < m_kinTreeJoints.size(); i++ )
        {
            delete m_kinTreeJoints[i];
        }
        m_kinTreeJoints.clear();
        m_mapKinTreeJoints.clear();

        for ( size_t i = 0; i < m_kinTreeVisuals.size(); i++ )
        {
            delete m_kinTreeVisuals[i];
        }
        m_kinTreeVisuals.clear();
        m_mapKinTreeVisuals.clear();

        for ( size_t i = 0; i < m_kinTreeCollisions.size(); i++ )
        {
            delete m_kinTreeCollisions[i];
        }
        m_kinTreeCollisions.clear();
        m_mapKinTreeCollisions.clear();

        for ( size_t i = 0; i < m_kinTreeBodies.size(); i++ )
        {
            delete m_kinTreeBodies[i];
        }
        m_kinTreeBodies.clear();
        m_mapKinTreeBodies.clear();

        for ( size_t i = 0; i < m_kinTreeActuators.size(); i++ )
        {
            delete m_kinTreeActuators[i];
        }
        m_kinTreeActuators.clear();
        m_mapKinTreeActuators.clear();

        for ( size_t i = 0; i < m_kinTreeSensors.size(); i++ )
        {
            delete m_kinTreeSensors[i];
        }
        m_kinTreeSensors.clear();
        m_mapKinTreeSensors.clear();
    }

    void TAgentKinTree::setActions( const std::vector< TScalar >& actions )
    {
        if ( actions.size() != m_kinTreeActuators.size() )
        {
            std::cout << "WARNING> actions size - mismatch" << std::endl;
            return;
        }

        for ( size_t i = 0; i < m_kinTreeActuators.size(); i++ )
        {
            m_kinTreeActuators[i]->ctrlValue = actions[i];
        }
    }

    int TAgentKinTree::getActionDim()
    {
        return m_kinTreeActuators.size();
    }

    void TAgentKinTree::_initializeKinTree()
    {
        // call specific construction functioality (virtual)
        _constructKinTree();
        // initialize the world transforms in the ...
        // kintree to some defaults (just for testing)
        _initializeWorldTransforms();
    }

    void TAgentKinTree::_updateAgentInternal( float dt )
    {
        // Update internal data from the kintree.
        // The bodies are updated by the internal backend.
        // We should update the joints, visuals and collisions ...
        // world transforms, for later usage in other sub systems
        for ( size_t i = 0; i < m_kinTreeBodies.size(); i++ )
        {
            _updateBody( m_kinTreeBodies[i] );
        }

        // update the sensors and actuators, as they are placed ...
        // in the objects updated before (joints, visuals, collisions, bodies)
        for ( size_t i = 0; i < m_kinTreeSensors.size(); i++ )
        {
            _updateSensor( m_kinTreeSensors[i] );
        }

        for ( size_t i = 0; i < m_kinTreeActuators.size(); i++ )
        {
            _updateActuator( m_kinTreeActuators[i] );
        }
    }

    void TAgentKinTree::_updateBody( TKinTreeBody* kinTreeBodyPtr )
    {
        if ( !kinTreeBodyPtr )
        {
            return;
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childVisuals.size(); i++ )
        {
            if ( kinTreeBodyPtr->childVisuals[i] )
            {
                _updateVisual( kinTreeBodyPtr->childVisuals[i] );
            }
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childCollisions.size(); i++ )
        {
            if ( kinTreeBodyPtr->childCollisions[i] )
            {
                _updateCollision( kinTreeBodyPtr->childCollisions[i] );
            }
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childJoints.size(); i++ )
        {
            if ( kinTreeBodyPtr->childJoints[i] )
            {
                _updateJoint( kinTreeBodyPtr->childJoints[i] );
            }
        }
    }

    void TAgentKinTree::_updateJoint( TKinTreeJoint* kinTreeJointPtr )
    {
        TKinTreeBody* _parent = kinTreeJointPtr->parentBodyPtr;

        if ( _parent )
        {
            kinTreeJointPtr->worldTransform = _parent->worldTransform * 
                                              kinTreeJointPtr->relTransform;
        }
        else
        {
            std::cout << "WARNING> joint " << kinTreeJointPtr->name << " "
                      << "does not have a parent body" << std::endl;
        }
    }

    void TAgentKinTree::_updateVisual( TKinTreeVisual* kinTreeVisualPtr )
    {
        TKinTreeBody* _parent = kinTreeVisualPtr->parentBodyPtr;

        if ( _parent )
        {
            kinTreeVisualPtr->geometry.worldTransform = _parent->worldTransform *
                                                        kinTreeVisualPtr->relTransform;
        }
        else
        {
            std::cout << "WARNING> visual " << kinTreeVisualPtr->name << " "
                      << "does not have a parent body" << std::endl;
        }

    }

    void TAgentKinTree::_updateCollision( TKinTreeCollision* kinTreeCollisionPtr )
    {
        TKinTreeBody* _parent = kinTreeCollisionPtr->parentBodyPtr;

        if ( _parent )
        {
            kinTreeCollisionPtr->geometry.worldTransform = _parent->worldTransform * 
                                                           kinTreeCollisionPtr->relTransform;
        }
        else
        {
            std::cout << "WARNING> collision " << kinTreeCollisionPtr->name << " "
                      << "does not have a parent body" << std::endl;
        }
    }

    void TAgentKinTree::_updateActuator( TKinTreeActuator* kinTreeActuatorPtr )
    {
        TKinTreeJoint* _joint = kinTreeActuatorPtr->jointPtr;

        if ( _joint )
        {
            kinTreeActuatorPtr->worldTransform = _joint->worldTransform;
        }
        else
        {
            std::cout << "WARNING> actuator " << kinTreeActuatorPtr->name << " "
                      << "does not have a joint to be attached to" << std::endl;
        }
    }

    void TAgentKinTree::_updateSensor( TKinTreeSensor* kinTreeSensorPtr )
    {
        if ( kinTreeSensorPtr->bodyPtr )
        {
            kinTreeSensorPtr->worldTransform = kinTreeSensorPtr->bodyPtr->worldTransform;
        }
        else if ( kinTreeSensorPtr->jointPtr )
        {
            kinTreeSensorPtr->worldTransform = kinTreeSensorPtr->jointPtr->worldTransform;
        }
    }

    std::vector< TKinTreeBody* > TAgentKinTree::getKinTreeBodies()
    {
        return m_kinTreeBodies;
    }

    std::vector< TKinTreeJoint* > TAgentKinTree::getKinTreeJoints()
    {
        return m_kinTreeJoints;
    }

    std::vector< TKinTreeVisual* > TAgentKinTree::getKinTreeVisuals()
    {
        return m_kinTreeVisuals;
    }

    std::vector< TKinTreeCollision* > TAgentKinTree::getKinTreeCollisions()
    {
        return m_kinTreeCollisions;
    }

    std::vector< TKinTreeActuator* > TAgentKinTree::getKinTreeActuators()
    {
        return m_kinTreeActuators;
    }

    std::vector< TKinTreeSensor* > TAgentKinTree::getKinTreeSensors()
    {
        return m_kinTreeSensors;
    }

}}
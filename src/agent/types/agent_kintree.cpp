
#include <agent/types/agent_kintree.h>

namespace tysoc {
namespace agent {


    TAgentKinTree::TAgentKinTree( const std::string& name,
                                  const TVec3& position )
        : TIAgent( name, position )
    {
        m_type  = AGENT_TYPE_KINTREE;

        m_modelTemplateType = "";
        m_rootBodyPtr       = NULL;
    }

    TAgentKinTree::~TAgentKinTree()
    {
        m_rootBodyPtr = NULL;

        for ( size_t i = 0; i < m_kinTreeMeshAssets.size(); i++ )
        {
            delete m_kinTreeMeshAssets[i];
        }
        m_kinTreeMeshAssets.clear();
        m_mapKinTreeMeshAssets.clear();

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

    void TAgentKinTree::getActuatorLimits( size_t actuatorIndx, TScalar& minCtrl, TScalar& maxCtrl )
    {
        if ( 0 > actuatorIndx || actuatorIndx >= m_kinTreeActuators.size()  )
        {
            std::cout << "WARNING> trying to query an our of range actuator" << std::endl;
            return;
        }

        minCtrl = m_kinTreeActuators[ actuatorIndx ]->minCtrl;
        maxCtrl = m_kinTreeActuators[ actuatorIndx ]->maxCtrl;
    }

    void TAgentKinTree::_initializeKinTree()
    {
        // call specific construction functioality (virtual)
        _constructKinTree();
        // create the default sensors for any agent (joint sensors)
        _constructDefaultSensors();
        // initialize the world transforms in the ...
        // kintree to some defaults (just for testing)
        _initializeWorldTransforms();
    }

    void TAgentKinTree::_constructDefaultSensors()
    {
        // Construct joint sensors for each joint
        for ( size_t i = 0; i < m_kinTreeJoints.size(); i++ )
        {
            if ( m_kinTreeJoints[i]->type != "continuous" &&
                 m_kinTreeJoints[i]->type != "revolute" &&
                 m_kinTreeJoints[i]->type != "hinge" &&
                 m_kinTreeJoints[i]->type != "prismatic" &&
                 m_kinTreeJoints[i]->type != "slide" )
            {
                // Only these types of joints should have a joint-sensor ...
                // associated with it
                continue;
            }

            auto _kinTreeJointSensor = new TKinTreeJointSensor();
            // set joint parent name
            _kinTreeJointSensor->jointName = m_kinTreeJoints[i]->name;
            // set an appropiate name (will be used by wrappers, like mujoco)
            _kinTreeJointSensor->name = std::string( "sensor_" ) + m_name + 
                                        std::string( "_" ) + m_kinTreeJoints[i]->name;
            // and just add it to the list of sensors
            m_kinTreeSensors.push_back( _kinTreeJointSensor );
        }

        // Construct joint sensors for each body
        for ( size_t i = 0; i < m_kinTreeBodies.size(); i++ )
        {
            auto _kinTreeBodySensor = new TKinTreeBodySensor();
            // set body parent name
            _kinTreeBodySensor->bodyName = m_kinTreeBodies[i]->name;
            // set an appropiate name (will be used by wrappers, like mujoco)
            _kinTreeBodySensor->name = std::string( "sensor" ) + m_name + 
                                       std::string( "_" ) + m_kinTreeBodies[i]->name;
            // and just add it to the list of sensors
            m_kinTreeSensors.push_back( _kinTreeBodySensor );
        }
    }

    void TAgentKinTree::_initializeWorldTransforms()
    {
        if ( m_rootBodyPtr )
        {
            // start from the root (set transform as the start position passed)
            m_rootBodyPtr->worldTransform.setPosition( m_position );
            // make an update in the tree (the default transforms ...
            // should give a results that makes sense, at least visually)
            _initializeBody( m_rootBodyPtr );

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
    }

    void TAgentKinTree::_initializeBody( TKinTreeBody* kinTreeBodyPtr )
    {
        if ( !kinTreeBodyPtr )
        {
            return;
        }

        if ( kinTreeBodyPtr->parentBodyPtr )
        {
            kinTreeBodyPtr->worldTransform = kinTreeBodyPtr->parentBodyPtr->worldTransform * 
                                             kinTreeBodyPtr->relTransform;
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

        for ( size_t i = 0; i < kinTreeBodyPtr->childBodies.size(); i++ )
        {
            if ( kinTreeBodyPtr->childBodies[i] )
            {
                _initializeBody( kinTreeBodyPtr->childBodies[i] );
            }
        }
    }

    void TAgentKinTree::_updateAgentInternal( float dt )
    {
        // Update internal data from the kintree.
        // The bodies are updated by the internal backend.
        // We should update the joints, visuals and collisions ...
        // world transforms, for later usage in other sub systems
        for ( size_t i = 0; i < m_kinTreeBodies.size(); i++ )
        {
            _updateBodyComponents( m_kinTreeBodies[i] );
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

        // update the global position of the kintree
        m_position = m_rootBodyPtr->worldTransform.getPosition();
    }

    void TAgentKinTree::_resetAgentInternal()
    {
        // send back the agent to the original position
        m_rootBodyPtr->worldTransform = TMat4();
        _initializeWorldTransforms();
    }

    void TAgentKinTree::_updateBodyComponents( TKinTreeBody* kinTreeBodyPtr )
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

    std::vector< TKinTreeMeshAsset* > TAgentKinTree::getKinTreeMeshAssets()
    {
        return m_kinTreeMeshAssets;
    }

    TKinTreeBody* TAgentKinTree::getRootBody()
    {
        return m_rootBodyPtr;
    }

    std::string TAgentKinTree::getModelTemplateType()
    {
        return m_modelTemplateType;
    }

    // Some logging functionality 
    // @REFACTOR|@CLEAN|@WUT
    // Could change this a bit by making each TKinTree(yadayadayada) have its ...
    // own serialization functionality, so we can print to console, save as a format, ...
    // or whatever we want. The code is already in a way here, so perhaps just making each ...
    // node a serializable could help a bit in the future

    std::string TAgentKinTree::toString()
    {
        std::string _res;

        if ( !m_rootBodyPtr )
            return _res;

        _res += "******************************************************\n\r";
        // show bodies info
        _res += "BODIES INFO: ----------------\n\r";
        _res += std::string( "name: " ) + m_name + std::string( "\n\r" );
        _res += std::string( "bodies: " ) + std::string( "\n\r" );
        for ( size_t i = 0; i < m_kinTreeBodies.size(); i++ )
        {
            _res += std::string( "\tbodyName: " ) + 
                    m_kinTreeBodies[i]->name + 
                    std::string( "\n\r" );

            _res += std::string( "\tbodyRelTransform: " ) +
                    TMat4::toString( m_kinTreeBodies[i]->relTransform ) +
                    std::string( "\n\r" );

            if ( m_kinTreeBodies[i]->inertiaPtr )
            {
                _res += std::string( "\tbodyMass: " ) + 
                        std::to_string( m_kinTreeBodies[i]->inertiaPtr->mass ) +
                        std::string( "\n\r" );

                auto _inertiaMat = TMat3( m_kinTreeBodies[i]->inertiaPtr->ixx,
                                          m_kinTreeBodies[i]->inertiaPtr->ixy,
                                          m_kinTreeBodies[i]->inertiaPtr->ixz,
                                          m_kinTreeBodies[i]->inertiaPtr->ixy,
                                          m_kinTreeBodies[i]->inertiaPtr->iyy,
                                          m_kinTreeBodies[i]->inertiaPtr->iyz,
                                          m_kinTreeBodies[i]->inertiaPtr->ixz,
                                          m_kinTreeBodies[i]->inertiaPtr->iyz,
                                          m_kinTreeBodies[i]->inertiaPtr->izz );

                _res += std::string( "\tbodyInertia: " ) + 
                        TMat3::toString( _inertiaMat ) +
                        std::string( "\n\r" );

                _res += std::string( "\tbodyInertiaRelFrame: " ) +
                        TMat4::toString( m_kinTreeBodies[i]->inertiaPtr->relTransform ) +
                        std::string( "\n\r" );
            }

            if ( m_kinTreeBodies[i]->parentBodyPtr )
            {
                _res += std::string( "\tbodyParentBody: " ) +
                        m_kinTreeBodies[i]->parentBodyPtr->name +
                        std::string( "\n\r" );
            }
            else
            {
                _res += std::string( "\tbodyParentBody: NULL\n\r" );
            }

            if ( m_kinTreeBodies[i]->parentJointPtr )
            {
                _res += std::string( "\tbodyParentJoint: " ) +
                        m_kinTreeBodies[i]->parentJointPtr->name +
                        std::string( "\n\r" );
            }
            else
            {
                _res += std::string( "\tbodyParentJoint: NULL\n\r" );
            }

            _res += std::string( "\tbodyChildVisuals: " );
            for ( size_t j = 0; j < m_kinTreeBodies[i]->childVisuals.size(); j++ )
            {
                _res += m_kinTreeBodies[i]->childVisuals[j]->name + 
                        ( ( j != ( m_kinTreeBodies[i]->childVisuals.size() - 1 ) ) ? 
                                    std::string( ", " ) : std::string( "" ) );
            }
            _res += std::string( "\n\r" );

            _res += std::string( "\tbodyChildCollisions: " );
            for ( size_t j = 0; j < m_kinTreeBodies[i]->childCollisions.size(); j++ )
            {
                _res += m_kinTreeBodies[i]->childCollisions[j]->name + 
                        ( ( j != ( m_kinTreeBodies[i]->childCollisions.size() - 1 ) ) ? 
                                    std::string( ", " ) : std::string( "" ) );
            }
            _res += std::string( "\n\r" );

            _res += std::string( "\tbodyChildJoints: " );
            for ( size_t j = 0; j < m_kinTreeBodies[i]->childJoints.size(); j++ )
            {
                _res += m_kinTreeBodies[i]->childJoints[j]->name + 
                        ( ( j != ( m_kinTreeBodies[i]->childJoints.size() - 1 ) ) ? 
                                    std::string( ", " ) : std::string( "" ) );
            }
            _res += std::string( "\n\r" );
        }

        _res += "-----------------------------\n\r";
        _res += "JOINTS INFO: ----------------\n\r";
        
        for ( size_t i = 0; i < m_kinTreeJoints.size(); i++ )
        {
            _res += std::string( "\tjoint.Name: " ) +
                    m_kinTreeJoints[i]->name + 
                    std::string( "\n\r" );

            _res += std::string( "\tjoint.Type: " ) +
                    m_kinTreeJoints[i]->type + 
                    std::string( "\n\r" );

            _res += std::string( "\tjoint.Axis: " ) +
                    TVec3::toString( m_kinTreeJoints[i]->axis ) + 
                    std::string( "\n\r" );

            _res += std::string( "\tjoint.RelTransform: " ) +
                    TMat4::toString( m_kinTreeJoints[i]->relTransform ) + 
                    std::string( "\n\r" );

            if ( m_kinTreeJoints[i]->parentBodyPtr )
            {
                _res += std::string( "\tjoint.ParentBody: " ) +
                        m_kinTreeJoints[i]->parentBodyPtr->name +
                        std::string( "\n\r" );
            }
            else
            {
                _res += std::string( "\tjoint.ParentBody: NULL\n\r" );
            }

            if ( m_kinTreeJoints[i]->childBodyPtr )
            {
                _res += std::string( "\tjoint.ChildBody: " ) +
                        m_kinTreeJoints[i]->childBodyPtr->name +
                        std::string( "\n\r" );
            }
            else
            {
                _res += std::string( "\tjoint.ChildBody: NULL\n\r" );
            }

            _res += std::string( "\tjoint.lowerLimit: " ) +
                    std::to_string( m_kinTreeJoints[i]->lowerLimit ) +
                    std::string( "\n\r" );

            _res += std::string( "\tjoint.upperLimit: " ) +
                    std::to_string( m_kinTreeJoints[i]->upperLimit ) +
                    std::string( "\n\r" );

            _res += std::string( "\tjoint.limited: " ) +
                    std::string( ( m_kinTreeJoints[i]->limited ) ? "true" : "false" ) +
                    std::string( "\n\r" );
        }

        _res += "-----------------------------\n\r";
        _res += "VISUALS INFO: ---------------\n\r";

        for ( size_t i = 0; i < m_kinTreeVisuals.size(); i++ )
        {
            _res += std::string( "\tvisual.Name: " ) +
                    m_kinTreeVisuals[i]->name + 
                    std::string( "\n\r" );

            _res += std::string( "\tvisual.Type: " ) +
                    m_kinTreeVisuals[i]->geometry.type + 
                    std::string( "\n\r" );

            _res += std::string( "\tvisual.MeshId: " ) +
                    m_kinTreeVisuals[i]->geometry.meshId + 
                    std::string( "\n\r" );

            _res += std::string( "\tvisual.Filename: " ) +
                    m_kinTreeVisuals[i]->geometry.filename + 
                    std::string( "\n\r" );

            _res += std::string( "\tvisual.Size: " ) +
                    TVec3::toString( m_kinTreeVisuals[i]->geometry.size ) + 
                    std::string( "\n\r" );

            _res += std::string( "\tvisual.RelTransform: " ) +
                    TMat4::toString( m_kinTreeVisuals[i]->relTransform ) + 
                    std::string( "\n\r" );

            if ( m_kinTreeVisuals[i]->parentBodyPtr )
            {
                _res += std::string( "\tvisual.ParentBody: " ) +
                        m_kinTreeVisuals[i]->parentBodyPtr->name +
                        std::string( "\n\r" );
            }
            else
            {
                _res += std::string( "\tvisual.ParentBody: NULL\n\r" );
            }
        }

        _res += "-----------------------------\n\r";
        _res += "COLLISIONS INFO: ------------\n\r";

        for ( size_t i = 0; i < m_kinTreeCollisions.size(); i++ )
        {
            _res += std::string( "\tcollision.Name: " ) +
                    m_kinTreeCollisions[i]->name + 
                    std::string( "\n\r" );

            _res += std::string( "\tcollision.Type: " ) +
                    m_kinTreeCollisions[i]->geometry.type + 
                    std::string( "\n\r" );

            _res += std::string( "\tcollision.MeshId: " ) +
                    m_kinTreeCollisions[i]->geometry.meshId + 
                    std::string( "\n\r" );

            _res += std::string( "\tcollision.Filename: " ) +
                    m_kinTreeCollisions[i]->geometry.filename + 
                    std::string( "\n\r" );

            _res += std::string( "\tcollision.Size: " ) +
                    TVec3::toString( m_kinTreeCollisions[i]->geometry.size ) + 
                    std::string( "\n\r" );

            _res += std::string( "\tcollision.RelTransform: " ) +
                    TMat4::toString( m_kinTreeCollisions[i]->relTransform ) + 
                    std::string( "\n\r" );

            if ( m_kinTreeCollisions[i]->parentBodyPtr )
            {
                _res += std::string( "\tcollision.ParentBody: " ) +
                        m_kinTreeCollisions[i]->parentBodyPtr->name +
                        std::string( "\n\r" );
            }
            else
            {
                _res += std::string( "\tcollision.ParentBody: NULL\n\r" );
            }
        }

        _res += "-----------------------------\n\r";
        _res += "SENSORS INFO: ---------------\n\r";

        for ( size_t i = 0; i < m_kinTreeSensors.size(); i++ )
        {
            _res += std::string( "\tsensor.Name: " ) +
                    m_kinTreeSensors[i]->name + 
                    std::string( "\n\r" );

            _res += std::string( "\tsensor.Type: " ) +
                    m_kinTreeSensors[i]->type + 
                    std::string( "\n\r" );

            if ( m_kinTreeSensors[i]->bodyPtr )
            {
                _res += std::string( "\tsensor.ParentBody: " ) +
                        m_kinTreeSensors[i]->bodyPtr->name +
                        std::string( "\n\r" );
            }
            else
            {
                _res += std::string( "\tsensor.ParentBody: NULL\n\r" );
            }

            if ( m_kinTreeSensors[i]->jointPtr )
            {
                _res += std::string( "\tsensor.ChildBody: " ) +
                        m_kinTreeSensors[i]->jointPtr->name +
                        std::string( "\n\r" );
            }
            else
            {
                _res += std::string( "\tsensor.ChildBody: NULL\n\r" );
            }
        }

        _res += "-----------------------------\n\r";
        _res += "ACTUATORS INFO: -------------\n\r";

        for ( size_t i = 0; i < m_kinTreeActuators.size(); i++ )
        {
            _res += std::string( "\tactuator.Name: " ) +
                    m_kinTreeSensors[i]->name + 
                    std::string( "\n\r" );

            _res += std::string( "\tactuator.Type: " ) +
                    m_kinTreeSensors[i]->type + 
                    std::string( "\n\r" );

            _res += std::string( "\tactuator.RelTransform: " ) +
                    TMat4::toString( m_kinTreeActuators[i]->relTransform ) + 
                    std::string( "\n\r" );

            _res += std::string( "\tactuator.minCtrl: " ) +
                    std::to_string( m_kinTreeActuators[i]->minCtrl ) +
                    std::string( "\n\r" );

            _res += std::string( "\tactuator.maxCtrl: " ) +
                    std::to_string( m_kinTreeActuators[i]->maxCtrl ) +
                    std::string( "\n\r" );

            if ( m_kinTreeActuators[i]->jointPtr )
            {
                _res += std::string( "\tactuator.ChildBody: " ) +
                        m_kinTreeActuators[i]->jointPtr->name +
                        std::string( "\n\r" );
            }
            else
            {
                _res += std::string( "\tactuator.ChildBody: NULL\n\r" );
            }
        }

        _res += "-----------------------------\n\r";
        _res += "TREE STRUCTURE: -------------\n\r";

        _res += _strTraverse( m_rootBodyPtr, 0 );

        _res += "-----------------------------\n\r";
        _res += "******************************************************\n\r";
    }

    std::string TAgentKinTree::_strTraverse( TKinTreeBody* kinTreeBodyPtr, size_t depth )
    {
        std::string _res;

        std::string _preTabs;
        for ( size_t i = 0; i < depth; i++ )
        {
            _preTabs += "\t";
        }

        _res += _preTabs;
        _res += "body: ";
        _res += kinTreeBodyPtr->name;
        _res += "\n\r";

        _res += _preTabs;
        _res += "childJoints: [";
        for ( size_t i = 0; i < kinTreeBodyPtr->childJoints.size(); i++ )
        {
            _res += kinTreeBodyPtr->childJoints[i]->name;
            _res += ( i != ( kinTreeBodyPtr->childJoints.size() - 1 ) ) ? ", " : "";
        }
        _res += "]\n\r";

        _res += _preTabs;
        _res += "childVisuals: [";
        for ( size_t i = 0; i < kinTreeBodyPtr->childVisuals.size(); i++ )
        {
            _res += kinTreeBodyPtr->childVisuals[i]->name;
            _res += ( i != ( kinTreeBodyPtr->childVisuals.size() - 1 ) ) ? ", " : "";
        }
        _res += "]\n\r";

        _res += _preTabs;
        _res += "childCollisions: [";
        for ( size_t i = 0; i < kinTreeBodyPtr->childCollisions.size(); i++ )
        {
            _res += kinTreeBodyPtr->childCollisions[i]->name;
            _res += ( i != ( kinTreeBodyPtr->childCollisions.size() - 1 ) ) ? ", " : "";
        }
        _res += "]\n\r";

        for ( size_t i = 0; i < kinTreeBodyPtr->childBodies.size(); i++ )
        {
            if ( !kinTreeBodyPtr->childBodies[i] )
                continue;

            _res += _strTraverse( kinTreeBodyPtr->childBodies[i], depth + 1 );
        }

        return _res;
    }

}}
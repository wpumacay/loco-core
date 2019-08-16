
#include <agent/agent_base.h>

namespace tysoc {
namespace agent {


    TAgent::TAgent( const std::string& name,
                    const TVec3& position,
                    const TVec3& rotation )
    {

        m_name = name;
        m_type  = AGENT_TYPE_BASE;

        m_position = position;
        m_rotation = rotation;
        m_startPosition = position;
        m_startRotation = rotation;

        m_modelFormat = MODEL_FORMAT_NONE;

        m_modelDataMjcfPtr  = NULL;
        m_modelDataUrdfPtr  = NULL;
        m_modelDataRlsimPtr = NULL;

        m_rootBodyPtr = NULL;
    }

    TAgent::TAgent( mjcf::GenericElement* modelDataPtr,
                    const std::string& name,
                    const TVec3& position,
                    const TVec3& rotation  ) : TAgent( name, position, rotation )
    {
        // use mjcf-format helpers to construct the kintree for this agent
        constructAgentFromModel( this, modelDataPtr );

        m_modelFormat = MODEL_FORMAT_MJCF;
        m_modelDataMjcfPtr = modelDataPtr;
    }

    TAgent::TAgent( urdf::UrdfModel* modelDataPtr,
                    const std::string& name,
                    const TVec3& position,
                    const TVec3& rotation  ) : TAgent( name, position, rotation )
    {
        // use urdf-format helpers to construct the kintree for this agent
        constructAgentFromModel( this, modelDataPtr );

        m_modelFormat = MODEL_FORMAT_URDF;
        m_modelDataUrdfPtr = modelDataPtr;
    }

    TAgent::TAgent( rlsim::RlsimModel* modelDataPtr,
                    const std::string& name,
                    const TVec3& position,
                    const TVec3& rotation  ) : TAgent( name, position, rotation )
    {
        // use rlsim-format helpers to construct the kintree for this agent
        constructAgentFromModel( this, modelDataPtr );

        m_modelFormat = MODEL_FORMAT_RLSIM;
        m_modelDataRlsimPtr = modelDataPtr;
    }

    TAgent::~TAgent()
    {
        TYSOC_LOG( "Destroying agent: " + m_name );

        m_rootBodyPtr = NULL;

        for ( size_t i = 0; i < meshAssets.size(); i++ )
        {
            delete meshAssets[i];
        }
        meshAssets.clear();
        meshAssetsMap.clear();

        for ( size_t i = 0; i < joints.size(); i++ )
        {
            delete joints[i];
        }
        joints.clear();
        jointsMap.clear();

        for ( size_t i = 0; i < visuals.size(); i++ )
        {
            delete visuals[i];
        }
        visuals.clear();
        visualsMap.clear();

        for ( size_t i = 0; i < collisions.size(); i++ )
        {
            delete collisions[i];
        }
        collisions.clear();
        collisionsMap.clear();

        for ( size_t i = 0; i < bodies.size(); i++ )
        {
            delete bodies[i];
        }
        bodies.clear();
        bodiesMap.clear();

        for ( size_t i = 0; i < actuators.size(); i++ )
        {
            delete actuators[i];
        }
        actuators.clear();
        actuatorsMap.clear();

        for ( size_t i = 0; i < sensors.size(); i++ )
        {
            delete sensors[i];
        }
        sensors.clear();
        sensorsMap.clear();
    }

    void TAgent::initialize()
    {
        // create the default sensors for any agent
        _constructDefaultSensors();
        // initialize the world transforms in the kintree to the initial configuration
        _initializeWorldTransforms();

        // make the specific initialization required by the derived agents
        _initializeAgentInternal();
    }

    void TAgent::update()
    {
        if ( !m_rootBodyPtr )
        {
            std::cout << "ERROR> kintree with undefined root body" << std::endl;
            return;
        }

    #ifndef UPDATE_TREE_RECURSIVE_DH
        _update_v1();
    #else 
        _update_v2();
    #endif

        // Then, update the sensors and actuators, as they are placed ...
        // in the objects updated before (joints, visuals, collisions, bodies)
        for ( size_t i = 0; i < sensors.size(); i++ )
            _updateSensor( sensors[i] );

        for ( size_t i = 0; i < actuators.size(); i++ )
            _updateActuator( actuators[i] );

        // update the global position of the kintree
        m_position = m_rootBodyPtr->worldTransform.getPosition();
        m_rotation = m_rootBodyPtr->worldTransform.getRotEuler();

        // make the specific updates required by the derived agents
        _updateAgentInternal();
    }

    void TAgent::_update_v1()
    {
        // Update internal data from the kintree. The bodies are ... 
        // updated by the internal backend. We should update the ... 
        // joints, visuals and collisions world transforms, for ...
        // later usage by other modules.
        for ( size_t i = 0; i < bodies.size(); i++ )
            _updateBodyComponents( bodies[i] );
    }

    void TAgent::_update_v2()
    {
        
    }

    void TAgent::reset()
    {
        // restore initial position
        m_position = m_startPosition;
        _initializeWorldTransforms();

        // make the specific reset steps required by the derived agents
        _resetAgentInternal();
    }

    void TAgent::setActions( const std::vector< TScalar >& actions )
    {
        if ( actions.size() != actuators.size() )
        {
            std::cout << "WARNING> actions size - mismatch" << std::endl;
            return;
        }

        for ( size_t i = 0; i < actuators.size(); i++ )
        {
            actuators[i]->ctrlValue = actions[i];
        }
    }

    TVec2 TAgent::getActuatorLimits( int actuatorIndx )
    {
        if ( 0 > actuatorIndx || actuatorIndx >= actuators.size()  )
        {
            std::cout << "WARNING> tried to query non-existent actuator: " 
                      << actuatorIndx << std::endl;
            return { 0.0, 0.0 };
        }

        return { actuators[ actuatorIndx ]->minCtrl,
                 actuators[ actuatorIndx ]->maxCtrl };
    }

    void TAgent::_constructDefaultSensors()
    {
        // Construct joint sensors for each joint
        for ( size_t i = 0; i < joints.size(); i++ )
        {
            if ( joints[i]->type != "continuous" &&
                 joints[i]->type != "revolute" &&
                 joints[i]->type != "hinge" &&
                 joints[i]->type != "prismatic" &&
                 joints[i]->type != "slide" )
            {
                // Only these types of joints should have a joint-sensor ...
                // associated with it
                continue;
            }

            auto _kinTreeJointSensor = new TKinTreeJointSensor();
            // set joint parent name
            _kinTreeJointSensor->jointName = joints[i]->name;
            // set an appropiate name (will be used by wrappers, like mujoco)
            _kinTreeJointSensor->name = std::string( "sensor_" ) + m_name + 
                                        std::string( "_" ) + joints[i]->name;
            // and just add it to the list of sensors
            sensors.push_back( _kinTreeJointSensor );
        }

        // Construct joint sensors for each body
        for ( size_t i = 0; i < bodies.size(); i++ )
        {
            auto _kinTreeBodySensor = new TKinTreeBodySensor();
            // set body parent name
            _kinTreeBodySensor->bodyName = bodies[i]->name;
            // set an appropiate name (will be used by wrappers, like mujoco)
            _kinTreeBodySensor->name = std::string( "sensor" ) + m_name + 
                                       std::string( "_" ) + bodies[i]->name;
            // and just add it to the list of sensors
            sensors.push_back( _kinTreeBodySensor );
        }
    }

    void TAgent::_initializeWorldTransforms()
    {
        if ( !m_rootBodyPtr )
            return;

        // start from the root (set transform as the start position passed)
        m_rootBodyPtr->worldTransform.setPosition( m_startPosition );
        m_rootBodyPtr->worldTransform.setRotation( TMat3::fromEuler( m_startRotation ) );

        // compensate it with the zero configuration transform
        m_rootBodyPtr->worldTransform = m_rootBodyPtr->worldTransform * m_zeroCompensation;

        // make an update in the tree (the default transforms ...
        // should give a results that makes sense, at least visually)
        _initializeBody( m_rootBodyPtr );

        // update the sensors and actuators, as they are placed ...
        // in the objects updated before (joints, visuals, collisions, bodies)
        for ( size_t i = 0; i < sensors.size(); i++ )
            _updateSensor( sensors[i] );

        for ( size_t i = 0; i < actuators.size(); i++ )
            _updateActuator( actuators[i] );
    }

    void TAgent::_initializeBody( TKinTreeBody* kinTreeBodyPtr )
    {
        if ( !kinTreeBodyPtr )
        {
            return;
        }

        // Update the frame of non-root bodies based on their parents' frame
        if ( kinTreeBodyPtr->parentBodyPtr )
        {
            kinTreeBodyPtr->worldTransform = kinTreeBodyPtr->parentBodyPtr->worldTransform * 
                                             kinTreeBodyPtr->relTransform;
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childVisuals.size(); i++ )
        {
            if ( kinTreeBodyPtr->childVisuals[i] )
                _updateVisual( kinTreeBodyPtr->childVisuals[i] );
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childCollisions.size(); i++ )
        {
            if ( kinTreeBodyPtr->childCollisions[i] )
                _updateCollision( kinTreeBodyPtr->childCollisions[i] );
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childJoints.size(); i++ )
        {
            if ( kinTreeBodyPtr->childJoints[i] )
                _updateJoint( kinTreeBodyPtr->childJoints[i] );
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childBodies.size(); i++ )
        {
            if ( kinTreeBodyPtr->childBodies[i] )
                _initializeBody( kinTreeBodyPtr->childBodies[i] );
        }
    }

    void TAgent::_updateBodyComponents( TKinTreeBody* kinTreeBodyPtr )
    {
        if ( !kinTreeBodyPtr )
        {
            return;
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childVisuals.size(); i++ )
        {
            if ( kinTreeBodyPtr->childVisuals[i] )
                _updateVisual( kinTreeBodyPtr->childVisuals[i] );
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childCollisions.size(); i++ )
        {
            if ( kinTreeBodyPtr->childCollisions[i] )
                _updateCollision( kinTreeBodyPtr->childCollisions[i] );
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childJoints.size(); i++ )
        {
            if ( kinTreeBodyPtr->childJoints[i] )
                _updateJoint( kinTreeBodyPtr->childJoints[i] );
        }
    }

    void TAgent::_updateJoint( TKinTreeJoint* kinTreeJointPtr )
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

    void TAgent::_updateVisual( TKinTreeVisual* kinTreeVisualPtr )
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

    void TAgent::_updateCollision( TKinTreeCollision* kinTreeCollisionPtr )
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

    void TAgent::_updateActuator( TKinTreeActuator* kinTreeActuatorPtr )
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

    void TAgent::_updateSensor( TKinTreeSensor* kinTreeSensorPtr )
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

    std::string TAgent::toString()
    {
        std::string _res;

        if ( !m_rootBodyPtr )
            return _res;

        _res += "******************************************************\n\r";
        _res += std::string( "name: " ) + m_name + std::string( "\n\r" );

        _res += "BODIES INFO: ----------------\n\r";

        _res += std::string( "bodies: " ) + std::string( "\n\r" );
        for ( size_t i = 0; i < bodies.size(); i++ )
            _res += agent::toString( this->bodies[i] );

        _res += "-----------------------------\n\r";

        _res += "JOINTS INFO: ----------------\n\r";
        
        for ( size_t i = 0; i < joints.size(); i++ )
            _res += agent::toString( this->joints[i] );

        _res += "-----------------------------\n\r";

        _res += "COLLISIONS INFO: ------------\n\r";

        for ( size_t i = 0; i < collisions.size(); i++ )
            _res += agent::toString( this->collisions[i] );

        _res += "-----------------------------\n\r";

        _res += "VISUALS INFO: ---------------\n\r";

        for ( size_t i = 0; i < visuals.size(); i++ )
            _res += agent::toString( this->visuals[i] );

        _res += "-----------------------------\n\r";

        _res += "SENSORS INFO: ---------------\n\r";

        for ( size_t i = 0; i < sensors.size(); i++ )
            _res += agent::toString( this->sensors[i] );

        _res += "-----------------------------\n\r";

        _res += "ACTUATORS INFO: -------------\n\r";

        for ( size_t i = 0; i < actuators.size(); i++ )
            _res += agent::toString( this->actuators[i] );

        _res += "-----------------------------\n\r";
        _res += "TREE STRUCTURE: -------------\n\r";

        _res += _strTraverse( m_rootBodyPtr, 0 );

        _res += "-----------------------------\n\r";
        _res += "******************************************************\n\r";
    }

    std::string TAgent::_strTraverse( TKinTreeBody* kinTreeBodyPtr, size_t depth )
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
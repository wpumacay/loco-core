
#include <agent/agent_base.h>

namespace tysoc {

    std::string toString( const eModelFormat& format )
    {
        if ( format == eModelFormat::NONE ) return "none";
        if ( format == eModelFormat::MJCF ) return "mjcf";
        if ( format == eModelFormat::URDF ) return "urdf";
        if ( format == eModelFormat::RLSIM ) return "rlsim";

        return "undefined";
    }

    std::string toString( const eAgentType& type )
    {
        if ( type == eAgentType::BASE ) return "base";

        return "undefined";
    }

    TAgent::TAgent( const std::string& name,
                    const TVec3& position,
                    const TVec3& rotation )
    {

        m_name = name;
        m_type  = eAgentType::BASE;

        m_position = position;
        m_rotation = rotation;
        m_startPosition = position;
        m_startRotation = rotation;

        m_modelFormat = eModelFormat::NONE;

        m_modelDataMjcfPtr  = nullptr;
        m_modelDataUrdfPtr  = nullptr;
        m_modelDataRlsimPtr = nullptr;

        m_rootBodyPtr = nullptr;
    }

    TAgent::TAgent( mjcf::GenericElement* modelDataPtr,
                    const std::string& name,
                    const TVec3& position,
                    const TVec3& rotation  ) : TAgent( name, position, rotation )
    {
        // use mjcf-format helpers to construct the kintree for this agent
        m_modelFormat = eModelFormat::MJCF;
        m_modelDataMjcfPtr = constructAgentFromModel( this, modelDataPtr );
    }

    TAgent::TAgent( urdf::UrdfModel* modelDataPtr,
                    const std::string& name,
                    const TVec3& position,
                    const TVec3& rotation  ) : TAgent( name, position, rotation )
    {
        // use urdf-format helpers to construct the kintree for this agent
        m_modelFormat = eModelFormat::URDF;
        m_modelDataUrdfPtr = constructAgentFromModel( this, modelDataPtr );
    }

    TAgent::TAgent( rlsim::RlsimModel* modelDataPtr,
                    const std::string& name,
                    const TVec3& position,
                    const TVec3& rotation  ) : TAgent( name, position, rotation )
    {
        // use rlsim-format helpers to construct the kintree for this agent
        m_modelFormat = eModelFormat::RLSIM;
        m_modelDataRlsimPtr = constructAgentFromModel( this, modelDataPtr );
    }

    TAgent::~TAgent()
    {
        LOCO_CORE_TRACE( "Destroying agent: {0}", m_name );

        m_rootBodyPtr = nullptr;
        m_modelDataMjcfPtr = nullptr;
        m_modelDataUrdfPtr = nullptr;
        m_modelDataRlsimPtr = nullptr;

        for ( auto _visual : visuals ) delete _visual;
        for ( auto _collision : collisions ) delete _collision;
        for ( auto _sensor : sensors ) delete _sensor;
        for ( auto _joint : joints ) delete _joint;
        for ( auto _body : bodies ) delete _body;

        visuals.clear();
        collisions.clear();
        actuators.clear();
        sensors.clear();
        joints.clear();
        bodies.clear();

        visualsMap.clear();
        collisionsMap.clear();
        actuatorsMap.clear();
        sensorsMap.clear();
        jointsMap.clear();
        bodiesMap.clear();
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
            LOCO_CORE_ERROR( "Agent {0} >> found kintree without root body", m_name );
            return;
        }

        // Update internal data from the kintree. The bodies are updated by the internal backend.
        // We should update the joints, visuals and collisions world transforms for later usage
        // by other modules.
        for ( auto _body : bodies )
            _updateBodyComponents( _body );

        // Then, update the sensors and actuators, as they are placed in the objects updated before 
        // (joints, visuals, collisions, bodies)
        for ( auto _sensor : sensors )
            _updateSensor( _sensor );

        for ( auto _actuator : actuators )
            _updateActuator( _actuator );

        // update the global position of the kintree
        m_position = m_rootBodyPtr->worldTransform.getPosition();
        m_rotation = m_rootBodyPtr->worldTransform.getRotEuler();

        // make the specific updates required by the derived agents
        _updateAgentInternal();
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
            LOCO_CORE_WARN( "Agent {0} >> only {1} control actions available, but {2} were given", m_name, actuators.size(), actions.size() );
            return;
        }

        for ( size_t i = 0; i < actuators.size(); i++ )
            actuators[i]->ctrlValue = actions[i];
    }

    TVec2 TAgent::getActuatorLimits( int actuatorIndx )
    {
        if ( actuatorIndx < 0 || actuatorIndx >= actuators.size()  )
        {
            LOCO_CORE_WARN( "Agent {0} >> requested actuator-index {0}, but valid bounds are [0-{1}]", m_name, actuatorIndx, actuators.size() - 1 );
            return { 1.0, -1.0 };
        }

        return actuators[actuatorIndx]->data.limits;
    }

    void TAgent::_constructDefaultSensors()
    {
        // Construct joint sensors for each joint
        for ( auto _joint : joints )
        {
            // For now, only these types of joints should have a joint-sensor associated with it
            if ( _joint->data.type != eJointType::REVOLUTE )
                continue;

            auto _kinTreeJointSensor = new TKinTreeJointSensor();
            // set joint parent name (used for indexing during construction)
            _kinTreeJointSensor->jointName = _joint->name;
            _kinTreeJointSensor->jointPtr = _joint;
            // set an appropiate name for later debugging
            _kinTreeJointSensor->name = std::string( "sensor_" ) + m_name + "_" + _joint->name;

            sensors.push_back( _kinTreeJointSensor );
        }

        // Construct joint sensors for each body
        for ( auto _body : bodies )
        {
            auto _kinTreeBodySensor = new TKinTreeBodySensor();
            // set body parent name (used for indexing during construction)
            _kinTreeBodySensor->bodyName = _body->name;
            _kinTreeBodySensor->bodyPtr = _body;
            // set an appropiate name for later debugging
            _kinTreeBodySensor->name = std::string( "sensor_" ) + m_name + "_" + _body->name;

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

        // initialize bodies and their components recursively (zero configuration)
        _initializeBody( m_rootBodyPtr );

        // update the sensors and actuators, as they are placed in the objects updated 
        // before (joints, visuals, collisions, bodies)
        for ( auto _sensor : sensors )
            _updateSensor( _sensor );
        for ( auto _actuator : actuators )
            _updateActuator( _actuator );
    }

    void TAgent::_initializeBody( TKinTreeBody* kinBody )
    {
        if ( !kinBody )
            return;

        // Update the frame of non-root bodies based on their parents and their 
        // initial local transform (zero-configuration | rest-configuration)
        if ( kinBody->parentBodyPtr )
            kinBody->worldTransform = kinBody->parentBodyPtr->worldTransform * kinBody->localTransformZero;

        for ( auto _visual : kinBody->visuals )
            if ( _visual )
                _updateVisual( _visual );

        for ( auto _collision : kinBody->collisions )
            if ( _collision )
                _updateCollision( _collision );

        for ( auto _joint : kinBody->joints )
            if ( _joint )
                _updateJoint( _joint );

        for ( auto _childBody : kinBody->children )
            if ( _childBody )
                _initializeBody( _childBody );
    }

    void TAgent::_updateBodyComponents( TKinTreeBody* kinBody )
    {
        if ( !kinBody )
            return;

        for ( auto _visual : kinBody->visuals )
            if ( _visual )
                _updateVisual( _visual );

        for ( auto _collision : kinBody->collisions )
            if ( _collision )
                _updateCollision( _collision );

        for ( auto _joint : kinBody->joints )
            if ( _joint )
                _updateJoint( _joint );
    }

    void TAgent::_updateJoint( TKinTreeJoint* kinJoint )
    {
        if ( !kinJoint->parentBodyPtr )
        {
            LOCO_CORE_WARN( "Agent {0} >> joint {1} doesn't have a parent body", m_name, kinJoint->name );
            return;
        }

        kinJoint->worldTransform = kinJoint->parentBodyPtr->worldTransform * kinJoint->data.localTransform;
    }

    void TAgent::_updateVisual( TKinTreeVisual* kinVisual )
    {
        if ( !kinVisual->parentBodyPtr )
        {
            LOCO_CORE_WARN( "Agent {0} >> visual {1} doesn't have a parent body", m_name, kinVisual->name );
            return;
        }

        kinVisual->worldTransform = kinVisual->parentBodyPtr->worldTransform * kinVisual->data.localTransform;
    }

    void TAgent::_updateCollision( TKinTreeCollision* kinCollision )
    {
        if( !kinCollision->parentBodyPtr )
        {
            LOCO_CORE_WARN( "Agent {0} >> collision {1} doesn't have a parent body", m_name, kinCollision->name );
            return;
        }

        kinCollision->worldTransform = kinCollision->parentBodyPtr->worldTransform * kinCollision->data.localTransform;
    }

    void TAgent::_updateActuator( TKinTreeActuator* kinActuator )
    {
        if( !kinActuator->jointPtr )
        {
            LOCO_CORE_WARN( "Agent {0} >> actuator {1} doesn't have a parent body", m_name, kinActuator->name );
            return;
        }

        kinActuator->worldTransform = kinActuator->jointPtr->worldTransform * kinActuator->data.localTransform;
    }

    void TAgent::_updateSensor( TKinTreeSensor* kinSensor )
    {
        if ( kinSensor->bodyPtr )
            kinSensor->worldTransform = kinSensor->bodyPtr->worldTransform * kinSensor->data.localTransform;

        else if ( kinSensor->jointPtr )
            kinSensor->worldTransform = kinSensor->jointPtr->worldTransform * kinSensor->data.localTransform;
    }

    std::string TAgent::toString()
    {
        if ( !m_rootBodyPtr )
            return "nullptr";

        std::string _strRep;

        _strRep += "******************************************************\n\r";
        _strRep += std::string( "name: " ) + m_name + std::string( "\n\r" );

        _strRep += "BODIES INFO: ----------------\n\r";

        _strRep += std::string( "bodies: " ) + std::string( "\n\r" );
        for ( auto _body : bodies )
            _strRep += tysoc::toString( _body );

        _strRep += "-----------------------------\n\r";

        _strRep += "JOINTS INFO: ----------------\n\r";
        
        for ( auto _joint : joints )
            _strRep += tysoc::toString( _joint );

        _strRep += "-----------------------------\n\r";

        _strRep += "COLLISIONS INFO: ------------\n\r";

        for ( auto _collision : collisions )
            _strRep += tysoc::toString( _collision );

        _strRep += "-----------------------------\n\r";

        _strRep += "VISUALS INFO: ---------------\n\r";

        for ( auto _visual : visuals )
            _strRep += tysoc::toString( _visual );

        _strRep += "-----------------------------\n\r";

        _strRep += "SENSORS INFO: ---------------\n\r";

        for ( auto _sensor : sensors )
            _strRep += tysoc::toString( _sensor );

        _strRep += "-----------------------------\n\r";

        _strRep += "ACTUATORS INFO: -------------\n\r";

        for ( auto _actuator : actuators )
            _strRep += tysoc::toString( _actuator );

        _strRep += "-----------------------------\n\r";
        _strRep += "TREE STRUCTURE: -------------\n\r";

        _strRep += _strTraverse( m_rootBodyPtr, 0 );

        _strRep += "-----------------------------\n\r";
        _strRep += "******************************************************\n\r";

        return _strRep;
    }

    std::string TAgent::_strTraverse( TKinTreeBody* kinBody, size_t depth )
    {
        if ( !kinBody )
            return "nullptr";

        std::string _strRep;

        // some tabs for a nicer format
        std::string _preTabs;
        for ( size_t i = 0; i < depth; i++ )
            _preTabs += "\t";

        _strRep += _preTabs;
        _strRep += "body: ";
        _strRep += kinBody->name;
        _strRep += "\n\r";

        _strRep += _preTabs;
        _strRep += "childJoints: |";
        for ( auto _joint : kinBody->joints )
            _strRep += _joint->name + " | ";
        _strRep += "\n\r";

        _strRep += _preTabs;
        _strRep += "childVisuals: [";
        for ( auto _visual : kinBody->visuals )
            _strRep += _visual->name + " | ";
        _strRep += "\n\r";

        _strRep += _preTabs;
        _strRep += "childCollisions: [";
        for ( auto _collision : kinBody->collisions )
            _strRep += _collision->name + " | ";
        _strRep += "\n\r";

        for ( auto _childBody : kinBody->children )
        {
            if ( _childBody )
                continue;

            _strRep += _strTraverse( _childBody, depth + 1 );
        }

        return _strRep;
    }

}
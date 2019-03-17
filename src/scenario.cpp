
#include <scenario.h>


namespace tysoc
{

    
    TScenario::TScenario()
    {
        m_state = IDLE;

        // initialize some mappings
        m_mapAgentsByType[ agent::AGENT_TYPE_SIMPLE ]  = std::vector< agent::TIAgent* >();
        m_mapAgentsByType[ agent::AGENT_TYPE_KINTREE ] = std::vector< agent::TIAgent* >();

        m_mapSensorsByType[ sensor::SENSOR_TYPE_INTRINSICS ]            = std::vector< sensor::TISensor* >();
        m_mapSensorsByType[ sensor::SENSOR_TYPE_EXTRINSICS_TERRAIN ]    = std::vector< sensor::TISensor* >();

        m_mapTerrainGeneratorsByType[ terrain::TERRAIN_TYPE_STATIC ] = std::vector< terrain::TITerrainGenerator* >();
        m_mapTerrainGeneratorsByType[ terrain::TERRAIN_TYPE_PROCEDURAL_SECTIONS_PATH ]   = std::vector< terrain::TITerrainGenerator* >();
        m_mapTerrainGeneratorsByType[ terrain::TERRAIN_TYPE_PROCEDURAL_SECTIONS_BLOCKS ] = std::vector< terrain::TITerrainGenerator* >();
    }
    
    
    TScenario::~TScenario()
    {
        m_state = FINISHED;

        // Clean the stored references as ...
        // the runtime is in charge to hold ...
        // all data of the simulation
        for ( size_t i = 0; i < m_agents.size(); i++ )
        {
            delete m_agents[i];
        }
        m_agents.clear();
        m_mapAgentsByName.clear();
        m_mapAgentsByType.clear();

        for ( size_t i = 0; i < m_sensors.size(); i++ )
        {
            delete m_sensors[i];
        }
        m_sensors.clear();
        m_mapSensorsByName.clear();
        m_mapSensorsByType.clear();

        for ( size_t i = 0; i < m_terrainGenerators.size(); i++ )
        {
            delete m_terrainGenerators[ i ];
        }
        m_terrainGenerators.clear();
        m_mapTerrainGeneratorsByName.clear();
        m_mapTerrainGeneratorsByType.clear();
    }

    ScenarioState TScenario::state()
    {
        return m_state;
    }

    void TScenario::addAgent( agent::TIAgent* agent )
    {
        if ( agent )
        {
            m_agents.push_back( agent );
            m_mapAgentsByName[ agent->name() ] = agent;
            m_mapAgentsByType[ agent->type() ].push_back( agent );
        }
    }

    void TScenario::addSensor( sensor::TISensor* sensor )
    {
        if ( sensor )
        {
            m_sensors.push_back( sensor );
            m_mapSensorsByName[ sensor->name() ] = sensor;
            m_mapSensorsByType[ sensor->type() ].push_back( sensor );
        }
    }

    void TScenario::addTerrainGenerator( terrain::TITerrainGenerator* terrainGenerator )
    {
        if ( terrainGenerator )
        {
            m_terrainGenerators.push_back( terrainGenerator );
            m_mapTerrainGeneratorsByName[ terrainGenerator->name() ] = terrainGenerator;
            m_mapTerrainGeneratorsByType[ terrainGenerator->type() ].push_back( terrainGenerator );
        }
    }

    void TScenario::addBody( sandbox::TBody* body )
    {
        if ( body )
        {
            m_bodies.push_back( body );
            m_mapBodiesByName[ body->name ] = body;
        }
    }

    void TScenario::addJoint( sandbox::TJoint* joint )
    {
        if ( joint )
        {
            m_joints.push_back( joint );
            m_mapJointsByName[ joint->name ] = joint;
        }
    }

    void TScenario::initialize()
    {
        // @TODO|@CHECK: should remove initialization? is it useful?
        for ( size_t i = 0; i < m_terrainGenerators.size(); i++ )
        {
            m_terrainGenerators[ i ]->initialize();
        }
    }

    void TScenario::update()
    {
        // Update terrain generators
        for ( size_t i = 0; i < m_terrainGenerators.size(); i++ )
        {
            m_terrainGenerators[ i ]->update();
        }

        // Update sensors
        for ( size_t i = 0; i < m_sensors.size(); i++ )
        {
            m_sensors[ i ]->update();
        }

        // Update agents
        for ( size_t i = 0; i < m_agents.size(); i++ )
        {
            m_agents[ i ]->update( 0 );
        }
    }

    agent::TIAgent* TScenario::getAgentByName( const std::string& name )
    {
        if ( m_mapAgentsByName.find( name ) != m_mapAgentsByName.end() )
        {
            return m_mapAgentsByName[ name ];
        }

        std::cout << "WARNING> agent with name: " 
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    sensor::TISensor* TScenario::getSensorByName( const std::string& name )
    {
        if ( m_mapSensorsByName.find( name ) != m_mapSensorsByName.end() )
        {
            return m_mapSensorsByName[ name ];
        }

        std::cout << "WARNING> sensor with name: "
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    terrain::TITerrainGenerator* TScenario::getTerrainGeneratorByName( const std::string& name )
    {
        if ( m_mapTerrainGeneratorsByName.find( name ) != m_mapTerrainGeneratorsByName.end() )
        {
            return m_mapTerrainGeneratorsByName[ name ];
        }

        std::cout << "WARNING> terrain generator with name: "
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    sandbox::TBody* TScenario::getBodyByName( const std::string& name )
    {
        if ( m_mapBodiesByName.find( name ) != m_mapBodiesByName.end() )
        {
            return m_mapBodiesByName[ name ];
        }

        std::cout << "WARNING> body with name: "
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    sandbox::TJoint* TScenario::getJointByName( const std::string& name )
    {
        if ( m_mapJointsByName.find( name ) != m_mapJointsByName.end() )
        {
            return m_mapJointsByName[ name ];
        }

        std::cout << "WARNING> joint with name: "
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    std::vector< terrain::TITerrainGenerator* > TScenario::getTerrainGenerators()
    {
        return m_terrainGenerators;
    }

    std::vector< agent::TIAgent* > TScenario::getAgents()
    {
        return m_agents;
    }

    std::vector< sensor::TISensor* > TScenario::getSensors()
    {
        return m_sensors;
    }

    std::vector< sandbox::TBody* > TScenario::getBodies()
    {
        return m_bodies;
    }

    std::vector< sandbox::TJoint* > TScenario::getJoints()
    {
        return m_joints;
    }

    std::vector< agent::TIAgent* > TScenario::getAgentsByType( const std::string& type )
    {
        if ( m_mapAgentsByType.find( type ) != m_mapAgentsByType.end() )
        {
            return m_mapAgentsByType[ type ];
        }

        std::cout << "WARNING> tried to get agents of non existent type: " << type << std::endl;

        return std::vector< agent::TIAgent* >();
    }

    std::vector< sensor::TISensor* > TScenario::getSensorsByType( const std::string& type )
    {
        if ( m_mapSensorsByType.find( type ) != m_mapSensorsByType.end() )
        {
            return m_mapSensorsByType[ type ];
        }

        std::cout << "WARNING> tried to get sensors of non existent type: " << type << std::endl;

        return std::vector< sensor::TISensor* >();
    }

    std::vector< terrain::TITerrainGenerator* > TScenario::getTerrainGeneratorsByType( const std::string& type )
    {
        if ( m_mapTerrainGeneratorsByType.find( type ) != m_mapTerrainGeneratorsByType.end() )
        {
            return m_mapTerrainGeneratorsByType[ type ];
        }

        std::cout << "WARNING> tried to get terrain generators of non existent type: " << type << std::endl;

        return std::vector< terrain::TITerrainGenerator* >();
    }

    bool TScenario::hasAgent( const std::string& agentName )
    {
        return m_mapAgentsByName.find( agentName ) != m_mapAgentsByName.end();
    }

    bool TScenario::hasTerrainGen( const std::string& terrainGenName )
    {
        return m_mapTerrainGeneratorsByName.find( terrainGenName ) != m_mapTerrainGeneratorsByName.end();
    }

    bool TScenario::hasSensor( const std::string& sensorName )
    {
        return m_mapSensorsByName.find( sensorName ) != m_mapSensorsByName.end();
    }

    bool TScenario::hasBody( const std::string& bodyName )
    {
        return m_mapBodiesByName.find( bodyName ) != m_mapBodiesByName.end();
    }

    bool TScenario::hasJoint( const std::string& jointName )
    {
        return m_mapJointsByName.find( jointName ) != m_mapJointsByName.end();
    }


}
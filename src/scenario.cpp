
#include <scenario.h>


namespace tysoc {

    TScenario::TScenario()
    {
        m_state = IDLE;

        // initialize some mappings
        m_mapBodiesByType[ eDynamicsType::DYNAMIC ]     = std::vector< TBody* >();
        m_mapBodiesByType[ eDynamicsType::STATIC ]      = std::vector< TBody* >();
        m_mapBodiesByType[ eDynamicsType::KINEMATIC ]   = std::vector< TBody* >();

        m_mapAgentsByType[ eAgentType::BASE ]  = std::vector< TAgent* >();

        m_mapSensorsByType[ eSensorType::PROP_JOINT ] = std::vector< TISensor* >();
        m_mapSensorsByType[ eSensorType::PROP_BODY ] = std::vector< TISensor* >();
        m_mapSensorsByType[ eSensorType::EXT_HEIGHTFIELD_1D ] = std::vector< TISensor* >();
        m_mapSensorsByType[ eSensorType::EXT_HEIGHTFIELD_2D ] = std::vector< TISensor* >();
        m_mapSensorsByType[ eSensorType::EXT_CAMERA_RGB ] = std::vector< TISensor* >();
        m_mapSensorsByType[ eSensorType::EXT_CAMERA_DEPTH ] = std::vector< TISensor* >();
        m_mapSensorsByType[ eSensorType::EXT_CAMERA_SEGMENTATION ] = std::vector< TISensor* >();

        m_mapTerrainGeneratorsByType[ TERRAIN_TYPE_STATIC ] = std::vector< TITerrainGenerator* >();
        m_mapTerrainGeneratorsByType[ TERRAIN_TYPE_PROCEDURAL_SECTIONS_PATH ]   = std::vector< TITerrainGenerator* >();
        m_mapTerrainGeneratorsByType[ TERRAIN_TYPE_PROCEDURAL_SECTIONS_BLOCKS ] = std::vector< TITerrainGenerator* >();
    }
    
    
    TScenario::~TScenario()
    {
        TYSOC_LOG( "Destroying scenario" );

        m_state = FINISHED;

        // Clean the stored references as ...
        // the runtime is in charge to hold ...
        // all data of the simulation
        for ( size_t i = 0; i < m_bodies.size(); i++ )
        {
            delete m_bodies[i];
        }
        m_bodies.clear();
        m_mapBodiesByName.clear();
        m_mapBodiesByType.clear();

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

    void TScenario::addBody( TBody* bodyPtr )
    {
        if ( !bodyPtr )
            return;

        m_bodies.push_back( bodyPtr );
        m_mapBodiesByName[ bodyPtr->name() ] = bodyPtr;
        m_mapBodiesByType[ bodyPtr->dyntype() ].push_back( bodyPtr );
    }

    void TScenario::addAgent( TAgent* agent )
    {
        if ( agent )
        {
            m_agents.push_back( agent );
            m_mapAgentsByName[ agent->name() ] = agent;
            m_mapAgentsByType[ agent->type() ].push_back( agent );
        }
    }

    void TScenario::addSensor( TISensor* sensor )
    {
        if ( sensor )
        {
            m_sensors.push_back( sensor );
            m_mapSensorsByName[ sensor->name() ] = sensor;
            m_mapSensorsByType[ sensor->type() ].push_back( sensor );
        }
    }

    void TScenario::addTerrainGenerator( TITerrainGenerator* terrainGenerator )
    {
        if ( terrainGenerator )
        {
            m_terrainGenerators.push_back( terrainGenerator );
            m_mapTerrainGeneratorsByName[ terrainGenerator->name() ] = terrainGenerator;
            m_mapTerrainGeneratorsByType[ terrainGenerator->type() ].push_back( terrainGenerator );
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
        for ( size_t i = 0; i < m_bodies.size(); i++ )
            m_bodies[i]->update();

        for ( size_t i = 0; i < m_terrainGenerators.size(); i++ )
            m_terrainGenerators[i]->update();

        for ( size_t i = 0; i < m_sensors.size(); i++ )
            m_sensors[i]->update();

        for ( size_t i = 0; i < m_agents.size(); i++ )
            m_agents[i]->update();
    }

    TBody* TScenario::getBodyByName( const std::string& name )
    {
        if ( m_mapBodiesByName.find( name ) != m_mapBodiesByName.end() )
            return m_mapBodiesByName[ name ];

        std::cout << "WARNING> body with name: " << name 
                  << " does not exist. Returning NULL instead." << std::endl;

        return NULL;
    }

    TAgent* TScenario::getAgentByName( const std::string& name )
    {
        if ( m_mapAgentsByName.find( name ) != m_mapAgentsByName.end() )
        {
            return m_mapAgentsByName[ name ];
        }

        std::cout << "WARNING> agent with name: " 
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    TISensor* TScenario::getSensorByName( const std::string& name )
    {
        if ( m_mapSensorsByName.find( name ) != m_mapSensorsByName.end() )
        {
            return m_mapSensorsByName[ name ];
        }

        std::cout << "WARNING> sensor with name: "
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    TITerrainGenerator* TScenario::getTerrainGeneratorByName( const std::string& name )
    {
        if ( m_mapTerrainGeneratorsByName.find( name ) != m_mapTerrainGeneratorsByName.end() )
        {
            return m_mapTerrainGeneratorsByName[ name ];
        }

        std::cout << "WARNING> terrain generator with name: "
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    std::vector< TITerrainGenerator* > TScenario::getTerrainGenerators()
    {
        return m_terrainGenerators;
    }

    std::vector< TBody* > TScenario::getBodies()
    {
        return m_bodies;
    }

    std::vector< TAgent* > TScenario::getAgents()
    {
        return m_agents;
    }

    std::vector< TISensor* > TScenario::getSensors()
    {
        return m_sensors;
    }

    std::vector< TBody* > TScenario::getBodiesByType( const eDynamicsType& type )
    {
        if ( m_mapBodiesByType.find( type ) != m_mapBodiesByType.end() )
            return m_mapBodiesByType[ type ];

        std::cout << "WARNING> tried to get bodies of non existent type: " << tysoc::toString( type ) << std::endl;

        return std::vector< TBody* >();
    }

    std::vector< TAgent* > TScenario::getAgentsByType( const eAgentType& type )
    {
        if ( m_mapAgentsByType.find( type ) != m_mapAgentsByType.end() )
        {
            return m_mapAgentsByType[ type ];
        }

        std::cout << "WARNING> tried to get agents of non existent type: " << toString( type ) << std::endl;

        return std::vector< TAgent* >();
    }

    std::vector< TISensor* > TScenario::getSensorsByType( const eSensorType& type )
    {
        if ( m_mapSensorsByType.find( type ) != m_mapSensorsByType.end() )
        {
            return m_mapSensorsByType[ type ];
        }

        std::cout << "WARNING> tried to get sensors of non existent type: " << tysoc::toString( type ) << std::endl;

        return std::vector< TISensor* >();
    }

    std::vector< TITerrainGenerator* > TScenario::getTerrainGeneratorsByType( const std::string& type )
    {
        if ( m_mapTerrainGeneratorsByType.find( type ) != m_mapTerrainGeneratorsByType.end() )
        {
            return m_mapTerrainGeneratorsByType[ type ];
        }

        std::cout << "WARNING> tried to get terrain generators of non existent type: " << type << std::endl;

        return std::vector< TITerrainGenerator* >();
    }

    bool TScenario::hasBody( const std::string& bodyName )
    {
        return m_mapBodiesByName.find( bodyName ) != m_mapBodiesByName.end();
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

}
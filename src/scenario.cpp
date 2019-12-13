
#include <scenario.h>


namespace tysoc {

    TScenario::TScenario()
    {

    }

    TScenario::~TScenario()
    {
        TYSOC_CORE_INFO( "Destroying scenario" );

        // Clean the stored references as ...
        // the runtime is in charge to hold ...
        // all data of the simulation
        for ( size_t i = 0; i < m_singleBodies.size(); i++ )
            delete m_singleBodies[i];

        m_singleBodies.clear();
        m_mapSingleBodiesByName.clear();

        for ( size_t i = 0; i < m_agents.size(); i++ )
            delete m_agents[i];

        m_agents.clear();
        m_mapAgentsByName.clear();

        for ( size_t i = 0; i < m_sensors.size(); i++ )
            delete m_sensors[i];

        m_sensors.clear();
        m_mapSensorsByName.clear();

        for ( size_t i = 0; i < m_terrainGenerators.size(); i++ )
            delete m_terrainGenerators[i];

        m_terrainGenerators.clear();
        m_mapTerrainGeneratorsByName.clear();
    }

    // @todo: should we handle ownership with unique_ptrs?
    void TScenario::addSingleBody( TSingleBody* body )
    {
        if ( !body )
            return;

        m_singleBodies.push_back( body );
        m_mapSingleBodiesByName[ body->name() ] = body;
    }

    // @todo: should we handle ownership with unique_ptrs?
    void TScenario::addCompoundBody( TCompoundBody* rootCompound )
    {
        if ( !rootCompound )
            return;

        m_compoundBodies.push_back( rootCompound );
        m_mapCompoundBodiesByName[ rootCompound->name() ] = rootCompound;
    }

    // @todo: should we handle ownership with unique_ptrs?
    void TScenario::addAgent( TAgent* agent )
    {
        if ( !agent )
            return;

        m_agents.push_back( agent );
        m_mapAgentsByName[ agent->name() ] = agent;
    }

    // @todo: should we handle ownership with unique_ptrs?
    void TScenario::addSensor( TISensor* sensor )
    {
        if ( !sensor )
            return;

        m_sensors.push_back( sensor );
        m_mapSensorsByName[ sensor->name() ] = sensor;
    }

    // @todo: should we handle ownership with unique_ptrs?
    void TScenario::addTerrainGenerator( TITerrainGenerator* terrainGenerator )
    {
        if ( !terrainGenerator )
            return;

        m_terrainGenerators.push_back( terrainGenerator );
        m_mapTerrainGeneratorsByName[ terrainGenerator->name() ] = terrainGenerator;
    }

    void TScenario::initialize()
    {
        // @TODO|@CHECK: should remove initialization? is it useful?
        for ( size_t i = 0; i < m_terrainGenerators.size(); i++ )
            m_terrainGenerators[ i ]->initialize();
    }

    void TScenario::update()
    {
        for ( size_t i = 0; i < m_singleBodies.size(); i++ )
            m_singleBodies[i]->update();

        for ( size_t i = 0; i < m_compoundBodies.size(); i++ )
            m_compoundBodies[i]->update();

        for ( size_t i = 0; i < m_terrainGenerators.size(); i++ )
            m_terrainGenerators[i]->update();

        for ( size_t i = 0; i < m_sensors.size(); i++ )
            m_sensors[i]->update();

        for ( size_t i = 0; i < m_agents.size(); i++ )
            m_agents[i]->update();
    }

    TSingleBody* TScenario::getSingleBodyByName( const std::string& name )
    {
        if ( m_mapSingleBodiesByName.find( name ) != m_mapSingleBodiesByName.end() )
            return m_mapSingleBodiesByName[name];

        TYSOC_CORE_WARN( "Scenario >>> single-body with name \"{0}\" does not exist. \
                          Returning nullptr instead.", name );

        return nullptr;
    }

    TCompoundBody* TScenario::getCompoundBodyByName( const std::string& name )
    {
        if ( m_mapCompoundBodiesByName.find( name ) != m_mapCompoundBodiesByName.end() )
            return m_mapCompoundBodiesByName[name];

        TYSOC_CORE_WARN( "Scenario >>> compound-body with name \"{0}\" does not exist. \
                          Returning nullptr instead.", name )

        return nullptr;
    }

    TAgent* TScenario::getAgentByName( const std::string& name )
    {
        if ( m_mapAgentsByName.find( name ) != m_mapAgentsByName.end() )
            return m_mapAgentsByName[name];

        TYSOC_CORE_WARN( "Scenario >>> agent with name \"{0}\" does not exist. \
                          Returning nullptr instead.", name );

        return nullptr;
    }

    TISensor* TScenario::getSensorByName( const std::string& name )
    {
        if ( m_mapSensorsByName.find( name ) != m_mapSensorsByName.end() )
            return m_mapSensorsByName[name];

        TYSOC_CORE_WARN( "Scenario >>> sensor with name \"{0}\" does not exist. \
                          Returning nullptr instead", name );

        return nullptr;
    }

    TITerrainGenerator* TScenario::getTerrainGeneratorByName( const std::string& name )
    {
        if ( m_mapTerrainGeneratorsByName.find( name ) != m_mapTerrainGeneratorsByName.end() )
            return m_mapTerrainGeneratorsByName[name];

        TYSOC_CORE_WARN( "Scenario >>> terrain generator with name \"{0}\" does not exist. \
                          Returning nullptr instead", name );

        return nullptr;
    }

    bool TScenario::hasSingleBody( const std::string& singleBodyName )
    {
        return m_mapSingleBodiesByName.find( singleBodyName ) != m_mapSingleBodiesByName.end();
    }

    bool TScenario::hasCompoundBody( const std::string& compoundBodyName )
    {
        return m_mapCompoundBodiesByName.find( compoundBodyName ) != m_mapCompoundBodiesByName.end();
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
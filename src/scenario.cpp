
#include <scenario.h>


namespace tysoc
{

    
    TScenario::TScenario()
    {
        m_state = IDLE;
    }
    
    
    TScenario::~TScenario()
    {
        m_state = FINISHED;

        // Clean the stored references as ...
        // the runtime is in charge to hold ...
        // all data of the simulation
        for ( auto it = m_agents.begin();
              it != m_agents.end();
              it++ )
        {
            delete it->second;
        }
        m_agents.clear();

        for ( auto it = m_sensors.begin();
              it != m_sensors.end();
              it++ )
        {
            delete it->second;
        }
        m_sensors.clear();

        for ( size_t i = 0; i < m_terrainGenerators.size(); i++ )
        {
            delete m_terrainGenerators[ i ];
        }
        m_terrainGenerators.clear();
    }

    ScenarioState TScenario::state()
    {
        return m_state;
    }

    void TScenario::addAgent( tysocagent::TAgent* agent )
    {
        if ( agent )
        {
            m_agents[ agent->name() ] = agent;
        }
    }

    void TScenario::addSensor( tysocsensor::TSensor* sensor )
    {
        if ( sensor )
        {
            m_sensors[ sensor->name() ] = sensor;
        }
    }

    void TScenario::addTerrainGenerator( tysocterrain::TTerrainGenerator* terrainGenerator )
    {
        if ( terrainGenerator )
        {
            m_terrainGenerators.push_back( terrainGenerator );
        }
    }

    void TScenario::initialize()
    {
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
        for ( auto it = m_sensors.begin(); it != m_sensors.end(); it++ )
        {
            it->second->update();
        }
    }

    tysocagent::TAgent* TScenario::getAgent( const std::string& name )
    {
        if ( m_agents.find( name ) != m_agents.end() )
        {
            return m_agents[ name ];
        }

        std::cout << "WARNING> agent with name: " 
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    tysocsensor::TSensor* TScenario::getSensor( const std::string& name )
    {
        if ( m_sensors.find( name ) != m_sensors.end() )
        {
            return m_sensors[ name ];
        }

        std::cout << "WARNING> sensor with name: "
                  << name << " does not exist" << std::endl;

        return NULL;
    }

    std::vector< tysocterrain::TTerrainGenerator* > TScenario::getTerrainGenerators()
    {
        return m_terrainGenerators;
    }

    std::map< std::string, tysocagent::TAgent* > TScenario::getAgents()
    {
        return m_agents;
    }

    std::map< std::string, tysocsensor::TSensor* > TScenario::getSensors()
    {
        return m_sensors;
    }
}
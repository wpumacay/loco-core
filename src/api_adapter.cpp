
#include <api_adapter.h>



namespace tysoc
{


    TTysocCommonApi::TTysocCommonApi()
    {
        m_scenarioPtr = NULL;
        m_primitivesSpawnerPtr = NULL;
    }

    TTysocCommonApi::~TTysocCommonApi()
    {
        if ( m_scenarioPtr )
        {
            delete m_scenarioPtr;
            m_scenarioPtr = NULL;
        }

        if ( m_primitivesSpawnerPtr )
        {
            delete m_primitivesSpawnerPtr;
            m_primitivesSpawnerPtr = NULL;
        }
    }

    void TTysocCommonApi::setScenario( tysoc::TScenario* scenarioPtr )
    {
        m_scenarioPtr = scenarioPtr;
    }

    void TTysocCommonApi::initialize()
    {
        if ( m_scenarioPtr )
        {
            m_scenarioPtr->initialize();
        }
    }

    void TTysocCommonApi::step()
    {
        if ( m_scenarioPtr )
        {
            m_scenarioPtr->update();
        }

        if ( m_primitivesSpawnerPtr )
        {
            // @TODO: Add wall time
            m_primitivesSpawnerPtr->update( 1.0 / 60.0 );
        }

        _preStep();
        _updateStep();
        _postStep();
    }

    void TTysocCommonApi::setAgentAction( const std::string& agentName, 
                                          const std::string& actuatorName,
                                          float actionValue )
    {
        if ( m_scenarioPtr )
        {
            auto _agent = m_scenarioPtr->getAgent( agentName );
            if ( _agent )
            {
                _agent->setCtrl( actuatorName, actionValue );
            }
        }
    }

    tysocsensor::TSensorMeasurement* TTysocCommonApi::getSensorMeasurement( const std::string sensorName )
    {
        if ( m_scenarioPtr )
        {
            auto _sensor = m_scenarioPtr->getSensor( sensorName );
            if ( !_sensor )
            {
                return _sensor->getSensorMeasurement();
            }
        }

        return NULL;
    }

    std::vector< tysocterrain::TTerrainGenerator* > TTysocCommonApi::getTerrainGenerators()
    {
        if ( m_scenarioPtr )
        {
            return m_scenarioPtr->getTerrainGenerators();
        }

        std::cout << "ERROR> there is no scenario to get terraingens from" << std::endl;

        return std::vector< tysocterrain::TTerrainGenerator* >();
    }

    std::map< std::string, tysocagent::TAgent* > TTysocCommonApi::getAgents()
    {
        if ( m_scenarioPtr )
        {
            return m_scenarioPtr->getAgents();
        }

        std::cout << "ERROR> there is no scenario to get agents from" << std::endl;

        return std::map< std::string, tysocagent::TAgent* >();
    }

    tysocUtils::TPrimitivesSpawner* TTysocCommonApi::getPrimitivesSpawner()
    {
        return m_primitivesSpawnerPtr;
    }
}
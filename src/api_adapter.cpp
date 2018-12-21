
#include <api_adapter.h>



namespace tysoc
{


    TTysocCommonApi::TTysocCommonApi()
    {
        m_scenarioPtr           = NULL;
        m_primitivesSpawnerPtr  = NULL;
        m_apiType               = API_TYPE_BASE;
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

    void TTysocCommonApi::setScenario( TScenario* scenarioPtr )
    {
        m_scenarioPtr = scenarioPtr;
    }

    TScenario* TTysocCommonApi::getScenario()
    {
        return m_scenarioPtr;
    }

    std::string TTysocCommonApi::getApiType()
    {
        return m_apiType;
    }

    void TTysocCommonApi::initialize()
    {
        if ( m_scenarioPtr )
        {
            m_scenarioPtr->initialize();
        }

        // take an initial step
        step();
    }

    void TTysocCommonApi::step()
    {
        // Step the simulation properly
        _preStep();
        _updateStep();
        _postStep();

        // Update the underlying data after the step
        
        if ( m_scenarioPtr )
        {
            m_scenarioPtr->update();
        }

        if ( m_primitivesSpawnerPtr )
        {
            // @TODO: Add wall time
            m_primitivesSpawnerPtr->update( 1.0 / 60.0 );
        }
    }

    utils::TPrimitivesSpawner* TTysocCommonApi::getPrimitivesSpawner()
    {
        return m_primitivesSpawnerPtr;
    }
}
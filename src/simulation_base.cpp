
#include <simulation_base.h>


namespace tysoc {


    TISimulation::TISimulation( TScenario* scenarioPtr )
    {
        m_runtimeType = "none";
        m_scenarioPtr = scenarioPtr;
    }

    TISimulation::~TISimulation()
    {
        m_scenarioPtr = NULL;

        for ( size_t q = 0; q < m_agentWrappers.size(); q++ )
        {
            delete m_agentWrappers[q];
        }
        m_agentWrappers.clear();

        for ( size_t q = 0; q < m_terrainGenWrappers.size(); q++ )
        {
            delete m_terrainGenWrappers[q];
        }
        m_terrainGenWrappers.clear();
    }

    bool TISimulation::initialize()
    {
        return _initializeInternal();
    }

    void TISimulation::step()
    {
        _preStepInternal();
        _simStepInternal();
        _postStepInternal();
    }

    void TISimulation::reset()
    {
        _resetInternal();
    }

    TScenario* TISimulation::scenario()
    {
        return m_scenarioPtr;
    }

    std::string TISimulation::type()
    {
        return m_runtimeType;
    }


}
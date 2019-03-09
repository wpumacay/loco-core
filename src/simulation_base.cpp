
#include <simulation_base.h>


namespace tysoc {


    TISimulation::TISimulation( TScenario* scenarioPtr,
                                const std::string& workingDir )
    {
        m_runtimeType = "none";
        m_scenarioPtr = scenarioPtr;
        m_workingDir  = workingDir;
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

    void* TISimulation::payload( const std::string& type )
    {
        return _constructPayloadInternal( type );
    }

    void* TISimulation::_constructPayloadInternal( const std::string& type )
    {
        // Do nothing. Just implement if needs to generate very, very, very specific ...
        // data for the visualizer, e.g. mujoco case needs to pass mjModel, mjData ...
        // and some other data structures for the viz to work
        return NULL;
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

        if ( m_scenarioPtr )
            m_scenarioPtr->update();
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

    std::map< std::string, std::vector<TScalar> > TISimulation::getVectorizedInfo()
    {
        return _getVectorizedInfoInternal();
    }

}
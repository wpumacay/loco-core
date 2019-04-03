
#include <simulation_base.h>


namespace tysoc {


    TISimulation::TISimulation( TScenario* scenarioPtr,
                                const std::string& workingDir )
    {
        m_runtimeType = "none";
        m_scenarioPtr = scenarioPtr;
        m_workingDir  = workingDir;
        m_visualizerPtr = NULL;

        m_isRunning = false;
    }

    TISimulation::~TISimulation()
    {
        m_visualizerPtr = NULL;
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

        for ( size_t q = 0; q < m_bodyWrappers.size(); q++ )
        {
            delete m_bodyWrappers[q];
        }
        m_bodyWrappers.clear();
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
        m_isRunning = true;

        return _initializeInternal();
    }

    void TISimulation::step()
    {
        _preStepInternal();
        if ( m_isRunning )
            _simStepInternal();
        _postStepInternal();

        if ( m_scenarioPtr )
            m_scenarioPtr->update();
    }

    void TISimulation::reset()
    {
        _resetInternal();
    }

    void TISimulation::togglePause()
    {
        m_isRunning = ( m_isRunning ) ? false : true;
    }

    void TISimulation::setVisualizer( viz::TIVisualizer* visualizerPtr )
    {
        m_visualizerPtr = visualizerPtr;
    }

    viz::TIVisualizer* TISimulation::getVisualizer()
    {
        return m_visualizerPtr;
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
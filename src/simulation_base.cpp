
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
        m_isDebugDrawingActive = false;
    }

    TISimulation::~TISimulation()
    {
        m_visualizerPtr = NULL;
        m_scenarioPtr = NULL;

        for ( size_t q = 0; q < m_agentWrappers.size(); q++ )
            delete m_agentWrappers[q];

        m_agentWrappers.clear();

        for ( size_t q = 0; q < m_terrainGenWrappers.size(); q++ )
            delete m_terrainGenWrappers[q];

        m_terrainGenWrappers.clear();
    }

    bool TISimulation::initialize()
    {
        m_isRunning = _initializeInternal();

        return m_isRunning;
    }

    void TISimulation::step()
    {
        // execute any functionality, or setup any resources prior to taking a step in the backend
        _preStep();

        // take an actual step in the backend (delegates to backend-specific code)
        if ( m_isRunning )
            _simStepInternal();

        // execute any functionality, or collect any information after taking a step in the backend
        _postStep();

        // update scenario high-level resources once a step has been taken in the backend
        if ( m_scenarioPtr )
            m_scenarioPtr->update();
    }

    void TISimulation::_preStep()
    {
        // hey agent-wrapper, make sure you set any low-level data from the user-data, like ctrls, etc.
        for ( size_t i = 0; i < m_agentWrappers.size(); i++ )
            m_agentWrappers[i]->preStep();

        // the same for you terrain-generators, perhaps the user wanted to move your internal things around
        for ( size_t i = 0; i < m_terrainGenWrappers.size(); i++ )
            m_terrainGenWrappers[i]->preStep();

        // delegate any extra backend-specifics (set any low-level info, etc.)
        _preStepInternal();
    }

    void TISimulation::_postStep()
    {
        // agent-wrapper, please, grab any information that you might need for the abstract resources
        for ( size_t i = 0; i < m_agentWrappers.size(); i++ )
            m_agentWrappers[i]->postStep();

        // you too terrain-generator, grab any info you might need to move|create|delete terrain sections, etc.
        for ( size_t i = 0; i < m_terrainGenWrappers.size(); i++ )
            m_terrainGenWrappers[i]->postStep();

        // and in case some extra stuff(low-level state) needs to be grabbed, just grab it here
        _postStepInternal();
    }

    void TISimulation::reset()
    {
        // hey wrapper, do your thing and reset your internal configuration, pretty please :D
        for ( size_t i = 0; i < m_agentWrappers.size(); i++ )
            m_agentWrappers[i]->reset();

        // you too terrain-gen, reset the resources to some initial position from a initial distirbution
        for ( size_t i = 0; i < m_terrainGenWrappers.size(); i++ )
            m_terrainGenWrappers[i]->reset();

        // and in case something was missing, reset any other resources that might need specific resets
        _resetInternal();
    }

    void TISimulation::togglePause()
    {
        m_isRunning = ( m_isRunning ) ? false : true;
    }

    void TISimulation::toggleDebugDraws()
    {
        m_isDebugDrawingActive = ( m_isDebugDrawingActive ) ? false : true;
    }

}
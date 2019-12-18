
#include <simulation_base.h>

namespace tysoc {

    TISimulation::TISimulation( TScenario* scenarioPtr )
    {
        m_runtimeType = "none";
        m_scenarioPtr = scenarioPtr;
        m_visualizerPtr = nullptr;

        m_isRunning = false;
        m_isDebugDrawingActive = false;
    }

    TISimulation::~TISimulation()
    {
        m_visualizerPtr = nullptr;
        m_scenarioPtr = nullptr;

        for ( auto _agentAdapter : m_agentWrappers )
            delete _agentAdapter;

        for ( auto _terrainGenAdapter : m_terrainGenWrappers )
            delete _terrainGenAdapter;

        for ( auto _collisionAdapter : m_collisionAdapters )
            delete _collisionAdapter;

        for ( auto _jointAdapter : m_jointAdapters )
            delete _jointAdapter;

        for ( auto _bodyAdapter : m_bodyAdapters )
            delete _bodyAdapter;

        for ( auto _compoundAdapter : m_compoundAdapters )
            delete _compoundAdapter;

        m_agentWrappers.clear();
        m_terrainGenWrappers.clear();
        m_collisionAdapters.clear();
        m_jointAdapters.clear();
        m_bodyAdapters.clear();
        m_compoundAdapters.clear();
    }

    bool TISimulation::initialize()
    {
        if ( m_scenarioPtr )
            m_scenarioPtr->initialize();

        /* build all resources required before assembling them in the init-internal stage */
        for ( auto _bodyAdapter : m_bodyAdapters )
            _bodyAdapter->build();

        for ( auto _compoundAdapter : m_compoundAdapters )
            _compoundAdapter->build();

        for ( auto _agentAdapter : m_agentWrappers )
            _agentAdapter->build();

        /* backend-specific initialization: assemble resources and create extra low-level resources */
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
    }

    void TISimulation::_preStep()
    {
        if ( m_scenarioPtr )
            m_scenarioPtr->preStep();

        // @todo: these calls|requests should be made through the high-level agent itself
        for ( auto _agentAdapter : m_agentWrappers )
            _agentAdapter->preStep();

        // @todo: these calls|requests should be made through the high-level terrain-gen. itself
        for ( auto _terrainGenAdapter : m_terrainGenWrappers )
            _terrainGenAdapter->preStep();

        // delegate any extra backend-specifics (set any low-level info, etc.)
        _preStepInternal();
    }

    void TISimulation::_postStep()
    {
        if ( m_scenarioPtr )
            m_scenarioPtr->postStep();

        // @todo: these calls|requests should be made through the high-level agent itself
        for ( auto _agentAdapter : m_agentWrappers )
            _agentAdapter->postStep();

        // @todo: these calls|requests should be made through the high-level terrain-gen. itself
        for ( auto _terrainGenAdapter : m_terrainGenWrappers )
            _terrainGenAdapter->postStep();

        // and in case some extra stuff(low-level state) needs to be grabbed, just grab it here
        _postStepInternal();
    }

    void TISimulation::reset()
    {
        if ( m_scenarioPtr )
            m_scenarioPtr->reset();

        // @todo: these calls|requests should be made through the high-level agent itself
        for ( auto _agentAdapter : m_agentWrappers )
            _agentAdapter->reset();

        // @todo: these calls|requests should be made through the high-level terrain-gen. itself
        for ( auto _terrainGenAdapter : m_terrainGenWrappers )
            _terrainGenAdapter->reset();

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
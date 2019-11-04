
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

        for ( auto _bodyAdapter : m_bodyAdapters )
            delete _bodyAdapter;

        m_agentWrappers.clear();
        m_terrainGenWrappers.clear();
        m_collisionAdapters.clear();
        m_bodyAdapters.clear();
    }

    bool TISimulation::initialize()
    {
        /* build all resources required before assembling them in the init-internal stage */
        for ( auto _agentAdapter : m_agentWrappers )
            _agentAdapter->build();

        for ( auto _bodyAdapter : m_bodyAdapters )
            _bodyAdapter->build();

        // initialize the simulation in a backend-specific way, by assembling and creating extra low-level resources
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
        for ( auto _agentAdapter : m_agentWrappers )
            _agentAdapter->preStep();

        // the same for you terrain-generators, perhaps the user wanted to move your internal things around
        for ( auto _terrainGenAdapter : m_terrainGenWrappers )
            _terrainGenAdapter->preStep();

        // @TODO: Change API names to pre-step and post-step (check if required)
        for ( auto _collisionAdapter : m_collisionAdapters )
            _collisionAdapter->update();

        // @TODO: Change API names to pre-step and post-step (check if required)
        for ( auto _bodyAdapter : m_bodyAdapters )
            _bodyAdapter->update();

        // delegate any extra backend-specifics (set any low-level info, etc.)
        _preStepInternal();
    }

    void TISimulation::_postStep()
    {
        // agent-wrapper, please, grab any information that you might need for the abstract resources
        for ( auto _agentAdapter : m_agentWrappers )
            _agentAdapter->postStep();

        // you too terrain-generator, grab any info you might need to move|create|delete terrain sections, etc.
        for ( auto _terrainGenAdapter : m_terrainGenWrappers )
            _terrainGenAdapter->postStep();

        // and in case some extra stuff(low-level state) needs to be grabbed, just grab it here
        _postStepInternal();
    }

    void TISimulation::reset()
    {
        // hey wrapper, do your thing and reset your internal configuration, pretty please :D
        for ( auto _agentAdapter : m_agentWrappers )
            _agentAdapter->reset();

        // you too terrain-gen, reset the resources to some initial position from a initial distirbution
        for ( auto _terrainGenAdapter : m_terrainGenWrappers )
            _terrainGenAdapter->reset();

        // reset the collision adapters as well
        for ( auto _collisionAdapter : m_collisionAdapters )
            _collisionAdapter->reset();

        // and don't forget the body adapters
        for ( auto _bodyAdapter : m_bodyAdapters )
            _bodyAdapter->reset();

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
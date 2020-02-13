
#include <loco_simulation.h>

namespace loco
{
    TISimulation::TISimulation( TScenario* scenarioRef )
    {
        LOCO_CORE_ASSERT( scenarioRef, "TISimulation >>> given nullptr for scenario-reference" );

        m_backendId = "none";
        m_scenarioRef = scenarioRef;
        m_running = false;

        /* DERIVED simulations should create adapters (from the scenario) as they see fit */
    }

    TISimulation::~TISimulation()
    {
        m_scenarioRef = nullptr;
        m_collisionAdapters.clear();
        m_jointAdapters.clear();
        m_singleBodyAdapters.clear();
    }

    void TISimulation::Initialize()
    {
        LOCO_CORE_ASSERT( m_scenarioRef, "TISimulation::Initialize >>> scenario-reference is nullptr" );

        // Build backend-specific resources for single-bodies (should recursively build collider and visual adapters)
        for ( auto& bodyAdapter : m_singleBodyAdapters )
            bodyAdapter->Build();

////         // Build backend-specific resources for compounds (should recursively build compound-bodies, joints, colliders and visual adapters)
////         for ( auto& compoundAdapter : m_compoundAdapters )
////             compoundAdapter->Build();
//// 
////         // Build backend-specific resources for kintree-agents (should recursively build adapters for its components: bodies, colliders, joints, etc.)
////         for ( auto& kintreeAgentAdapter : m_kintreeAgentAdapters )
////             kintreeAgentAdapter->Build();

        // Initialize backend resources (assemble them, set internal configuration, etc.)
        m_running = _InitializeInternal();

        // Initialize the scenario to its required initial simulation state
        if ( m_running )
            m_scenarioRef->Initialize();
    }

    void TISimulation::Step()
    {
        _PreStep();

        if ( m_running )
            _SimStepInternal();

        _PostStep();
    }

    void TISimulation::_PreStep()
    {
        if ( m_scenarioRef )
            m_scenarioRef->PreStep();

        _PreStepInternal();
    }

    void TISimulation::_PostStep()
    {
        if ( m_scenarioRef )
            m_scenarioRef->PostStep();

        _PostStepInternal();
    }

    void TISimulation::Reset()
    {
        if ( m_scenarioRef )
            m_scenarioRef->Reset();

        _ResetInternal();
    }

    void TISimulation::Pause()
    {
        m_running = false;
    }

    void TISimulation::Resume()
    {
        m_running = true;
    }

    /********************************************************************************
    *                               Null-simulation impl                            *
    ********************************************************************************/

    TNullSimulation::TNullSimulation( TScenario* scenarioRef )
        : TISimulation( scenarioRef )
    {
        m_backendId = "null";

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TNullSimulation @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TNullSimulation @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TNullSimulation::~TNullSimulation()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TNullSimulation @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TNullSimulation @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    bool TNullSimulation::_InitializeInternal()
    {
        return true;
    }

    void TNullSimulation::_PreStepInternal()
    {
        // just do nothing here
    }

    void TNullSimulation::_SimStepInternal()
    {
        // just do nothing here
    }

    void TNullSimulation::_PostStepInternal()
    {
        // just do nothing here
    }

    void TNullSimulation::_ResetInternal()
    {
        // just do nothing here
    }
}
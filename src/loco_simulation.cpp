
#include <loco_simulation.h>

namespace loco
{
    //// @sanitycheck
    //// How the simulation-creation process works :
    ////    * The key functions are both the constructor and _InitializeInternal method
    ////    * Within the constructor, the specific backend implementation can choose to instantiate
    ////      the resources it needs (e.g. create Raisim::World, or similar) or just declare
    ////      which resources will be used and later create them.
    ////    * Within the _InitializeInternal method, extra backend-resources can be created (e.g. create
    ////      MuJoCo's mjModel and mjData), but keep in mind that any backend-resources that need to be
    ////      created by the adapters (on their "Build" methods) might (or might not) need to use some
    ////      reference to an initialize backend (e.g. Raisim requires a handle to its raisim::World 
    ///       object to create resources like bodies and articulated systems).

    TISimulation::TISimulation( TScenario* scenarioRef )
    {
        LOCO_CORE_ASSERT( scenarioRef, "TISimulation >>> given nullptr for scenario-reference" );

        m_BackendId = "none";
        m_ScenarioRef = scenarioRef;
        m_VisualizerRef = nullptr;
        m_Running = false;
        m_WorldTime = 0.0;
        m_FixedTimeStep = 0.002;
        m_Gravity = { 0.0, 0.0, -9.81 };

        /* DERIVED simulations should create adapters (from the scenario) as they see fit */
    }

    TISimulation::~TISimulation()
    {
        m_ScenarioRef = nullptr;
        m_VisualizerRef = nullptr;
        m_SingleBodyAdapters.clear();
        m_SingleBodyAdaptersRecycled.clear();
    }

    void TISimulation::Initialize()
    {
        LOCO_CORE_ASSERT( m_ScenarioRef, "TISimulation::Initialize >>> scenario-reference is nullptr" );

        // Build backend-specific resources for single-bodies (should recursively build collider and visual adapters)
        for ( auto& bodyAdapter : m_SingleBodyAdapters )
            bodyAdapter->Build();

////         // Build backend-specific resources for compounds (should recursively build compound-bodies, joints, colliders and visual adapters)
////         for ( auto& compoundAdapter : m_compoundAdapters )
////             compoundAdapter->Build();
//// 
////         // Build backend-specific resources for kintree-agents (should recursively build adapters for its components: bodies, colliders, joints, etc.)
////         for ( auto& kintreeAgentAdapter : m_kintreeAgentAdapters )
////             kintreeAgentAdapter->Build();

        // Initialize backend resources (assemble them, set internal configuration, etc.)
        m_Running = _InitializeInternal();

        // Initialize the scenario to its required initial simulation state
        if ( m_Running )
            m_ScenarioRef->Initialize();
    }

    void TISimulation::Step( const TScalar& dt )
    {
        _PreStep();

        if ( m_Running )
            _SimStepInternal( dt );

        _PostStep();
    }

    void TISimulation::_PreStep()
    {
        if ( m_ScenarioRef )
            m_ScenarioRef->PreStep();

        _PreStepInternal();
    }

    void TISimulation::_PostStep()
    {
        if ( m_ScenarioRef )
            m_ScenarioRef->PostStep();

        _PostStepInternal();
        _CollectDetached();
    }

    void TISimulation::Reset()
    {
        if ( m_ScenarioRef )
            m_ScenarioRef->Reset();

        _ResetInternal();
    }

    void TISimulation::Pause()
    {
        m_Running = false;
    }

    void TISimulation::Resume()
    {
        m_Running = true;
    }

    void TISimulation::_CollectDetached()
    {
        for ( ssize_t i = 0; i < m_SingleBodyAdapters.size(); i++ )
        {
            if ( !m_SingleBodyAdapters[i]->detached() )
                continue;
            m_SingleBodyAdaptersRecycled.push_back( std::move( m_SingleBodyAdapters[i] ) );
            m_SingleBodyAdapters.erase( m_SingleBodyAdapters.begin() + (i--) );
        }
    }

    void TISimulation::SetVisualizer( visualizer::TIVisualizer* visualizerRef )
    {
        m_VisualizerRef = visualizerRef;
        _SetVisualizerInternal( m_VisualizerRef );
    }

    void TISimulation::SetTimeStep( const TScalar& time_step )
    {
        m_FixedTimeStep = time_step;
        _SetTimeStepInternal( time_step );
    }

    void TISimulation::SetGravity( const TVec3& gravity )
    {
        m_Gravity = gravity;
        _SetGravityInternal( gravity );
    }

    /********************************************************************************
    *                               Null-simulation impl                            *
    ********************************************************************************/

    TNullSimulation::TNullSimulation( TScenario* scenarioRef )
        : TISimulation( scenarioRef )
    {
        m_BackendId = "null";

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TNullSimulation @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TNullSimulation @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TNullSimulation::~TNullSimulation()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TNullSimulation @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TNullSimulation @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }
}
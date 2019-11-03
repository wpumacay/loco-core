
#include <runtime.h>

namespace tysoc {

    TRuntime::TRuntime( const std::string& dlpathSim,
                        const std::string& dlpathViz )
    {
        m_fcnCreateSim = NULL;
        m_fcnCreateAgent = NULL;
        m_fcnCreateTerrainGen = NULL;
        m_fcnCreateViz = NULL;

        m_libraryHandleSim = NULL;
        m_libraryHandleViz = NULL;

        m_dlpathSim = dlpathSim;
        m_dlpathViz = dlpathViz;

        m_visualizerPtr = NULL;
        m_simulationPtr = NULL;

        _loadLibraryFcns();
    }

    TRuntime::~TRuntime()
    {
        m_fcnCreateSim = NULL;
        m_fcnCreateAgent = NULL;
        m_fcnCreateTerrainGen = NULL;
        m_fcnCreateViz = NULL;

        if ( m_visualizerPtr )
        {
            delete m_visualizerPtr;
            m_visualizerPtr = NULL;
        }

        if ( m_simulationPtr )
        {
            delete m_simulationPtr;
            m_simulationPtr = NULL;
        }

        if ( m_libraryHandleSim )
        {
            dlclose( m_libraryHandleSim );
            m_libraryHandleSim = NULL;
        }

        if ( m_libraryHandleViz )
        {
            dlclose( m_libraryHandleViz );
            m_libraryHandleSim = NULL;
        }
    }

    void TRuntime::step()
    {
        if ( m_simulationPtr )
            m_simulationPtr->step();

        if ( m_visualizerPtr )
            m_visualizerPtr->update();
    }

    void TRuntime::_loadLibraryFcns()
    {
        // Grab a handle to the simulation library
        if ( m_dlpathSim != "" )
        {
            m_libraryHandleSim = dlopen( m_dlpathSim.c_str(), RTLD_NOW );
            if ( !m_libraryHandleSim )
            {
                std::cout << "ERROR> while loading simulation dllibrary: " << dlerror() << std::endl;
                return;
            }

            /* 
            *   Grab from the simulation library the symbols related to the creation ...
            *   of the components of the simulation, like the simulation itself, agent ...
            *   creation, terrain creation and sensors creation
            */
    
            // Grab simulation creation function
            m_fcnCreateSim = _loadFcnFromLibrarySim< FcnCreateSim* >( "simulation_create" );
            // and the agent creation functions
            m_fcnCreateAgent = _loadFcnFromLibrarySim< FcnCreateAgent* >( "agent_createFromAbstract" );
            // and the terrain-generator creation functions
            m_fcnCreateTerrainGen = _loadFcnFromLibrarySim< FcnCreateTerrainGen* >( "terrain_createFromAbstract" );

            // Grab body and collision adapters
            m_fcnCreateBodyAdapter      = _loadFcnFromLibrarySim< FcnCreateBodyAdapter* >( "simulation_createBodyAdapter" );
            m_fcnCreateCollisionAdapter = _loadFcnFromLibrarySim< FcnCreateCollisionAdapter* >( "simulation_createCollisionAdapter" );
        }

        // Grab a handle to the visualization library
        if ( m_dlpathViz != "" )
        {
            m_libraryHandleViz = dlopen( m_dlpathViz.c_str(), RTLD_NOW );
            if ( !m_libraryHandleViz )
            {
                std::cout << "ERROR> while loading visualization dllibrary: " << dlerror() << std::endl;
                return;
            }

            /*
            *   Grab from the visualization library the symbols for the creation of visualizer objects
            */
            m_fcnCreateViz          = _loadFcnFromLibraryViz< FcnCreateViz* >( "visualizer_createVisualizer" );
            m_fcnCreateVizDrawable  = _loadFcnFromLibraryViz< FcnCreateVizDrawable*>( "visualizer_createDrawable" );
        }
    }

    TISimulation* TRuntime::createSimulation( TScenario* scenarioPtr )
    {
        if ( !m_libraryHandleSim )
            return NULL;

        if ( m_simulationPtr )
            delete m_simulationPtr;
        
        m_simulationPtr = m_fcnCreateSim( scenarioPtr );

        if ( m_visualizerPtr )
            m_simulationPtr->setVisualizer( m_visualizerPtr );

        return m_simulationPtr;
    }

    void TRuntime::destroySimulation()
    {
        if ( m_simulationPtr )
            delete m_simulationPtr;

        m_simulationPtr = NULL;
    }

    TAgentWrapper* TRuntime::createAgent( TAgent* agentPtr )
    {
        return m_fcnCreateAgent( agentPtr );
    }
        
    TTerrainGenWrapper* TRuntime::createTerrainGen( TITerrainGenerator* terrainGenPtr )
    {
        return m_fcnCreateTerrainGen( terrainGenPtr );
    }

    TIVisualizer* TRuntime::createVisualizer( TScenario* scenarioPtr )
    {
        if ( !m_libraryHandleViz )
            return NULL;

        if ( m_visualizerPtr )
            delete m_visualizerPtr;

        m_visualizerPtr = m_fcnCreateViz( scenarioPtr );

        if ( m_simulationPtr )
        {
            m_simulationPtr->setVisualizer( m_visualizerPtr );
            m_visualizerPtr->setSimulation( m_simulationPtr );
        }

        return m_visualizerPtr;
    }

    void TRuntime::destroyVisualizer()
    {
        if ( m_visualizerPtr )
            delete m_visualizerPtr;

        m_visualizerPtr = NULL;
    }

}
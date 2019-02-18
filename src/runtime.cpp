
#include <runtime.h>

namespace tysoc {

    using namespace viz;

    TRuntime::TRuntime( const std::string& dlpathSim,
                        const std::string& dlpathViz )
    {
        m_fcnCreateSim = NULL;
        m_fcnCreateAgentFromAbstract = NULL;
        m_fcnCreateAgentFromFile = NULL;
        m_fcnCreateAgentFromId = NULL;
        m_fcnCreateTerrainGenFromAbstract = NULL;
        m_fcnCreateTerrainGenFromParams = NULL;

        m_fcnCreateViz = NULL;

        m_libraryHandleSim = NULL;
        m_libraryHandleViz = NULL;

        m_dlpathSim = dlpathSim;
        m_dlpathViz = dlpathViz;

        _loadLibraryFcns();
    }

    TRuntime::~TRuntime()
    {
        m_fcnCreateSim = NULL;
        m_fcnCreateAgentFromAbstract = NULL;
        m_fcnCreateAgentFromFile = NULL;
        m_fcnCreateAgentFromId = NULL;
        m_fcnCreateTerrainGenFromAbstract = NULL;
        m_fcnCreateTerrainGenFromParams = NULL;

        m_fcnCreateViz = NULL;

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

    void TRuntime::_loadLibraryFcns()
    {
        // Grab a handle to the simulation library
        m_libraryHandleSim = dlopen( m_dlpathSim.c_str(), RTLD_NOW );
        if ( !m_libraryHandleSim )
        {
            std::cout << "ERROR> while loading simulation dllibrary: " << dlerror() << std::endl;
            return;
        }

        // Grab a handle to the visualization library
        m_libraryHandleViz = dlopen( m_dlpathViz.c_str(), RTLD_NOW );
        if ( !m_libraryHandleViz )
        {
            std::cout << "ERROR> while loading visualization dllibrary: " << dlerror() << std::endl;
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
        m_fcnCreateAgentFromAbstract    = _loadFcnFromLibrarySim< FcnCreateAgentFromAbstract* >( "agent_createFromAbstract" );
        m_fcnCreateAgentFromFile        = _loadFcnFromLibrarySim< FcnCreateAgentFromFile* >( "agent_createFromFile" );
        m_fcnCreateAgentFromId          = _loadFcnFromLibrarySim< FcnCreateAgentFromId* >( "agent_createFromId" );
        // and the terrain-generator creation functions
        m_fcnCreateTerrainGenFromAbstract   = _loadFcnFromLibrarySim< FcnCreateTerrainGenFromAbstract* >( "terrain_createFromAbstract" );
        m_fcnCreateTerrainGenFromParams     = _loadFcnFromLibrarySim< FcnCreateTerrainGenFromParams* >( "terrain_createFromParams" );

        /*
        *   Grab from the visualization library the symbols for the visualizer creation
        */
        m_fcnCreateViz = _loadFcnFromLibraryViz< FcnCreateViz* >( "visualizer_create" );
    }

    TISimulation* TRuntime::createSimulation( TScenario* scenarioPtr )
    {
        return m_fcnCreateSim( scenarioPtr );
    }

    TKinTreeAgentWrapper* TRuntime::createAgent( agent::TAgentKinTree* kinTreeAgentPtr )
    {
        return m_fcnCreateAgentFromAbstract( kinTreeAgentPtr );
    }

    TKinTreeAgentWrapper* TRuntime::createAgent( const std::string& name,
                                                 const std::string& filename )
    {
        return m_fcnCreateAgentFromFile( name, filename );
    }

    TKinTreeAgentWrapper* TRuntime::createAgent( const std::string& name,
                                                 const std::string& format,
                                                 const std::string& id )
    {
        return m_fcnCreateAgentFromId( name, format, id );
    }
        
    TTerrainGenWrapper* TRuntime::createTerrainGen( terrain::TITerrainGenerator* terrainGenPtr )
    {
        return m_fcnCreateTerrainGenFromAbstract( terrainGenPtr );
    }

    TTerrainGenWrapper* TRuntime::createTerrainGen( const std::string& name,
                                                    const TGenericParams& params )
    {
        return m_fcnCreateTerrainGenFromParams( name, params );
    }

    TIVisualizer* TRuntime::createVisualizer( TScenario* scenarioPtr )
    {
        return m_fcnCreateViz( scenarioPtr );
    }

}
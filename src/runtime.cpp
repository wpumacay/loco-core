
#include <runtime.h>


namespace tysoc {


    TRuntime::TRuntime( const std::string& dlpath )
    {
        m_fcnCreateSim = NULL;
        m_fcnCreateAgentFromAbstract = NULL;
        m_fcnCreateAgentFromFile = NULL;
        m_fcnCreateAgentFromId = NULL;
        m_fcnCreateTerrainGenFromAbstract = NULL;
        m_fcnCreateTerrainGenFromParams = NULL;

        m_libraryHandle = NULL;

        m_dlpath = dlpath;

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

        if ( m_libraryHandle )
        {
            dlclose( m_libraryHandle );
            m_libraryHandle = NULL;
        }
    }

    void TRuntime::_loadLibraryFcns()
    {
        // Grab a handle to the library
        m_libraryHandle = dlopen( m_dlpath.c_str(), RTLD_NOW );
        if ( !m_libraryHandle )
        {
            std::cout << "ERROR> while loading dllibrary: " << dlerror() << std::endl;
            return;
        }

        /* 
        *   Grab from the library the symbols related to the creation of the ...
        *   components of the simulation, like the simulation itself, agent ...
        *   creation, terrain creation and sensors creation
        */

        // Grab simulation creation function
        m_fcnCreateSim = _loadFcnFromLibrary< FcnCreateSim* >( "simulation_create" );
        // and the agent creation functions
        m_fcnCreateAgentFromAbstract    = _loadFcnFromLibrary< FcnCreateAgentFromAbstract* >( "agent_createFromAbstract" );
        m_fcnCreateAgentFromFile        = _loadFcnFromLibrary< FcnCreateAgentFromFile* >( "agent_createFromFile" );
        m_fcnCreateAgentFromId          = _loadFcnFromLibrary< FcnCreateAgentFromId* >( "agent_createFromId" );
        // and the terrain-generator creation functions
        m_fcnCreateTerrainGenFromAbstract   = _loadFcnFromLibrary< FcnCreateTerrainGenFromAbstract* >( "terrain_createFromAbstract" );
        m_fcnCreateTerrainGenFromParams     = _loadFcnFromLibrary< FcnCreateTerrainGenFromParams* >( "terrain_createFromParams" );
    }

    TISimulation* TRuntime::createSimulation()
    {
        return m_fcnCreateSim();
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

}
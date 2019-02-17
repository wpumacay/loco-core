
#pragma once

#include <simulation_base.h>
#include <dlfcn.h>

namespace tysoc {


    class TRuntime
    {

        protected :

        /* 
        *   Fcn pointers to the library functionality
        */

        FcnCreateSim* m_fcnCreateSim;

        FcnCreateAgentFromAbstract* m_fcnCreateAgentFromAbstract;
        FcnCreateAgentFromFile*     m_fcnCreateAgentFromFile;
        FcnCreateAgentFromId*       m_fcnCreateAgentFromId;

        FcnCreateTerrainGenFromAbstract*    m_fcnCreateTerrainGenFromAbstract;
        FcnCreateTerrainGenFromParams*      m_fcnCreateTerrainGenFromParams;

        /* Handle to the library */
        void* m_libraryHandle;
        /* Path to the library to be loaded */
        std::string m_dlpath;
        /* Loading function: loads functions from library to fcn pointers above */
        void _loadLibraryFcns();

        /* Loads a single function from the current library, given its name */
        template< typename FunctionTypePtr >
        FunctionTypePtr _loadFcnFromLibrary( const std::string& fcnName )
        {
            auto _fcnPtr = ( FunctionTypePtr ) dlsym( m_libraryHandle, fcnName.c_str() );
            if ( !_fcnPtr )
            {
                std::cout << "ERROR> while loading symbol: " << dlerror() << std::endl;
                return NULL;
            }

            std::cout << "INFO> successfully loaded symbol: " << fcnName << std::endl;
            return _fcnPtr;
        }

        public :

        /**
        *   Creates a runtime with a specific type. It loads the library .so ...
        *   (or .dll) accordingly
        */
        TRuntime( const std::string& dlpath );
        ~TRuntime();

        TISimulation* createSimulation();

        TKinTreeAgentWrapper* createAgent( agent::TAgentKinTree* kinTreeAgentPtr );
        TKinTreeAgentWrapper* createAgent( const std::string& name,
                                           const std::string& filename );
        TKinTreeAgentWrapper* createAgent( const std::string& name,
                                           const std::string& format,
                                           const std::string& id );
        
        TTerrainGenWrapper* createTerrainGen( terrain::TITerrainGenerator* terrainGenPtr );
        TTerrainGenWrapper* createTerrainGen( const std::string& name,
                                              const TGenericParams& params );

    };


}
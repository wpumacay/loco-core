
#pragma once

#include <simulation_base.h>
#include <viz/viz.h>
#include <dlfcn.h>

namespace tysoc {

    using namespace viz;

    class TRuntime
    {

        protected :

        /* 
        *   Fcn pointers to the library functionality
        */

        FcnCreateSim* m_fcnCreateSim;
        FcnCreateViz* m_fcnCreateViz;

        FcnCreateAgentFromAbstract* m_fcnCreateAgentFromAbstract;
        FcnCreateAgentFromFile*     m_fcnCreateAgentFromFile;
        FcnCreateAgentFromId*       m_fcnCreateAgentFromId;

        FcnCreateTerrainGenFromAbstract*    m_fcnCreateTerrainGenFromAbstract;
        FcnCreateTerrainGenFromParams*      m_fcnCreateTerrainGenFromParams;

        /* Handle to the simulation library */
        void* m_libraryHandleSim;
        /* Path to the simulation library to be loaded */
        std::string m_dlpathSim;

        /* Handle to the visualization library */
        void* m_libraryHandleViz;
        /* Path to the visualization library to be loaded */
        std::string m_dlpathViz;

        /* Loading function: loads functions from library to fcn pointers above */
        void _loadLibraryFcns();

        /* Loads a single function from the simulation library, given its name */
        template< typename FunctionTypePtr >
        FunctionTypePtr _loadFcnFromLibrarySim( const std::string& fcnName )
        {
            auto _fcnPtr = ( FunctionTypePtr ) dlsym( m_libraryHandleSim, fcnName.c_str() );
            if ( !_fcnPtr )
            {
                std::cout << "ERROR> while loading simulation-symbol: " << dlerror() << std::endl;
                return NULL;
            }

            std::cout << "INFO> successfully loaded simulation-symbol: " << fcnName << std::endl;
            return _fcnPtr;
        }

        /* Loads a single function from the visualization, given its name */
        template< typename FunctionTypePtr >
        FunctionTypePtr _loadFcnFromLibraryViz( const std::string& fcnName )
        {
            auto _fcnPtr = ( FunctionTypePtr ) dlsym( m_libraryHandleViz, fcnName.c_str() );
            if ( !_fcnPtr )
            {
                std::cout << "ERROR> while loading visualization-symbol: " << dlerror() << std::endl;
                return NULL;
            }

            std::cout << "INFO> successfully loaded visualization-symbol: " << fcnName << std::endl;
            return _fcnPtr;
        }

        TIVisualizer* m_visualizerPtr;
        TISimulation* m_simulationPtr;

        std::string m_workingDir;

        public :

        /**
        *   Creates a runtime with a specific type. It loads the library .so ...
        *   (or .dll) accordingly.

        *   @param dlpathSim    Path to the library for simulation
        *   @param dlpathViz    Path to the library for visualization
        */
        TRuntime( const std::string& dlpathSim,
                  const std::string& dlpathViz,
                  const std::string& workingDir = TYSOC_PATH_WORKING_DIR );
        ~TRuntime();

        void step();

        TISimulation* createSimulation( TScenario* scenarioPtr = NULL );
        void destroySimulation();

        TKinTreeAgentWrapper* createAgent( agent::TAgentKinTree* kinTreeAgentPtr );
        TKinTreeAgentWrapper* createAgent( const std::string& name,
                                           const std::string& filename );
        TKinTreeAgentWrapper* createAgent( const std::string& name,
                                           const std::string& format,
                                           const std::string& id );
        
        TTerrainGenWrapper* createTerrainGen( terrain::TITerrainGenerator* terrainGenPtr );
        TTerrainGenWrapper* createTerrainGen( const std::string& name,
                                              const TGenericParams& params );

        TIVisualizer* createVisualizer( TScenario* scenarioPtr = NULL );
        void destroyVisualizer();

        std::string workingDir();
    };


}
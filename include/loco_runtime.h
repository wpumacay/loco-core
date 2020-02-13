
#pragma once

#include <loco_logger.h>
#include <loco_simulation.h>
#include <visualizer/loco_visualizer.h>

#include <adapters/loco_body_adapter.h>
#include <adapters/loco_collision_adapter.h>

#include <dlfcn.h>

// @firsttodo : reimplement this class

namespace loco
{
    class TRuntime
    {

    public :

        /**
        *   Creates a runtime with a specific type. It loads the library .so ...
        *   (or .dll) accordingly.
        *
        *   @param dlpathSim    Path to the library for simulation
        *   @param dlpathViz    Path to the library for visualization
        */
        TRuntime( const std::string& dlpathSim,
                  const std::string& dlpathViz );
        ~TRuntime();

        void step();

        TISimulation* createSimulation( TScenario* scenarioPtr = nullptr );

        TAgentWrapper* createAgent( TAgent* agentPtr );

        TTerrainGenWrapper* createTerrainGen( TITerrainGenerator* terrainGenPtr );

        TIVisualizer* createVisualizer( TScenario* scenarioPtr = nullptr );

        void destroySimulation();

        void destroyVisualizer();

    protected :

        /* Loading function: loads functions from library to fcn pointers above */
        void _loadLibraryFcns();

        /* Loads a single function from the simulation library, given its name */ // @firsttodo: move to impl.hpp
        template< typename FunctionTypePtr >
        FunctionTypePtr _loadFcnFromLibrarySim( const std::string& fcnName )
        {
            auto _fcnPtr = ( FunctionTypePtr ) dlsym( m_libraryHandleSim, fcnName.c_str() );
            if ( !_fcnPtr )
            {
                LOCO_CORE_ERROR( "Couldn't load simulation-symbol: {0}. Error message: {1}", fcnName, dlerror() );
                return nullptr;
            }

            LOCO_CORE_INFO( "Successfully loaded simulation-symbol: {0}", fcnName );
            return _fcnPtr;
        }

        /* Loads a single function from the visualization, given its name */
        template< typename FunctionTypePtr >
        FunctionTypePtr _loadFcnFromLibraryViz( const std::string& fcnName )
        {
            auto _fcnPtr = ( FunctionTypePtr ) dlsym( m_libraryHandleViz, fcnName.c_str() );
            if ( !_fcnPtr )
            {
                LOCO_CORE_ERROR( "Couldn't load visualization-symbol: {0}. Error message: {1}", fcnName, dlerror() );
                return nullptr;
            }

            LOCO_CORE_INFO( "Successfully loaded visualization-symbol: {0}", fcnName );
            return _fcnPtr;
        }

        FcnCreateSim* m_fcnCreateSim;

        FcnCreateViz* m_fcnCreateViz;
        FcnCreateVizDrawable* m_fcnCreateVizDrawable;

        FcnCreateBodyAdapter*         m_fcnCreateBodyAdapter;
        FcnCreateCollisionAdapter*    m_fcnCreateCollisionAdapter;

        FcnCreateAgent*        m_fcnCreateAgent;
        FcnCreateTerrainGen*   m_fcnCreateTerrainGen;

        /* Handle to the simulation library */
        void* m_libraryHandleSim;
        /* Path to the simulation library to be loaded */
        std::string m_dlpathSim;

        /* Handle to the visualization library */
        void* m_libraryHandleViz;
        /* Path to the visualization library to be loaded */
        std::string m_dlpathViz;

        TIVisualizer* m_visualizerPtr;
        TISimulation* m_simulationPtr;
    };

}
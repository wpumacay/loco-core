#pragma once

#include <loco_config.h>
#include <loco_logger.h>
#include <loco_simulation.h>
#include <visualizer/loco_visualizer.h>
// @todo-windows: search for alternative in windows-platform
#include <dlfcn.h>

namespace loco
{
    class TISimulation;
    class TIVisualizer;

    typedef TISimulation* FcnCreateSim( TScenario* scenarioRef );
    typedef TIVisualizer* FcnCreateViz( TScenario* scenarioRef,
                                        size_t windowWidth, size_t windowHeight,
                                        bool windowResizable, bool renderOffscreen );

    class TRuntime
    {

    public :

        /// Creates a runtime with a specific type. It loads the library .so (or .dll) accordingly
        ///
        /// @param dlpathSim    Path to the library for simulation
        /// @param dlpathViz    Path to the library for visualization
        TRuntime( const std::string& dlpathSim,
                  const std::string& dlpathViz );

        ~TRuntime();

        TISimulation* CreateSimulation( TScenario* scenarioRef );

        TIVisualizer* CreateVisualizer( TScenario* scenarioRef,
                                        size_t windowWidth = 1280,
                                        size_t windowHeight = 800,
                                        bool windowResizable = true,
                                        bool renderOffscreen = false );

        void DestroySimulation();

        void DestroyVisualizer();

        std::string dlpathSim() const { return m_dlpathSimulation; }

        std::string dlpathViz() const { return m_dlpathVisualizer; }

        TISimulation* GetCurrentSimulation() { return m_simulation.get(); }

        const TISimulation* GetCurrentSimulation() const { return m_simulation.get(); }

        TIVisualizer* GetCurrentVisualizer() { return m_visualizer.get(); }

        const TIVisualizer* GetCurrentVisualizer() const { return m_visualizer.get(); }

    protected :

        // Loads required symbols|functions from shared libraries
        void _LoadLibraryFunctions( const std::string& dlpathSim,
                                    const std::string& dlpathViz );

        // Load a single symbol from the simulation library given its name
        void* _LoadFcnFromSimulationLibrary( const std::string& fcnName );

        // Load a single symbol from the visualizer library given its name
        void* _LoadFcnFromVisualizerLibrary( const std::string& fcnName );

        // Function-pointer (loaded from sim-library) that creates a backend-specific simulation
        FcnCreateSim* m_fcnCreateSimulation;
        // Function pointer (loaded from viz-library) that creates a backend-specific visualizer
        FcnCreateViz* m_fcnCreateVisualizer;
        // Handle to the shared library that contains backend-specific simulation symbols
        void* m_libHandleSimulation;
        // Handle to the shared library that contains backend-specific visualization symbols
        void* m_libHandleVisualizer;
        // Path to the shared library (.so,.dll,.dylib) that contains backend-specific simulation symbols
        std::string m_dlpathSimulation;
        // Path to the shared library (.so,.dll,.dylib) that contains backend-specific visualization library
        std::string m_dlpathVisualizer;
        // Simulation object (loaded from a specific backend)
        std::unique_ptr<TISimulation> m_simulation;
        // Visualizer object (loaded from a specific backend)
        std::unique_ptr<TIVisualizer> m_visualizer;
    };
}
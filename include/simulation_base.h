
#pragma once

#include <scenario.h>
#include <agent_wrapper.h>
#include <terrain_wrapper.h>
#include <viz/viz.h>

namespace tysoc {

    class TIVisualizer;
    class TAgentWrapper;

    /**
    *   Simulation Interface. This is the base class for all types of backend ...
    *   specific physics simulations. This serves as a wrapper for a scenario, ...
    *   which is the structure that contains all agents, terraingens and sensors.
    *   
    *   A simulation is initialized from a given scenario. If no scenario is ...
    *   passed, then a default scenario is created. If a scenario is passed, ...
    *   then all components of that scenario (agents, terraingens, and sensors) ...
    *   are wrapped and these wrappers are stored in the simulation. This is ...
    *   similar to "define and then run" (tensorflow). However, during execution ...
    *   the user should be able to add extra agents, terraingens or sensors ...
    *   via direct addition of wrapped objects using this class "add" methods.
    *
    *
    *   Backend specific functionality should extend this class, and implement ...
    *   the necessary code in the following virtual methods:
    *
    *   > _initializeInternal: backend-specific initialization functionality. This ...
    *                          serves as an entry point for the simulation, and here ...
    *                          we can add functionality to initialize necessary ...
    *                          data for the specific backend, e.g. creating ...
    *                          the mjModel and mjData structures for mujoco, or ...
    *                          creating the bt("TYPE")World in bullet.
    *
    *   > _updateInternal: backend-specific update functionality. This is the ...
    *                      function that gets called every simulation step and ...
    *                      it should contain code to update one step of the ...
    *                      simulation using the specific backend, e.g. mj_step ...
    *                      for mujoco.
    *
    *   > _resetInternal: backend-specific reset functionality. Here we should ...
    *                     place the code that is used when trying to reset the ...
    *                     simulation, i.e. when requesting an episode reset. Here ...
    *                     could place functionality like moving every sim component ...
    *                     to a given new starting position.
    *
    */
    class TISimulation
    {

        protected :

        /* Identifier of the backend being used (mujoco|bullet|raisim|dart|...) */
        std::string m_runtimeType;

        /* A reference to the scenario being instantiated */
        TScenario* m_scenarioPtr;

        /* Agent wrappers instantiated for the high-level agents in the  scenario */
        std::vector< TAgentWrapper* > m_agentWrappers;

        /* Terrain-generator wrappers instantiated for the high-level terrain-generators in the scenario */
        std::vector< TTerrainGenWrapper* > m_terrainGenWrappers;

        /* A reference to the visualizer (to grab frame-data, debug-draws, etc. */
        TIVisualizer* m_visualizerPtr;

        /* Collision adapters instantiated as part of the high-level bodies in the scenario */
        std::vector< TICollisionAdapter* > m_collisionAdapters;

        /* Body adapters instantiated for the high-level bodies in the scenario */
        std::vector< TIBodyAdapter* > m_bodyAdapters;

        /* Executes functionality used before taking a simulation step in the backend */
        void _preStep();

        /* Executes functionality used after taking a simulation step in the backend */
        void _postStep();

        /* Initialize some backend-specific resources */
        virtual bool _initializeInternal() = 0;

        /* Execute extra functionality used before taking a simulation step in the backend */
        virtual void _preStepInternal() = 0;

        /* Execute functionality for a simulation step in a backend */
        virtual void _simStepInternal() = 0;

        /* Execute functionality after a sim-step was taken in the backend */
        virtual void _postStepInternal() = 0;

        /* Reset some backend-specific resources */
        virtual void _resetInternal() = 0;

        bool m_isRunning;
        bool m_isDebugDrawingActive;

        public :

        /* Creates a simulation that instantiates the given scenario in a specific backend*/
        TISimulation( TScenario* scenarioPtr );

        /* Releases low-level backend resources and wrappers created to instantiate the given ascenario */
        virtual ~TISimulation();

        /* Initializes all low-level resources used to instantiate the scenario in the backend */
        bool initialize();

        /* Calls functionality used for taking a simulation step */
        void step();

        void reset();

        void togglePause();

        void toggleDebugDraws();

        bool isRunning() { return m_isRunning; }

        bool isDebugDrawingActive() { return m_isDebugDrawingActive; }

        void setVisualizer( TIVisualizer* visualizerPtr ) { m_visualizerPtr = visualizerPtr; }

        TIVisualizer* visualizer() { return m_visualizerPtr; }

        TScenario* scenario() { return m_scenarioPtr; }

        std::string type() { return m_runtimeType; }

    };

    // Function pointers definitions for loading backend-specific functionality

    typedef TISimulation* FcnCreateSim( TScenario* scenarioPtr );

}
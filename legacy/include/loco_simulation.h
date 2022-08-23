
#pragma once

#include <loco_scenario.h>
#include <primitives/loco_single_body_adapter.h>
#include <kinematic_trees/loco_kinematic_tree_adapter.h>
#include <visualizer/loco_visualizer.h>

namespace loco {
namespace visualizer {
    class TIVisualizer;
}}

namespace loco
{
    /// Simulation interface for all backend-specific simulations
    ///
    /// @details 
    /// A simulation is initialized from a given scenario. If no scenario is
    /// passed, then a default scenario is created. If a scenario is passed,
    /// then all components of that scenario (agents, terraingens, and sensors)
    /// are wrapped and these wrappers are stored in the simulation. This is
    /// similar to "define and then run" (tensorflow).
    ///
    /// Backend-specific simulations should extend this interface, and implement
    /// the necessary code in the following virtual methods:
    class TISimulation
    {
    public :

        /// Creates a simulation and all required resources to simulate a given scenario
        ///
        /// @param scenario_ref     A reference to the scenario to be simulated
        TISimulation( TScenario* scenario_ref );

        /// Releases all resources created for the simulation of a given scenario
        virtual ~TISimulation();

        /// Creates and initializes the internal state of the simulation
        void Initialize();

        /// Takes a fixed-time step in the simulation
        ///
        /// @details
        /// The simulation is advanced forward by multiples of the @ref m_FixedTimeStep (e.g. 0.001s), so
        /// ideally the desired step should be a multiple of this quantity. The @ref m_WorldTime counter
        /// is updated with this multiple value, so the actual world time of the simulation might
        /// be slightly different from the wall-time.
        ///
        /// @param dt   Desired amount of time that the user wants to step forward the simulation
        void Step( const TScalar& dt = 0.0 );

        /// Resets the simulation and its components to its initial state
        void Reset();

        /// Pauses the current execution of the simulation. Steps will not have effect
        void Pause();

        /// Resumes the current execution of the simulation. Steps will have normal effect
        void Resume();

        /// Updates the internal fixed time-step taken by the engine
        ///
        /// @details
        /// The fixed time-step consists of the amount of time in the simulation in between discrete
        /// steps of the simulation, and it's the amount of time used by the internal engine's integrator
        /// to advance the simulation. Each discrete step usually involves forward integration, collision
        /// detection, constraint resolutions, etc. Note that the total amount of time required by all
        /// these components might actually be smaller than the fixed time-step taken. For example, this
        /// simulation cost might be of 0.1 milliseconds (for a fast physics engine), but the fixed time-step
        /// used by the integrator might only be 1 millisecond. In this case we say that the engine can
        /// simulate faster than real-time.
        ///
        /// @param time_step    Fixed amount of time used by the internal engine's integrator to advance
        ///                     the simulation in between discrete steps of the simulation
        void SetTimeStep( const TScalar& time_step );

        /// Updates the value of the simulation's gravity vector
        void SetGravity( const TVec3& gravity );

        /// Updates the reference to the visualizer
        void SetVisualizer( visualizer::TIVisualizer* visualizerRef );

        /// Returns whether or not the simulation is currently running or if it's paused
        bool running() const { return m_Running; }

        /// Returns a mutable reference to the scenario currently being simulated
        TScenario* scenario() { return m_ScenarioRef; }

        /// Returns an unmutable reference to the scenario currently being simulated
        const TScenario* scenario() const { return m_ScenarioRef; }

        /// Returns a mutable reference to the visualizer currently rendering the scenario
        visualizer::TIVisualizer* visualizer() { return m_VisualizerRef; }

        /// Returns an unmutable reference to the visualizer currently rendering the scenario
        const visualizer::TIVisualizer* visualizer() const { return m_VisualizerRef; }

        /// Returns the string identifier of the internal physics backend being used for simulation
        std::string backendId() const { return m_BackendId; }

        /// Returns the value of the fixed time-step taken by the internal engine's integrator
        TScalar time_step() const { return m_FixedTimeStep; }

        /// Returns the current simulation-time, increased in multipled of fixed time-step every simulation step
        TScalar world_time() const { return m_WorldTime; }

    protected :

        /// Takes a pre-step, a.k.a. setup prior to a simulation step in the specific backend
        void _PreStep();

        /// Takes a post-step, a.k.a. state-gather after taking a simulation step in the specific backend
        void _PostStep();

        /// Collects all detached objects from the simulation (recycles resources)
        void _CollectDetached();

        /// Internal method used to create and initialize all required resources for the simulation
        virtual bool _InitializeInternal() = 0;

        /// Internal method used to setup any extra required backend-specific state prior to taking a simulation step
        virtual void _PreStepInternal() = 0;

        /// Internal method used to take a backend-specific step for this simulation
        virtual void _SimStepInternal( const TScalar& dt ) = 0;

        /// Internal method used to collect any extra required backend-specific information after a simulation step was taken
        virtual void _PostStepInternal() = 0;

        /// Internal method used to reset any backend-specific state for this simulation
        virtual void _ResetInternal() = 0;

        /// Internal method used to update the fixed time-step of the backend-specific engine's integrator
        virtual void _SetTimeStepInternal( const TScalar& time_step ) = 0;

        /// Internal method used to update the gravity vector of the backend specific engine
        virtual void _SetGravityInternal( const TVec3& gravity ) = 0;

        /// Internal method used to setup|collect any required information from the visualizer in charge of rendering the scenario being simulated
        virtual void _SetVisualizerInternal( visualizer::TIVisualizer* visualizerRef ) {};

    protected :

        // Identifier of the backend being used (mujoco|bullet|raisim|dart|...)
        std::string m_BackendId;

        // A reference to the scenario being instantiated (we don't take ownership)
        TScenario* m_ScenarioRef;

        // A reference to the visualizer (if available) (we don't take ownership)
        visualizer::TIVisualizer* m_VisualizerRef;

        /// Container used to store the single-body resources used for the simulation
        std::vector< std::unique_ptr<primitives::TISingleBodyAdapter> > m_SingleBodyAdapters;

        /// Container used to store recycled single-body resources previously in use by the simulation
        std::vector< std::unique_ptr<primitives::TISingleBodyAdapter> > m_SingleBodyAdaptersRecycled;

        /// Container used to store the kinematic-tree resources used for the simulation
        std::vector< std::unique_ptr<kintree::TIKinematicTreeAdapter> > m_KinematicTreeAdapters;

        /// Container used to store recycled kinematic-tree resources previously in use by the simulation
        std::vector< std::unique_ptr<kintree::TIKinematicTreeAdapter> > m_KinematicTreeAdaptersRecycled;

        // Fixed time-step used for the internal engine's integrator
        TScalar m_FixedTimeStep;

        /// World-time of the simulation (only increases in multiples of the fixed time-step)
        TScalar m_WorldTime;

        /// Value of gravity-vector used for the simulation
        TVec3 m_Gravity;

        // Whether or not the simulation is running
        bool m_Running;
    };

    /// Functions pointer type, used while loading symbols from the appropriate dynamic libraries
    typedef TISimulation* FcnCreateSim( TScenario* scenarioRef );

    /// @brief Dummy simulation object to use as replacement when no backend available
    ///
    /// @details
    /// This class implements just a dummy simulation that can be drop in if no backend is
    /// available, but still want to try out other features like the visualizer, parsers, etc.
    class TNullSimulation : public TISimulation
    {
    public:

        /// Creates a dummy simulation object, which is not related to any backend
        TNullSimulation( TScenario* scenarioRef );

        /// Releases this dymmy simulation
        ~TNullSimulation();

    protected :

        // Documentation inherited
        bool _InitializeInternal() override { return true; }

        // Documentation inherited
        void _PreStepInternal() override {}

        // Documentation inherited
        void _SimStepInternal( const TScalar& dt ) override {}

        // Documentation inherited
        void _PostStepInternal() override {}

        // Documentation inherited
        void _ResetInternal() override {}

        // Documentation inherited
        void _SetTimeStepInternal( const TScalar& time_step ) override {}

        // Documentation inherited
        void _SetGravityInternal( const TVec3& gravity ) override {}
    };
}

#pragma once

#include <loco_scenario.h>
#include <primitives/loco_single_body_adapter.h>
#include <primitives/loco_single_body_collider_adapter.h>
//// #include <adapters/loco_compound_adapter.h>
//// #include <adapters/loco_kintree_agent_adapter.h>
//// #include <adapters/loco_terrain_generator_adapter.h>
#include <visualizer/loco_visualizer.h>

namespace loco
{
    class TIVisualizer;

    /// @brief Simulation interface for all backend-specific simulations
    ///
    /// @details 
    /// A simulation is initialized from a given scenario. If no scenario is
    /// passed, then a default scenario is created. If a scenario is passed,
    /// then all components of that scenario (agents, terraingens, and sensors)
    /// are wrapped and these wrappers are stored in the simulation. This is
    /// similar to "define and then run" (tensorflow). However, during execution
    /// the user should be able to add extra agents, terraingens or sensors
    /// via direct addition of wrapped objects using this class "add" methods.
    ///
    /// Backend-specific simulations should extend this class, and implement
    /// the necessary code in the following virtual methods:
    class TISimulation
    {
    public :

        TISimulation( TScenario* scenarioRef );

        virtual ~TISimulation();

        void Initialize();

        void Step();

        void Reset();

        void Pause();

        void Resume();

        void SetVisualizer( TIVisualizer* visualizerRef );

        bool running() const { return m_running; }

        TScenario* scenario() { return m_scenarioRef; }

        const TScenario* scenario() const { return m_scenarioRef; }

        TIVisualizer* visualizer() { return m_visualizerRef; }

        const TIVisualizer* visualizer() const { return m_visualizerRef; }

        std::string backendId() const { return m_backendId; }

    protected :

        // Takes a pre-step, a.k.a. setup prior to a simulation step in the specific backend
        void _PreStep();

        // Takes a post-step, a.k.a. state-gather after taking a simulation step in the specific backend
        void _PostStep();

        virtual bool _InitializeInternal() = 0;

        virtual void _PreStepInternal() = 0;

        virtual void _SimStepInternal() = 0;

        virtual void _PostStepInternal() = 0;

        virtual void _ResetInternal() = 0;

        virtual void _SetVisualizerInternal( TIVisualizer* visualizerRef ) {};

    protected :

        // Identifier of the backend being used (mujoco|bullet|raisim|dart|...)
        std::string m_backendId;

        // A reference to the scenario being instantiated (we don't take ownership)
        TScenario* m_scenarioRef;

        // A reference to the visualizer (if available) (we don't take ownership)
        TIVisualizer* m_visualizerRef;

        // Adapters for single-body API
        std::vector< std::unique_ptr<TISingleBodyAdapter> > m_singleBodyAdapters;
        std::vector< std::unique_ptr<TISingleBodyColliderAdapter> > m_collisionAdapters;

        // Whether or not the simulation is running
        bool m_running;
    };

    typedef TISimulation* FcnCreateSim( TScenario* scenarioRef );

    /// @brielf Dummy simulation object to use as replacement when no backend available
    ///
    /// @details
    /// This class implements just a dummy simulation that can be drop in if no backend is
    /// available, but still want to try out other features like the visualizer, parsers, etc.
    class TNullSimulation : public TISimulation
    {
    public:

        TNullSimulation( TScenario* scenarioRef );

        ~TNullSimulation();

    protected :

        bool _InitializeInternal() override;

        void _PreStepInternal() override;

        void _SimStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;
    };
}
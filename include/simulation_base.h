
#pragma once

#include <scenario.h>
#include <agent_wrapper.h>
#include <terrain_wrapper.h>


namespace tysoc {

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

        std::string m_runtimeType;
        TScenario*  m_scenarioPtr;

        std::vector< TKinTreeAgentWrapper* > m_agentWrappers;
        std::vector< TTerrainGenWrapper* > m_terrainGenWrappers;

        virtual bool _initializeInternal() = 0;
        virtual void _preStepInternal() = 0;
        virtual void _simStepInternal() = 0;
        virtual void _postStepInternal() = 0;
        virtual void _resetInternal() = 0;

        public :

        TISimulation( TScenario* scenarioPtr );
        virtual ~TISimulation();

        bool initialize();
        void step();
        void reset();

        TScenario* scenario();
        std::string type();

    };


    // Function pointers definitions for loading backend-specific functionality

    typedef TISimulation* FcnCreateSim();

    typedef TKinTreeAgentWrapper* FcnCreateAgentFromAbstract( agent::TAgentKinTree* kinTreeAgentPtr );
    typedef TKinTreeAgentWrapper* FcnCreateAgentFromFile( const std::string& name,
                                                          const std::string& filename );
    typedef TKinTreeAgentWrapper* FcnCreateAgentFromId( const std::string& name,
                                                        const std::string& format,
                                                        const std::string& id );

    typedef TTerrainGenWrapper* FcnCreateTerrainGenFromAbstract( terrain::TITerrainGenerator* terrainGenPtr );
    typedef TTerrainGenWrapper* FcnCreateTerrainGenFromParams( const std::string& name,
                                                               const TGenericParams& params );
}
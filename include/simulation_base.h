
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
        std::string m_workingDir;

        std::vector< TKinTreeAgentWrapper* > m_agentWrappers;
        std::vector< TTerrainGenWrapper* > m_terrainGenWrappers;

        virtual bool _initializeInternal() = 0;
        virtual void _preStepInternal() = 0;
        virtual void _simStepInternal() = 0;
        virtual void _postStepInternal() = 0;
        virtual void _resetInternal() = 0;
        virtual std::map< std::string, std::vector<TScalar> > _getVectorizedInfoInternal() = 0;

        virtual void* _constructPayloadInternal( const std::string& type );

        public :

        TISimulation( TScenario* scenarioPtr,
                      const std::string& workingDir );
        virtual ~TISimulation();

        bool initialize();
        void step();
        void reset();

        void* payload( const std::string& type );
        TScenario* scenario();
        std::string type();

        /*
        *   Returns a dictionary with vectorized information of the simulation.
        *   This can be used to feed directly to a neural network, like some ...
        *   packages do (mujoco-py retrieves the whole qpos[2:], qvel[:], and ...
        *   external forces into an observation consisting of a single np array)
        *
        *
        *   
        */
        std::map< std::string, std::vector<TScalar> > getVectorizedInfo();

    };


    // Function pointers definitions for loading backend-specific functionality

    typedef TISimulation* FcnCreateSim( TScenario* scenarioPtr, const std::string& workingDir );

    typedef TKinTreeAgentWrapper* FcnCreateAgentFromAbstract( agent::TAgentKinTree* kinTreeAgentPtr,
                                                              const std::string& workingDir );
    typedef TKinTreeAgentWrapper* FcnCreateAgentFromFile( const std::string& name,
                                                          const std::string& filename,
                                                          const std::string& workingDir );
    typedef TKinTreeAgentWrapper* FcnCreateAgentFromId( const std::string& name,
                                                        const std::string& format,
                                                        const std::string& id,
                                                        const std::string& workingDir );

    typedef TTerrainGenWrapper* FcnCreateTerrainGenFromAbstract( terrain::TITerrainGenerator* terrainGenPtr,
                                                                 const std::string& workingDir );
    typedef TTerrainGenWrapper* FcnCreateTerrainGenFromParams( const std::string& name,
                                                               const TGenericParams& params,
                                                               const std::string& workingDir );
}

#pragma once

#include <scenario.h>
#include <agent_wrapper.h>
#include <terrain_wrapper.h>


namespace tysoc {

    class TISimulation
    {

        protected :

        std::string m_runtimeType;
        TScenario*  m_scenarioPtr;

        std::vector< TKinTreeAgentWrapper* > m_agentWrappers;
        std::vector< TTerrainGenWrapper* > m_terrainGenWrappers;

        virtual void _initializeInternal() = 0;
        virtual void _updateInternal() = 0;
        virtual void _resetInternal() = 0;

        public :

        TISimulation();
        virtual ~TISimulation();

        void initialize();
        void update();
        void reset();

        TScenario* scenario();
        std::string type();

    };


    // Function pointers definitions for loading backend-specific functionality

    typedef TISimulation* FcnCreateSim();

    typedef TKinTreeAgentWrapper* FcnCreateAgentFromAbstract( const std::string& name,
                                                              agent::TAgentKinTree* kinTreeAgentPtr );
    typedef TKinTreeAgentWrapper* FcnCreateAgentFromFile( const std::string& name,
                                                          const std::string& filename );
    typedef TKinTreeAgentWrapper* FcnCreateAgentFromId( const std::string& name,
                                                        const std::string& format,
                                                        const std::string& id );

    typedef TTerrainGenWrapper* FcnCreateTerrainGenFromAbstract( const std::string& name,
                                                                 terrain::TITerrainGenerator* terrainGenPtr );
    typedef TTerrainGenWrapper* FcnCreateTerrainGenFromParams( const std::string& name,
                                                               const TGenericParams& params );
}
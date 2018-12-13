
#pragma once


#include <scenario.h>
#include <utils/primitives_spawner.h>

namespace tysoc
{

    // This is the base adapter from which other ...
    // concrete apis will inherit from (mujoco, bullet, etc.)
    class TTysocCommonApi
    {

        protected :

        TScenario* m_scenarioPtr;
        tysocUtils::TPrimitivesSpawner* m_primitivesSpawnerPtr;

        virtual void _preStep() = 0; // collect info "for" specific backend
        virtual void _updateStep() = 0; // simulate with specific backend
        virtual void _postStep() = 0; // collect infor "from" specific backend

        public :

        TTysocCommonApi();
        ~TTysocCommonApi();

        void setScenario( TScenario* scenarioPtr );
        TScenario* getScenario() { return m_scenarioPtr; }

        void initialize();
        void step();
        void setAgentAction( const std::string& agentName, 
                             const std::string& actuatorName,
                             float actionValue );

        sensor::TSensorMeasurement* getSensorMeasurement( const std::string sensorName );
        std::vector< terrain::TTerrainGenerator* > getTerrainGenerators();
        std::map< std::string, agent::TAgent* > getAgents();
        std::vector< agent::TIAgent* > getIAgents();
        tysocUtils::TPrimitivesSpawner* getPrimitivesSpawner();
    };

}
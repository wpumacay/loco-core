
#pragma once


#include <scenario.h>


namespace tysoc
{

    // This is the base adapter from which other ...
    // concrete apis will inherit from (mujoco, bullet, etc.)
    class TTysocCommonApi
    {

        protected :

        tysoc::TScenario* m_scenarioPtr;

        virtual void _preStep() = 0; // collect info "for" specific backend
        virtual void _updateStep() = 0; // simulate with specific backend
        virtual void _postStep() = 0; // collect infor "from" specific backend

        public :

        TTysocCommonApi();
        ~TTysocCommonApi();

        void setScenario( tysoc::TScenario* scenarioPtr );

        void initialize();
        void step();
        void setAgentAction( const std::string& agentName, 
                             const std::string& actuatorName,
                             float actionValue );

        tysocsensor::TSensorMeasurement* getSensorMeasurement( const std::string sensorName );
        std::vector< tysocterrain::TTerrainGenerator* > getTerrainGenerators();
        std::map< std::string, tysocagent::TAgent* > getAgents();
    };

}
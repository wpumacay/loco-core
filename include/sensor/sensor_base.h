
#pragma once

#include <terrain/terrain.h>
#include <agent/agent.h>

namespace tysocsensor
{

    struct TSensorMeasurement
    {
        std::string type;
    };


    class TSensor
    {

        protected :
        
        std::string m_name;

        tysocterrain::TTerrainGenerator* m_terrainGenPtr;
        tysocagent::TAgent* m_agentPtr;

        public :

        TSensor( tysocagent::TAgent* agentPtr,
                 tysocterrain::TTerrainGenerator* terrainGenPtr );
        ~TSensor();

        std::string name();

        /**
        * Should update measurements here, given terraingen and agent
        */
        virtual void update() = 0;

        /**
        * Collects the polymorphic sensor measurement
        */
        virtual TSensorMeasurement* getSensorMeasurement() = 0;
    };

    
}
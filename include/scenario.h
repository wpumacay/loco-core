
#pragma once

#include <terrain/terrain.h>
#include <agent/agent_base.h>
#include <agent/agent.h>
#include <sensor/sensor.h>

namespace tysoc
{

    enum ScenarioState
    {
        IDLE,
        RUNNING,
        FINISHED
    };

    class TScenario
    {
        protected :

        std::map< std::string, agent::TAgent* > m_agents;
        std::map< std::string, sensor::TSensor* > m_sensors;
        std::vector< terrain::TTerrainGenerator* > m_terrainGenerators;

        ScenarioState m_state;

        std::vector< agent::TIAgent* > m_iagents;

        public :

        TScenario();
        ~TScenario();

        ScenarioState state();

        void addAgent( agent::TAgent* agent );
        void addIAgent( agent::TIAgent* agent );
        void addSensor( sensor::TSensor* sensor );
        void addTerrainGenerator( terrain::TTerrainGenerator* terrainGenerator );

        agent::TAgent* getAgent( const std::string& name );
        sensor::TSensor* getSensor( const std::string& name );

        std::map< std::string, agent::TAgent* > getAgents();
        std::map< std::string, sensor::TSensor* > getSensors();
        std::vector< terrain::TTerrainGenerator* > getTerrainGenerators();

        std::vector< agent::TIAgent* > getIAgents();

        virtual void initialize();
        virtual void update();
    };

}
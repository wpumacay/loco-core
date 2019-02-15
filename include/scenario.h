
#pragma once

#include <agent/agent.h>
#include <terrain/terrain.h>
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

        ScenarioState m_state;

        std::vector< agent::TIAgent* >                              m_agents;
        std::map< std::string, agent::TIAgent* >                    m_mapAgentsByName;
        std::map< std::string, std::vector< agent::TIAgent* > >     m_mapAgentsByType;

        std::vector< sensor::TISensor* >                             m_sensors;
        std::map< std::string, sensor::TISensor* >                   m_mapSensorsByName;
        std::map< std::string, std::vector< sensor::TISensor* > >    m_mapSensorsByType;

        std::vector< terrain::TITerrainGenerator* >                          m_terrainGenerators;
        std::map< std::string, terrain::TITerrainGenerator* >                m_mapTerrainGeneratorsByName;
        std::map< std::string, std::vector< terrain::TITerrainGenerator* > > m_mapTerrainGeneratorsByType;

        public :

        TScenario();
        ~TScenario();

        ScenarioState state();

        void addAgent( agent::TIAgent* agent );
        void addSensor( sensor::TISensor* sensor );
        void addTerrainGenerator( terrain::TITerrainGenerator* terrainGenerator );

        std::vector< agent::TIAgent* >                  getAgents();
        std::vector< sensor::TISensor* >                getSensors();
        std::vector< terrain::TITerrainGenerator* >     getTerrainGenerators();

        agent::TIAgent*                 getAgentByName( const std::string& name );
        sensor::TISensor*               getSensorByName( const std::string& name );
        terrain::TITerrainGenerator*    getTerrainGeneratorByName( const std::string& name );

        std::vector< agent::TIAgent* >                  getAgentsByType( const std::string& type );
        std::vector< sensor::TISensor* >                getSensorsByType( const std::string& type );
        std::vector< terrain::TITerrainGenerator* >     getTerrainGeneratorsByType( const std::string& type );

        bool hasAgent( const std::string& agentName );
        bool hasTerrainGen( const std::string& terrainGenName );
        bool hasSensor( const std::string& sensorName );

        virtual void initialize();
        virtual void update();
    };

}
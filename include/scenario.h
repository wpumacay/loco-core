
#pragma once

#include <agent/agent.h>
#include <terrain/terrain.h>
#include <sensor/sensor.h>

#include <components/body.h>

namespace tysoc
{
    /**
    *   States the scenario can be in:
    *    - IDLE     : scenario is active, but not currently being ...
    *                 simulated by the physics backend.
    *    - RUNNING  : scenario is active and being simulated by ...
    *                 a physics backend.
    *    - FINISHED : scenario is inactive and awaiting to be reused|destroyed.
    */
    enum ScenarioState
    {
        IDLE,
        RUNNING,
        FINISHED
    };

    /**
    *   A holder containing all elements to be simulated|visualized, like
    *   agents, terrain generators and sensors. It's the access point presented
    *   to the user to interact with these components of the simulation.
    */
    class TScenario
    {

    public :

        /**
        *   Creates an empty scenario (no agents, no terrain-generators, no sensors).
        */
        TScenario();
        ~TScenario();

        /**
        *   Returns the state of this scenario
        */
        ScenarioState state();

        void addBody( TBody* bodyPtr );
        void addAgent( TAgent* agent );
        void addSensor( TISensor* sensor );
        void addTerrainGenerator( TITerrainGenerator* terrainGenerator );

        std::vector< TBody* > getBodies();
        std::vector< TAgent* > getAgents();
        std::vector< TISensor* > getSensors();
        std::vector< TITerrainGenerator* > getTerrainGenerators();

        TBody* getBodyByName( const std::string& name );
        TAgent* getAgentByName( const std::string& name );
        TISensor* getSensorByName( const std::string& name );
        TITerrainGenerator* getTerrainGeneratorByName( const std::string& name );

        std::vector< TBody* > getBodiesByType( const eDynamicsType& type );
        std::vector< TAgent* > getAgentsByType( const eAgentType& type );
        std::vector< TISensor* > getSensorsByType( const eSensorType& type );
        std::vector< TITerrainGenerator* > getTerrainGeneratorsByType( const std::string& type );

        bool hasBody( const std::string& bodyName );
        bool hasAgent( const std::string& agentName );
        bool hasTerrainGen( const std::string& terrainGenName );
        bool hasSensor( const std::string& sensorName );

        virtual void initialize();
        virtual void update();

    protected :

        ScenarioState m_state;

        std::vector< TBody* > m_bodies;
        std::map< std::string, TBody* > m_mapBodiesByName;
        std::map< eDynamicsType, std::vector< TBody* > > m_mapBodiesByType;

        std::vector< TAgent* > m_agents;
        std::map< std::string, TAgent* > m_mapAgentsByName;
        std::map< eAgentType, std::vector< TAgent* > > m_mapAgentsByType;

        std::vector< TISensor* > m_sensors;
        std::map< std::string, TISensor* > m_mapSensorsByName;
        std::map< eSensorType, std::vector< TISensor* > > m_mapSensorsByType;

        std::vector< TITerrainGenerator* > m_terrainGenerators;
        std::map< std::string, TITerrainGenerator* > m_mapTerrainGeneratorsByName;
        std::map< std::string, std::vector< TITerrainGenerator* > > m_mapTerrainGeneratorsByType;
    };

}
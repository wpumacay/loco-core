
#pragma once

#include <agent/agent.h>
#include <terrain/terrain.h>
#include <sensor/sensor.h>

#include <components/bodies.h>

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
        void addAgent( agent::TAgent* agent );
        void addSensor( sensor::TISensor* sensor );
        void addTerrainGenerator( terrain::TITerrainGenerator* terrainGenerator );

        std::vector< TBody* >                       getBodies();
        std::vector< agent::TAgent* >               getAgents();
        std::vector< sensor::TISensor* >            getSensors();
        std::vector< terrain::TITerrainGenerator* > getTerrainGenerators();

        TBody*                          getBodyByName( const std::string& name );
        agent::TAgent*                  getAgentByName( const std::string& name );
        sensor::TISensor*               getSensorByName( const std::string& name );
        terrain::TITerrainGenerator*    getTerrainGeneratorByName( const std::string& name );

        std::vector< TBody* >                           getBodiesByType( const eDynamicsType& type );
        std::vector< agent::TAgent* >                   getAgentsByType( const agent::eAgentType& type );
        std::vector< sensor::TISensor* >                getSensorsByType( const eSensorType& type );
        std::vector< terrain::TITerrainGenerator* >     getTerrainGeneratorsByType( const std::string& type );

        bool hasBody( const std::string& bodyName );
        bool hasAgent( const std::string& agentName );
        bool hasTerrainGen( const std::string& terrainGenName );
        bool hasSensor( const std::string& sensorName );

        virtual void initialize();
        virtual void update();

    protected :

        ScenarioState m_state;

        std::vector< TBody* >                               m_bodies;
        std::map< std::string, TBody* >                     m_mapBodiesByName;
        std::map< eDynamicsType, std::vector< TBody* > >    m_mapBodiesByType;

        std::vector< agent::TAgent* >                               m_agents;
        std::map< std::string, agent::TAgent* >                     m_mapAgentsByName;
        std::map< agent::eAgentType, std::vector< agent::TAgent* > >       m_mapAgentsByType;

        std::vector< sensor::TISensor* >                            m_sensors;
        std::map< std::string, sensor::TISensor* >                  m_mapSensorsByName;
        std::map< eSensorType, std::vector< sensor::TISensor* > >   m_mapSensorsByType;

        std::vector< terrain::TITerrainGenerator* >                             m_terrainGenerators;
        std::map< std::string, terrain::TITerrainGenerator* >                   m_mapTerrainGeneratorsByName;
        std::map< std::string, std::vector< terrain::TITerrainGenerator* > >    m_mapTerrainGeneratorsByType;
    };

}
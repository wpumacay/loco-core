
#pragma once

#include <agent/agent.h>
#include <terrain/terrain.h>
#include <sensor/sensor.h>

#include <components/body.h>

namespace tysoc
{
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

        void addSingleBody( TSingleBody* bodyRef );
        void addCompoundBody( TCompoundBody* rootCompoundRef );
        void addAgent( TAgent* agent );
        void addSensor( TISensor* sensor );
        void addTerrainGenerator( TITerrainGenerator* terrainGenerator );

        std::vector< TSingleBody* > getSingleBodies();
        std::vector< TCompoundBody* > getCompoundBodies();
        std::vector< TAgent* > getAgents();
        std::vector< TISensor* > getSensors();
        std::vector< TITerrainGenerator* > getTerrainGenerators();

        TSingleBody* getSingleBodyByName( const std::string& name );
        TCompoundBody* getCompoundBodyByName( const std::string& name );
        TAgent* getAgentByName( const std::string& name );
        TISensor* getSensorByName( const std::string& name );
        TITerrainGenerator* getTerrainGeneratorByName( const std::string& name );

        bool hasSingleBody( const std::string& bodyName );
        bool hasCompoundBody( const std::string& compoundName );
        bool hasAgent( const std::string& agentName );
        bool hasTerrainGen( const std::string& terrainGenName );
        bool hasSensor( const std::string& sensorName );

        virtual void initialize();
        virtual void update();

    protected :

        ScenarioState m_state;

        std::vector< TSingleBody* > m_singleBodies;
        std::map< std::string, TSingleBody* > m_mapSingleBodiesByName;

        std::vector< TCompoundBody* > m_compoundBodies;
        std::map< std::string, TCompoundBody* > m_mapCompoundBodiesByName;

        std::vector< TAgent* > m_agents;
        std::map< std::string, TAgent* > m_mapAgentsByName;

        std::vector< TISensor* > m_sensors;
        std::map< std::string, TISensor* > m_mapSensorsByName;

        std::vector< TITerrainGenerator* > m_terrainGenerators;
        std::map< std::string, TITerrainGenerator* > m_mapTerrainGeneratorsByName;
    };

}
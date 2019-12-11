
#pragma once

#include <agent/agent.h>
#include <terrain/terrain.h>
#include <sensor/sensor.h>

#include <components/body.h>
#include <primitives/single_body.h>
#include <compounds/compound_body.h>

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

        TScenario();
        ~TScenario();

        void initialize();
        void update();

        void addSingleBody( TSingleBody* body );
        void addCompoundBody( TCompoundBody* rootCompound );
        void addAgent( TAgent* agent );
        void addSensor( TISensor* sensor );
        void addTerrainGenerator( TITerrainGenerator* terrainGenerator );

        std::vector< TSingleBody* > getSingleBodies() const { return m_singleBodies; }
        std::vector< TCompoundBody* > getCompoundBodies() const { return m_compoundBodies; }
        std::vector< TAgent* > getAgents() const { return m_agents; }
        std::vector< TISensor* > getSensors() const { return m_sensors; }
        std::vector< TITerrainGenerator* > getTerrainGenerators() const { return m_terrainGenerators; }

        TSingleBody* getSingleBodyByName( const std::string& name );
        TCompoundBody* getCompoundBodyByName( const std::string& name );
        TAgent* getAgentByName( const std::string& name );
        TISensor* getSensorByName( const std::string& name );
        TITerrainGenerator* getTerrainGeneratorByName( const std::string& name );

        bool hasSingleBody( const std::string& singleBodyName );
        bool hasCompoundBody( const std::string& compoundBodyName );
        bool hasAgent( const std::string& agentName );
        bool hasTerrainGen( const std::string& terrainGenName );
        bool hasSensor( const std::string& sensorName );

    protected :

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
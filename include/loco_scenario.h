#pragma once

#include <primitives/loco_single_body.h>
//// #include <compounds/compound.h>
//// #include <agent/agent_kintree.h>
//// #include <sensor/sensor_kintree.h>
//// #include <terrain/terrain_generator.h>

namespace loco
{
    class TScenario
    {
    public :

        TScenario();
        ~TScenario();

        void Initialize();
        void PreStep();
        void PostStep();
        void Reset();

        TSingleBody* AddSingleBody( std::unique_ptr< TSingleBody > body );
        //// TCompound* AddCompound( std::unique_ptr< TCompound > compound );
        //// TKinTreeAgent* AddKinTreeAgent( std::unique_ptr< TKinTreeAgent > kinTreeAgent );
        //// TISensor* AddKinTreeSensor( std::unique_ptr< TISensor > kinTreeSensor );
        //// TITerrainGenerator* AddTerrainGenerator( std::unique_ptr< TITerrainGenerator > terrainGenerator );

        const TSingleBody* GetSingleBodyByName( const std::string& name ) const;
        //// const TCompound* GetCompoundBodyByName( const std::string& name ) const;
        //// const TKinTreeAgent* GetKinTreeAgentByName( const std::string& name ) const;
        //// const TKinTreeSensor* GetKinTreeSensorByName( const std::string& name ) const;
        //// const TTerrainGenerator* GetTerrainGeneratorByName( const std::string& name ) const;

        TSingleBody* GetSingleBodyByName( const std::string& name );
        //// TCompound* GetCompoundBodyByName( const std::string& name );
        //// TKinTreeAgent* GetKinTreeAgentByName( const std::string& name );
        //// TKinTreeSensor* GetKinTreeSensorByName( const std::string& name );
        //// TTerrainGenerator* GetTerrainGeneratorByName( const std::string& name );

        size_t GetNumSingleBodies() const;
        //// size_t GetNumCompounds() const;
        //// size_t GetNumKinTreeAgents() const;
        //// size_t GetNumKinTreeSensors() const;
        //// size_t GetNumTerrainGenerators() const;

        bool HasSingleBodyNamed( const std::string& name ) const;
        //// bool HasCompoundNamed( const std::string& name) const;
        //// bool HasKinTreeAgentNamed( const std::string& name ) const;
        //// bool HasKinTreeSensorNamed( const std::string& name ) const;
        //// bool HasTerrainGeneratorNamed( const std::string& name ) const;

        std::vector<const TSingleBody*> GetSingleBodiesList() const;
        //// std::vector<const TCompound*> GetCompoundsList() const;
        //// std::vector<const TKinTreeAgent*> GetKinTreeAgentsList() const;
        //// std::vector<const TKinTreeSensor*> GetKinTreeSensorsList() const;
        //// std::vector<const TTerrainGenerator*> GetTerrainGeneratorsList() const;

        std::vector<TSingleBody*> GetSingleBodiesList();
        //// std::vector<TCompound*> GetCompoundsList();
        //// std::vector<TKinTreeAgent*> GetKinTreeAgentsList();
        //// std::vector<TKinTreeSensor*> GetKinTreeSensorsList();
        //// std::vector<TTerrainGenerator*> GetTerrainGeneratorsList();

    private :

        const TSingleBody* _get_singleBody( ssize_t index ) const;
        //// const TCompound* _get_compound( ssize_t index ) const;
        //// const TKinTreeAgent* _get_kintreeAgent( ssize_t index ) const;
        //// const TKinTreeSensor* _get_kintreeSensor( ssize_t index ) const;
        //// const TTerrainGenerator* _get_terrainGenerator( ssize_t index ) const;

        TSingleBody* _get_mutable_singleBody( ssize_t index );
        //// TCompound* _get_mutable_compound( ssize_t index );
        //// TKinTreeAgent* _get_mutable_kintreeAgent( ssize_t index );
        //// TKinTreeSensor* _get_mutable_kintreeSensor( ssize_t index );
        //// TTerrainGenerator* _get_mutable_terrainGenerator( ssize_t index );

    private :

        // Containers of owned elements (passed by the user)
        std::vector< std::unique_ptr< TSingleBody > > m_singleBodies;
        //// std::vector< std::unique_ptr< TCompound > > m_compounds;
        //// std::vector< std::unique_ptr< TKinTreeAgent > > m_kinTreeAgents;
        //// std::vector< std::unique_ptr< TKinTreeSensor > > m_kinTreeSensors;
        //// std::vector< std::unique_ptr< TTerrainGenerator > > m_terrainGenerators;

        // Maps from components' names to their cooresponding indices in their std-vector containers
        std::map< std::string, ssize_t > m_singleBodiesMap;
        //// std::map< std::string, TCompound* > m_compoundsMap;
        //// std::map< std::string, TKinTreeAgent* > m_kinTreeAgentsMap;
        //// std::map< std::string, TKinTreeSensor* > m_kinTreeSensorsMap;
        //// std::map< std::string, TTerrainGenerator* > m_terrainGeneratorsMap;
    };
}
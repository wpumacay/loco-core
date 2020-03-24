#pragma once

#include <visualizer/loco_visualizer_drawable.h>
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
        void DetachSim();
        void DetachViz();

        TDrawable* AddDrawable( std::unique_ptr< TDrawable > drawable );
        TSingleBody* AddSingleBody( std::unique_ptr< TSingleBody > body );
        //// TCompound* AddCompound( std::unique_ptr< TCompound > compound );
        //// TKinTreeAgent* AddKinTreeAgent( std::unique_ptr< TKinTreeAgent > kinTreeAgent );
        //// TISensor* AddKinTreeSensor( std::unique_ptr< TISensor > kinTreeSensor );
        //// TITerrainGenerator* AddTerrainGenerator( std::unique_ptr< TITerrainGenerator > terrainGenerator );

        TDrawable* GetDrawableByName( const std::string& name );
        TSingleBody* GetSingleBodyByName( const std::string& name );
        //// TCompound* GetCompoundBodyByName( const std::string& name );
        //// TKinTreeAgent* GetKinTreeAgentByName( const std::string& name );
        //// TKinTreeSensor* GetKinTreeSensorByName( const std::string& name );
        //// TTerrainGenerator* GetTerrainGeneratorByName( const std::string& name );

        const TDrawable* GetDrawableByName( const std::string& name ) const;
        const TSingleBody* GetSingleBodyByName( const std::string& name ) const;
        //// const TCompound* GetCompoundBodyByName( const std::string& name ) const;
        //// const TKinTreeAgent* GetKinTreeAgentByName( const std::string& name ) const;
        //// const TKinTreeSensor* GetKinTreeSensorByName( const std::string& name ) const;
        //// const TTerrainGenerator* GetTerrainGeneratorByName( const std::string& name ) const;

        void RemoveDrawableByName( const std::string& name );
        void RemoveSingleBodyByName( const std::string& name );
        //// void RemoveCompoundByName( const std::string& name );
        //// void RemoveKinematicTreeByName( const std::string& name );
        //// void RemoveKinematicTreeSensorByName( const std::string& name );
        //// void RemoveTerrainGeneratorByName( const std::string& name );

        size_t GetNumDrawables() const;
        size_t GetNumSingleBodies() const;
        //// size_t GetNumCompounds() const;
        //// size_t GetNumKinTreeAgents() const;
        //// size_t GetNumKinTreeSensors() const;
        //// size_t GetNumTerrainGenerators() const;

        bool HasDrawableNamed( const std::string& name ) const;
        bool HasSingleBodyNamed( const std::string& name ) const;
        //// bool HasCompoundNamed( const std::string& name) const;
        //// bool HasKinTreeAgentNamed( const std::string& name ) const;
        //// bool HasKinTreeSensorNamed( const std::string& name ) const;
        //// bool HasTerrainGeneratorNamed( const std::string& name ) const;

        std::vector<TDrawable*> GetDrawablesList();
        std::vector<TSingleBody*> GetSingleBodiesList();
        //// std::vector<TCompound*> GetCompoundsList();
        //// std::vector<TKinTreeAgent*> GetKinTreeAgentsList();
        //// std::vector<TKinTreeSensor*> GetKinTreeSensorsList();
        //// std::vector<TTerrainGenerator*> GetTerrainGeneratorsList();

        std::vector<const TDrawable*> GetDrawablesList() const;
        std::vector<const TSingleBody*> GetSingleBodiesList() const;
        //// std::vector<const TCompound*> GetCompoundsList() const;
        //// std::vector<const TKinTreeAgent*> GetKinTreeAgentsList() const;
        //// std::vector<const TKinTreeSensor*> GetKinTreeSensorsList() const;
        //// std::vector<const TTerrainGenerator*> GetTerrainGeneratorsList() const;

        void remove_drawable( ssize_t index );
        void remove_single_body( ssize_t index );
        //// void remove_compound( ssize_t index );
        //// void remove_kinematic_tree( ssize_t index );
        //// void remove_kinematic_tree_sensor( ssize_t index );
        //// void remove_terrain_generator( ssize_t index );

        TDrawable* get_mutable_drawable( ssize_t index );
        TSingleBody* get_mutable_singleBody( ssize_t index );
        //// TCompound* get_mutable_compound( ssize_t index );
        //// TKinTreeAgent* get_mutable_kintreeAgent( ssize_t index );
        //// TKinTreeSensor* get_mutable_kintreeSensor( ssize_t index );
        //// TTerrainGenerator* get_mutable_terrainGenerator( ssize_t index );

        const TDrawable* get_drawable( ssize_t index ) const;
        const TSingleBody* get_singleBody( ssize_t index ) const;
        //// const TCompound* get_compound( ssize_t index ) const;
        //// const TKinTreeAgent* get_kintreeAgent( ssize_t index ) const;
        //// const TKinTreeSensor* get_kintreeSensor( ssize_t index ) const;
        //// const TTerrainGenerator* get_terrainGenerator( ssize_t index ) const;

    private :

        std::vector< std::unique_ptr< TDrawable > > m_Drawables;
        std::vector< std::unique_ptr< TSingleBody > > m_SingleBodies;
        //// std::vector< std::unique_ptr< TCompound > > m_Compounds;
        //// std::vector< std::unique_ptr< TKinTreeAgent > > m_KinematicTrees;
        //// std::vector< std::unique_ptr< TKinTreeSensor > > m_KinematicTreeSensors;
        //// std::vector< std::unique_ptr< TTerrainGenerator > > m_TerrainGenerators;

        std::map< std::string, ssize_t > m_DrawablesMap;
        std::map< std::string, ssize_t > m_SingleBodiesMap;
        //// std::map< std::string, TCompound* > m_CompoundsMap;
        //// std::map< std::string, TKinTreeAgent* > m_KinematicTreesMap;
        //// std::map< std::string, TKinTreeSensor* > m_KinematicTreeSensorsMap;
        //// std::map< std::string, TTerrainGenerator* > m_TerrainGeneratorsMap;
    };
}
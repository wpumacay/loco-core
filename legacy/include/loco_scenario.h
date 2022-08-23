#pragma once

#include <visualizer/loco_visualizer_drawable.h>
#include <primitives/loco_single_body.h>
#include <kinematic_trees/loco_kinematic_tree.h>
#include <terrain_generators/loco_terrain_generator.h>

namespace loco {
namespace terrain {
    class TITerrainGenerator;
}}

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

        visualizer::TDrawable* AddDrawable( std::unique_ptr<visualizer::TDrawable> drawable );
        primitives::TSingleBody* AddSingleBody( std::unique_ptr<primitives::TSingleBody> body );
        kintree::TKinematicTree* AddKinematicTree( std::unique_ptr<kintree::TKinematicTree> kintree );
        terrain::TITerrainGenerator* AddTerrainGenerator( std::unique_ptr<terrain::TITerrainGenerator> terraingen );

        visualizer::TDrawable* GetDrawableByName( const std::string& name );
        primitives::TSingleBody* GetSingleBodyByName( const std::string& name );
        kintree::TKinematicTree* GetKinematicTreeByName( const std::string& name );
        terrain::TITerrainGenerator* GetTerrainGeneratorByName( const std::string& name );

        const visualizer::TDrawable* GetDrawableByName( const std::string& name ) const;
        const primitives::TSingleBody* GetSingleBodyByName( const std::string& name ) const;
        const kintree::TKinematicTree* GetKinematicTreeByName( const std::string& name ) const;
        const terrain::TITerrainGenerator* GetTerrainGeneratorByName( const std::string& name ) const;

        void RemoveDrawableByName( const std::string& name );
        void RemoveSingleBodyByName( const std::string& name );
        void RemoveKinematicTreeByName( const std::string& name );
        void RemoveTerrainGeneratorByName( const std::string& name );

        size_t GetNumDrawables() const { return m_Drawables.size(); }
        size_t GetNumSingleBodies() const { return m_SingleBodies.size(); }
        size_t GetNumKinematicTrees() const { return m_KinematicTrees.size(); }
        size_t GetNumTerrainGenerators() const { return m_TerrainGenerators.size(); }

        bool HasDrawableNamed( const std::string& name ) const;
        bool HasSingleBodyNamed( const std::string& name ) const;
        bool HasKinematicTreeNamed( const std::string& name ) const;
        bool HasTerrainGeneratorNamed( const std::string& name ) const;

        std::vector<visualizer::TDrawable*> GetDrawablesList();
        std::vector<primitives::TSingleBody*> GetSingleBodiesList();
        std::vector<kintree::TKinematicTree*> GetKinematicTreesList();
        std::vector<terrain::TITerrainGenerator*> GetTerrainGeneratorsList();

        std::vector<const visualizer::TDrawable*> GetDrawablesList() const;
        std::vector<const primitives::TSingleBody*> GetSingleBodiesList() const;
        std::vector<const kintree::TKinematicTree*> GetKinematicTreesList() const;
        std::vector<const terrain::TITerrainGenerator*> GetTerrainGeneratorsList() const;

        void remove_drawable( ssize_t index );
        void remove_single_body( ssize_t index );
        void remove_kinematic_tree( ssize_t index );
        void remove_terrain_generator( ssize_t index );

        visualizer::TDrawable* get_mutable_drawable( ssize_t index );
        primitives::TSingleBody* get_mutable_singleBody( ssize_t index );
        kintree::TKinematicTree* get_mutable_kinematic_tree( ssize_t index );
        terrain::TITerrainGenerator* get_mutable_terrainGenerator( ssize_t index );

        const visualizer::TDrawable* get_drawable( ssize_t index ) const;
        const primitives::TSingleBody* get_single_body( ssize_t index ) const;
        const kintree::TKinematicTree* get_kinematic_tree( ssize_t index ) const;
        const terrain::TITerrainGenerator* get_terrain_generator( ssize_t index ) const;

    private :

        std::vector<std::unique_ptr<visualizer::TDrawable>> m_Drawables;
        std::vector<std::unique_ptr<primitives::TSingleBody>> m_SingleBodies;
        std::vector<std::unique_ptr<kintree::TKinematicTree>> m_KinematicTrees;
        std::vector<std::unique_ptr<terrain::TITerrainGenerator>> m_TerrainGenerators;

        std::unordered_map<std::string, ssize_t> m_DrawablesMap;
        std::unordered_map<std::string, ssize_t> m_SingleBodiesMap;
        std::unordered_map<std::string, ssize_t> m_KinematicTreesMap;
        std::unordered_map<std::string, ssize_t> m_TerrainGeneratorsMap;
    };
}
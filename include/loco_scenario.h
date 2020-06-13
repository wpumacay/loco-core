#pragma once

#include <visualizer/loco_visualizer_drawable.h>
#include <primitives/loco_single_body.h>
#include <kinematic_trees/loco_kinematic_tree.h>
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

        TDrawable* AddDrawable( std::unique_ptr<TDrawable> drawable );
        primitives::TSingleBody* AddSingleBody( std::unique_ptr<primitives::TSingleBody> body );
        kintree::TKinematicTree* AddKinematicTree( std::unique_ptr<kintree::TKinematicTree> kintree );
        //// TITerrainGenerator* AddTerrainGenerator( std::unique_ptr< TITerrainGenerator > terrainGenerator );

        TDrawable* GetDrawableByName( const std::string& name );
        primitives::TSingleBody* GetSingleBodyByName( const std::string& name );
        kintree::TKinematicTree* GetKinematicTreeByName( const std::string& name );
        //// TTerrainGenerator* GetTerrainGeneratorByName( const std::string& name );

        const TDrawable* GetDrawableByName( const std::string& name ) const;
        const primitives::TSingleBody* GetSingleBodyByName( const std::string& name ) const;
        const kintree::TKinematicTree* GetKinematicTreeByName( const std::string& name ) const;
        //// const TTerrainGenerator* GetTerrainGeneratorByName( const std::string& name ) const;

        void RemoveDrawableByName( const std::string& name );
        void RemoveSingleBodyByName( const std::string& name );
        void RemoveKinematicTreeByName( const std::string& name );
        //// void RemoveTerrainGeneratorByName( const std::string& name );

        size_t GetNumDrawables() const;
        size_t GetNumSingleBodies() const;
        size_t GetNumKinematicTrees() const;
        //// size_t GetNumTerrainGenerators() const;

        bool HasDrawableNamed( const std::string& name ) const;
        bool HasSingleBodyNamed( const std::string& name ) const;
        bool HasKinematicTreeNamed( const std::string& name ) const;
        //// bool HasTerrainGeneratorNamed( const std::string& name ) const;

        std::vector<TDrawable*> GetDrawablesList();
        std::vector<primitives::TSingleBody*> GetSingleBodiesList();
        std::vector<kintree::TKinematicTree*> GetKinematicTreesList();
        //// std::vector<TTerrainGenerator*> GetTerrainGeneratorsList();

        std::vector<const TDrawable*> GetDrawablesList() const;
        std::vector<const primitives::TSingleBody*> GetSingleBodiesList() const;
        std::vector<const kintree::TKinematicTree*> GetKinematicTreesList() const;
        //// std::vector<const TTerrainGenerator*> GetTerrainGeneratorsList() const;

        void remove_drawable( ssize_t index );
        void remove_single_body( ssize_t index );
        void remove_kinematic_tree( ssize_t index );
        //// void remove_terrain_generator( ssize_t index );

        TDrawable* get_mutable_drawable( ssize_t index );
        primitives::TSingleBody* get_mutable_singleBody( ssize_t index );
        kintree::TKinematicTree* get_mutable_kinematic_tree( ssize_t index );
        //// TTerrainGenerator* get_mutable_terrainGenerator( ssize_t index );

        const TDrawable* get_drawable( ssize_t index ) const;
        const primitives::TSingleBody* get_singleBody( ssize_t index ) const;
        const kintree::TKinematicTree* get_kinematic_tree( ssize_t index ) const;
        //// const TTerrainGenerator* get_terrainGenerator( ssize_t index ) const;

    private :

        std::vector<std::unique_ptr<TDrawable>> m_Drawables;
        std::vector<std::unique_ptr<primitives::TSingleBody>> m_SingleBodies;
        std::vector<std::unique_ptr<kintree::TKinematicTree>> m_KinematicTrees;
        //// std::vector< std::unique_ptr< TTerrainGenerator > > m_TerrainGenerators;

        std::map<std::string, ssize_t> m_DrawablesMap;
        std::map<std::string, ssize_t> m_SingleBodiesMap;
        std::map<std::string, ssize_t> m_KinematicTreesMap;
        //// std::map< std::string, TTerrainGenerator* > m_TerrainGeneratorsMap;
    };
}
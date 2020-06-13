
#include <loco_scenario.h>

namespace loco
{
    TScenario::TScenario()
    {
    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TScenario @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TScenario @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TScenario::~TScenario()
    {
        m_SingleBodies.clear();
        m_KinematicTrees.clear();
        //// m_TerrainGenerators.clear();

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TScenario @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TScenario @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TScenario::Initialize()
    {
        for ( auto& drawable : m_Drawables )
            drawable->Initialize();

        for ( auto& single_body : m_SingleBodies )
            single_body->Initialize();

        for ( auto& kinematic_tree : m_KinematicTrees )
            kinematic_tree->Initialize();

////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->Initialize();
    }

    void TScenario::PreStep()
    {
        for ( auto& drawable : m_Drawables )
            drawable->PreStep();

        for ( auto& single_body : m_SingleBodies )
            single_body->PreStep();

        for ( auto& kinematic_tree : m_KinematicTrees )
            kinematic_tree->PreStep();

////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->PreStep();
    }

    void TScenario::PostStep()
    {
        for ( auto& drawable : m_Drawables )
            drawable->PostStep();

        for ( auto& single_body : m_SingleBodies )
            single_body->PostStep();

        for ( auto& kinematic_tree : m_KinematicTrees )
            kinematic_tree->PostStep();

////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->PostStep();
}

    void TScenario::Reset()
    {
        for ( auto& drawable : m_Drawables )
            drawable->Reset();

        for ( auto& single_body : m_SingleBodies )
            single_body->Reset();

        for ( auto& kinematic_tree : m_KinematicTrees )
            kinematic_tree->Reset();

////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->Reset();
    }

    void TScenario::DetachSim()
    {
        for ( auto& drawable : m_Drawables )
            drawable->DetachSim();

        for ( auto& single_body : m_SingleBodies )
            single_body->DetachSim();

        for ( auto& kinematic_tree : m_KinematicTrees )
            kinematic_tree->DetachSim();

////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->DetachSim();
    }

    void TScenario::DetachViz()
    {
        for ( auto& drawable : m_Drawables )
            drawable->DetachViz();

        for ( auto& single_body : m_SingleBodies )
            single_body->DetachViz();

        for ( auto& kinematic_tree : m_KinematicTrees )
            kinematic_tree->DetachViz();

////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->DetachViz();
    }

    TDrawable* TScenario::AddDrawable( std::unique_ptr<TDrawable> drawable )
    {
        LOCO_CORE_ASSERT( drawable, "TScenario::AddDrawable >>> tried adding nullptr" );

        if ( HasDrawableNamed( drawable->name() ) )
        {
            LOCO_CORE_WARN( "TScenario::AddDrawable >>> drawable with name {0} already exists. \
                             New one won't be added, returning nullptr instead.", drawable->name() );
            return nullptr;
        }

        m_DrawablesMap[drawable->name()] = m_Drawables.size();
        m_Drawables.push_back( std::move( drawable ) );
        return m_Drawables.back().get();
    }

    primitives::TSingleBody* TScenario::AddSingleBody( std::unique_ptr<primitives::TSingleBody> single_body )
    {
        LOCO_CORE_ASSERT( single_body, "TScenario::AddSingleBody >>> tried adding nullptr" );

        if ( HasSingleBodyNamed( single_body->name() ) )
        {
            LOCO_CORE_WARN( "TScenario::AddSingleBody >>> single-body with name {0} already exists. \
                             New one won't be added, returning nullptr instead.", single_body->name() );
            return nullptr;
        }

        m_SingleBodiesMap[single_body->name()] = m_SingleBodies.size();
        m_SingleBodies.push_back( std::move( single_body ) );
        return m_SingleBodies.back().get();
    }

     kintree::TKinematicTree* TScenario::AddKinematicTree( std::unique_ptr<kintree::TKinematicTree> kinematic_tree )
     {
         LOCO_CORE_ASSERT( kinematic_tree, "TScenario::AddKinematicTree >>> tried adding nullptr" );
 
         if ( HasKinematicTreeNamed( kinematic_tree->name() ) )
         {
             LOCO_CORE_WARN( "TScenario::AddKinematicTree >>> kinematic_tree with name {0} already exists. \
                              New one won't be added, returning nullptr instead.", kinematic_tree->name() );
             return nullptr;
         }
 
         m_KinematicTreesMap[kinematic_tree->name()] = m_KinematicTrees.size();
         m_KinematicTrees.push_back( std::move( kinematic_tree ) );
         return m_KinematicTrees.back().get();
     }

////     TTerrainGenerator* TScenario::AddTerrainGenerator( std::unique_ptr< TTerrainGenerator > terrainGenerator )
////     {
////         LOCO_CORE_ASSERT( terrainGenerator, "TScenario::AddTerrainGenerator >>> tried adding nullptr" );
//// 
////         if ( HasTerrainGeneratorNamed( terrainGenerator->name() ) )
////         {
////             LOCO_CORE_WARN( "TScenario::AddTerrainGenerator >>> terrainGenerator with name {0} already exists. \
////                              New one won't be added, returning nullptr instead.", terrainGenerator->name() );
////             return nullptr;
////         }
//// 
////         m_TerrainGeneratorsMap[terrainGenerator->name()] = m_TerrainGenerators.size();
////         m_TerrainGenerators.push_back( std::move( terrainGenerator ) );
////         return m_TerrainGenerators.back().get();
////     }

    const TDrawable* TScenario::GetDrawableByName( const std::string& name ) const
    {
        if ( !HasDrawableNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::GetDrawableByName >>> drawable with name {0} not found in the scenario", name );
            return nullptr;
        }

        return get_drawable( m_DrawablesMap.at( name ) );
    }

    const primitives::TSingleBody* TScenario::GetSingleBodyByName( const std::string& name ) const
    {
        if( !HasSingleBodyNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::GetSingleBodyByName >>> single-body with name {0} not found in the scenario", name );
            return nullptr;
        }

        return get_singleBody( m_SingleBodiesMap.at( name ) );
    }

    const kintree::TKinematicTree* TScenario::GetKinematicTreeByName( const std::string& name ) const
    {
        if( !HasKinematicTreeNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::GetKinematicTreeByName >>> kintree with name {0} not found", name );
            return nullptr;
        }

        return get_kinematic_tree( m_KinematicTreesMap.at( name ) );
    }

////     const TTerrainGenerator* TScenario::GetTerrainGeneratorByName( const std::string& name ) const
////     {
////         if( !HasTerrainGeneratorNamed( name ) )
////         {
////             LOCO_CORE_ERROR( "TScenario::GetTerrainGeneratorByName >>> terrainGenerator with name {0} not found", name );
////             return nullptr;
////         }
//// 
////         return get_terrainGenerator( m_TerrainGeneratorsMap.at( name ) );
////     }

    TDrawable* TScenario::GetDrawableByName( const std::string& name )
    {
        if( !HasSingleBodyNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::GetSingleBodyByName >>> single-body with name {0} not found in the scenario", name );
            return nullptr;
        }

        return get_mutable_drawable( m_DrawablesMap.at( name ) );
    }

    primitives::TSingleBody* TScenario::GetSingleBodyByName( const std::string& name )
    {
        if( !HasSingleBodyNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::GetSingleBodyByName >>> single-body with name {0} not found", name );
            return nullptr;
        }

        return get_mutable_singleBody( m_SingleBodiesMap.at( name ) );
    }

    kintree::TKinematicTree* TScenario::GetKinematicTreeByName( const std::string& name )
    {
        if( !HasKinematicTreeNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::GetKinematicTreeByName >>> kintree with name {0} not found", name );
            return nullptr;
        }

        return get_mutable_kinematic_tree( m_KinematicTreesMap.at( name ) );
    }

////     TTerrainGenerator* TScenario::GetTerrainGeneratorByName( const std::string& name )
////     {
////         if( !HasTerrainGeneratorNamed( name ) )
////         {
////             LOCO_CORE_ERROR( "TScenario::GetTerrainGeneratorByName >>> terrainGenerator with name {0} not found", name );
////             return nullptr;
////         }
//// 
////         return get_mutable_terrainGenerator( m_TerrainGeneratorsMap.at( name ) );
////     }

    void TScenario::RemoveDrawableByName( const std::string& name )
    {
        if ( !HasDrawableNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::RemoveDrawableByName >>> tried removing unexistent drawable \"{0}\"", name );
            return;
        }
        const ssize_t idx_drawable = m_DrawablesMap.at( name );
        remove_drawable( idx_drawable );
        for ( auto kv : m_DrawablesMap )
            if ( kv.second > idx_drawable )
                m_DrawablesMap[kv.first]--;
        m_DrawablesMap.erase( name );
    }

    void TScenario::RemoveSingleBodyByName( const std::string& name )
    {
        if ( !HasSingleBodyNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::RemoveSingleBodyByName >>> tried removing unexistent single-body \"{0}\"", name );
            return;
        }
        const ssize_t idx_single_body = m_SingleBodiesMap.at( name );
        remove_single_body( idx_single_body );
        for ( auto kv : m_SingleBodiesMap )
            if ( kv.second > idx_single_body )
                m_SingleBodiesMap[kv.first]--;
        m_SingleBodiesMap.erase( name );
    }

    void TScenario::RemoveKinematicTreeByName( const std::string& name )
    {
        if ( !HasKinematicTreeNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::RemoveKinematicTreeByName >>> tried removing unexistent kinematic tree \"{0}\"", name );
            return;
        }
        const ssize_t idx_kinematic_tree = m_KinematicTreesMap.at( name );
        remove_kinematic_tree( idx_kinematic_tree );
        for ( auto kv : m_KinematicTreesMap )
            if ( kv.second > idx_kinematic_tree )
                m_KinematicTreesMap[kv.first]--;
        m_KinematicTreesMap.erase( name );
    }

    // void TScenario::RemoveTerrainGeneratorByName( const std::string& name )
    // {
    //     if ( !HasTerrainGeneratorNamed( name ) )
    //     {
    //         LOCO_CORE_ERROR( "TScenario::RemoveTerrainGeneratorByName >>> tried removing unexistent terrain generator \"{0}\"", name );
    //         return;
    //     }
    //     const ssize_t idx_terrain_generator = m_TerrainGeneratorsMap.at( name );
    //     remove_terrain_generator( idx_terrain_generator );
    //     for ( auto kv : m_TerrainGeneratorsMap )
    //         if ( kv.second > idx_terrain_generator )
    //             m_TerrainGeneratorsMap[kv.first]--;
    //     m_TerrainGeneratorsMap.erase( name );
    // }

    size_t TScenario::GetNumDrawables() const
    {
        return m_Drawables.size();
    }

    size_t TScenario::GetNumSingleBodies() const
    {
        return m_SingleBodies.size();
    }

    size_t TScenario::GetNumKinematicTrees() const
    {
        return m_KinematicTrees.size();
    }

////     size_t TScenario::GetNumTerrainGenerators() const
////     {
////         return m_TerrainGenerators.size();
////     }

    bool TScenario::HasDrawableNamed( const std::string& name ) const
    {
        return m_DrawablesMap.find( name ) != m_DrawablesMap.end();
    }

    bool TScenario::HasSingleBodyNamed( const std::string& name ) const
    {
        return m_SingleBodiesMap.find( name ) != m_SingleBodiesMap.end();
    }

    bool TScenario::HasKinematicTreeNamed( const std::string& name ) const
    {
        return m_KinematicTreesMap.find( name ) != m_KinematicTreesMap.end();
    }

////     bool TScenario::HasTerrainGeneratorNamed( const std::string& name ) const
////     {
////         return m_TerrainGeneratorsMap.find( name ) != m_TerrainGeneratorsMap.end();
////     }

    std::vector<const TDrawable*> TScenario::GetDrawablesList() const
    {
        std::vector<const TDrawable*> drawables_list;
        for ( auto& drawable : m_Drawables )
            drawables_list.push_back( drawable.get() );
        return drawables_list;
    }

    std::vector<const primitives::TSingleBody*> TScenario::GetSingleBodiesList() const
    {
        std::vector<const primitives::TSingleBody*> single_bodies_list;
        for ( auto& single_body : m_SingleBodies )
            single_bodies_list.push_back( single_body.get() );
        return single_bodies_list;
    }

    std::vector<const kintree::TKinematicTree*> TScenario::GetKinematicTreesList() const
    {
        std::vector<const kintree::TKinematicTree*> kinematic_trees_list;
        for ( auto& kinematic_tree : m_KinematicTrees )
            kinematic_trees_list.push_back( kinematic_tree.get() );
        return kinematic_trees_list;
    }

////     std::vector<const TTerrainGenerator*> TScenario::GetTerrainGeneratorsList() const
////     {
////         std::vector<const TTerrainGenerator*> _terrainGeneratorsList;
////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             _terrainGeneratorsList.push_back( terrainGenerator.get() );
////         return _terrainGeneratorsList;
////     }

    std::vector<TDrawable*> TScenario::GetDrawablesList()
    {
        std::vector<TDrawable*> drawables_list;
        for ( auto& drawable : m_Drawables )
            drawables_list.push_back( drawable.get() );
        return drawables_list;
    }

    std::vector<primitives::TSingleBody*> TScenario::GetSingleBodiesList()
    {
        std::vector<primitives::TSingleBody*> single_bodies_list;
        for ( auto& single_body : m_SingleBodies )
            single_bodies_list.push_back( single_body.get() );
        return single_bodies_list;
    }

    std::vector<kintree::TKinematicTree*> TScenario::GetKinematicTreesList()
    {
        std::vector<kintree::TKinematicTree*> kinematic_trees_list;
        for ( auto& kinematic_tree : m_KinematicTrees )
            kinematic_trees_list.push_back( kinematic_tree.get() );
        return kinematic_trees_list;
    }

////     std::vector<TTerrainGenerator*> TScenario::GetTerrainGeneratorsList()
////     {
////         std::vector<TTerrainGenerator*> _terrainGeneratorsList;
////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             _terrainGeneratorsList.push_back( terrainGenerator.get() );
////         return _terrainGeneratorsList;
////     }

    void TScenario::remove_drawable( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_Drawables.size() ), "TScenario::remove_drawable >>> \
                          index {0} out of range [0,{1}]", index, m_Drawables.size() - 1 );
        m_Drawables.erase( m_Drawables.begin() + index );
    }

    void TScenario::remove_single_body( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_SingleBodies.size() ), "TScenario::remove_single_body >>> \
                          index {0} out of range [0,{1}]", index, m_SingleBodies.size() - 1 );
        m_SingleBodies.erase( m_SingleBodies.begin() + index );
    }

    void TScenario::remove_kinematic_tree( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_KinematicTrees.size() ), "TScenario::remove_kinematic_tree >>> \
                          index {0} out of range [0,{1}]", index, m_KinematicTrees.size() - 1 );
        m_KinematicTrees.erase( m_KinematicTrees.begin() + index );
    }

////     void TScenario::remove_terrain_generator( ssize_t index )
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_TerrainGenerators.size() ), "TScenario::remove_terrain_generator >>> \
////                           index {0} out of range [0,{1}]", index, m_TerrainGenerators.size() - 1 );
////         m_TerrainGenerators.erase( m_TerrainGenerators.begin() + index );
////     }

    const TDrawable* TScenario::get_drawable( ssize_t index ) const
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_Drawables.size() ), "TScenario::get_drawable >>> \
                          index {0} out of range [0,{1}]", index, m_Drawables.size() );
        return m_Drawables[index].get();
    }

    const primitives::TSingleBody* TScenario::get_singleBody( ssize_t index ) const
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_SingleBodies.size() ), "TScenario::get_singleBody >>> \
                          index {0} out of range [0,{1}]", index, m_SingleBodies.size() );
        return m_SingleBodies[index].get();
    }

    const kintree::TKinematicTree* TScenario::get_kinematic_tree( ssize_t index ) const
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_KinematicTrees.size() ), "TScenario::get_kinematic_tree >>> \
                          index {0} out of range [0,{1}]", index, m_KinematicTrees.size() );
        return m_KinematicTrees[index].get();
    }

////     const TTerrainGenerator* TScenario::get_terrainGenerator( ssize_t index ) const
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_TerrainGenerators.size() ), "TScenario::get_terrainGenerator >>> \
////                           index {0} out of range [0,{1}]", index, m_TerrainGenerators.size() );
////         return m_TerrainGenerators[index].get();
////     }

    TDrawable* TScenario::get_mutable_drawable( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_Drawables.size() ), "TScenario::get_mutable_drawable >>> \
                          index {0} our of range [0,...,{1}]", index, m_Drawables.size() );
        return m_Drawables[index].get();
    }

    primitives::TSingleBody* TScenario::get_mutable_singleBody( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_SingleBodies.size() ), "TScenario::get_mutable_singleBody >>> \
                          index {0} out of range [0,...,{1}]", index, m_SingleBodies.size() );
        return m_SingleBodies[index].get();
    }

    kintree::TKinematicTree* TScenario::get_mutable_kinematic_tree( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_KinematicTrees.size() ), "TScenario::get_mutable_kinematic_tree >>> \
                          index {0} out of range [0,...,{1}]", index, m_KinematicTrees.size() );
        return m_KinematicTrees[index].get();
    }

////     TTerrainGenerator* TScenario::get_mutable_terrainGenerator( ssize_t index )
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_TerrainGenerators.size() ), "TScenario::get_mutable_terrainGenerator >>> \
////                           index {0} out of range [0,...,{1}]", index, m_TerrainGenerators.size() );
////         return m_TerrainGenerators[index].get();
////     }
}
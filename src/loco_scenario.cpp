
#include <loco_scenario.h>

namespace loco
{
    TScenario::TScenario()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TScenario @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TScenario @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TScenario::~TScenario()
    {
        m_SingleBodies.clear();
        //// m_Compounds.clear();
        //// m_KinematicTrees.clear();
        //// m_KinematicTreeSensors.clear();
        //// m_TerrainGenerators.clear();

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TScenario @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TScenario @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TDrawable* TScenario::AddDrawable( std::unique_ptr< TDrawable > drawable )
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

    TSingleBody* TScenario::AddSingleBody( std::unique_ptr< TSingleBody > singleBody )
    {
        LOCO_CORE_ASSERT( singleBody, "TScenario::AddSingleBody >>> tried adding nullptr" );

        if ( HasSingleBodyNamed( singleBody->name() ) )
        {
            LOCO_CORE_WARN( "TScenario::AddSingleBody >>> single-body with name {0} already exists. \
                             New one won't be added, returning nullptr instead.", singleBody->name() );
            return nullptr;
        }

        m_SingleBodiesMap[singleBody->name()] = m_SingleBodies.size();
        m_SingleBodies.push_back( std::move( singleBody ) );
        return m_SingleBodies.back().get();
    }

////     TCompound* TScenario::AddCompound( std::unique_ptr< TCompound > compound )
////     {
////         LOCO_CORE_ASSERT( compound, "TScenario::AddCompound >>> tried adding nullptr" );
//// 
////         if ( HasCompoundNamed( compound->name() ) )
////         {
////             LOCO_CORE_WARN( "TScenario::AddCompound >>> compound with name {0} already exists. \
////                              New one won't be added, returning nullptr instead.", compound->name() );
////             return nullptr;
////         }
//// 
////         m_CompoundsMap[compound->name()] = m_Compounds.size();
////         m_Compounds.push_back( std::move( compound ) );
////         return m_Compounds.back().get();
////     }
//// 
////     TKinTreeAgent* TScenario::AddKinTreeAgent( std::unique_ptr< TKinTreeAgent > kinTreeAgent )
////     {
////         LOCO_CORE_ASSERT( kinTreeAgent, "TScenario::AddKinTreeAgent >>> tried adding nullptr" );
//// 
////         if ( HasKinTreeAgentNamed( kinTreeAgent->name() ) )
////         {
////             LOCO_CORE_WARN( "TScenario::AddKinTreeAgent >>> kinTreeAgent with name {0} already exists. \
////                              New one won't be added, returning nullptr instead.", kinTreeAgent->name() );
////             return nullptr;
////         }
//// 
////         m_KinematicTreesMap[kinTreeAgent->name()] = m_KinematicTrees.size();
////         m_KinematicTrees.push_back( std::move( kinTreeAgent ) );
////         return m_KinematicTrees.back().get();
////     }
//// 
////     TKinTreeSensor* TScenario::AddKinTreeSensor( std::unique_ptr< TKinTreeSensor > kinTreeSensor )
////     {
////         LOCO_CORE_ASSERT( kinTreeSensor, "TScenario::AddKinTreeSensor >>> tried adding nullptr" );
//// 
////         if ( HasKinTreeSensorNamed( kinTreeSensor->name() ) )
////         {
////             LOCO_CORE_WARN( "TScenario::AddKinTreeSensor >>> kinTreeSensor with name {0} already exists. \
////                              New one won't be added, returning nullptr instead.", kinTreeSensor->name() );
////             return nullptr;
////         }
//// 
////         m_KinematicTreeSensorsMap[kinTreeSensor->name()] = m_KinematicTreeSensors.size();
////         m_KinematicTreeSensors.push_back( std::move( kinTreeSensor ) );
////         return m_KinematicTreeSensors.back().get();
////     }
//// 
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

    void TScenario::Initialize()
    {
        for ( auto& drawable : m_Drawables )
            drawable->Initialize();

        for ( auto& singleBody : m_SingleBodies )
            singleBody->Initialize();

////         for ( auto& compound : m_Compounds )
////             compound->Initialize();
//// 
////         for ( auto& kinTreeAgent : m_KinematicTrees )
////             kinTreeAgent->Initialize();
//// 
////         for ( auto& kinTreeSensor : m_KinematicTreeSensors )
////             kinTreeSensor->Initialize();
//// 
////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->Initialize();
    }

    void TScenario::PreStep()
    {
        for ( auto& drawable : m_Drawables )
            drawable->PreStep();

        for ( auto& singleBody : m_SingleBodies )
            singleBody->PreStep();

////         for ( auto& compound : m_Compounds )
////             compound->PreStep();
//// 
////         for ( auto& kinTreeAgent : m_KinematicTrees )
////             kinTreeAgent->PreStep();
//// 
////         for ( auto& kinTreeSensor : m_KinematicTreeSensors )
////             kinTreeSensor->PreStep();
//// 
////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->PreStep();
    }

    void TScenario::PostStep()
    {
        for ( auto& drawable : m_Drawables )
            drawable->PostStep();

        for ( auto& singleBody : m_SingleBodies )
            singleBody->PostStep();

////         for ( auto& compound : m_Compounds )
////             compound->PostStep();
//// 
////         for ( auto& kinTreeAgent : m_KinematicTrees )
////             kinTreeAgent->PostStep();
//// 
////         for ( auto& kinTreeSensor : m_KinematicTreeSensors )
////             kinTreeSensor->PostStep();
//// 
////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->PostStep();
}

    void TScenario::Reset()
    {
        for ( auto& drawable : m_Drawables )
            drawable->Reset();

        for ( auto& singleBody : m_SingleBodies )
            singleBody->Reset();

////         for ( auto& compound : m_Compounds )
////             compound->Reset();
//// 
////         for ( auto& kinTreeAgent : m_KinematicTrees )
////             kinTreeAgent->Reset();
//// 
////         for ( auto& kinTreeSensor : m_KinematicTreeSensors )
////             kinTreeSensor->Reset();
//// 
////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->Reset();
    }

    void TScenario::DetachSim()
    {
        for ( auto& drawable : m_Drawables )
            drawable->DetachSim();

        for ( auto& singleBody : m_SingleBodies )
            singleBody->DetachSim();

////         for ( auto& compound : m_Compounds )
////             compound->DetachSim();
//// 
////         for ( auto& kinTreeAgent : m_KinematicTrees )
////             kinTreeAgent->DetachSim();
//// 
////         for ( auto& kinTreeSensor : m_KinematicTreeSensors )
////             kinTreeSensor->DetachSim();
//// 
////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->DetachSim();
    }

    void TScenario::DetachViz()
    {
        for ( auto& drawable : m_Drawables )
            drawable->DetachViz();

        for ( auto& singleBody : m_SingleBodies )
            singleBody->DetachViz();

////         for ( auto& compound : m_Compounds )
////             compound->DetachViz();
//// 
////         for ( auto& kinTreeAgent : m_KinematicTrees )
////             kinTreeAgent->DetachViz();
//// 
////         for ( auto& kinTreeSensor : m_KinematicTreeSensors )
////             kinTreeSensor->DetachViz();
//// 
////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             terrainGenerator->DetachViz();
    }

    const TDrawable* TScenario::GetDrawableByName( const std::string& name ) const
    {
        if ( !HasDrawableNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::GetDrawableByName >>> drawable with name {0} not found in the scenario", name );
            return nullptr;
        }

        return get_drawable( m_DrawablesMap.at( name ) );
    }

    const TSingleBody* TScenario::GetSingleBodyByName( const std::string& name ) const
    {
        if( !HasSingleBodyNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::GetSingleBodyByName >>> single-body with name {0} not found in the scenario", name );
            return nullptr;
        }

        return get_singleBody( m_SingleBodiesMap.at( name ) );
    }

////     const TCompound* TScenario::GetCompoundBodyByName( const std::string& name ) const
////     {
////         if( !HasCompoundBodyNamed( name ) )
////         {
////             LOCO_CORE_ERROR( "TScenario::GetTerrainGeneratorByName >>> terrainGenerator with name {0} not found", name );
////             return nullptr;
////         }
//// 
////         return get_compound( m_CompoundsMap.at( name ) );
////     }
//// 
////     const TKinTreeAgent* TScenario::GetKinTreeAgentByName( const std::string& name ) const
////     {
////         if( !HasKinTreeAgentNamed( name ) )
////         {
////             LOCO_CORE_ERROR( "TScenario::GetKinTreeAgentByName >>> kintreeAgent with name {0} not found", name );
////             return nullptr;
////         }
//// 
////         return get_kintreeAgent( m_KinematicTreesMap.at( name ) );
////     }
//// 
////     const TKinTreeSensor* TScenario::GetKinTreeSensorByName( const std::string& name ) const
////     {
////         if( !HasKinTreeSensorNamed( name ) )
////         {
////             LOCO_CORE_ERROR( "TScenario::GetKinTreeSensorByName >>> kintreeSensor with name {0} not found", name );
////             return nullptr;
////         }
//// 
////         return get_kintreeSensor( m_KinematicTreeSensorsMap.at( name ) );
////     }
//// 
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

    TSingleBody* TScenario::GetSingleBodyByName( const std::string& name )
    {
        if( !HasSingleBodyNamed( name ) )
        {
            LOCO_CORE_ERROR( "TScenario::GetSingleBodyByName >>> single-body with name {0} not found", name );
            return nullptr;
        }

        return get_mutable_singleBody( m_SingleBodiesMap.at( name ) );
    }

////     TCompound* TScenario::GetCompoundBodyByName( const std::string& name )
////     {
////         if( !HasCompoundBodyNamed( name ) )
////         {
////             LOCO_CORE_ERROR( "TScenario::GetCompoundBodyByName >>> compound with name {0} not found", name );
////             return nullptr;
////         }
//// 
////         return get_mutable_compound( m_CompoundsMap.at( name ) );
////     }
//// 
////     TKinTreeAgent* TScenario::GetKinTreeAgentByName( const std::string& name )
////     {
////         if( !HasKinTreeAgentNamed( name ) )
////         {
////             LOCO_CORE_ERROR( "TScenario::GetKinTreeAgentByName >>> kintreeAgent with name {0} not found", name );
////             return nullptr;
////         }
//// 
////         return get_mutable_kintreeAgent( m_KinematicTreesMap.at( name ) );
////     }
//// 
////     TKinTreeSensor* TScenario::GetKinTreeSensorByName( const std::string& name )
////     {
////         if( !HasKinTreeSensorNamed( name ) )
////         {
////             LOCO_CORE_ERROR( "TScenario::GetKinTreeSensorByName >>> kintreeSensor with name {0} not found", name );
////             return nullptr;
////         }
//// 
////         return get_mutable_kintreeSensor( m_KinematicTreeSensorsMap.at( name ) );
////     }
//// 
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

    // void TScenario::RemoveCompoundByName( const std::string& name )
    // {
    //     if ( !HasCompoundNamed( name ) )
    //     {
    //         LOCO_CORE_ERROR( "TScenario::RemoveCompoundByName >>> tried removing unexistent compound \"{0}\"", name );
    //         return;
    //     }
    //     const ssize_t idx_compound = m_CompoundsMap.at( name );
    //     remove_compound( idx_compound );
    //     for ( auto kv : m_CompoundsMap )
    //         if ( kv.second > idx_compound )
    //             m_CompoundsMap[kv.first]--;
    //     m_CompoundsMap.erase( name );
    // }

    // void TScenario::RemoveKinematicTreeByName( const std::string& name )
    // {
    //     if ( !HasKinematicTreeNamed( name ) )
    //     {
    //         LOCO_CORE_ERROR( "TScenario::RemoveKinematicTreeByName >>> tried removing unexistent kinematic tree \"{0}\"", name );
    //         return;
    //     }
    //     const ssize_t idx_kinematic_tree = m_KinematicTreesMap.at( name );
    //     remove_kinematic_tree( idx_kinematic_tree );
    //     for ( auto kv : m_KinematicTreesMap )
    //         if ( kv.second > idx_kinematic_Tree )
    //             m_KinematicTreesMap[kv.first]--;
    //     m_KinematicTreesMap.erase( name );
    // }

    // void TScenario::RemoveKinematicTreeSensorByName( const std::string& name )
    // {
    //     if ( !HasKinematicTreeSensorNamed( name ) )
    //     {
    //         LOCO_CORE_ERROR( "TScenario::RemoveKinematicTreeSensorByName >>> tried removing unexistent kinematic tree sensor \"{0}\"", name );
    //         return;
    //     }
    //     const ssize_t idx_kinematic_tree_sensor = m_KinematicTreeSensorsMap.at( name );
    //     remove_kinematic_tree_sensor( idx_kinematic_tree_sensor );
    //     for ( auto kv : m_KinematicTreeSensorsMap )
    //         if ( kv.second > idx_kinematic_tree_sensor )
    //             m_KinematicTreeSensorsMap[kv.first]--;
    //     m_KinematicTreeSensorsMap.erase( name );
    // }

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

////     size_t TScenario::GetNumCompounds() const
////     {
////         return m_Compounds.size();
////     }
//// 
////     size_t TScenario::GetNumKinTreeAgents() const
////     {
////         return m_KinematicTrees.size();
////     }
//// 
////     size_t TScenario::GetNumKinTreeSensors() const
////     {
////         return m_KinematicTreeSensors.size();
////     }
//// 
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

////     bool TScenario::HasCompoundNamed( const std::string& name ) const
////     {
////         return m_CompoundsMap.find( name ) != m_CompoundsMap.end();
////     }
//// 
////     bool TScenario::HasKinTreeAgentNamed( const std::string& name ) const
////     {
////         return m_KinematicTreesMap.find( name ) != m_KinematicTreesMap.end();
////     }
//// 
////     bool TScenario::HasKinTreeSensorNamed( const std::string& name ) const
////     {
////         return m_KinematicTreeSensorsMap.find( name ) != m_KinematicTreeSensorsMap.end();
////     }
//// 
////     bool TScenario::HasTerrainGeneratorNamed( const std::string& name ) const
////     {
////         return m_TerrainGeneratorsMap.find( name ) != m_TerrainGeneratorsMap.end();
////     }

    std::vector<const TDrawable*> TScenario::GetDrawablesList() const
    {
        std::vector<const TDrawable*> _drawablesList;
        for ( auto& drawable : m_Drawables )
            _drawablesList.push_back( drawable.get() );
        return _drawablesList;
    }

    std::vector<const TSingleBody*> TScenario::GetSingleBodiesList() const
    {
        std::vector<const TSingleBody*> _singleBodiesList;
        for ( auto& singleBody : m_SingleBodies )
            _singleBodiesList.push_back( singleBody.get() );
        return _singleBodiesList;
    }

////     std::vector<const TCompound*> TScenario::GetCompoundsList() const
////     {
////         std::vector<const TCompound*> _compoundsList;
////         for ( auto& compound : m_Compounds )
////             _compoundsList.push_back( compound.get() );
////         return _compoundsList;
////     }
//// 
////     std::vector<const TKinTreeAgent*> TScenario::GetKinTreeAgentsList() const
////     {
////         std::vector<const TKinTreeAgent*> _kintreeAgentsList;
////         for ( auto& kintreeAgent : m_KinematicTrees )
////             _kintreeAgentsList.push_back( kinTreeAgent.get() );
////         return _kintreeAgentsList;
////     }
//// 
////     std::vector<const TKinTreeSensor*> TScenario::GetKinTreeSensorsList() const
////     {
////         std::vector<const TKinTreeSensor*> _kintreeSensorsList;
////         for ( auto& kintreeSensor : m_KinematicTreeSensors )
////             _kintreeSensorsList.push_back( kinTreeSensor.get() );
////         return _kintreeSensorsList;
////     }
//// 
////     std::vector<const TTerrainGenerator*> TScenario::GetTerrainGeneratorsList() const
////     {
////         std::vector<const TTerrainGenerator*> _terrainGeneratorsList;
////         for ( auto& terrainGenerator : m_TerrainGenerators )
////             _terrainGeneratorsList.push_back( terrainGenerator.get() );
////         return _terrainGeneratorsList;
////     }

    std::vector< TDrawable* > TScenario::GetDrawablesList()
    {
        std::vector<TDrawable*> _drawablesList;
        for ( auto& drawable : m_Drawables )
            _drawablesList.push_back( drawable.get() );
        return _drawablesList;
    }

    std::vector< TSingleBody* > TScenario::GetSingleBodiesList()
    {
        std::vector<TSingleBody*> _singleBodiesList;
        for ( auto& singleBody : m_SingleBodies )
            _singleBodiesList.push_back( singleBody.get() );
        return _singleBodiesList;
    }

////     std::vector<TCompound*> TScenario::GetCompoundsList()
////     {
////         std::vector<TCompound*> _compoundsList;
////         for ( auto& compound : m_Compounds )
////             _compoundsList.push_back( compound.get() );
////         return _compoundsList;
////     }
//// 
////     std::vector<TKinTreeAgent*> TScenario::GetKinTreeAgentsList()
////     {
////         std::vector<TKinTreeAgent*> _kintreeAgentsList;
////         for ( auto& kintreeAgent : m_KinematicTrees )
////             _kintreeAgentsList.push_back( kinTreeAgent.get() );
////         return _kintreeAgentsList;
////     }
//// 
////     std::vector<TKinTreeSensor*> TScenario::GetKinTreeSensorsList()
////     {
////         std::vector<TKinTreeSensor*> _kintreeSensorsList;
////         for ( auto& kintreeSensor : m_KinematicTreeSensors )
////             _kintreeSensorsList.push_back( kinTreeSensor.get() );
////         return _kintreeSensorsList;
////     }
//// 
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

////     void TScenario::remove_compound( ssize_t index )
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_Compounds.size() ), "TScenario::remove_compound >>> \
////                           index {0} out of range [0,{1}]", index, m_Compounds.size() - 1 );
////         m_Compounds.erase( m_Compounds.begin() + index );
////     }

////     void TScenario::remove_kinematic_tree( ssize_t index )
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_KinematicTrees.size() ), "TScenario::remove_kinematic_tree >>> \
////                           index {0} out of range [0,{1}]", index, m_KinematicTrees.size() - 1 );
////         m_KinematicTrees.erase( m_KinematicTrees.begin() + index );
////     }

////     void TScenario::remove_kinematic_tree_sensor( ssize_t index )
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_KinematicTreeSensors.size() ), "TScenario::remove_kinematic_tree_sensor >>> \
////                           index {0} out of range [0,{1}]", index, m_KinematicTreeSensors.size() - 1 );
////         m_KinematicTreeSensors.erase( m_KinematicTreeSensors.begin() + index );
////     }

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

    const TSingleBody* TScenario::get_singleBody( ssize_t index ) const
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_SingleBodies.size() ), "TScenario::get_singleBody >>> \
                          index {0} out of range [0,{1}]", index, m_SingleBodies.size() );
        return m_SingleBodies[index].get();
    }

////     const TCompound* TScenario::get_compound( ssize_t index ) const
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_Compounds.size() ), "TScenario::get_compound >>> \
////                           index {0} out of range [0,{1}]", index, m_Compounds.size() );
////         return m_Compounds[index].get();
////     }
//// 
////     const TKinTreeAgent* TScenario::get_kintreeAgent( ssize_t index ) const
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_KinematicTrees.size() ), "TScenario::get_kintreeAgent >>> \
////                           index {0} out of range [0,{1}]", index, m_KinematicTrees.size() );
////         return m_KinematicTrees[index].get();
////     }
//// 
////     const TKinTreeSensor* TScenario::get_kintreeSensor( ssize_t index ) const
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_KinematicTreeSensors.size() ), "TScenario::get_kintreeSensor >>> \
////                           index {0} out of range [0,{1}]", index, m_KinematicTreeSensors.size() );
////         return m_KinematicTreeSensors[index].get();
////     }
//// 
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

    TSingleBody* TScenario::get_mutable_singleBody( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_SingleBodies.size() ), "TScenario::get_mutable_singleBody >>> \
                          index {0} out of range [0,...,{1}]", index, m_SingleBodies.size() );
        return m_SingleBodies[index].get();
    }

////     TCompound* TScenario::get_mutable_compound( ssize_t index )
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_Compounds.size() ), "TScenario::get_mutable_compound >>> \
////                           index {0} out of range [0,...,{1}]", index, m_Compounds.size() );
////         return m_Compounds[index].get();
////     }
//// 
////     TKinTreeAgent* TScenario::get_mutable_kintreeAgent( ssize_t index )
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_KinematicTrees.size() ), "TScenario::get_mutable_kintreeAgent >>> \
////                           index {0} out of range [0,...,{1}]", index, m_KinematicTrees.size() );
////         return m_KinematicTrees[index].get();
////     }
//// 
////     TKinTreeSensor* TScenario::get_mutable_kintreeSensor( ssize_t index )
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_KinematicTreeSensors.size() ), "TScenario::get_mutable_kintreeSensor >>> \
////                           index {0} out of range [0,...,{1}]", index, m_KinematicTreeSensors.size() );
////         return m_KinematicTreeSensors[index].get();
////     }
//// 
////     TTerrainGenerator* TScenario::get_mutable_terrainGenerator( ssize_t index )
////     {
////         LOCO_CORE_ASSERT( ( index >= 0 && index < m_TerrainGenerators.size() ), "TScenario::get_mutable_terrainGenerator >>> \
////                           index {0} out of range [0,...,{1}]", index, m_TerrainGenerators.size() );
////         return m_TerrainGenerators[index].get();
////     }
}
#pragma once

#include <visualizer/loco_visualizer_drawable.h>
#include <primitives/loco_single_body.h>
//// #include <compounds/compound.h>
//// #include <agent/agent_kintree.h>
//// #include <sensor/sensor_kintree.h>
//// #include <terrain/terrain_generator.h>

#define RETURN_DRAWABLE_PRIMITIVE(arg_name,arg_shape,arg_size,arg_color) \
            auto vis_data = loco::TVisualData();\
            vis_data.type = arg_shape;\
            vis_data.size = arg_size;\
            vis_data.ambient = arg_color;\
            vis_data.diffuse = arg_color;\
            vis_data.specular = arg_color;\
            vis_data.shininess = 128.0f;\
            return AddDrawable( std::make_unique<TDrawable>( arg_name, vis_data ) );

#define RETURN_SINGLE_BODY_PRIMITIVE(arg_name,arg_position,arg_rotation,arg_shape,arg_size,arg_collisionGroup,arg_collisionMask,arg_mass,arg_dyntype) \
            auto vis_data = loco::TVisualData();\
            vis_data.type = arg_shape;\
            vis_data.size = arg_size;\
            vis_data.ambient = { 0.7, 0.5, 0.3 };\
            vis_data.diffuse = { 0.7, 0.5, 0.3 };\
            vis_data.specular = { 0.7, 0.5, 0.3 };\
            vis_data.shininess = 128.0f;\
            auto col_data = loco::TCollisionData();\
            col_data.type = arg_shape;\
            col_data.size = arg_size;\
            col_data.collisionGroup = arg_collisionGroup;\
            col_data.collisionMask = arg_collisionMask;\
            auto body_data = loco::TBodyData();\
            body_data.dyntype = arg_dyntype;\
            body_data.inertia.mass = arg_mass;\
            body_data.collision = col_data;\
            body_data.visual = vis_data;\
            return AddSingleBody( std::make_unique<TSingleBody>( arg_name, body_data, arg_position, arg_rotation ) );


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

        TDrawable* CreateBoxDrawable( const std::string& name,
                                      const TVec3& extents,
                                      const TVec3& color );
        TDrawable* CreateSphereDrawable( const std::string& name,
                                         TScalar radius,
                                         const TVec3& color );
        TDrawable* CreateCylinderDrawable( const std::string& name,
                                           TScalar radius, TScalar height,
                                           const TVec3& color );
        TDrawable* CreateCapsuleDrawable( const std::string& name,
                                          TScalar radius, TScalar height,
                                          const TVec3& color );
        TDrawable* CreateEllipsoidDrawable( const std::string& name,
                                            const TVec3& radii,
                                            const TVec3& color );
        TDrawable* CreateMeshDrawable( const std::string& name,
                                       const std::string& filename,
                                       TScalar scale,
                                       const TVec3& color );
        TDrawable* CreateMeshDrawable( const std::string& name,
                                       const std::vector<float>& vertices,
                                       const std::vector<int>& faces,
                                       TScalar scale,
                                       const TVec3& color );

        TSingleBody* CreateBox( const std::string& name,
                                const TVec3& position,
                                const TMat3& rotation,
                                const TVec3& extents,
                                eDynamicsType dyntype = eDynamicsType::DYNAMIC,
                                int collisionGroup = 1,
                                int collisionMask = 1,
                                TScalar mass = -1.0 );
        TSingleBody* CreateSphere( const std::string& name,
                                   const TVec3& position,
                                   const TMat3& rotation,
                                   TScalar radius,
                                   eDynamicsType dyntype = eDynamicsType::DYNAMIC,
                                   int collisionGroup = 1,
                                   int collisionMask = 1,
                                   TScalar mass = -1.0 );
        TSingleBody* CreateCylinder( const std::string& name,
                                     const TVec3& position,
                                     const TMat3& rotation,
                                     TScalar radius, TScalar height,
                                     eDynamicsType dyntype = eDynamicsType::DYNAMIC,
                                     int collisionGroup = 1,
                                     int collisionMask = 1,
                                     TScalar mass = -1.0 );
        TSingleBody* CreateCapsule( const std::string& name,
                                    const TVec3& position,
                                    const TMat3& rotation,
                                    TScalar radius, TScalar height,
                                    eDynamicsType dyntype = eDynamicsType::DYNAMIC,
                                    int collisionGroup = 1,
                                    int collisionMask = 1,
                                    TScalar mass = -1.0 );
        TSingleBody* CreateEllipsoid( const std::string& name,
                                      const TVec3& position,
                                      const TMat3& rotation,
                                      const TVec3& radii,
                                      eDynamicsType dyntype = eDynamicsType::DYNAMIC,
                                      int collisionGroup = 1,
                                      int collisionMask = 1,
                                      TScalar mass = -1.0 );
        TSingleBody* CreateMesh( const std::string& name,
                                 const TVec3& position,
                                 const TMat3& rotation,
                                 const std::string& filename,
                                 TScalar scale,
                                 eDynamicsType dyntype = eDynamicsType::DYNAMIC,
                                 int collisionGroup = 1,
                                 int collisionMask = 1,
                                 TScalar mass = -1.0 );
        TSingleBody* CreateMesh( const std::string& name,
                                 const TVec3& position,
                                 const TMat3& rotation,
                                 const std::vector<float>& vertices,
                                 const std::vector<int>& faces,
                                 TScalar scale,
                                 eDynamicsType dyntype = eDynamicsType::DYNAMIC,
                                 int collisionGroup = 1,
                                 int collisionMask = 1,
                                 TScalar mass = -1.0 );
        TSingleBody* CreateHeightfield( const std::string& name,
                                        const TVec3& position,
                                        const TMat3& rotation,
                                        ssize_t num_width_samples,
                                        ssize_t num_depth_samples,
                                        TScalar width_extent,
                                        TScalar depth_extent,
                                        const std::vector<float>& heights,
                                        int collisionGroup = 1,
                                        int collisionMask = 1 );

        TSingleBody* CreatePlane( const std::string& name,
                                  TScalar width, TScalar depth,
                                  int collisionGroup = 1,
                                  int collisionMask = 1 );

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

#include <loco.h>
#include <random>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_EDITOR;

const size_t NUM_BOXES      = 4;
const size_t NUM_SPHERES    = 4;
const size_t NUM_CYLINDERS  = 4;
const size_t NUM_CAPSULES   = 4;
const size_t NUM_ELLIPSOIDS = 4;
const size_t NUM_MESHES     = 1;

const double min_position = -3.0;
const double max_position = 3.0;
const double min_rotation = -loco::PI / 2.0;
const double max_rotation = loco::PI / 2.0;

std::default_random_engine g_rnd_generator;
std::uniform_real_distribution<loco::TScalar> g_rnd_dist_position( min_position, max_position );
std::uniform_real_distribution<loco::TScalar> g_rnd_dist_rotation( min_rotation, max_rotation );

loco::TVec3 random_position();
loco::TVec3 random_rotation();

std::unique_ptr<loco::TSingleBody> create_mesh( const std::string& name,
                                                const loco::TVec3& scale,
                                                const std::string& mesh_filepath_collider,
                                                const std::string& mesh_filepath_visual,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler );

std::unique_ptr<loco::TSingleBody> create_body( const std::string& name,
                                                const loco::eShapeType& shape,
                                                const loco::TVec3& size,
                                                const loco::eDynamicsType& dyntype,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler,
                                                const loco::TVec3& color = { 0.7f, 0.5f, 0.3f } );

int main( int argc, char* argv[] )
{
    loco::TLogger::Init();

    if ( argc > 1 )
    {
        std::string choice_backend = argv[1];
        if ( choice_backend == "mujoco" )
            PHYSICS_BACKEND = loco::config::physics::MUJOCO;
        else if ( choice_backend == "bullet" )
            PHYSICS_BACKEND = loco::config::physics::BULLET;
        else if ( choice_backend == "dart" )
            PHYSICS_BACKEND = loco::config::physics::DART;
        else if ( choice_backend == "raisim" )
            PHYSICS_BACKEND = loco::config::physics::RAISIM;
        else if ( choice_backend == "none" )
            PHYSICS_BACKEND = loco::config::physics::NONE;
    }

    LOCO_TRACE( "Physics-Backend: {0}", PHYSICS_BACKEND );
    LOCO_TRACE( "Rendering-Backend: {0}", RENDERING_BACKEND );

    const loco::TVec3 orientation = { loco::PI / 3, loco::PI / 4, loco::PI / 6 };
    //// const loco::TVec3 orientation = { loco::PI / 2, 0.0f, 0.0f };
    //// const loco::TVec3 orientation = { 0.0f, 0.0f, 0.0f };

    auto scenario = std::make_unique<loco::TScenario>();
    scenario->AddSingleBody( create_body( "floor", loco::eShapeType::PLANE, { 10.0f, 10.0f, 1.0f },
                                          loco::eDynamicsType::STATIC, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.3f, 0.5f, 0.7f } ) );
    for ( ssize_t i = 0; i < NUM_BOXES; i++ )
        scenario->AddSingleBody( create_body( "box_" + std::to_string( i ), loco::eShapeType::BOX, { 0.2f, 0.3f, 0.4f },
                                              loco::eDynamicsType::DYNAMIC, random_position(), random_rotation() ) );
    for ( ssize_t i = 0; i < NUM_SPHERES; i++ )
        scenario->AddSingleBody( create_body( "sphere_" + std::to_string( i ), loco::eShapeType::SPHERE, { 0.1f, 0.1f, 0.1f },
                                              loco::eDynamicsType::DYNAMIC, random_position(), random_rotation() ) );
    for ( ssize_t i = 0; i < NUM_CYLINDERS; i++ )
        scenario->AddSingleBody( create_body( "cylinder_" + std::to_string( i ), loco::eShapeType::CYLINDER, { 0.2f, 0.5f, 1.0f },
                                              loco::eDynamicsType::DYNAMIC, random_position(), random_rotation() ) );
    for ( ssize_t i = 0; i < NUM_CAPSULES; i++ )
        scenario->AddSingleBody( create_body( "capsule_" + std::to_string( i ), loco::eShapeType::CAPSULE, { 0.2f, 0.5f, 1.0f },
                                              loco::eDynamicsType::DYNAMIC, random_position(), random_rotation() ) );
    for ( ssize_t i = 0; i < NUM_ELLIPSOIDS; i++ )
        scenario->AddSingleBody( create_body( "ellipsoid_" + std::to_string( i ), loco::eShapeType::ELLIPSOID, { 0.2f, 0.3f, 0.4f },
                                              loco::eDynamicsType::DYNAMIC, random_position(), random_rotation() ) );
    for ( ssize_t i = 0; i < NUM_MESHES; i++ )
        scenario->AddSingleBody( create_mesh( "monkey_", { 0.25, 0.25, 0.25 },
                                              loco::PATH_RESOURCES + "meshes/monkey.stl",
                                              loco::PATH_RESOURCES + "meshes/monkey.obj",
                                              random_position(), random_rotation() ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    while ( visualizer->IsActive() )
    {
        if ( auto simulation = runtime->GetCurrentSimulation() )
            simulation->Step();

        visualizer->Update();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}

loco::TVec3 random_position()
{
    return { g_rnd_dist_position( g_rnd_generator ),
             g_rnd_dist_position( g_rnd_generator ),
             2.0 };
}

loco::TVec3 random_rotation()
{
    return { g_rnd_dist_rotation( g_rnd_generator ),
             g_rnd_dist_rotation( g_rnd_generator ),
             g_rnd_dist_rotation( g_rnd_generator ) };
}

std::unique_ptr<loco::TSingleBody> create_mesh( const std::string& name,
                                                const loco::TVec3& scale,
                                                const std::string& mesh_filepath_collider,
                                                const std::string& mesh_filepath_visual,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler )
{
    auto col_data = loco::TCollisionData();
    col_data.type = loco::eShapeType::MESH;
    col_data.size = scale;
    col_data.mesh_data.filename = mesh_filepath_collider;
    auto vis_data = loco::TVisualData();
    vis_data.type = loco::eShapeType::MESH;
    vis_data.size = scale;
    vis_data.mesh_data.filename = mesh_filepath_visual;
    auto body_data = loco::TBodyData();
    body_data.collision = col_data;
    body_data.visual = vis_data;
    body_data.dyntype = loco::eDynamicsType::DYNAMIC;

    auto body_obj = std::make_unique<loco::TSingleBody>( name, body_data, position, tinymath::rotation( euler ) );
    return std::move( body_obj );
}

std::unique_ptr<loco::TSingleBody> create_body( const std::string& name,
                                                const loco::eShapeType& shape,
                                                const loco::TVec3& size,
                                                const loco::eDynamicsType& dyntype,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler,
                                                const loco::TVec3& color )
{
    auto col_data = loco::TCollisionData();
    col_data.type = shape;
    col_data.size = size;
    auto vis_data = loco::TVisualData();
    vis_data.type = shape;
    vis_data.size = size;
    vis_data.ambient = color;
    vis_data.diffuse = color;
    vis_data.specular = color;
    auto body_data = loco::TBodyData();
    body_data.collision = col_data;
    body_data.visual = vis_data;
    body_data.dyntype = dyntype;

    auto body_obj = std::make_unique<loco::TSingleBody>( name, body_data, position, tinymath::rotation( euler ) );
    return std::move( body_obj );
}
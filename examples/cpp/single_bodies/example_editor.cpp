
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

int main( int argc, char* argv[] )
{
    loco::InitUtils();

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

    //// const loco::TMat3 rotation = tinymath::rotation( loco::TVec3( loco::PI / 3, loco::PI / 4, loco::PI / 6 ) );
    //// const loco::TMat3 rotation = tinymath::rotation( loco::TVec3( loco::PI / 2, 0.0f, 0.0f ) );
    const loco::TMat3 rotation = tinymath::rotation( loco::TVec3( 0.0f, 0.0f, 0.0f ) );

    auto scenario = std::make_unique<loco::TScenario>();
    scenario->AddSingleBody( std::make_unique<loco::primitives::TPlane>( "floor", 10.0f, 10.0f, loco::TVec3(), loco::TMat3() ) );
    for ( ssize_t i = 0; i < NUM_BOXES; i++ )
        scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "box_" + std::to_string( i ), loco::TVec3( 0.2f, 0.3f, 0.4f ),
                                                               random_position(), tinymath::rotation( random_rotation() ) ) );
    for ( ssize_t i = 0; i < NUM_SPHERES; i++ )
        scenario->AddSingleBody( std::make_unique<loco::primitives::TSphere>( "sphere_" + std::to_string( i ), 0.1f,
                                                                  random_position(), tinymath::rotation( random_rotation() ) ) );
    for ( ssize_t i = 0; i < NUM_CYLINDERS; i++ )
        scenario->AddSingleBody( std::make_unique<loco::primitives::TCylinder>( "cylinder_" + std::to_string( i ), 0.2f, 0.5f,
                                                                    random_position(), tinymath::rotation( random_rotation() ) ) );
    for ( ssize_t i = 0; i < NUM_CAPSULES; i++ )
        scenario->AddSingleBody( std::make_unique<loco::primitives::TCapsule>( "capsule_" + std::to_string( i ), 0.2f, 0.5f,
                                                                   random_position(), tinymath::rotation( random_rotation() ) ) );
    for ( ssize_t i = 0; i < NUM_ELLIPSOIDS; i++ )
        scenario->AddSingleBody( std::make_unique<loco::primitives::TEllipsoid>( "ellipsoid_" + std::to_string( i ), loco::TVec3( 0.2f, 0.3f, 0.4f ),
                                                                     random_position(), tinymath::rotation( random_rotation() ) ) );
    for ( ssize_t i = 0; i < NUM_MESHES; i++ )
        scenario->AddSingleBody( std::make_unique<loco::primitives::TMesh>( "monkey_",
                                                                loco::PATH_RESOURCES + "meshes/monkey.stl",
                                                                loco::PATH_RESOURCES + "meshes/monkey.obj",
                                                                0.25f, random_position(), tinymath::rotation( random_rotation() ) ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    auto floor_ref = scenario->GetSingleBodyByName( "floor" );
    floor_ref->drawable()->ChangeTexture( "built_in_chessboard" );
    floor_ref->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );

    while ( visualizer->IsActive() )
    {
        if ( auto simulation = runtime->GetCurrentSimulation() )
            simulation->Step();

        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
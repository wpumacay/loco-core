
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

const int nx_samples_hfield_1 = 25;
const int ny_samples_hfield_1 = 25;
const float x_extent_hfield_1 = 10.0f;
const float y_extent_hfield_1 = 10.0f;
std::vector<float> create_hfield_paraboloid()
{
    std::vector<float> heights;
    for ( ssize_t i = 0; i < ny_samples_hfield_1; i++ )
    {
        for ( ssize_t j = 0; j < nx_samples_hfield_1; j++ )
        {
            float x = x_extent_hfield_1 * ( (float)j / nx_samples_hfield_1 - 0.5f );
            float y = y_extent_hfield_1 * ( (float)i / ny_samples_hfield_1 - 0.5f );
            heights.push_back( 10.0f * ( x * x + y * y ) / 
                                ( x_extent_hfield_1 * x_extent_hfield_1 + 
                                  y_extent_hfield_1 * y_extent_hfield_1 ) );
        }
    }
    return heights;
}

const int nx_samples_hfield_2 = 25;
const int ny_samples_hfield_2 = 25;
const float x_extent_hfield_2 = 10.0f;
const float y_extent_hfield_2 = 10.0f;
const int hfield_octaves = 4;
const float hfield_persistance = 0.5f;
const float hfield_lacunarity = 2.0f;
const float hfield_noise_scale = 5.0f;
std::vector<float> create_hfield_perlin_noise()
{
    tinyutils::PerlinNoise::Config( hfield_octaves, hfield_persistance, hfield_lacunarity, hfield_noise_scale );

    std::vector<float> heights;
    for ( ssize_t i = 0; i < ny_samples_hfield_2; i++ )
    {
        for ( ssize_t j = 0; j < nx_samples_hfield_2; j++ )
        {
            float x = x_extent_hfield_2 * ( (float)j / nx_samples_hfield_2 - 0.5f );
            float y = y_extent_hfield_2 * ( (float)i / ny_samples_hfield_2 - 0.5f );
            heights.push_back( std::max( 0.0f, tinyutils::PerlinNoise::Sample2d( x, y ) ) );
        }
    }
    return heights;
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

    auto scenario = std::make_unique<loco::TScenario>();
    auto floor = scenario->AddSingleBody( std::make_unique<loco::TPlane>( "floor", 40.0f, 40.0f, loco::TVec3( 0.0f, 0.0f, -0.001f ), loco::TMat3() ) );
    floor->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );

    auto sphere = scenario->AddSingleBody( std::make_unique<loco::TSphere>( "sphere", 0.1f, loco::TVec3( -1.0, -1.0, 3.0 ), loco::TMat3() ) );
    auto hfield_1 = scenario->AddSingleBody( std::make_unique<loco::THeightfield>( "hfield_1",
                                                                                   nx_samples_hfield_1, ny_samples_hfield_1,
                                                                                   x_extent_hfield_1, y_extent_hfield_1,
                                                                                   create_hfield_paraboloid(),
                                                                                   loco::TVec3( -5.0, -5.0, 0.0 ), loco::TMat3() ) );
    hfield_1->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );
    auto hfield_2 = scenario->AddSingleBody( std::make_unique<loco::THeightfield>( "hfield_2",
                                                                                   nx_samples_hfield_2, ny_samples_hfield_2,
                                                                                   x_extent_hfield_2, y_extent_hfield_2,
                                                                                   create_hfield_perlin_noise(),
                                                                                   loco::TVec3( 5.0, 5.0, 0.0 ), loco::TMat3() ) );
    hfield_2->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_1->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_2->drawable()->ChangeTexture( "built_in_chessboard" );

    while ( visualizer->IsActive() )
    {
        if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_ESCAPE ) )
            break;
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_R ) )
            simulation->Reset();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_P ) )
            simulation->running() ? simulation->Pause() : simulation->Resume();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_SPACE ) )
            sphere->AddForceCOM( { 0.0, 0.0, 1000.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_UP ) )
            sphere->AddForceCOM( { 0.0, 200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_DOWN ) )
            sphere->AddForceCOM( { 0.0, -200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_RIGHT ) )
            sphere->AddForceCOM( { 200.0, 0.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_LEFT ) )
            sphere->AddForceCOM( { -200.0, 0.0, 0.0 } );

        simulation->Step();
        visualizer->Update();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
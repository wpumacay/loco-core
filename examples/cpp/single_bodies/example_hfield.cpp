
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

const int nx_samples_hfield_1 = 40;
const int ny_samples_hfield_1 = 40;
const float x_extent_hfield_1 = 16.0f;
const float y_extent_hfield_1 = 8.0f;
std::vector<float> create_hfield_paraboloid();

std::vector<float> create_hfield_gaussian_bumps();

std::vector<float> create_hfield_deeploco_terrain();

std::vector<float> create_hfield_deepterrainrl_terrain_chunk();

std::vector<float> create_hfield_deepmimic_terrain_chunk();

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

    auto scenario = std::make_unique<loco::TScenario>();
    auto sphere = scenario->CreateSphere( "sphere", { 0.0, 0.0, 2.0 }, loco::TMat3(), 0.1 );
    auto hfield_1 = scenario->CreateHeightfield( "hfield_1", { 0.0, 0.0, 0.0 }, loco::TMat3(),
                                                 nx_samples_hfield_1, ny_samples_hfield_1,
                                                 x_extent_hfield_1, y_extent_hfield_1,
                                                 create_hfield_paraboloid() );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    hfield_1->drawable()->ChangeTexture( "built_in_chessboard" );

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

std::vector<float> create_hfield_paraboloid()
{
    const int nx_samples = nx_samples_hfield_1;
    const int ny_samples = ny_samples_hfield_1;
    const float x_extent = x_extent_hfield_1;
    const float y_extent = y_extent_hfield_1;

    std::vector<float> heights;
    for ( ssize_t i = 0; i < ny_samples; i++ )
    {
        for ( ssize_t j = 0; j < nx_samples; j++ )
        {
            float x = x_extent * ( (float)i / nx_samples - 0.5f );
            float y = y_extent * ( (float)j / ny_samples - 0.5f );
            heights.push_back( 15.0f * ( x * x + y * y ) / ( x_extent * x_extent + y_extent * y_extent ) );
        }
    }
    return heights;
}

std::vector<float> create_hfield_gaussian_bumps()
{
    return {};
}

std::vector<float> create_hfield_deeploco_terrain()
{
    return {};
}

std::vector<float> create_hfield_deepterrainrl_terrain_chunk()
{
    return {};
}

std::vector<float> create_hfield_deepmimic_terrain_chunk()
{
    return {};
}
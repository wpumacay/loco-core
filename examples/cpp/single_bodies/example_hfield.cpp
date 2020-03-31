
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

const int nx_samples_hfield_1 = 25;
const int ny_samples_hfield_1 = 25;
const float x_extent_hfield_1 = 10.0f;
const float y_extent_hfield_1 = 10.0f;
std::vector<float> create_hfield_paraboloid();

const int nx_samples_hfield_2 = 25;
const int ny_samples_hfield_2 = 25;
const float x_extent_hfield_2 = 10.0f;
const float y_extent_hfield_2 = 10.0f;
const int hfield_octaves = 4;
const float hfield_persistance = 0.5f;
const float hfield_lacunarity = 2.0f;
const float hfield_noise_scale = 5.0f;
std::vector<float> create_hfield_perlin_noise();

const int nx_samples_hfield_3 = 401;
const int ny_samples_hfield_3 = 401;
const float x_extent_hfield_3 = 40.0f;
const float y_extent_hfield_3 = 40.0f;
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
    auto floor = scenario->CreatePlane( "floor", { 0.0, 0.0, -0.001f }, loco::TMat3(), 40, 40 );
    auto sphere = scenario->CreateSphere( "sphere", { 0.0, 0.0, 2.0 }, loco::TMat3(), 0.1 );
    auto hfield_1 = scenario->CreateHeightfield( "hfield_1", { 0.0, 0.0, 0.0 }, loco::TMat3(),
                                                 nx_samples_hfield_1, ny_samples_hfield_1,
                                                 x_extent_hfield_1, y_extent_hfield_1,
                                                 create_hfield_paraboloid() );
    auto hfield_2 = scenario->CreateHeightfield( "hfield_2", { 10.0, 10.0, 0.0 }, loco::TMat3(),
                                                 nx_samples_hfield_2, ny_samples_hfield_2,
                                                 x_extent_hfield_2, y_extent_hfield_2,
                                                 create_hfield_perlin_noise() );
    auto hfield_3 = scenario->CreateHeightfield( "hfield_3", { -25.0, -25.0, 0.0 }, loco::TMat3(),
                                                 nx_samples_hfield_3, ny_samples_hfield_3,
                                                 x_extent_hfield_3, y_extent_hfield_3,
                                                 create_hfield_deeploco_terrain() );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_1->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_2->drawable()->ChangeTexture( "built_in_chessboard" );
    //// hfield_3->drawable()->ChangeTexture( "built_in_chessboard" );

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

std::vector<float> create_hfield_perlin_noise()
{
    // Initialize perlin-noise generator
    loco::TNoiseGenerator::Init();
    loco::TNoiseGenerator::Config( hfield_octaves, hfield_persistance, hfield_lacunarity, hfield_noise_scale );

    std::vector<float> heights;
    for ( ssize_t i = 0; i < ny_samples_hfield_2; i++ )
    {
        for ( ssize_t j = 0; j < nx_samples_hfield_2; j++ )
        {
            float x = x_extent_hfield_2 * ( (float)j / nx_samples_hfield_2 - 0.5f );
            float y = y_extent_hfield_2 * ( (float)i / ny_samples_hfield_2 - 0.5f );
            heights.push_back( std::max( 0.0f, loco::TNoiseGenerator::GetNoise2d( x, y ) ) );
        }
    }
    return heights;
}

std::vector<float> create_hfield_deeploco_terrain()
{
    const std::string terrain_file = loco::PATH_RESOURCES + "txt/terrain_deeploco.txt";
    std::fstream fhandle( terrain_file.c_str(), std::ios::in );
    std::string line = "";
    const ssize_t num_rows = 401;
    const ssize_t num_cols = 401;
    std::vector<float> heights( num_rows * num_cols );
    ssize_t i_line = 0;
    while( getline( fhandle, line ) )
    {
        auto vec_str = loco::Split( line, '\t' );
        assert( vec_str.size() == num_cols );
        for ( ssize_t j = 0; j < num_cols; j++ )
            heights[i_line + num_rows * j] = std::stof( vec_str[j] );
        i_line++;
    }
    assert( i_line == num_rows );
    // Make sure all heights are above 0.0f
    float min_height = *std::min_element( heights.cbegin(), heights.cend() );
    for ( ssize_t k = 0; k < heights.size(); k++ )
        heights[k] -= min_height;
    return heights;
}

std::vector<float> create_hfield_deepterrainrl_terrain_chunk()
{
    return {};
}

std::vector<float> create_hfield_deepmimic_terrain_chunk()
{
    return {};
}
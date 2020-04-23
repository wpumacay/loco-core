
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

const float x_extent_hfield_1 = 10.0f;
const float y_extent_hfield_1 = 10.0f;
const float x_step_hfield_1 = 0.05f;
const float y_step_hfield_1 = 2.0f;
const int nx_samples_hfield_1 = std::ceil( x_extent_hfield_1 / x_step_hfield_1 ) + 1;
const int ny_samples_hfield_1 = std::ceil( y_extent_hfield_1 / y_step_hfield_1 ) + 1;
const int num_samples_hfield_1 = nx_samples_hfield_1 * ny_samples_hfield_1;
std::vector<float> create_hfield_deepterrainrl_terrain_chunk_1();

const float x_extent_hfield_2 = 10.0f;
const float y_extent_hfield_2 = 10.0f;
const float x_step_hfield_2 = 0.05f;
const float y_step_hfield_2 = 2.0f;
const int nx_samples_hfield_2 = std::ceil( x_extent_hfield_2 / x_step_hfield_2 ) + 1;
const int ny_samples_hfield_2 = std::ceil( y_extent_hfield_2 / y_step_hfield_2 ) + 1;
const int num_samples_hfield_2 = nx_samples_hfield_2 * ny_samples_hfield_2;
std::vector<float> create_hfield_deepterrainrl_terrain_chunk_2();

const float x_extent_hfield_3 = 10.0f;
const float y_extent_hfield_3 = 10.0f;
const float x_step_hfield_3 = 0.05f;
const float y_step_hfield_3 = 2.0f;
const int nx_samples_hfield_3 = std::ceil( x_extent_hfield_3 / x_step_hfield_3 ) + 1;
const int ny_samples_hfield_3 = std::ceil( y_extent_hfield_3 / y_step_hfield_3 ) + 1;
const int num_samples_hfield_3 = nx_samples_hfield_3 * ny_samples_hfield_3;
std::vector<float> create_hfield_deepterrainrl_terrain_chunk_3();

const int hfield_octaves = 4;
const float hfield_persistance = 0.5f;
const float hfield_lacunarity = 2.0f;
const float hfield_noise_scale = 10.0f;

int main( int argc, char* argv[] )
{
    loco::TLogger::Init();
    loco::TNoiseGenerator::Init();
    loco::TNoiseGenerator::Config( hfield_octaves, hfield_persistance, hfield_lacunarity, hfield_noise_scale );

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

    auto sphere = scenario->AddSingleBody( std::make_unique<loco::TSphere>( "sphere", 0.1f, loco::TVec3( 0.0, 0.0, 5.0 ), loco::TMat3() ) );
    auto hfield_1 = scenario->AddSingleBody( std::make_unique<loco::THeightfield>( "hfield_1",
                                                                                   nx_samples_hfield_1, ny_samples_hfield_1,
                                                                                   x_extent_hfield_1, y_extent_hfield_1,
                                                                                   create_hfield_deepterrainrl_terrain_chunk_1(),
                                                                                   loco::TVec3(), loco::TMat3() ) );
    hfield_1->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );
    auto hfield_2 = scenario->AddSingleBody( std::make_unique<loco::THeightfield>( "hfield_2",
                                                                                   nx_samples_hfield_2, ny_samples_hfield_2,
                                                                                   x_extent_hfield_2, y_extent_hfield_2,
                                                                                   create_hfield_deepterrainrl_terrain_chunk_2(),
                                                                                   loco::TVec3( 11.0, 0.0, 0.0 ), loco::TMat3() ) );
    hfield_2->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );
    auto hfield_3 = scenario->AddSingleBody( std::make_unique<loco::THeightfield>( "hfield_3",
                                                                                   nx_samples_hfield_3, ny_samples_hfield_3,
                                                                                   x_extent_hfield_3, y_extent_hfield_3,
                                                                                   create_hfield_deepterrainrl_terrain_chunk_3(),
                                                                                   loco::TVec3( 22.0, 0.0, 0.0 ), loco::TMat3() ) );
    hfield_3->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_1->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_2->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_3->drawable()->ChangeTexture( "built_in_chessboard" );

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

std::vector<float> create_hfield_deepterrainrl_terrain_chunk_1()
{
    struct SlopeSegment
    {
        float length;
        float slope;
        bool has_obstacle;
        float obstacle_start;
        float obstacle_length;
    };

    const int num_segments = 4;
    const float segment_length = x_extent_hfield_1 / num_segments;
    const float obstacle_length = x_step_hfield_1 * 5.0f;
    const float obstacle_height = 0.3f;
    std::vector<SlopeSegment> segments = { { segment_length, 0.5f, true, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), obstacle_length },
                                           { segment_length, 0.25f, true, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), obstacle_length },
                                           { segment_length, -0.25f, true, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), obstacle_length },
                                           { segment_length, -0.5f, true, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), obstacle_length } };
    float z_seg_start = 1.0f;
    int idx_seg = 0;
    float cum_seg_length = segments[idx_seg].length;
    bool in_obstacle = false;
    float obstacle_z = 0.0f;
    std::vector<float> heights( num_samples_hfield_1, 0.0f );
    for ( ssize_t j = 0; j < nx_samples_hfield_1; j++ )
    {
        float x = j * x_step_hfield_1;
        if ( x >= cum_seg_length )
        {
            z_seg_start += segments[idx_seg].slope * segments[idx_seg].length;
            idx_seg = std::min( 3, idx_seg + 1 );
            cum_seg_length += segments[idx_seg].length;
            in_obstacle = false;
        }
        float x_seg_start = idx_seg * segment_length;
        float z = z_seg_start + segments[idx_seg].slope * ( x - x_seg_start );
        if ( segments[idx_seg].has_obstacle )
        {
            if ( in_obstacle )
            {
                if ( x < ( x_seg_start + segments[idx_seg].obstacle_start + segments[idx_seg].obstacle_length ) )
                {
                    z = obstacle_z;
                }
                else
                {
                    in_obstacle = false;
                    z += loco::TNoiseGenerator::GetNoise1d( x );
                }
            }
            else
            {
                if ( x >= ( x_seg_start + segments[idx_seg].obstacle_start ) )
                {
                    in_obstacle = true;
                    z += loco::TNoiseGenerator::GetNoise1d( x );
                    obstacle_z = z + obstacle_height;
                }
                else
                {
                    z += loco::TNoiseGenerator::GetNoise1d( x );
                }
            }
        }
        for ( ssize_t i = 0; i < ny_samples_hfield_1; i++ )
            heights[i * nx_samples_hfield_1 + j] = z;
    }
    return heights;
}

std::vector<float> create_hfield_deepterrainrl_terrain_chunk_2()
{
    struct FlatSegment
    {
        float length;
        float height;
        bool has_obstacle;
        float obstacle_start;
        float obstacle_length;
    };

    const int num_segments = 4;
    const float segment_length = x_extent_hfield_2 / num_segments;
    const float obstacle_length = x_step_hfield_2 * 5.0f;
    const float obstacle_height = 0.3f;
    std::vector<FlatSegment> segments = { { segment_length, 0.5f, true, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), obstacle_length },
                                          { segment_length, 1.0f, true, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), obstacle_length },
                                          { segment_length, 0.75f, true, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), obstacle_length },
                                          { segment_length, 0.25f, true, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), obstacle_length } };
    int idx_seg = 0;
    float base_height_z = 1.0f;
    float cum_seg_length = segments[idx_seg].length;
    bool in_obstacle = false;
    float obstacle_z = 0.0f;
    std::vector<float> heights( num_samples_hfield_2, 0.0f );
    for ( ssize_t j = 0; j < nx_samples_hfield_2; j++ )
    {
        float x = j * x_step_hfield_2;
        if ( x >= cum_seg_length )
        {
            idx_seg = std::min( 3, idx_seg + 1 );
            cum_seg_length += segments[idx_seg].length;
            in_obstacle = false;
        }
        float x_seg_start = idx_seg * segment_length;
        float z = base_height_z + segments[idx_seg].height;
        if ( segments[idx_seg].has_obstacle )
        {
            if ( in_obstacle )
            {
                if ( x < ( x_seg_start + segments[idx_seg].obstacle_start + segments[idx_seg].obstacle_length ) )
                    z = obstacle_z;
                else
                    in_obstacle = false;
            }
            else
            {
                if ( x >= ( x_seg_start + segments[idx_seg].obstacle_start ) )
                {
                    in_obstacle = true;
                    obstacle_z = z + obstacle_height;
                }
            }
        }
        for ( ssize_t i = 0; i < ny_samples_hfield_2; i++ )
            heights[i * nx_samples_hfield_2 + j] = z;
    }
    return heights;
}

std::vector<float> create_hfield_deepterrainrl_terrain_chunk_3()
{
    struct GapSegment
    {
        float length;
        float height;
        float gap_start;
        float gap_length;
    };

    const int num_segments = 4;
    const float segment_length = x_extent_hfield_3 / num_segments;
    const float gap_length = x_step_hfield_3 * 5.0f;
    const float gap_height = 0.3f;
    std::vector<GapSegment> segments = { { segment_length, 0.5f, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), gap_length },
                                         { segment_length, 0.5f, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), gap_length },
                                         { segment_length, 0.5f, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), gap_length },
                                         { segment_length, 0.5f, LOCO_RANDOM_UNIFORM( 0.3f * segment_length, 0.7f * segment_length ), gap_length } };
    int idx_seg = 0;
    float base_height_z = 1.0f;
    float cum_seg_length = segments[idx_seg].length;
    bool in_gap = false;
    std::vector<float> heights( num_samples_hfield_3, 0.0f );
    for ( ssize_t j = 0; j < nx_samples_hfield_3; j++ )
    {
        float x = j * x_step_hfield_3;
        if ( x >= cum_seg_length )
        {
            idx_seg = std::min( 3, idx_seg + 1 );
            cum_seg_length += segments[idx_seg].length;
            in_gap = false;
        }
        float x_seg_start = idx_seg * segment_length;
        float z = base_height_z + segments[idx_seg].height;

            if ( in_gap )
            {
                if ( x < ( x_seg_start + segments[idx_seg].gap_start + segments[idx_seg].gap_length ) )
                    z = base_height_z - 0.8f;
                else
                    in_gap = false;
            }
            else
            {
                if ( x >= ( x_seg_start + segments[idx_seg].gap_start ) )
                    in_gap = true;
            }
        for ( ssize_t i = 0; i < ny_samples_hfield_3; i++ )
            heights[i * nx_samples_hfield_3 + j] = z;
    }
    return heights;
}
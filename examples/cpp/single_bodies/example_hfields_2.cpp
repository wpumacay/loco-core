
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

const int nx_samples_hfield = 401;
const int ny_samples_hfield = 401;
const float x_extent_hfield = 80.0f;
const float y_extent_hfield = 80.0f;
std::vector<float> create_hfield_deeploco_terrain()
{
    const std::string terrain_file = loco::PATH_RESOURCES + "txt/terrain_deeploco.txt";
    std::fstream fhandle( terrain_file.c_str(), std::ios::in );
    std::string line = "";
    const ssize_t num_rows = ny_samples_hfield;
    const ssize_t num_cols = nx_samples_hfield;
    std::vector<float> heights( num_rows * num_cols );
    ssize_t i_line = 0;
    while( getline( fhandle, line ) )
    {
        auto vec_str = tinyutils::Split( line, '\t' );
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

    auto sphere = scenario->AddSingleBody( std::make_unique<loco::TSphere>( "sphere", 0.1f, loco::TVec3( 0.0, 0.0, 10.0 ), loco::TMat3() ) );
    auto hfield = scenario->AddSingleBody( std::make_unique<loco::THeightfield>( "hfield",
                                                                                 nx_samples_hfield, ny_samples_hfield,
                                                                                 x_extent_hfield, y_extent_hfield,
                                                                                 create_hfield_deeploco_terrain(),
                                                                                 loco::TVec3(), loco::TMat3() ) );
    hfield->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );

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
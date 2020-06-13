
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

const int nx_samples_hfield = 25;
const int ny_samples_hfield = 25;
const float x_extent_hfield = 10.0f;
const float y_extent_hfield = 10.0f;
const int hfield_octaves = 4;
const float hfield_persistance = 0.5f;
const float hfield_lacunarity = 2.0f;
const float hfield_noise_scale = 5.0f;
std::vector<float> create_hfield_perlin_noise( float x_offset, float y_offset )
{
    tinyutils::PerlinNoise::Config( hfield_octaves, hfield_persistance, hfield_lacunarity, hfield_noise_scale );

    std::vector<float> heights;
    for ( ssize_t i = 0; i < ny_samples_hfield; i++ )
    {
        for ( ssize_t j = 0; j < nx_samples_hfield; j++ )
        {
            float x = x_extent_hfield * ( (float)j / nx_samples_hfield - 0.5f ) + x_offset;
            float y = y_extent_hfield * ( (float)i / ny_samples_hfield - 0.5f ) + y_offset;
            heights.push_back( std::max( 0.0f, tinyutils::PerlinNoise::Sample2d( x, y ) ) );
        }
    }
    return heights;
}

const loco::TVec3 hfield_1_position = { -( x_extent_hfield / 2.0f + 1.0f ), -( x_extent_hfield / 2.0f + 1.0f ), 0.0f };
const loco::TVec3 hfield_2_position = { -( x_extent_hfield / 2.0f + 1.0f ), ( x_extent_hfield / 2.0f + 1.0f ), 0.0f };
const loco::TVec3 hfield_3_position = { ( x_extent_hfield / 2.0f + 1.0f ), ( x_extent_hfield / 2.0f + 1.0f ), 0.0f };
const loco::TVec3 hfield_4_position = { ( x_extent_hfield / 2.0f + 1.0f ), -( x_extent_hfield / 2.0f + 1.0f ), 0.0f };

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
    auto floor = scenario->AddSingleBody( std::make_unique<loco::primitives::TPlane>( "floor", 40.0f, 40.0f, loco::TVec3( 0.0f, 0.0f, -0.001f ), loco::TMat3() ) );
    auto user_sphere = scenario->AddSingleBody( std::make_unique<loco::primitives::TSphere>( "user_sphere", 0.1f, loco::TVec3( 0.0, 0.0, 3.0 ), loco::TMat3() ) );

    auto hfield_1 = scenario->AddSingleBody( std::make_unique<loco::primitives::THeightfield>( "hfield_1",
                                                                                   nx_samples_hfield, ny_samples_hfield,
                                                                                   x_extent_hfield, y_extent_hfield,
                                                                                   create_hfield_perlin_noise( hfield_1_position.x(), hfield_1_position.y() ),
                                                                                   hfield_1_position, loco::TMat3() ) );
    auto hfield_2 = scenario->AddSingleBody( std::make_unique<loco::primitives::THeightfield>( "hfield_2",
                                                                                   nx_samples_hfield, ny_samples_hfield,
                                                                                   x_extent_hfield, y_extent_hfield,
                                                                                   create_hfield_perlin_noise( hfield_2_position.x(), hfield_2_position.y() ),
                                                                                   hfield_2_position, loco::TMat3() ) );
    auto hfield_3 = scenario->AddSingleBody( std::make_unique<loco::primitives::THeightfield>( "hfield_3",
                                                                                   nx_samples_hfield, ny_samples_hfield,
                                                                                   x_extent_hfield, y_extent_hfield,
                                                                                   create_hfield_perlin_noise( hfield_3_position.x(), hfield_3_position.y() ),
                                                                                   hfield_3_position, loco::TMat3() ) );
    auto hfield_4 = scenario->AddSingleBody( std::make_unique<loco::primitives::THeightfield>( "hfield_4",
                                                                                   nx_samples_hfield, ny_samples_hfield,
                                                                                   x_extent_hfield, y_extent_hfield,
                                                                                   create_hfield_perlin_noise( hfield_4_position.x(), hfield_4_position.y() ),
                                                                                   hfield_4_position, loco::TMat3() ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    floor->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );
    hfield_1->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );
    hfield_2->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );
    hfield_3->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );
    hfield_4->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );
    hfield_1->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_2->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_3->drawable()->ChangeTexture( "built_in_chessboard" );
    hfield_4->drawable()->ChangeTexture( "built_in_chessboard" );

    std::array<loco::TVec3, 4> offsets = { hfield_1_position, hfield_2_position, hfield_3_position, hfield_4_position };
    std::array<loco::primitives::TSingleBody*, 4> hfields = { hfield_1, hfield_2, hfield_3, hfield_4 };
    ssize_t indx_hfield = 0;

    while ( visualizer->IsActive() )
    {
        if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_ESCAPE ) )
            break;
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_R ) )
            simulation->Reset();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_P ) )
            simulation->running() ? simulation->Pause() : simulation->Resume();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_SPACE ) )
            user_sphere->AddForceCOM( { 0.0, 0.0, 1000.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_UP ) )
            user_sphere->AddForceCOM( { 0.0, 200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_DOWN ) )
            user_sphere->AddForceCOM( { 0.0, -200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_RIGHT ) )
            user_sphere->AddForceCOM( { 200.0, 0.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_LEFT ) )
            user_sphere->AddForceCOM( { -200.0, 0.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_H ) )
        {
            for ( ssize_t i = 0; i < 4; i++ )
            {
                float x_offset = offsets[(i + indx_hfield) % 4].x();
                float y_offset = offsets[(i + indx_hfield) % 4].y();
                static_cast<loco::primitives::THeightfield*>( hfields[i] )->SetHeights( create_hfield_perlin_noise( x_offset, y_offset ) );
            }
            indx_hfield = (indx_hfield + 1) % 4;
        }

        simulation->Step(1. / 60.);
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}

#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

const loco::TVec3 COM_TETRAHEDRON = { 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f };

std::vector<float> TETRAHEDRON_VERTICES = { 0.0f - COM_TETRAHEDRON.x(), 0.0f - COM_TETRAHEDRON.y(), 0.0f - COM_TETRAHEDRON.z(),
                                            1.0f - COM_TETRAHEDRON.x(), 0.0f - COM_TETRAHEDRON.y(), 0.0f - COM_TETRAHEDRON.z(),
                                            0.0f - COM_TETRAHEDRON.x(), 1.0f - COM_TETRAHEDRON.y(), 0.0f - COM_TETRAHEDRON.z(),
                                            0.0f - COM_TETRAHEDRON.x(), 0.0f - COM_TETRAHEDRON.y(), 1.0f - COM_TETRAHEDRON.z() };
std::vector<int> TETRAHEDRON_FACES = { 0, 1, 3,
                                       0, 2, 1,
                                       0, 3, 2,
                                       1, 2, 3 };

const loco::TVec3 COM_RAMP = { 0.0f, 7.0f / 9.0f, 4.0f / 9.0f };

std::vector<float> RAMP_VERTICES = { 1.0f - COM_RAMP.x(), 0.0f - COM_RAMP.y(), 0.0f - COM_RAMP.z(),
                                     1.0f - COM_RAMP.x(), 2.0f - COM_RAMP.y(), 0.0f - COM_RAMP.z(),
                                     1.0f - COM_RAMP.x(), 1.0f - COM_RAMP.y(), 1.0f - COM_RAMP.z(),
                                     1.0f - COM_RAMP.x(), 0.0f - COM_RAMP.y(), 1.0f - COM_RAMP.z(),
                                    -1.0f - COM_RAMP.x(), 0.0f - COM_RAMP.y(), 0.0f - COM_RAMP.z(),
                                    -1.0f - COM_RAMP.x(), 2.0f - COM_RAMP.y(), 0.0f - COM_RAMP.z(),
                                    -1.0f - COM_RAMP.x(), 1.0f - COM_RAMP.y(), 1.0f - COM_RAMP.z(),
                                    -1.0f - COM_RAMP.x(), 0.0f - COM_RAMP.y(), 1.0f - COM_RAMP.z() };
std::vector<int> RAMP_FACES = { 0, 1, 2,
                                0, 2, 3,
                                0, 4, 5,
                                0, 5, 1,
                                0, 3, 7,
                                0, 7, 4,
                                2, 6, 7,
                                2, 7, 3,
                                1, 5, 6,
                                1, 6, 2,
                                4, 7, 6,
                                4, 6, 5 };

std::pair<std::vector<float>, std::vector<int>> create_path_part( ssize_t idx )
{
    const float height = 1.0;
    const float inner_rad = 2.0;
    const float outer_rad = 3.0;
    const float dtheta = 2.0 * loco::PI / 12.0;
    const float ctheta = std::cos( dtheta * idx );
    const float stheta = std::sin( dtheta * idx );
    const float ctheta_n = std::cos( dtheta * ( idx + 1 ) );
    const float stheta_n = std::sin( dtheta * ( idx + 1 ) );

    const float half_rad = 0.5* ( inner_rad + outer_rad );
    const loco::TVec3 com_position = { half_rad * std::cos( ( idx + 0.5f ) * dtheta ),
                                       half_rad * std::sin( ( idx + 0.5f ) * dtheta ),
                                       0.5 * height };

    std::vector<float> vertices = { inner_rad * ctheta   - com_position.x(), inner_rad * stheta   - com_position.y(), 0.5f * height,
                                    outer_rad * ctheta   - com_position.x(), outer_rad * stheta   - com_position.y(), 0.5f * height,
                                    outer_rad * ctheta_n - com_position.x(), outer_rad * stheta_n - com_position.y(), 0.5f * height,
                                    inner_rad * ctheta_n - com_position.x(), inner_rad * stheta_n - com_position.y(), 0.5f * height,
                                    inner_rad * ctheta   - com_position.x(), inner_rad * stheta   - com_position.y(), -0.5f * height,
                                    outer_rad * ctheta   - com_position.x(), outer_rad * stheta   - com_position.y(), -0.5f * height,
                                    outer_rad * ctheta_n - com_position.x(), outer_rad * stheta_n - com_position.y(), -0.5f * height,
                                    inner_rad * ctheta_n - com_position.x(), inner_rad * stheta_n - com_position.y(), -0.5f * height };
    std::vector<int> faces = { 0, 1, 2,
                               0, 2, 3,
                               0, 4, 5,
                               0, 5, 1,
                               0, 3, 7,
                               0, 7, 4,
                               2, 6, 7,
                               2, 7, 3,
                               1, 5, 6,
                               1, 6, 2,
                               4, 7, 6,
                               4, 6, 5 };

    return { vertices, faces };
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
    scenario->AddSingleBody( std::make_unique<loco::TPlane>( "floor", 10.0f, 10.0f, loco::TVec3(), loco::TMat3() ) );
    scenario->AddSingleBody( std::make_unique<loco::TSphere>( "sphere", 0.1f, loco::TVec3( 1.0f, -1.0f, 2.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::TMesh>( "tetrahedron_0",
                                                            TETRAHEDRON_VERTICES,
                                                            TETRAHEDRON_FACES,
                                                            1.0f, loco::TVec3( -1.0f, -1.0f, 1.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::TMesh>( "tetrahedron_1",
                                                            TETRAHEDRON_VERTICES,
                                                            TETRAHEDRON_FACES,
                                                            0.5f, loco::TVec3( -1.0f, 1.0f, 1.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::TMesh>( "ramp_0",
                                                            RAMP_VERTICES,
                                                            RAMP_FACES,
                                                            0.3f, loco::TVec3( 1.0f, 1.0f, 1.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::TMesh>( "ramp_1",
                                                            RAMP_VERTICES,
                                                            RAMP_FACES,
                                                            0.5f, loco::TVec3( 1.0f, -1.0f, 1.0f ), rotation ) );
    for ( ssize_t i = 0; i < 12; i++ )
    {
        const float height = 1.0f;
        const float inner_rad = 2.0f;
        const float outer_rad = 3.0f;
        const float half_rad = 0.5f* ( inner_rad + outer_rad );
        const float dtheta = 2.0f * loco::PI / 12.0f;

        const loco::TVec3 com_position = { half_rad * std::cos( ( i + 0.5f ) * dtheta ),
                                           half_rad * std::sin( ( i + 0.5f ) * dtheta ),
                                           0.5f * height };

        auto vertex_data = create_path_part( i );
        scenario->AddSingleBody( std::make_unique<loco::TMesh>( "path_part_" + std::to_string( i ),
                                                                vertex_data.first,
                                                                vertex_data.second,
                                                                1.0f, com_position, loco::TMat3(),
                                                                loco::eDynamicsType::STATIC ) );
    }

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, loco::config::rendering::GLVIZ_GLFW );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    auto sphere_ref = scenario->GetSingleBodyByName( "sphere" );
    auto floor_ref = scenario->GetSingleBodyByName( "floor" );
    floor_ref->drawable()->ChangeTexture( "built_in_chessboard" );
    floor_ref->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );
    for ( ssize_t i = 0; i < 2; i++ )
        if ( auto ramp_ref = scenario->GetSingleBodyByName( "ramp_" + std::to_string( i ) ) )
            ramp_ref->drawable()->ChangeColor( { 0.3f, 0.7f, 0.5f } );

    for ( ssize_t i = 0; i < 12; i++ )
        if ( auto path_part_ref = scenario->GetSingleBodyByName( "path_part_" + std::to_string( i ) ) )
            path_part_ref->drawable()->ChangeColor( { 0.2f, 0.8f, 0.2f } );

    while ( visualizer->IsActive() )
    {
        if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_ESCAPE ) )
            break;
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_R ) )
            simulation->Reset();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_P ) )
            simulation->running() ? simulation->Pause() : simulation->Resume();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_L ) )
            for ( auto single_body : scenario->GetSingleBodiesList() )
                single_body->SetLinearVelocity( { 0.0, 0.0, 5.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_A ) )
            for ( auto single_body : scenario->GetSingleBodiesList() )
                single_body->SetAngularVelocity( { 0.0, 0.0, 3.1415 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_SPACE ) )
            sphere_ref->AddForceCOM( { 0.0, 0.0, 1000.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_UP ) )
            sphere_ref->AddForceCOM( { 0.0, 200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_DOWN ) )
            sphere_ref->AddForceCOM( { 0.0, -200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_RIGHT ) )
            sphere_ref->AddForceCOM( { 200.0, 0.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_LEFT ) )
            sphere_ref->AddForceCOM( { -200.0, 0.0, 0.0 } );

        simulation->Step();
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
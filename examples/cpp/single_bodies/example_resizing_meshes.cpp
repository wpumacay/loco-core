
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

std::array<std::vector<float>, 4> meshes_vertices = { TETRAHEDRON_VERTICES, TETRAHEDRON_VERTICES, RAMP_VERTICES, RAMP_VERTICES };
std::array<std::vector<int>, 4> meshes_indices = { TETRAHEDRON_FACES, TETRAHEDRON_FACES, RAMP_FACES, RAMP_FACES };

const loco::TVec3 mesh_1_position = { -2.0f, 0.0f, 1.0f };
const loco::TVec3 mesh_2_position = { -1.0f, 0.0f, 1.0f };
const loco::TVec3 mesh_3_position = { 1.0f, 0.0f, 1.0f };
const loco::TVec3 mesh_4_position = { 2.0f, 0.0f, 1.0f };

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
    auto user_sphere = scenario->AddSingleBody( std::make_unique<loco::TSphere>( "user_sphere", 0.1f, loco::TVec3( 0.0, 0.0, 3.0 ), loco::TMat3() ) );

    auto mesh_1 = scenario->AddSingleBody( std::make_unique<loco::TMesh>( "mesh_1",
                                                                          TETRAHEDRON_VERTICES,
                                                                          TETRAHEDRON_FACES,
                                                                          1.0f, mesh_1_position, loco::TMat3() ) );
    auto mesh_2 = scenario->AddSingleBody( std::make_unique<loco::TMesh>( "mesh_2",
                                                                          TETRAHEDRON_VERTICES,
                                                                          TETRAHEDRON_FACES,
                                                                          0.5f, mesh_2_position, loco::TMat3() ) );
    auto mesh_3 = scenario->AddSingleBody( std::make_unique<loco::TMesh>( "mesh_3",
                                                                          RAMP_VERTICES,
                                                                          RAMP_FACES,
                                                                          0.3f, mesh_3_position, loco::TMat3() ) );
    auto mesh_4 = scenario->AddSingleBody( std::make_unique<loco::TMesh>( "mesh_4",
                                                                          RAMP_VERTICES,
                                                                          RAMP_FACES,
                                                                          0.5f, mesh_4_position, loco::TMat3() ) );


    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    floor->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );
    mesh_1->drawable()->ChangeColor( { 0.8f, 0.1f, 0.1f } );
    mesh_2->drawable()->ChangeColor( { 0.1f, 0.8f, 0.1f } );
    mesh_3->drawable()->ChangeColor( { 0.1f, 0.1f, 0.8f } );
    mesh_4->drawable()->ChangeColor( { 0.5f, 0.5f, 0.5f } );

    std::array<loco::TSingleBody*, 4> meshes = { mesh_1, mesh_2, mesh_3, mesh_4 };
    ssize_t indx_mesh = 0;

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
                static_cast<loco::TMesh*>( meshes[i] )->SetVertexData( meshes_vertices[(i + indx_mesh) % 4],
                                                                       meshes_indices[(i + indx_mesh) % 4] );
            indx_mesh = (indx_mesh + 1) % 4;
        }

        simulation->Step(1. / 60.);
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();
    return 0;
}
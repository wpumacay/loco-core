
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

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

    const loco::TVec3 floor_position = { 0.0, 0.0, 0.0 };
    const loco::TMat3 floor_rotation = loco::TMat3();
    const float floor_width = 20.0f;
    const float floor_depth = 20.0f;
    const int floor_colgroup = 0x7fffffff;
    const int floor_colmask = 0x7fffffff;
    auto floor = scenario->AddSingleBody( std::make_unique<loco::TPlane>( "floor",
                                                                          floor_width, floor_depth,
                                                                          floor_position, floor_rotation,
                                                                          floor_colgroup, floor_colmask ) );
    floor->drawable()->ChangeColor( { 0.3, 0.5, 0.7 } );
    floor->collider()->ChangeFriction( 0.1 );

    auto box_1 = scenario->AddSingleBody( std::make_unique<loco::TBox>( "box_1", loco::TVec3( 0.2f, 0.2f, 0.2f ), loco::TVec3( 1.0, 1.0, 0.1 ), loco::TMat3(),
                                                                        loco::eDynamicsType::DYNAMIC ) );
    box_1->SetInitialLinearVelocity( { 0.0, 5.0, 0.0 } );
    box_1->drawable()->ChangeColor( { 0.7, 0.5, 0.3 } );
    box_1->collider()->ChangeFriction( 0.4 );

    auto box_2 = scenario->AddSingleBody( std::make_unique<loco::TBox>( "box_2", loco::TVec3( 0.2f, 0.2f, 0.2f ), loco::TVec3( 2.0, 1.0, 0.1 ), loco::TMat3(),
                                                                        loco::eDynamicsType::DYNAMIC ) );
    box_2->SetInitialLinearVelocity( { 0.0, 5.0, 0.0 } );
    box_2->drawable()->ChangeColor( { 0.3, 0.7, 0.5 } );
    box_2->collider()->ChangeFriction( 0.9 );

    auto sphere_1 = scenario->AddSingleBody( std::make_unique<loco::TSphere>( "sphere_1", 0.2f, loco::TVec3( 3.0, 1.0, 0.2 ), loco::TMat3(),
                                                                              loco::eDynamicsType::DYNAMIC ) );
    sphere_1->SetInitialLinearVelocity( { 0.0, 0.5, 0.0 } );
    sphere_1->drawable()->ChangeColor( { 0.5, 0.3, 0.7 } );
    sphere_1->collider()->ChangeFriction( 0.4 );

    auto sphere_2 = scenario->AddSingleBody( std::make_unique<loco::TSphere>( "sphere_2", 0.2f, loco::TVec3( 4.0, 1.0, 0.2 ), loco::TMat3(),
                                                                              loco::eDynamicsType::DYNAMIC ) );
    sphere_2->SetInitialLinearVelocity( { 0.0, 0.5, 0.0 } );
    sphere_2->drawable()->ChangeColor( { 0.5, 0.5, 0.5 } );
    sphere_2->collider()->ChangeFriction( 0.9 );

    // Should collide with all but 2_2
    auto sphere_user = scenario->AddSingleBody( std::make_unique<loco::TSphere>( "sphere_user", 0.1f, loco::TVec3( 0.0, 0.0, 0.1 ), loco::TMat3(),
                                                                                 loco::eDynamicsType::DYNAMIC ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    sphere_user->drawable()->ChangeTexture( "img_grid" );
    box_1->drawable()->ChangeTexture( "img_grid" );
    box_2->drawable()->ChangeTexture( "img_grid" );
    sphere_1->drawable()->ChangeTexture( "img_grid" );
    sphere_2->drawable()->ChangeTexture( "img_grid" );

    while ( visualizer->IsActive() )
    {
        if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_ESCAPE ) )
            break;
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_R ) )
            simulation->Reset();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_P ) )
            simulation->running() ? simulation->Pause() : simulation->Resume();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_SPACE ) )
            sphere_user->AddForceCOM( { 0.0, 0.0, 1000.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_UP ) )
            sphere_user->AddForceCOM( { 0.0, 200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_DOWN ) )
            sphere_user->AddForceCOM( { 0.0, -200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_RIGHT ) )
            sphere_user->AddForceCOM( { 200.0, 0.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_LEFT ) )
            sphere_user->AddForceCOM( { -200.0, 0.0, 0.0 } );

        simulation->Step( 1. / 60. );
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();
    return 0;
}
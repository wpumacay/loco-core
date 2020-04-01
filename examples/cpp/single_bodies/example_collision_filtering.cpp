
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

// Collision filtering works in the following way (mechanism from ODE physics engine):
//  * Each collider in a body has both collisionGroup and collisionMask filter options
//  * A pair of bodies is considered for collision-checking if the expression below is true:
//      ( collisionGroup-1 & collisionMask-2 ) || ( collisionGroup-2 & collisionMask-1 )

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

    const loco::TVec3 floor_position = { 0.0, 0.0, 0.0 };
    const loco::TMat3 floor_rotation = loco::TMat3();
    const float floor_width = 20.0f;
    const float floor_depth = 20.0f;
    const int floor_colgroup = 0x7fffffff;
    const int floor_colmask = 0x7fffffff;
    auto floor = scenario->CreatePlane( "floor", floor_position, floor_rotation,
                                        floor_width, floor_depth, floor_colgroup, floor_colmask );

    // Create some sphere-pairs, one in each pair moving in the direction of the other
    // such that they would collide if to their collision groups and masks match

    auto sphere_1_1 = scenario->CreateSphere( "sphere_1_1", { 1.0, 1.0, 0.2 }, loco::TMat3(), 0.2,
                                              loco::eDynamicsType::DYNAMIC, 0b0001, 0b0001 );
    auto sphere_1_2 = scenario->CreateSphere( "sphere_1_2", { 1.0, 3.0, 0.2 }, loco::TMat3(), 0.2,
                                              loco::eDynamicsType::DYNAMIC, 0b1111, 0b1111 );
    sphere_1_1->SetInitialLinearVelocity( { 0.0, 0.5, 0.0 } );
    sphere_1_1->drawable()->ChangeColor( { 0.7, 0.5, 0.3 } );
    sphere_1_2->drawable()->ChangeColor( { 0.7, 0.5, 0.3 } );

    auto sphere_2_1 = scenario->CreateSphere( "sphere_2_1", { 2.0, 1.0, 0.2 }, loco::TMat3(), 0.2,
                                              loco::eDynamicsType::DYNAMIC, 0b0001, 0b0001 );
    auto sphere_2_2 = scenario->CreateSphere( "sphere_2_2", { 2.0, 3.0, 0.2 }, loco::TMat3(), 0.2,
                                              loco::eDynamicsType::DYNAMIC, 0b0010, 0b0010 );
    sphere_2_1->SetInitialLinearVelocity( { 0.0, 0.5, 0.0 } );
    sphere_2_1->drawable()->ChangeColor( { 0.3, 0.7, 0.5 } );
    sphere_2_2->drawable()->ChangeColor( { 0.3, 0.7, 0.5 } );

    auto sphere_3_1 = scenario->CreateSphere( "sphere_3_1", { 3.0, 1.0, 0.2 }, loco::TMat3(), 0.2,
                                              loco::eDynamicsType::DYNAMIC, 0b0011, 0b0011 );
    auto sphere_3_2 = scenario->CreateSphere( "sphere_3_2", { 3.0, 3.0, 0.2 }, loco::TMat3(), 0.2,
                                              loco::eDynamicsType::DYNAMIC, 0b0010, 0b0010 );
    sphere_3_1->SetInitialLinearVelocity( { 0.0, 0.5, 0.0 } );
    sphere_3_1->drawable()->ChangeColor( { 0.5, 0.3, 0.7 } );
    sphere_3_2->drawable()->ChangeColor( { 0.5, 0.3, 0.7 } );

    auto sphere_user = scenario->CreateSphere( "sphere_user", { 0.0, 0.0, 0.1 }, loco::TMat3(), 0.1,
                                              loco::eDynamicsType::DYNAMIC, 0b0001, 0b0001 );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    sphere_user->drawable()->ChangeTexture( "img_grid" );
    sphere_1_1->drawable()->ChangeTexture( "img_grid" );
    sphere_1_2->drawable()->ChangeTexture( "img_grid" );
    sphere_2_1->drawable()->ChangeTexture( "img_grid" );
    sphere_2_2->drawable()->ChangeTexture( "img_grid" );
    sphere_3_1->drawable()->ChangeTexture( "img_grid" );
    sphere_3_2->drawable()->ChangeTexture( "img_grid" );

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

        simulation->Step();
        visualizer->Update();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
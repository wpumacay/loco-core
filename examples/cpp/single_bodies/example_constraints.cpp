
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

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
    auto sphere = scenario->CreateSphere( "sphere", { 0.0, 0.0, 5.0 }, loco::TMat3(), 0.1 );
    auto block = scenario->CreateBox( "block", { 0.0, 0.0, 2.0 }, tinymath::rotationX<loco::TScalar>( loco::PI / 3.0f ),
                                      { 0.2, 0.2, 1.0 } );
    auto door = scenario->CreateBox( "door", { -0.5, 1.0, 0.1 }, tinymath::rotationY<loco::TScalar>( loco::PI / 2.0f ),
                                      { 0.2, 0.2, 1.0 } );

    auto block_constraint = std::make_unique<loco::TSingleBodyRevoluteConstraint>( "block_constraint",
                                                                                   loco::TMat4(),
                                                                                   loco::TVec3( 1.0f, 0.0f, 0.0f ) );
    block->SetConstraint( std::move( block_constraint ) );

    auto door_constraint = std::make_unique<loco::TSingleBodyRevoluteConstraint>( "door_constraint",
                                                                                  loco::TMat4( loco::TMat3(), { 0.0f, 0.0f, 0.5f } ),
                                                                                  loco::TVec3( 1.0f, 0.0f, 0.0f ) );
    door->SetConstraint( std::move( door_constraint ) );

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
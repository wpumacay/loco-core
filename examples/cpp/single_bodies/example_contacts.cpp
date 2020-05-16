
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

    //// const loco::TMat3 rotation = tinymath::rotation( loco::TVec3( loco::PI / 3, loco::PI / 4, loco::PI / 6 ) );
    //// const loco::TMat3 rotation = tinymath::rotation( loco::TVec3( loco::PI / 2, 0.0f, 0.0f ) );
    const loco::TMat3 rotation = tinymath::rotation( loco::TVec3( 0.0f, 0.0f, 0.0f ) );

    auto scenario = std::make_unique<loco::TScenario>();
    auto floor = scenario->AddSingleBody( std::make_unique<loco::TPlane>( "floor", 10.0f, 10.0f, loco::TVec3(), loco::TMat3() ) );
    auto sphere = scenario->AddSingleBody( std::make_unique<loco::TSphere>( "sphere", 0.1f, loco::TVec3( 1.0f, -1.0f, 2.0f ), loco::TMat3() ) );
    auto goal = scenario->AddSingleBody( std::make_unique<loco::TSphere>( "goal", 0.4f, loco::TVec3( 0.0f, 0.0f, 0.4f ), loco::TMat3() ) );
    auto box = scenario->AddSingleBody( std::make_unique<loco::TBox>( "box", loco::TVec3( 0.2f, 0.3f, 0.4f ), loco::TVec3( -1.0f, -1.0f, 2.0f ), rotation ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    floor->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );

    goal->drawable()->ChangeTexture( "built_in_chessboard" );
    goal->drawable()->ChangeColor( { 0.8f, 0.1f, 0.1f } );

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
            sphere->AddForceCOM( { 0.0, 0.0, 1000.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_UP ) )
            sphere->AddForceCOM( { 0.0, 200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_DOWN ) )
            sphere->AddForceCOM( { 0.0, -200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_RIGHT ) )
            sphere->AddForceCOM( { 200.0, 0.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_LEFT ) )
            sphere->AddForceCOM( { -200.0, 0.0, 0.0 } );

        for ( auto contact : sphere->collider()->contacts() )
        {
            if ( goal->collider()->name() == contact.name )
            {
                LOCO_TRACE( "Reached goal, restarting demo" );
                simulation->Reset();
            }
            else if ( box->collider()->name() == contact.name )
            {
                LOCO_TRACE( "Reached box, restarting only the position of the sphere" );
                sphere->SetPosition( { 1.0f, -1.0f, 2.0f } );
            }
        }

        simulation->Step( 1. / 60. );
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
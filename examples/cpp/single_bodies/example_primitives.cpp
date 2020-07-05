
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
    scenario->AddSingleBody( std::make_unique<loco::primitives::TPlane>( "floor", 10.0f, 10.0f, loco::TVec3(), loco::TMat3() ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "box", loco::TVec3( 0.2f, 0.3f, 0.4f ), loco::TVec3( -1.0f, -1.0f, 2.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TSphere>( "sphere", 0.1f, loco::TVec3( 1.0f, -1.0f, 2.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TCylinder>( "cylinder", 0.2f, 0.5f, loco::TVec3( -1.0f, 1.0f, 2.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TCapsule>( "capsule", 0.2f, 0.5f, loco::TVec3( 1.0f, 1.0f, 2.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TEllipsoid>( "ellipsoid", loco::TVec3( 0.2f, 0.3f, 0.4f ), loco::TVec3( 0.0f, 0.0f, 2.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TConvexMesh>( "monkey",
                                                                        loco::PATH_RESOURCES + "meshes/monkey.stl",
                                                                        loco::PATH_RESOURCES + "meshes/monkey.obj",
                                                                        0.2f, loco::TVec3( 2.0f, 2.0f, 2.0f ), rotation ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    auto sphere_ref = scenario->GetSingleBodyByName( "sphere" );
    auto floor_ref = scenario->GetSingleBodyByName( "floor" );
    floor_ref->drawable()->ChangeTexture( "built_in_chessboard" );
    floor_ref->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );

    loco::visualizer::eRenderMode render_mode = loco::visualizer::eRenderMode::NORMAL;
    tinyutils::Profiler::BeginSession( "whole-framework" );
    while ( visualizer->IsActive() )
    {
        LOCO_CORE_PROFILE_SCOPE_IN_SESSION("simulation-loop","whole-framework");
        tinyutils::Clock::Tick();
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
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_N ) )
            render_mode = loco::visualizer::eRenderMode::NORMAL;
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_S ) )
            render_mode = loco::visualizer::eRenderMode::SEMANTIC;
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_D ) )
            render_mode = loco::visualizer::eRenderMode::DEPTH;

        simulation->Step();
        visualizer->Render( render_mode );
        tinyutils::Clock::Tock();
        LOCO_TRACE( "fps        : {0}", tinyutils::Clock::GetAvgFps() );
        LOCO_TRACE( "time-step  : {0}", tinyutils::Clock::GetAvgTimeStep() );
    }
    tinyutils::Profiler::EndSession( "whole-framework" );

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
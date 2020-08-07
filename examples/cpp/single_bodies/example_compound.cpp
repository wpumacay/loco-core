
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
    scenario->AddSingleBody( std::make_unique<loco::primitives::TSphere>( "sphere", 0.1f, loco::TVec3( 1.0f, -1.0f, 2.0f ), rotation ) );

    auto compound = std::make_unique<loco::primitives::TCompound>( "humanoid-top", loco::TVec3( 1.0f, 1.0f, 2.0f ), rotation );

    auto head_data = loco::TShapeData();
    head_data.type = loco::eShapeType::SPHERE;
    head_data.size = { 0.09f, 0.0f, 0.0f };
    auto head_tf = loco::TMat4( loco::TMat3(), loco::TVec3( 0.0f, 0.0f, 0.19f ) );
    compound->AddChild( head_data, head_tf );

    auto torso_data = loco::TShapeData();
    torso_data.type = loco::eShapeType::CAPSULE;
    torso_data.size = { 0.07f, 0.14f, 0.0f };
    auto torso_tf = loco::TMat4( tinymath::rotation( loco::TVec3( loco::PI / 2, 0.0f, 0.0f ) ), loco::TVec3( 0.0f, 0.0f, 0.0f ) );
    compound->AddChild( torso_data, torso_tf );

    auto waist_data = loco::TShapeData();
    waist_data.type = loco::eShapeType::CAPSULE;
    waist_data.size = { 0.06f, 0.12f, 0.0f };
    auto waist_tf = loco::TMat4( tinymath::rotation( loco::TVec3( loco::PI / 2, 0.0f, 0.0f ) ), loco::TVec3( -0.01f, 0.0f, -0.12f ) );
    compound->AddChild( waist_data, waist_tf );

    scenario->AddSingleBody( std::move( compound ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    auto sphere_ref = scenario->GetSingleBodyByName( "sphere" );
    auto floor_ref = scenario->GetSingleBodyByName( "floor" );
    floor_ref->drawable()->ChangeTexture( "built_in_chessboard" );
    floor_ref->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );

    constexpr float sim_step = 1.0f / 60.0f;
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

        simulation->Step( sim_step );
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
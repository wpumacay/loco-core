
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
    auto floor = scenario->AddSingleBody( std::make_unique<loco::primitives::TPlane>( "floor", 10.0f, 10.0f, 
                                                                          loco::TVec3(), loco::TMat3() ) );

    auto sphere = scenario->AddSingleBody( std::make_unique<loco::primitives::TSphere>( "sphere", 0.1f,
                                                                            loco::TVec3( 0.0, 0.0, 5.0 ),
                                                                            loco::TMat3() ) );
    auto sphere_constraint = std::make_unique<loco::primitives::TSingleBodyUniversal3dConstraint>( "sphere_constraint" );
    sphere->SetConstraint( std::move( sphere_constraint ) );

    auto block = scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "block", loco::TVec3( 0.2, 0.2, 1.0 ),
                                                                        loco::TVec3( 0.0, 0.0, 2.0 ),
                                                                        tinymath::rotationX<loco::TScalar>( loco::PI / 3.0f ) ) );
    auto block_constraint = std::make_unique<loco::primitives::TSingleBodyRevoluteConstraint>( "block_constraint",
                                                                                   loco::TMat4(),
                                                                                   loco::TVec3( 1.0f, 0.0f, 0.0f ) );
    block->SetConstraint( std::move( block_constraint ) );

    auto door = scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "door", loco::TVec3( 0.2, 0.2, 1.0 ),
                                                                       loco::TVec3( -0.5, 1.0, 0.1 ),
                                                                       tinymath::rotationY<loco::TScalar>( loco::PI / 2.0f ) ) );
    auto door_constraint = std::make_unique<loco::primitives::TSingleBodyRevoluteConstraint>( "door_constraint",
                                                                                  loco::TMat4( loco::TMat3(),
                                                                                               { 0.0f, 0.0f, 0.5f } ),
                                                                                  loco::TVec3( 1.0f, 0.0f, 0.0f ) );
    door->SetConstraint( std::move( door_constraint ) );

    auto platform = scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "platform", loco::TVec3( 1.0, 1.0, 0.2 ),
                                                                           loco::TVec3( 0.0, -2.0, 2.0 ),
                                                                           tinymath::rotationY<loco::TScalar>( loco::PI / 3.0f ) ) );
    auto platform_constraint = std::make_unique<loco::primitives::TSingleBodyPrismaticConstraint>( "platform_constraint",
                                                                                       loco::TMat4( loco::TMat3(),
                                                                                                    { 0.0f, 0.0f, 0.0f } ),
                                                                                       loco::TVec3( 0.0f, 0.0f, 1.0f ) );
    platform->SetConstraint( std::move( platform_constraint ) );

    auto rod = scenario->AddSingleBody( std::make_unique<loco::primitives::TCapsule>( "rod", 0.1f, 1.0f,
                                                                          loco::TVec3( 1.0, 0.0, 1.0 ),
                                                                          loco::TMat3() ) );
    auto rod_constraint = std::make_unique<loco::primitives::TSingleBodySphericalConstraint>( "rod_constraint",
                                                                                  loco::TMat4( loco::TMat3(),
                                                                                               loco::TVec3( 0.0f, 0.0f, 0.5f ) ) );
    rod->SetConstraint( std::move( rod_constraint ) );

    auto lego_1 = scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "lego_1", loco::TVec3( 0.4, 0.2, 0.2 ),
                                                                         loco::TVec3( -1.0, 0.0, 0.1 ),
                                                                         loco::TMat3() ) );
    auto lego_1_constraint = std::make_unique<loco::primitives::TSingleBodyPlanarConstraint>( "lego_1_constraint" );
    lego_1->SetConstraint( std::move( lego_1_constraint ) );

    auto lego_2 = scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "lego_2", loco::TVec3( 0.4, 0.2, 0.2 ),
                                                                         loco::TVec3( -0.8, 0.0, 0.301 ),
                                                                         loco::TMat3() ) );
    auto lego_2_constraint = std::make_unique<loco::primitives::TSingleBodyPlanarConstraint>( "lego_2_constraint" );
    lego_2->SetConstraint( std::move( lego_2_constraint ) );

    auto lego_3 = scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "lego_3", loco::TVec3( 0.4, 0.2, 0.2 ),
                                                                         loco::TVec3( -1.2, 0.0, 0.301 ),
                                                                         loco::TMat3() ) );
    auto lego_3_constraint = std::make_unique<loco::primitives::TSingleBodyPlanarConstraint>( "lego_3_constraint" );
    lego_3->SetConstraint( std::move( lego_3_constraint ) );

    auto lego_4 = scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "lego_4", loco::TVec3( 0.4, 0.2, 0.2 ),
                                                                         loco::TVec3( -1.0, 0.0, 0.501 ),
                                                                         loco::TMat3() ) );
    auto lego_4_constraint = std::make_unique<loco::primitives::TSingleBodyPlanarConstraint>( "lego_4_constraint" );
    lego_4->SetConstraint( std::move( lego_4_constraint ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    floor->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );

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
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
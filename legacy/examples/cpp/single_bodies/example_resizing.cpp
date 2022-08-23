
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

    const loco::TVec3 pos_box = { -3.0f, 0.0f, 0.2f };
    const loco::TVec3 pos_sphere = { -2.0f, 0.0f, 0.1f };
    const loco::TVec3 pos_cylinder = { -1.0f, 0.0f, 0.25f };
    const loco::TVec3 pos_capsule = { 1.0f, 0.0f, 0.35f };
    const loco::TVec3 pos_ellipsoid = { 2.0f, 0.0f, 0.4f };
    const loco::TVec3 pos_mesh = { 3.0f, 0.0f, 0.5f };

    auto scenario = std::make_unique<loco::TScenario>();
    auto floor = scenario->AddSingleBody( std::make_unique<loco::primitives::TPlane>( "floor", 10.0f, 10.0f, loco::TVec3(), loco::TMat3() ) );
    auto box = scenario->AddSingleBody( std::make_unique<loco::primitives::TBox>( "box", loco::TVec3( 0.2f, 0.3f, 0.4f ), pos_box, loco::TMat3() ) );
    auto sphere = scenario->AddSingleBody( std::make_unique<loco::primitives::TSphere>( "sphere", 0.1f, pos_sphere, loco::TMat3() ) );
    auto cylinder = scenario->AddSingleBody( std::make_unique<loco::primitives::TCylinder>( "cylinder", 0.2f, 0.5f, pos_cylinder, loco::TMat3() ) );
    auto capsule = scenario->AddSingleBody( std::make_unique<loco::primitives::TCapsule>( "capsule", 0.2f, 0.5f, pos_capsule, loco::TMat3() ) );
    auto ellipsoid = scenario->AddSingleBody( std::make_unique<loco::primitives::TEllipsoid>( "ellipsoid", loco::TVec3( 0.2f, 0.3f, 0.4f ), pos_ellipsoid, loco::TMat3() ) );
    auto mesh = scenario->AddSingleBody( std::make_unique<loco::primitives::TConvexMesh>( "monkey",
                                                                        loco::PATH_RESOURCES + "meshes/monkey.stl",
                                                                        loco::PATH_RESOURCES + "meshes/monkey.obj",
                                                                        0.2f, pos_mesh, loco::TMat3() ) );

    auto user_sphere = scenario->AddSingleBody( std::make_unique<loco::primitives::TSphere>( "user_sphere", 0.1f, loco::TVec3( 0.0f, 0.0f, 0.1f ), loco::TMat3() ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    floor->drawable()->ChangeTexture( "built_in_chessboard" );
    floor->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );
    user_sphere->drawable()->ChangeTexture( "built_in_chessboard" );
    user_sphere->drawable()->ChangeColor( { 0.1f, 0.1f, 0.8f } );

    bool increasing = true;

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
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_0 ) )
            increasing = !increasing;
        else
        {
            if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_1 ) )
                static_cast<loco::primitives::TBox*>( box )->SetWidth( static_cast<loco::primitives::TBox*>( box )->width() + ( increasing ? 0.1f : -0.1f ) );
            else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_2 ) )
                static_cast<loco::primitives::TBox*>( box )->SetDepth( static_cast<loco::primitives::TBox*>( box )->depth() + ( increasing ? 0.1f : -0.1f ) );
            else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_3 ) )
                static_cast<loco::primitives::TBox*>( box )->SetHeight( static_cast<loco::primitives::TBox*>( box )->height() + ( increasing ? 0.1f : -0.1f ) );
            else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_4 ) )
                static_cast<loco::primitives::TSphere*>( sphere )->SetRadius( static_cast<loco::primitives::TSphere*>( sphere )->radius() + ( increasing ? 0.1f : -0.1f ) );
            else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_5 ) )
                static_cast<loco::primitives::TCylinder*>( cylinder )->SetRadius( static_cast<loco::primitives::TCylinder*>( cylinder )->radius() + ( increasing ? 0.1f : -0.1f ) );
            else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_6 ) )
                static_cast<loco::primitives::TCylinder*>( cylinder )->SetHeight( static_cast<loco::primitives::TCylinder*>( cylinder )->height() + ( increasing ? 0.1f : -0.1f ) );
            else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_7 ) )
                static_cast<loco::primitives::TCapsule*>( capsule )->SetRadius( static_cast<loco::primitives::TCapsule*>( capsule )->radius() + ( increasing ? 0.1f : -0.1f ) );
            else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_8 ) )
                static_cast<loco::primitives::TCapsule*>( capsule )->SetHeight( static_cast<loco::primitives::TCapsule*>( capsule )->height() + ( increasing ? 0.1f : -0.1f ) );
            else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_9 ) )
                static_cast<loco::primitives::TConvexMesh*>( mesh )->SetScale( static_cast<loco::primitives::TConvexMesh*>( mesh )->scale() + ( increasing ? 0.01f : -0.01f ) );
        }

        simulation->Step(1./60.);
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();
    return 0;
}
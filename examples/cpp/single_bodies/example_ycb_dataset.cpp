
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

    const loco::TMat3 rotation = tinymath::rotation( loco::TVec3( loco::PI / 3, loco::PI / 4, loco::PI / 6 ) );
    //// const loco::TMat3 rotation = tinymath::rotation( loco::TVec3( loco::PI / 2, 0.0f, 0.0f ) );
    //// const loco::TMat3 rotation = tinymath::rotation( loco::TVec3( 0.0f, 0.0f, 0.0f ) );

    auto scenario = std::make_unique<loco::TScenario>();
    scenario->AddSingleBody( std::make_unique<loco::primitives::TPlane>( "floor", 10.0f, 10.0f, loco::TVec3(), loco::TMat3() ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TConvexMesh>( "mesh_ycb_1",
                                                            loco::PATH_RESOURCES + "meshes/ycb/001_chips_can/tsdf/nontextured.stl",
                                                            loco::PATH_RESOURCES + "meshes/ycb/001_chips_can/tsdf/textured.obj",
                                                            1.0f, loco::TVec3( 0.0f, 0.0f, 1.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TConvexMesh>( "mesh_ycb_2",
                                                            loco::PATH_RESOURCES + "meshes/ycb/002_master_chef_can/tsdf/nontextured.stl",
                                                            loco::PATH_RESOURCES + "meshes/ycb/002_master_chef_can/tsdf/textured.obj",
                                                            1.0f, loco::TVec3( 0.0f, -1.0f, 1.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TConvexMesh>( "mesh_ycb_3",
                                                            loco::PATH_RESOURCES + "meshes/ycb/003_cracker_box/tsdf/nontextured.stl",
                                                            loco::PATH_RESOURCES + "meshes/ycb/003_cracker_box/tsdf/textured.obj",
                                                            1.0f, loco::TVec3( 1.0f, -1.0f, 1.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TConvexMesh>( "mesh_ycb_4",
                                                            loco::PATH_RESOURCES + "meshes/ycb/004_sugar_box/tsdf/nontextured.stl",
                                                            loco::PATH_RESOURCES + "meshes/ycb/004_sugar_box/tsdf/textured.obj",
                                                            1.0f, loco::TVec3( -1.0f, 1.0f, 1.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TConvexMesh>( "mesh_ycb_5",
                                                            loco::PATH_RESOURCES + "meshes/ycb/005_tomato_soup_can/tsdf/nontextured.stl",
                                                            loco::PATH_RESOURCES + "meshes/ycb/005_tomato_soup_can/tsdf/textured.obj",
                                                            1.0f, loco::TVec3( 1.0f, 1.0f, 1.0f ), rotation ) );
    scenario->AddSingleBody( std::make_unique<loco::primitives::TConvexMesh>( "mesh_ycb_6",
                                                            loco::PATH_RESOURCES + "meshes/ycb/006_mustard_bottle/tsdf/nontextured.stl",
                                                            loco::PATH_RESOURCES + "meshes/ycb/006_mustard_bottle/tsdf/textured.obj",
                                                            1.0f, loco::TVec3( -1.0f, -1.0f, 1.0f ), rotation ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    for ( ssize_t i = 1; i <= 6; i++ )
        if ( auto ycb_mesh_ref = scenario->GetSingleBodyByName( "mesh_ycb_" + std::to_string( i ) ) )
            ycb_mesh_ref->drawable()->ChangeColor( { 1.0f, 1.0f, 1.0f } );

    auto floor_ref = scenario->GetSingleBodyByName( "floor" );
    floor_ref->drawable()->ChangeTexture( "built_in_chessboard" );
    floor_ref->drawable()->ChangeColor( { 0.3f, 0.5f, 0.7f } );

    while ( visualizer->IsActive() )
    {
        if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_ESCAPE ) )
            break;
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_R ) )
            simulation->Reset();

        simulation->Step();
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
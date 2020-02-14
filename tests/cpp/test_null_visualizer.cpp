
#include <loco.h>
#include <gtest/gtest.h>

TEST( TestLocoNullVisualizer, TestNullVisualizerFunctionality )
{
    auto vis_data = loco::TVisualData();
    vis_data.type = loco::eShapeType::CAPSULE;
    vis_data.size = { 0.1, 0.2, 0.1 };
    auto col_data = loco::TCollisionData();
    col_data.type = loco::eShapeType::CAPSULE;
    col_data.size = { 0.1, 0.2, 0.1 };

    auto body_data = loco::TBodyData();
    body_data.dyntype = loco::eDynamicsType::DYNAMIC;
    body_data.collision = col_data;
    body_data.visual = vis_data;

    auto body_obj = std::make_unique<loco::TSingleBody>( "body_0", body_data, tinymath::Vector3f( 1.0, 1.0, 1.0 ), tinymath::Matrix3f() );
    auto scenario = std::make_unique<loco::TScenario>();
    scenario->AddSingleBody( std::move( body_obj ) );

    auto visualizer = std::make_unique<loco::TNullVisualizer>( scenario.get(), 1024, 768, true, false );
    auto cameraRef = visualizer->CreateCamera( "cam_orbit_0", 
                                               loco::eVizCameraType::ORBIT,
                                               { 3.0f, 3.0f, 3.0f },
                                               { 0.0f, 0.0f, 0.0f } );
    auto lightRef = visualizer->CreateLight( "light_point_0",
                                             loco::eVizLightType::POINT,
                                             { 0.4f, 0.4f, 0.4f },
                                             { 0.8f, 0.8f, 0.8f },
                                             { 0.8f, 0.8f, 0.8f } );
    visualizer->Initialize();
    visualizer->Update();
    visualizer->Reset();

    cameraRef->SetPosition( { 5.0f, 5.0f, 5.0f } );
    cameraRef->SetTarget( { 0.0f, 0.0f, 1.0f } );

    lightRef->SetPosition( { 0.0f, 0.0f, 7.0f } );
    lightRef->ChangeIntensity( 0.9f );

    EXPECT_FALSE( visualizer->HasCameraNamed( "foooooooo" ) );
    EXPECT_FALSE( visualizer->HasLightNamed( "fuuuuuuuu" ) );
    EXPECT_TRUE( visualizer->HasCameraNamed( "cam_orbit_0" ) );
    EXPECT_TRUE( visualizer->HasLightNamed( "light_point_0" ) );
    EXPECT_TRUE( visualizer->GetCameraByName( "cam_orbit_0" ) != nullptr );
    EXPECT_TRUE( visualizer->GetLightByName( "light_point_0" ) != nullptr );
    EXPECT_TRUE( tinymath::allclose( visualizer->GetCameraByName( "cam_orbit_0" )->position(), cameraRef->position() ) );
    EXPECT_TRUE( tinymath::allclose( visualizer->GetLightByName( "light_point_0" )->ambient(), lightRef->ambient() ) );

    EXPECT_EQ( cameraRef->type(), loco::eVizCameraType::ORBIT );
    EXPECT_TRUE( tinymath::allclose( cameraRef->position(), { 5.0f, 5.0f, 5.0f } ) );
    EXPECT_TRUE( tinymath::allclose( cameraRef->target(), { 0.0f, 0.0f, 1.0f } ) );

    EXPECT_EQ( lightRef->type(), loco::eVizLightType::POINT );
    EXPECT_TRUE( tinymath::allclose( lightRef->position(), { 0.0f, 0.0f, 7.0f } ) );
    EXPECT_TRUE( tinymath::allclose( lightRef->ambient(), { 0.4f, 0.4f, 0.4f } ) );
    EXPECT_TRUE( tinymath::allclose( lightRef->diffuse(), { 0.8f, 0.8f, 0.8f } ) );
    EXPECT_TRUE( tinymath::allclose( lightRef->specular(), { 0.8f, 0.8f, 0.8f } ) );
    EXPECT_TRUE( std::abs( lightRef->intensity() - 0.9f ) < 1e-6 );
}
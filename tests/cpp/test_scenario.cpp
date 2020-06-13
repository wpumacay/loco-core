
#include <loco.h>
#include <gtest/gtest.h>

TEST( TestLocoScenario, TestScenarioConstructor )
{
    auto scenario = std::make_unique<loco::TScenario>();
    scenario->Initialize();
    scenario->PreStep();
    scenario->PostStep();
    scenario->Reset();
    EXPECT_EQ( scenario->HasSingleBodyNamed( "foo" ), false );
    EXPECT_EQ( scenario->GetSingleBodyByName( "foo" ), nullptr );
    EXPECT_EQ( scenario->GetSingleBodiesList().size(), 0 );
    EXPECT_EQ( scenario->GetNumSingleBodies(), 0 );
}

TEST( TestLocoScenario, TestScenarioPrimitives )
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

    auto body_obj = std::make_unique<loco::primitives::TSingleBody>( "body_0", body_data, tinymath::Vector3f( 1.0, 1.0, 1.0 ), tinymath::Matrix3f() );
    auto scenario = std::make_unique<loco::TScenario>();
    scenario->AddSingleBody( std::move( body_obj ) );
    EXPECT_EQ( scenario->HasSingleBodyNamed( "body_0" ), true );
    EXPECT_NE( scenario->GetSingleBodyByName( "body_0" ), nullptr );
    EXPECT_EQ( scenario->GetSingleBodiesList().size(), 1 );
    EXPECT_EQ( scenario->GetSingleBodiesList().back()->name(), "body_0" );
    EXPECT_EQ( scenario->GetNumSingleBodies(), 1 );
}

#include <loco.h>
#include <gtest/gtest.h>

tinymath::Matrix3f rotationMatrix_axisX( float angle )
{
    auto mat = tinymath::Matrix3f();
    mat( 1, 1 ) = std::cos( angle ); mat( 1, 2 ) = -std::sin( angle );
    mat( 2, 1 ) = std::sin( angle ); mat( 2, 2 ) = std::cos( angle );
    return mat;
}

tinymath::Matrix4f createTestTransform_local()
{
    auto rotation = rotationMatrix_axisX( loco::PI / 4 );
    auto position = tinymath::Vector3f( -0.0, -0.1, -0.2 );
    return tinymath::Matrix4f( rotation, position );
}

tinymath::Matrix4f createTestTransform_world()
{
    auto rotation = rotationMatrix_axisX( loco::PI / 6 );
    auto position = tinymath::Vector3f( 0.0, 0.0, 2.0 );
    return tinymath::Matrix4f( rotation, position );
}

TEST( TestLocoComponents, TestCollisionObject )
{
    auto col_data = loco::TCollisionData();
    col_data.type = loco::eShapeType::BOX;
    col_data.size = { 0.1, 0.2, 0.3 };

    auto col_obj = std::make_unique<loco::primitives::TSingleBodyCollider>( "col_0", col_data );
    // Call methods (nothing should happen, as the collider is not in a simulation)
    col_obj->Initialize();
    col_obj->PreStep();
    col_obj->PostStep();
    col_obj->Reset();
    // Check that all read_write properties work (visible and wireframe props require adapters to work)
    col_obj->SetVisible( true );
    col_obj->SetWireframe( true );
    col_obj->ChangeSize( { 0.2, 0.2, 0.2 } );
    col_obj->ChangeCollisionGroup( 1 );
    col_obj->ChangeCollisionMask( 2 );
    EXPECT_EQ( col_obj->visible(), true );
    EXPECT_EQ( col_obj->wireframe(), true );
    EXPECT_TRUE( tinymath::allclose( col_obj->size(), { 0.2, 0.2, 0.2 } ) );
    EXPECT_EQ( col_obj->collisionGroup(), 1 );
    EXPECT_EQ( col_obj->collisionMask(), 2 );
}

TEST( TestLocoComponents, TestVisualObject )
{
    auto vis_data = loco::TVisualData();
    vis_data.type = loco::eShapeType::SPHERE;
    vis_data.size = { 0.1, 0.1, 0.1 };

    auto vis_obj = std::make_unique<loco::visualizer::TDrawable>( "vis_0", vis_data );
    // Call methods (nothing should happen, as the visual is not part of a visualizer)
    vis_obj->Initialize();
    vis_obj->PreStep();
    vis_obj->PostStep();
    vis_obj->Reset();
    // Check that all read_write properties work (visible and wireframe props )
    vis_obj->SetVisible( true );
    vis_obj->SetWireframe( true );
    vis_obj->ChangeSize( { 0.2, 0.2, 0.2 } );
    vis_obj->ChangeAmbientColor( { 0.725, 0.525, 0.325 } );
    vis_obj->ChangeDiffuseColor( { 0.750, 0.550, 0.350 } );
    vis_obj->ChangeSpecularColor( { 0.775, 0.575, 0.375 } );
    vis_obj->ChangeShininess( 128.0 );
    vis_obj->SetLocalTransform( createTestTransform_local() );
    EXPECT_EQ( vis_obj->visible(), true );
    EXPECT_EQ( vis_obj->wireframe(), true );
    EXPECT_TRUE( tinymath::allclose( vis_obj->size(), { 0.2, 0.2, 0.2 } ) );
    EXPECT_TRUE( tinymath::allclose( vis_obj->ambient(), { 0.725, 0.525, 0.325 } ) );
    EXPECT_TRUE( tinymath::allclose( vis_obj->diffuse(), { 0.750, 0.550, 0.350 } ) );
    EXPECT_TRUE( tinymath::allclose( vis_obj->specular(), { 0.775, 0.575, 0.375 } ) );
    EXPECT_TRUE( std::abs( vis_obj->shininess() - 128.0 ) < 1e-6 );
    EXPECT_TRUE( tinymath::allclose( vis_obj->local_tf(), createTestTransform_local() ) );
    EXPECT_TRUE( tinymath::allclose( vis_obj->local_rot(), rotationMatrix_axisX( loco::PI / 4 ) ) );
    EXPECT_TRUE( tinymath::allclose( vis_obj->local_pos(), { -0.0, -0.1, -0.2 } ) );
}

TEST( TestLocoComponents, TestSingleBodyObject )
{
    auto vis_data = loco::TVisualData();
    vis_data.type = loco::eShapeType::CAPSULE;
    vis_data.size = { 0.1, 0.2, 0.1 };
    vis_data.localTransform = createTestTransform_local();
    auto col_data = loco::TCollisionData();
    col_data.type = loco::eShapeType::CAPSULE;
    col_data.size = { 0.1, 0.2, 0.1 };

    auto body_data = loco::TBodyData();
    body_data.dyntype = loco::eDynamicsType::DYNAMIC;
    body_data.collision = col_data;
    body_data.visual = vis_data;

    auto body_obj = std::make_unique<loco::primitives::TSingleBody>( "body_0", body_data, tinymath::Vector3f( 1.0, 1.0, 1.0 ), tinymath::Matrix3f() );
    // Call methods (nothing should happen, as the body is not in a simulation)
    body_obj->Initialize();
    body_obj->PreStep();
    body_obj->PostStep();
    body_obj->Reset();
    // Check that all read_write properties work
    body_obj->SetTransform( createTestTransform_world() );
    EXPECT_EQ( body_obj->dyntype(), loco::eDynamicsType::DYNAMIC );
    EXPECT_TRUE( tinymath::allclose( body_obj->tf(), createTestTransform_world() ) );
    EXPECT_TRUE( tinymath::allclose( body_obj->rot(), rotationMatrix_axisX( loco::PI / 6 ) ) );
    EXPECT_TRUE( tinymath::allclose( body_obj->pos(), { 0.0, 0.0, 2.0 } ) );
    auto vis_ref = body_obj->drawable();
    auto col_ref = body_obj->collider();
    auto expected_tf_drawable = body_obj->tf() * createTestTransform_local();
    auto expected_tf_collider = body_obj->tf();
    EXPECT_TRUE( tinymath::allclose( vis_ref->tf(), expected_tf_drawable ) );
    EXPECT_TRUE( tinymath::allclose( vis_ref->pos(), tinymath::Vector3f( expected_tf_drawable.col( 3 ) ) ) );
    EXPECT_TRUE( tinymath::allclose( vis_ref->rot(), tinymath::Matrix3f( expected_tf_drawable ) ) );
    EXPECT_TRUE( tinymath::allclose( col_ref->tf(), expected_tf_collider ) );
    EXPECT_TRUE( tinymath::allclose( col_ref->pos(), tinymath::Vector3f( expected_tf_collider.col( 3 ) ) ) );
    EXPECT_TRUE( tinymath::allclose( col_ref->rot(), tinymath::Matrix3f( expected_tf_collider ) ) );
    // Check that changing visuals|collisions works fine
    auto vis_data_2 = loco::TVisualData();
    vis_data_2.type = loco::eShapeType::CYLINDER;
    vis_data_2.size = { 0.1, 0.2, 0.1 };
    auto col_data_2 = loco::TCollisionData();
    col_data_2.type = loco::eShapeType::CYLINDER;
    col_data_2.size = { 0.1, 0.2, 0.1 };

    auto vis_obj_2 = std::make_unique<loco::visualizer::TDrawable>( "vis_2_replacement", vis_data_2 );
    auto col_obj_2 = std::make_unique<loco::primitives::TSingleBodyCollider>( "col_2_replacement", col_data_2 );
    body_obj->SetDrawable( std::move( vis_obj_2 ) );
    body_obj->SetCollider( std::move( col_obj_2 ) );
    EXPECT_EQ( body_obj->drawable()->name(), "vis_2_replacement" );
    EXPECT_EQ( body_obj->collider()->name(), "col_2_replacement" );
    // Local transforms for these visual|collision are identity, so their 'tf' should be the same as their parents'
    EXPECT_TRUE( tinymath::allclose( body_obj->drawable()->tf(), createTestTransform_world() ) );
    EXPECT_TRUE( tinymath::allclose( body_obj->collider()->tf(), createTestTransform_world() ) );
}

#include <loco.h>
#include <gtest/gtest.h>

// Just tests that the loading functionality doesn't crash
TEST( TestLocoUtilsXmlElement, TestXmlElementMjcfLoadFile )
{
    loco::TLogger::Init();
    auto root = loco::parsing::TElement::CreateFromXmlFile( loco::parsing::eSchemaType::MJCF,
                                                            loco::PATH_TEMPLATES_MJCF + "pr2.xml" );
    LOCO_TRACE( "root:\n{0}", root->ToString() );
}

TEST( TestLocoUtilsXmlElement, TestXmlElementMjcfLoadString )
{
    loco::TLogger::Init();
    const std::string sample_mjcf =
        "<mujoco model=\"template_ball\">\n"
        "  <worldbody>\n"
        "    <body name=\"sphere_body\" pos=\"1.0 1.0 2.0\">\n"
        "      <joint name=\"sphere_freejoint\" type=\"free\"/>\n"
        "      <geom name=\"sphere_geom\" pos=\"0 0 0\" size=\"0.1 0.1 0.1\"/>\n"
        "    </body>\n"
        "  </worldbody>\n"
        "</mujoco>\n";
    auto root = loco::parsing::TElement::CreateFromXmlString( loco::parsing::eSchemaType::MJCF,
                                                              sample_mjcf );
    EXPECT_EQ( root->GetString( "model" ), "template_ball" );
    auto world_body = root->get_child( 0 );
    EXPECT_EQ( world_body->num_children(), 1 );
    auto body = world_body->get_child( 0 );
    EXPECT_EQ( body->GetString( "name" ), "sphere_body" );
    EXPECT_TRUE( tinymath::allclose( body->GetVec3( "pos" ), { 1.0, 1.0, 2.0 } ) );
    auto joint = body->GetFirstChildOfType( "joint" );
    EXPECT_EQ( joint->GetString( "name" ), "sphere_freejoint" );
    EXPECT_EQ( joint->GetString( "type" ), "free" );
    auto geom = body->GetFirstChildOfType( "geom" );
    EXPECT_EQ( geom->GetString( "name" ), "sphere_geom" );
    EXPECT_TRUE( tinymath::allclose( geom->GetVec3( "pos" ), { 0.0, 0.0, 0.0 } ) );
    EXPECT_TRUE( tinymath::allclose( geom->GetVec3( "size" ), { 0.1, 0.1, 0.1 } ) );

    LOCO_TRACE( "root:\n{0}", root->ToString() );
}

// Just tests that this functionality doesn't crash
TEST( TestLocoUtilsXmlElement, TestXmlElementMjcfProgrammatical )
{
    auto root = std::make_unique<loco::parsing::TElement>( "mujoco", loco::parsing::eSchemaType::MJCF );
    auto world_body = root->Add( "worldbody" );
    {
        auto body = world_body->Add( "body" );
        body->SetString( "name", "sphere_body" );
        body->SetVec3( "pos", { 0.0f, 0.0f, 1.0f } );
        auto geom = body->Add( "geom" );
        geom->SetString( "name", "sphere_geom" );
        geom->SetVec3( "size", { 0.1f, 0.1f, 0.1f } );
        geom->SetVec3( "pos", { 0.0f, 0.0f, 0.0f } );
        auto joint = body->Add( "freejoint" );
        joint->SetString( "name", "sphere_freejoint" );
    }

    root->SaveToXml( "sample_mjcf_sphere.xml" );
}

// Just tests that the loading functionality doesn't crash
TEST( TestLocoUtilsXmlElement, TextXmlElementUrdfLoadFile )
{
    loco::TLogger::Init();
    auto root = loco::parsing::TElement::CreateFromXmlFile( loco::parsing::eSchemaType::URDF,
                                                            loco::PATH_TEMPLATES_URDF + "nao.urdf" );
    LOCO_TRACE( "num_children: {0}", root->num_children() );
    LOCO_TRACE( "root:\n{0}", root->ToString() );
}

TEST( TestLocoUtilsXmlElement, TestXmlElementUrdfString )
{
    loco::TLogger::Init();
    const std::string sample_urdf =
        "<robot name=\"cube\">\n"
        "  <link name=\"baseLink\">\n"
        "    <inertial>\n"
        "      <origin rpy=\"0.01 0.02 0.03\" xyz=\"0.01 0.02 0.03\"/>\n"
        "      <mass value=\"1.0\"/>\n"
        "      <inertia ixx=\"1\" ixy=\"0\" ixz=\"0\" iyy=\"1\" iyz=\"0\" izz=\"1\"/>\n"
        "    </inertial>\n"
        "    <visual>\n"
        "      <origin rpy=\"0.01 0.02 0.03\" xyz=\"0.01 0.02 0.03\"/>\n"
        "      <geometry>\n"
        "        <mesh filename=\"cube.obj\" scale=\"1 1 1\"/>\n"
        "      </geometry>\n"
        "      <material name=\"white\">\n"
        "        <color rgba=\"1 1 1 1\"/>\n"
        "      </material>\n"
        "    </visual>\n"
        "    <collision>\n"
        "      <origin rpy=\"0.01 0.02 0.03\" xyz=\"0.01 0.02 0.03\"/>\n"
        "      <geometry>\n"
        "        <box size=\"1 1 1\"/>\n"
        "      </geometry>\n"
        "    </collision>\n"
        "  </link>\n"
        "</robot>\n";

    auto root = loco::parsing::TElement::CreateFromXmlString( loco::parsing::eSchemaType::URDF,
                                                              sample_urdf );
    EXPECT_EQ( root->GetString( "name" ), "cube" );
    EXPECT_EQ( root->num_children(), 1 );
    auto link = root->GetFirstChildOfType( "link" );
    EXPECT_EQ( link->GetString( "name" ), "baseLink" );
    EXPECT_EQ( link->num_children(), 3 );

    auto inertial = link->GetFirstChildOfType( "inertial" );
    EXPECT_EQ( inertial->num_children(), 3 );
    auto inertial_origin = inertial->GetFirstChildOfType( "origin" );
    EXPECT_TRUE( tinymath::allclose( inertial_origin->GetVec3( "rpy" ), { 0.01, 0.02, 0.03 } ) );
    EXPECT_TRUE( tinymath::allclose( inertial_origin->GetVec3( "xyz" ), { 0.01, 0.02, 0.03 } ) );
    auto inertial_mass = inertial->GetFirstChildOfType( "mass" );
    EXPECT_TRUE( std::abs( inertial_mass->GetFloat( "value" ) - 1.0f ) < 1e-6 );
    auto inertial_inertia = inertial->GetFirstChildOfType( "inertia" );
    EXPECT_TRUE( std::abs( inertial_inertia->GetFloat( "ixx" ) - 1.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( inertial_inertia->GetFloat( "ixy" ) - 0.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( inertial_inertia->GetFloat( "ixz" ) - 0.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( inertial_inertia->GetFloat( "iyy" ) - 1.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( inertial_inertia->GetFloat( "iyz" ) - 0.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( inertial_inertia->GetFloat( "izz" ) - 1.0f ) < 1e-6 );

    auto visual = link->GetFirstChildOfType( "visual" );
    EXPECT_EQ( visual->num_children(), 3 );
    auto visual_origin = visual->GetFirstChildOfType( "origin" );
    EXPECT_TRUE( tinymath::allclose( visual_origin->GetVec3( "rpy" ), { 0.01, 0.02, 0.03 } ) );
    EXPECT_TRUE( tinymath::allclose( visual_origin->GetVec3( "xyz" ), { 0.01, 0.02, 0.03 } ) );
    auto visual_geom = visual->GetFirstChildOfType( "geometry" );
    EXPECT_EQ( visual_geom->num_children(), 1 );
    auto visual_geom_mesh = visual_geom->GetFirstChildOfType( "mesh" );
    EXPECT_EQ( visual_geom_mesh->GetString( "filename" ), "cube.obj" );
    EXPECT_TRUE( tinymath::allclose( visual_geom_mesh->GetVec3( "scale" ), { 1.0, 1.0, 1.0 } ) );
    auto visual_material = visual->GetFirstChildOfType( "material" );
    EXPECT_EQ( visual_material->num_children(), 1 );
    auto visual_material_color = visual_material->GetFirstChildOfType( "color" );
    EXPECT_TRUE( tinymath::allclose( visual_material_color->GetVec4( "rgba" ), { 1., 1., 1., 1. } ) );

    auto collision = link->GetFirstChildOfType( "collision" );
    EXPECT_EQ( collision->num_children(), 2 );
    auto collision_origin = collision->GetFirstChildOfType( "origin" );
    EXPECT_TRUE( tinymath::allclose( collision_origin->GetVec3( "rpy" ), { 0.01, 0.02, 0.03 } ) );
    EXPECT_TRUE( tinymath::allclose( collision_origin->GetVec3( "xyz" ), { 0.01, 0.02, 0.03 } ) );
    auto collision_geom = collision->GetFirstChildOfType( "geometry" );
    EXPECT_EQ( collision_geom->num_children(), 1 );
    auto collision_geom_box = collision_geom->GetFirstChildOfType( "box" );
    EXPECT_TRUE( tinymath::allclose( collision_geom_box->GetVec3( "size" ), { 1., 1., 1. } ) );

    LOCO_TRACE( "root:\n{0}", root->ToString() );
}

// Just tests that this functionality doesn't crash
TEST( TestLocoUtilsXmlElement, TextXmlElementUrdfProgrammatical )
{
    auto root = std::make_unique<loco::parsing::TElement>( "robot", loco::parsing::eSchemaType::URDF );

    auto link = root->Add( "link" );
    {
        link->SetString( "name", "box_link" );
        auto visual = link->Add( "visual" );
        {
            auto origin = visual->Add( "origin" );
            origin->SetArrayFloat( "xyz", { 0.0f, 0.0f, 0.0f } );
            origin->SetArrayFloat( "rpy", { 0.0f, 0.0f, 0.0f } );
            auto geometry = visual->Add( "geometry" );
            {
                auto box = geometry->Add( "box" );
                box->SetVec3( "size", { 0.2f, 0.2f, 0.2f } );
            }
            auto material = visual->Add( "material" );
            material->SetString( "name", "gray" );
            {
                auto color = material->Add( "color" );
                color->SetVec4( "rgba", { 0.5f, 0.5f, 0.5f, 1.0f } );
            }
        }

        auto collision = link->Add( "collision" );
        {
            auto origin = collision->Add( "origin" );
            origin->SetArrayFloat( "xyz", { 0.0f, 0.0f, 0.0f } );
            origin->SetArrayFloat( "rpy", { 0.0f, 0.0f, 0.0f } );
            auto geometry = collision->Add( "geometry" );
            {
                auto box = geometry->Add( "box" );
                box->SetVec3( "size", { 0.2f, 0.2f, 0.2f } );
            }
        }
    }

    root->SaveToXml( "sample_urdf_box.urdf" );
}
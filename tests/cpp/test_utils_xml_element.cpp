
#include <loco.h>
#include <gtest/gtest.h>

TEST( TestLocoUtilsXmlElement, TestXmlElementMjcfLoad )
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
        "    <body name=\"sphere_body\" pos=\"0 0 0\">\n"
        "      <joint name=\"sphere_freejoint\"/>\n"
        "      <geom name=\"sphere_geom\" pos=\"0 0 0\" size=\"0.1 0.1 0.1\"/>\n"
        "    </body>\n"
        "  </worldbody>\n"
        "</mujoco>\n";
    auto root = loco::parsing::TElement::CreateFromXmlString( loco::parsing::eSchemaType::MJCF,
                                                              sample_mjcf );
    LOCO_TRACE( "root:\n{0}", root->ToString() );
}

TEST( TestLocoUtilsXmlElement, TestXmlElementMjcfProgrammatical )
{
    auto root = std::make_unique<loco::parsing::TElement>( "mujoco", loco::parsing::eSchemaType::MJCF );
    auto& world_body = root->Add( "worldbody" );
    {
        auto& body = world_body.Add( "body" );
        body.SetString( "name", "sphere_body" );
        body.SetVec3( "pos", { 0.0f, 0.0f, 1.0f } );
        auto& geom = body.Add( "geom" );
        geom.SetString( "name", "sphere_geom" );
        geom.SetVec3( "size", { 0.1f, 0.1f, 0.1f } );
        geom.SetVec3( "pos", { 0.0f, 0.0f, 0.0f } );
        auto& joint = body.Add( "freejoint" );
        joint.SetString( "name", "sphere_freejoint" );
    }

    root->SaveToXml( "sample_mjcf_sphere.xml" );
}

TEST( TestLocoUtilsXmlElement, TextXmlElementUrdfLoad )
{
    loco::TLogger::Init();
    auto root = loco::parsing::TElement::CreateFromXmlFile( loco::parsing::eSchemaType::URDF,
                                                            loco::PATH_TEMPLATES_URDF + "nao.urdf" );
    LOCO_TRACE( "num_children: {0}", root->num_children() );
    LOCO_TRACE( "root:\n{0}", root->ToString() );
}

TEST( TestLocoUtilsXmlElement, TextXmlElementUrdfProgrammatical )
{
    auto root = std::make_unique<loco::parsing::TElement>( "robot", loco::parsing::eSchemaType::URDF );

    auto& link = root->Add( "link" );
    {
        link.SetString( "name", "box_link" );
        auto& visual = link.Add( "visual" );
        {
            auto& origin = visual.Add( "origin" );
            origin.SetArrayFloat( "xyz", { 0.0f, 0.0f, 0.0f } );
            origin.SetArrayFloat( "rpy", { 0.0f, 0.0f, 0.0f } );
            auto& geometry = visual.Add( "geometry" );
            {
                auto& box = geometry.Add( "box" );
                box.SetVec3( "size", { 0.2f, 0.2f, 0.2f } );
            }
            auto& material = visual.Add( "material" );
            material.SetString( "name", "gray" );
            {
                auto& color = material.Add( "color" );
                color.SetVec4( "rgba", { 0.5f, 0.5f, 0.5f, 1.0f } );
            }
        }

        auto& collision = link.Add( "collision" );
        {
            auto& origin = collision.Add( "origin" );
            origin.SetArrayFloat( "xyz", { 0.0f, 0.0f, 0.0f } );
            origin.SetArrayFloat( "rpy", { 0.0f, 0.0f, 0.0f } );
            auto& geometry = collision.Add( "geometry" );
            {
                auto& box = geometry.Add( "box" );
                box.SetVec3( "size", { 0.2f, 0.2f, 0.2f } );
            }
        }
    }

    root->SaveToXml( "sample_urdf_box.urdf" );
}
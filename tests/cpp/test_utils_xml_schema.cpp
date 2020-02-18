
#include <loco.h>
#include <gtest/gtest.h>

TEST( TestLocoUtilsXmlSchema, TestXmlSchemaMjcf )
{
    const std::string MJCF_SCHEMA_PATH = loco::PATH_RESOURCES + "xml/schema_mjcf.xml";
    auto schema_mjcf = std::make_unique<loco::parsing::TMjcfSchema>( MJCF_SCHEMA_PATH );
    std::cout << schema_mjcf->ToString() << std::endl;
}

TEST( TestLocoUtilsXmlSchema, TestXmlSchemaUrdf )
{
    const std::string URDF_SCHEMA_PATH = loco::PATH_RESOURCES + "xml/schema_urdf.xml";
    auto schema_urdf = std::make_unique<loco::parsing::TUrdfSchema>( URDF_SCHEMA_PATH );
    std::cout << schema_urdf->ToString() << std::endl;
}

#include <loco.h>
#include <gtest/gtest.h>

TEST( TestLocoParsingHelpers, TestParsingHelpers_1 )
{
    const std::string str_test_split_1 = "str_foo;str_bar;str_fun";
    auto fields_1 = loco::parsing::Split( str_test_split_1, ';' );
    EXPECT_EQ( fields_1.size(), 3 );
    EXPECT_EQ( fields_1[0], "str_foo" );
    EXPECT_EQ( fields_1[1], "str_bar" );
    EXPECT_EQ( fields_1[2], "str_fun" );

    const std::string str_test_split_2 = "str;foo str;bar str;fun str;var";
    auto fields_2 = loco::parsing::Split( str_test_split_2, ' ' );
    EXPECT_EQ( fields_2.size(), 4 );
    EXPECT_EQ( fields_2[0], "str;foo" );
    EXPECT_EQ( fields_2[1], "str;bar" );
    EXPECT_EQ( fields_2[2], "str;fun" );
    EXPECT_EQ( fields_2[3], "str;var" );

    const std::string str_test_split_3 = "1.1f  3.2f     55.3f";
    auto fields_3 = loco::parsing::Split( str_test_split_3, ' ' );
    EXPECT_EQ( fields_3.size(), 3 );
    EXPECT_TRUE( std::abs( std::stof( fields_3[0] ) - 1.1f ) < 1e-6 );
    EXPECT_TRUE( std::abs( std::stof( fields_3[1] ) - 3.2f ) < 1e-6 );
    EXPECT_TRUE( std::abs( std::stof( fields_3[2] ) - 55.3f ) < 1e-6 );
}

TEST( TestLocoParsingHelpers, TestParsingHelpers_2 )
{
    const std::string str_test_vec3 = "1.0f 2.0f 3.0f";
    EXPECT_TRUE( tinymath::allclose( loco::parsing::StringToVec3( str_test_vec3 ), { 1.0f, 2.0f, 3.0f } ) );

    const std::string str_test_vec4 = "0.1f 0.2f  0.3f   0.4f";
    EXPECT_TRUE( tinymath::allclose( loco::parsing::StringToVec4( str_test_vec4 ), { 0.1f, 0.2f, 0.3f, 0.4f } ) );

    const std::string str_test_array_int = "1 2 4 8";
    auto array_int = loco::parsing::StringToArrayInt( str_test_array_int );
    EXPECT_EQ( array_int.ndim, 4 );
    EXPECT_EQ( array_int[0], 1 );
    EXPECT_EQ( array_int[1], 2 );
    EXPECT_EQ( array_int[2], 4 );
    EXPECT_EQ( array_int[3], 8 );

    const std::string str_test_array_float = "1.0f 2.0f 4.0f 8.0f";
    auto array_float = loco::parsing::StringToArrayFloat( str_test_array_float );
    EXPECT_EQ( array_float.ndim, 4 );
    EXPECT_TRUE( std::abs( array_float[0] - 1.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( array_float[1] - 2.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( array_float[2] - 4.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( array_float[3] - 8.0f ) < 1e-6 );
}

TEST( TestLocoParsingHelpers, TestParsingHelpers_3 )
{
    const std::string str_test_xmlstring =
    "<test_element int=\"2\" float=\"2.0\" str=\"test_str\" vec3=\"1 2 3\" vec4=\"0.1 0.2 0.3 1\" arrint=\"1 2 3 4 5\" arrfloat=\"0.1 0.2 0.3 0.4 0.5\"/>";
    tinyxml2::XMLDocument xml_doc;
    EXPECT_EQ( xml_doc.Parse( str_test_xmlstring.c_str() ), tinyxml2::XML_SUCCESS );
    auto xml_element = xml_doc.FirstChildElement();
    EXPECT_TRUE( xml_element != nullptr );
    EXPECT_EQ( loco::parsing::xml::GetAttributeInt( xml_element, "int" ), 2 );
    EXPECT_EQ( loco::parsing::xml::GetAttributeInt( xml_element, "int_foo", 3 ), 3 );
    EXPECT_TRUE( std::abs( loco::parsing::xml::GetAttributeFloat( xml_element, "float" ) - 2.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( loco::parsing::xml::GetAttributeFloat( xml_element, "float_foo", 3 ) - 3.0f ) < 1e-6 );
    EXPECT_EQ( loco::parsing::xml::GetAttributeString( xml_element, "str" ), "test_str" );
    EXPECT_EQ( loco::parsing::xml::GetAttributeString( xml_element, "str_foo", "default_str" ), "default_str" );
    EXPECT_TRUE( tinymath::allclose( loco::parsing::xml::GetAttributeVec3( xml_element, "vec3" ), { 1., 2., 3. } ) );
    EXPECT_TRUE( tinymath::allclose( loco::parsing::xml::GetAttributeVec3( xml_element, "vec3_foo", { 0.1, 0.2, 0.3 } ), { 0.1, 0.2, 0.3 } ) );
    EXPECT_TRUE( tinymath::allclose( loco::parsing::xml::GetAttributeVec4( xml_element, "vec4" ), { 0.1, 0.2, 0.3, 1. } ) );
    EXPECT_TRUE( tinymath::allclose( loco::parsing::xml::GetAttributeVec4( xml_element, "vec4_foo", { 1.0, 2.0, 3.0, 4.0 } ), { 1.0, 2.0, 3.0, 4.0 } ) );
    auto arr_int = loco::parsing::xml::GetAttributeArrayInt( xml_element, "arrint" );
    EXPECT_EQ( arr_int.ndim, 5 );
    EXPECT_EQ( arr_int[0], 1 );
    EXPECT_EQ( arr_int[1], 2 );
    EXPECT_EQ( arr_int[2], 3 );
    EXPECT_EQ( arr_int[3], 4 );
    EXPECT_EQ( arr_int[4], 5 );
    arr_int = loco::parsing::xml::GetAttributeArrayInt( xml_element, "arrint_foo", { 1, 2, 4, 8, 16, 32 } );
    EXPECT_EQ( arr_int.ndim, 6 );
    EXPECT_EQ( arr_int[0], 1 );
    EXPECT_EQ( arr_int[1], 2 );
    EXPECT_EQ( arr_int[2], 4 );
    EXPECT_EQ( arr_int[3], 8 );
    EXPECT_EQ( arr_int[4], 16 );
    EXPECT_EQ( arr_int[5], 32 );
    auto arr_float = loco::parsing::xml::GetAttributeArrayFloat( xml_element, "arrfloat" );
    EXPECT_EQ( arr_float.ndim, 5 );
    EXPECT_TRUE( std::abs( arr_float[0] - 0.1f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[1] - 0.2f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[2] - 0.3f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[3] - 0.4f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[4] - 0.5f ) < 1e-6 );
    arr_float = loco::parsing::xml::GetAttributeArrayFloat( xml_element, "arrfloat_foo", { 0.1, 0.2, 0.4, 0.8, 1.6, 3.2 } );
    EXPECT_EQ( arr_float.ndim, 6 );
    EXPECT_TRUE( std::abs( arr_float[0] - 0.1f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[1] - 0.2f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[2] - 0.4f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[3] - 0.8f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[4] - 1.6f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[5] - 3.2f ) < 1e-6 );
}

TEST( TestLocoParsingHelpers, TestParsingHelpers_4 )
{
    const std::string str_test_jsonstring =
    "{ \"test_element\": { \"int\": 2, \"float\": 2.0, \"str\": \"test_str\", \"vec3\": [1,2,3], \"vec4\": [0.1,0.2,0.3,1], \"arrint\": [1,2,3,4,5], \"arrfloat\": [0.1,0.2,0.3,0.4,0.5] } }";
    nlohmann::json json_obj = nlohmann::json::parse( str_test_jsonstring );
    auto json_element = json_obj["test_element"];
    EXPECT_EQ( loco::parsing::json::GetAttributeInt( json_element, "int" ), 2 );
    EXPECT_EQ( loco::parsing::json::GetAttributeInt( json_element, "int_foo", 3 ), 3 );
    EXPECT_TRUE( std::abs( loco::parsing::json::GetAttributeFloat( json_element, "float" ) - 2.0f ) < 1e-6 );
    EXPECT_TRUE( std::abs( loco::parsing::json::GetAttributeFloat( json_element, "float_foo", 3 ) - 3.0f ) < 1e-6 );
    EXPECT_EQ( loco::parsing::json::GetAttributeString( json_element, "str" ), "test_str" );
    EXPECT_EQ( loco::parsing::json::GetAttributeString( json_element, "str_foo", "default_str" ), "default_str" );
    EXPECT_TRUE( tinymath::allclose( loco::parsing::json::GetAttributeVec3( json_element, "vec3" ), { 1., 2., 3. } ) );
    EXPECT_TRUE( tinymath::allclose( loco::parsing::json::GetAttributeVec3( json_element, "vec3_foo", { 0.1, 0.2, 0.3 } ), { 0.1, 0.2, 0.3 } ) );
    EXPECT_TRUE( tinymath::allclose( loco::parsing::json::GetAttributeVec4( json_element, "vec4" ), { 0.1, 0.2, 0.3, 1. } ) );
    EXPECT_TRUE( tinymath::allclose( loco::parsing::json::GetAttributeVec4( json_element, "vec4_foo", { 1.0, 2.0, 3.0, 4.0 } ), { 1.0, 2.0, 3.0, 4.0 } ) );
    auto arr_int = loco::parsing::json::GetAttributeArrayInt( json_element, "arrint" );
    EXPECT_EQ( arr_int.ndim, 5 );
    EXPECT_EQ( arr_int[0], 1 );
    EXPECT_EQ( arr_int[1], 2 );
    EXPECT_EQ( arr_int[2], 3 );
    EXPECT_EQ( arr_int[3], 4 );
    EXPECT_EQ( arr_int[4], 5 );
    arr_int = loco::parsing::json::GetAttributeArrayInt( json_element, "arrint_foo", { 1, 2, 4, 8, 16, 32 } );
    EXPECT_EQ( arr_int.ndim, 6 );
    EXPECT_EQ( arr_int[0], 1 );
    EXPECT_EQ( arr_int[1], 2 );
    EXPECT_EQ( arr_int[2], 4 );
    EXPECT_EQ( arr_int[3], 8 );
    EXPECT_EQ( arr_int[4], 16 );
    EXPECT_EQ( arr_int[5], 32 );
    auto arr_float = loco::parsing::json::GetAttributeArrayFloat( json_element, "arrfloat" );
    EXPECT_EQ( arr_float.ndim, 5 );
    EXPECT_TRUE( std::abs( arr_float[0] - 0.1f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[1] - 0.2f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[2] - 0.3f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[3] - 0.4f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[4] - 0.5f ) < 1e-6 );
    arr_float = loco::parsing::json::GetAttributeArrayFloat( json_element, "arrfloat_foo", { 0.1, 0.2, 0.4, 0.8, 1.6, 3.2 } );
    EXPECT_EQ( arr_float.ndim, 6 );
    EXPECT_TRUE( std::abs( arr_float[0] - 0.1f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[1] - 0.2f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[2] - 0.4f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[3] - 0.8f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[4] - 1.6f ) < 1e-6 );
    EXPECT_TRUE( std::abs( arr_float[5] - 3.2f ) < 1e-6 );
}
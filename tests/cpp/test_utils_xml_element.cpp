
#include <loco.h>
#include <gtest/gtest.h>

TEST( TestLocoUtilsXmlElement, TestXmlElementMjcfProgrammatical )
{
////     auto root = std::make_unique<loco::parsing::TElement>( "mujoco" );
////     auto world_body = root->Add( "worldbody" );
////     auto sphere_body = world_body->Add( "body" );
////     sphere_body->SetString( "name", "sphere_body" );
////     sphere_body->SetVec3( "pos", { 0.0f, 0.0f, 1.0f } );
////     auto sphere_geom = sphere_body->Add( "geom" );
////     sphere_geom->SetString( "name", "sphere_geom" );
////     sphere_geom->SetVec3( "size", { 0.1f, 0.1f, 0.1f } );
////     sphere_geom->SetVec3( "pos", { 0.0f, 0.0f, 0.0f } );
////     auto sphere_joint = sphere_body->Add( "freejoint" );
////     sphere_joint->SetString( "name", "sphere_freejoint" );
//// 
////     root->SaveToXml( "sample_mjcf_sphere.xml" );
}

TEST( TestLocoUtilsXmlElement, TextXmlElementUrdfProgrammatical )
{
    
}
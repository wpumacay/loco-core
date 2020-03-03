
#include <loco.h>
#include <gtest/gtest.h>

TEST( TestLocoDataComponents, TestCoreComponents )
{
    auto col_data = loco::TCollisionData();
    col_data.type = loco::eShapeType::SPHERE;
    col_data.size = { 0.1f, 0.1f, 0.1f };
    col_data.collisionGroup = 1;
    col_data.collisionMask = 2;
    col_data.friction = { 1.0f, 0.01f, 0.001f };
    col_data.density = 900.0f;
    col_data.localTransform = tinymath::Matrix4f( tinymath::Matrix3f(), tinymath::Vector3f( 0.1, -0.1, 0.2 ) );

    auto vis_data = loco::TVisualData();
    vis_data.type = loco::eShapeType::SPHERE;
    vis_data.size = { 0.1f, 0.1f, 0.1f };
    vis_data.ambient = { 0.8f, 0.4f, 0.2f };
    vis_data.diffuse = { 0.7f, 0.3f, 0.1f };
    vis_data.specular = { 0.75f, 0.35f, 0.15f };
    vis_data.shininess = 64.0f;
    vis_data.texture = "built_in_chessboard";
    vis_data.localTransform = tinymath::Matrix4f( tinymath::Matrix3f(), tinymath::Vector3f( 0.1, -0.1, 0.2 ) );

    auto jnt_data = loco::TJointData();
    jnt_data.type = loco::eJointType::REVOLUTE;
    jnt_data.axis = { 1.0f, 0.0f, 0.0f };
    jnt_data.limits = { -loco::PI / 2.0f, loco::PI / 2.0f };
    jnt_data.stiffness = 2.0f;
    jnt_data.armature = 0.5f;
    jnt_data.damping = 1.0f;
    jnt_data.ref = loco::PI / 10.0f;
    jnt_data.localTransform = tinymath::Matrix4f( tinymath::rotation( tinymath::Vector3f( loco::PI / 4, loco::PI / 5, loco::PI / 6 ) ),
                                             tinymath::Vector3f( 0.1, 0.2, -0.3 ) );

    auto act_data = loco::TActuatorData();
    act_data.type = loco::eActuatorType::PD_CONTROLLER;
    act_data.limits = { -loco::PI / 3, loco::PI / 3 };
    act_data.gear.ndim = 1;
    act_data.gear[0] = 150.0f;
    act_data.kp = 100.0f;
    act_data.kv = 10.0f;
    act_data.localTransform = tinymath::Matrix4f( tinymath::rotation( tinymath::Vector3f( loco::PI / 4, loco::PI / 5, loco::PI / 6 ) ),
                                                  tinymath::Vector3f( 0.1, 0.2, -0.3 ) );

    auto sen_data = loco::TSensorData();
    sen_data.type = loco::eSensorType::EXT_CAMERA_RGB;
    sen_data.localTransform = tinymath::Matrix4f( tinymath::Matrix3f(), tinymath::Vector3f( 0.0, 0.1, 0.2 ) );

    auto inertia_data = loco::TInertialData();
    inertia_data.mass = 10.0f;
    inertia_data.ixx = 0.1f;
    inertia_data.iyy = 0.2f;
    inertia_data.izz = 0.1f;
    inertia_data.ixy = 0.001f;
    inertia_data.ixz = 0.002f;
    inertia_data.iyz = 0.003f;
    inertia_data.localTransform = tinymath::Matrix4f( tinymath::Matrix3f(), tinymath::Vector3f( -0.0, -0.1, -0.2 ) );

    auto body_data = loco::TBodyData();
    body_data.dyntype = loco::eDynamicsType::DYNAMIC;
    body_data.inertia = inertia_data;
    body_data.localTransformZero = tinymath::Matrix4f( tinymath::Matrix3f(), tinymath::Vector3f( -0.01f, 0.02f, -0.03f ) );
    body_data.collision = col_data;
    body_data.visual = vis_data;
}
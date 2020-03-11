
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_EDITOR;

std::unique_ptr<loco::TSingleBody> create_mesh( const std::string& name,
                                                const loco::TVec3& scale,
                                                const std::string& mesh_filepath_collider,
                                                const std::string& mesh_filepath_visual,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler )
{
    auto col_data = loco::TCollisionData();
    col_data.type = loco::eShapeType::MESH;
    col_data.size = scale;
    col_data.mesh_data.filename = mesh_filepath_collider;
    auto vis_data = loco::TVisualData();
    vis_data.type = loco::eShapeType::MESH;
    vis_data.size = scale;
    vis_data.mesh_data.filename = mesh_filepath_visual;
    auto body_data = loco::TBodyData();
    body_data.collision = col_data;
    body_data.visual = vis_data;
    body_data.dyntype = loco::eDynamicsType::DYNAMIC;

    auto body_obj = std::make_unique<loco::TSingleBody>( name, body_data, position, tinymath::rotation( euler ) );
    return std::move( body_obj );
}

std::unique_ptr<loco::TSingleBody> create_body( const std::string& name,
                                                const loco::eShapeType& shape,
                                                const loco::TVec3& size,
                                                const loco::eDynamicsType& dyntype,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler,
                                                const loco::TVec3& color = { 0.7f, 0.5f, 0.3f } )
{
    auto col_data = loco::TCollisionData();
    col_data.type = shape;
    col_data.size = size;
    auto vis_data = loco::TVisualData();
    vis_data.type = shape;
    vis_data.size = size;
    vis_data.ambient = color;
    vis_data.diffuse = color;
    vis_data.specular = color;
    auto body_data = loco::TBodyData();
    body_data.collision = col_data;
    body_data.visual = vis_data;
    body_data.dyntype = dyntype;

    auto body_obj = std::make_unique<loco::TSingleBody>( name, body_data, position, tinymath::rotation( euler ) );
    return std::move( body_obj );
}

int main( int argc, char* argv[] )
{
    loco::TLogger::Init();

    LOCO_TRACE( "Physics-Backend: {0}", PHYSICS_BACKEND );
    LOCO_TRACE( "Rendering-Backend: {0}", RENDERING_BACKEND );

    const loco::TVec3 orientation = { loco::PI / 3, loco::PI / 4, loco::PI / 6 };
    //// const loco::TVec3 orientation = { loco::PI / 2, 0.0f, 0.0f };
    //// const loco::TVec3 orientation = { 0.0f, 0.0f, 0.0f };

    auto scenario = std::make_unique<loco::TScenario>();
    scenario->AddSingleBody( create_body( "floor", loco::eShapeType::PLANE, { 10.0f, 10.0f, 1.0f },
                                          loco::eDynamicsType::STATIC, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.3f, 0.5f, 0.7f } ) );
    scenario->AddSingleBody( create_body( "box", loco::eShapeType::BOX, { 0.2f, 0.3f, 0.4f },
                                          loco::eDynamicsType::DYNAMIC, { -1.0f, -1.0f, 2.0f }, orientation ) );
    scenario->AddSingleBody( create_body( "sphere", loco::eShapeType::SPHERE, { 0.1f, 0.1f, 0.1f },
                                          loco::eDynamicsType::DYNAMIC, { 1.0f, -1.0f, 2.0f }, orientation ) );
    scenario->AddSingleBody( create_body( "cylinder", loco::eShapeType::CYLINDER, { 0.2f, 0.5f, 1.0f },
                                          loco::eDynamicsType::DYNAMIC, { -1.0f, 1.0f, 2.0f }, orientation ) );
    scenario->AddSingleBody( create_body( "capsule", loco::eShapeType::CAPSULE, { 0.2f, 0.5f, 1.0f },
                                          loco::eDynamicsType::DYNAMIC, { 1.0f, 1.0f, 2.0f }, orientation ) );
    scenario->AddSingleBody( create_body( "ellipsoid", loco::eShapeType::ELLIPSOID, { 0.2f, 0.3f, 0.4f },
                                          loco::eDynamicsType::DYNAMIC, { 0.0f, 0.0f, 2.0f }, orientation ) );
    scenario->AddSingleBody( create_mesh( "monkey", { 0.25, 0.25, 0.25 }, 
                                          loco::PATH_RESOURCES + "meshes/monkey.stl",
                                          loco::PATH_RESOURCES + "meshes/monkey.obj",
                                          { 2.0f, 2.0f, 2.0f }, orientation ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    while ( visualizer->IsActive() )
    {
        if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_ESCAPE ) )
            break;

        if ( auto simulation = runtime->GetCurrentSimulation() )
            simulation->Step();

        visualizer->Update();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}
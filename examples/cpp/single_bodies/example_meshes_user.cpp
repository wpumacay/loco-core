
#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;

std::vector<float> TETRAHEDRON_VERTICES = { 0.0, 0.0, 0.0,
                                            1.0, 0.0, 0.0,
                                            0.0, 1.0, 0.0,
                                            0.0, 0.0, 1.0 };
std::vector<int> TETRAHEDRON_FACES = { 0, 1, 3,
                                       0, 2, 1,
                                       0, 3, 2,
                                       1, 2, 3 };

std::vector<float> RAMP_VERTICES = { 1.0, 0.0, 0.0,
                                     1.0, 2.0, 0.0,
                                     1.0, 1.0, 1.0,
                                     1.0, 0.0, 1.0,
                                    -1.0, 0.0, 0.0,
                                    -1.0, 2.0, 0.0,
                                    -1.0, 1.0, 1.0,
                                    -1.0, 0.0, 1.0 };
std::vector<int> RAMP_FACES = { 0, 1, 2,
                                0, 2, 3,
                                0, 4, 5,
                                0, 5, 1,
                                0, 3, 7,
                                0, 7, 4,
                                2, 6, 7,
                                2, 7, 3,
                                1, 5, 6,
                                1, 6, 2,
                                4, 7, 6,
                                4, 6, 5 };

std::unique_ptr<loco::TSingleBody> create_body( const std::string& name,
                                                const loco::eShapeType& shape,
                                                const loco::TVec3& size,
                                                const loco::eDynamicsType& dyntype,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler,
                                                const loco::TVec3& color = { 0.7f, 0.5f, 0.3f } );

std::unique_ptr<loco::TSingleBody> create_tetrahedron( const std::string& name,
                                                       const loco::TVec3& scale,
                                                       const loco::TVec3& position,
                                                       const loco::TVec3& euler );

std::unique_ptr<loco::TSingleBody> create_ramp( const std::string& name,
                                                const loco::TVec3& scale,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler );

int main( int argc, char* argv[] )
{
    loco::TLogger::Init();

    if ( argc > 1 )
    {
        std::string choice_backend = argv[1];
        if ( choice_backend == "mujoco" )
            PHYSICS_BACKEND = loco::config::physics::MUJOCO;
        else if ( choice_backend == "bullet" )
            PHYSICS_BACKEND = loco::config::physics::BULLET;
        else if ( choice_backend == "dart" )
            PHYSICS_BACKEND = loco::config::physics::DART;
        else if ( choice_backend == "raisim" )
            PHYSICS_BACKEND = loco::config::physics::RAISIM;
    }

    LOCO_TRACE( "Backend: {0}", PHYSICS_BACKEND );

    const loco::TVec3 orientation = { 0.0f, 0.0f, 0.0f };
    //// const loco::TVec3 orientation = { loco::PI / 2, 0.0f, 0.0f };
    //// const loco::TVec3 orientation = { loco::PI / 3, loco::PI / 4, loco::PI / 6 };

    auto scenario = std::make_unique<loco::TScenario>();
    scenario->AddSingleBody( create_body( "floor", loco::eShapeType::PLANE, { 10.0f, 10.0f, 1.0f },
                                          loco::eDynamicsType::STATIC, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.3f, 0.5f, 0.7f } ) );
    scenario->AddSingleBody( create_body( "sphere", loco::eShapeType::SPHERE, { 0.1f, 0.1f, 0.1f },
                                          loco::eDynamicsType::DYNAMIC, { 1.0f, -1.0f, 2.0f }, orientation ) );
    scenario->AddSingleBody( create_tetrahedron( "tetra_0", { 1.0f, 1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f }, orientation ) );
    scenario->AddSingleBody( create_tetrahedron( "tetra_1", { 0.5f, 0.5f, 0.5f }, { -1.0f, 1.0f, 1.0f }, orientation ) );
    scenario->AddSingleBody( create_ramp( "ramp_0", { 0.3f, 0.3f, 0.3f }, { 1.0f, 1.0f, 1.0f }, orientation ) );
    scenario->AddSingleBody( create_ramp( "ramp_1", { 0.5f, 0.5f, 0.5f }, { 1.0f, -1.0f, 1.0f }, orientation ) );

    auto lizardon_data = loco::TVisualData();
    lizardon_data.type = loco::eShapeType::MESH;
    lizardon_data.size = { 0.1, 0.1, 0.1 };
    lizardon_data.mesh_data.filename = "/home/gregor/Documents/repos/tysoc_core/ext/tiny_renderer/res/models/pokemons/lizardon/lizardon.obj";
    auto lizardon = std::make_unique<loco::TDrawable>( "lizardon", lizardon_data );
    lizardon->SetPosition( { 0.0, 0.0, 4.0 } );
    scenario->AddDrawable( std::move( lizardon ) );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, loco::config::rendering::GLVIZ_GLFW );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    auto sphere_ref = scenario->GetSingleBodyByName( "sphere" );

    while ( visualizer->IsActive() )
    {
        if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_ESCAPE ) )
            break;
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_R ) )
            simulation->Reset();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_P ) )
            simulation->running() ? simulation->Pause() : simulation->Resume();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_L ) )
            for ( auto single_body : scenario->GetSingleBodiesList() )
                single_body->SetLinearVelocity( { 0.0, 0.0, 5.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_A ) )
            for ( auto single_body : scenario->GetSingleBodiesList() )
                single_body->SetAngularVelocity( { 0.0, 0.0, 3.1415 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_SPACE ) )
            sphere_ref->AddForceCOM( { 0.0, 0.0, 1000.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_UP ) )
            sphere_ref->AddForceCOM( { 0.0, 200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_DOWN ) )
            sphere_ref->AddForceCOM( { 0.0, -200.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_RIGHT ) )
            sphere_ref->AddForceCOM( { 200.0, 0.0, 0.0 } );
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_LEFT ) )
            sphere_ref->AddForceCOM( { -200.0, 0.0, 0.0 } );

        simulation->Step();
        visualizer->Update();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();

    return 0;
}

std::unique_ptr<loco::TSingleBody> create_body( const std::string& name,
                                                const loco::eShapeType& shape,
                                                const loco::TVec3& size,
                                                const loco::eDynamicsType& dyntype,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler,
                                                const loco::TVec3& color )
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

std::unique_ptr<loco::TSingleBody> create_tetrahedron( const std::string& name,
                                                       const loco::TVec3& scale,
                                                       const loco::TVec3& position,
                                                       const loco::TVec3& euler )
{
    auto col_data = loco::TCollisionData();
    col_data.type = loco::eShapeType::MESH;
    col_data.size = scale;
    col_data.mesh_data.vertices = TETRAHEDRON_VERTICES;
    col_data.mesh_data.faces = TETRAHEDRON_FACES;
    auto vis_data = loco::TVisualData();
    vis_data.type = loco::eShapeType::MESH;
    vis_data.size = scale;
    vis_data.mesh_data.vertices = TETRAHEDRON_VERTICES;
    vis_data.mesh_data.faces = TETRAHEDRON_FACES;
    vis_data.ambient = { 0.7, 0.5, 0.3 };
    vis_data.diffuse = { 0.7, 0.5, 0.3 };
    vis_data.specular = { 0.7, 0.5, 0.3 };
    vis_data.shininess = 90.0;
    auto body_data = loco::TBodyData();
    body_data.collision = col_data;
    body_data.visual = vis_data;
    body_data.dyntype = loco::eDynamicsType::DYNAMIC;

    auto body_obj = std::make_unique<loco::TSingleBody>( name, body_data, position, tinymath::rotation( euler ) );
    return std::move( body_obj );
}

std::unique_ptr<loco::TSingleBody> create_ramp( const std::string& name,
                                                const loco::TVec3& scale,
                                                const loco::TVec3& position,
                                                const loco::TVec3& euler )
{
    auto col_data = loco::TCollisionData();
    col_data.type = loco::eShapeType::MESH;
    col_data.size = scale;
    col_data.mesh_data.vertices = RAMP_VERTICES;
    col_data.mesh_data.faces = RAMP_FACES;
    auto vis_data = loco::TVisualData();
    vis_data.type = loco::eShapeType::MESH;
    vis_data.size = scale;
    vis_data.mesh_data.vertices = RAMP_VERTICES;
    vis_data.mesh_data.faces = RAMP_FACES;
    vis_data.ambient = { 0.3, 0.7, 0.5 };
    vis_data.diffuse = { 0.3, 0.7, 0.5 };
    vis_data.specular = { 0.3, 0.7, 0.5 };
    vis_data.shininess = 90.0;
    auto body_data = loco::TBodyData();
    body_data.collision = col_data;
    body_data.visual = vis_data;
    body_data.dyntype = loco::eDynamicsType::DYNAMIC;

    auto body_obj = std::make_unique<loco::TSingleBody>( name, body_data, position, tinymath::rotation( euler ) );
    return std::move( body_obj );

    return nullptr;
}

#include <loco.h>

std::string PHYSICS_BACKEND = loco::config::physics::NONE;
std::string RENDERING_BACKEND = loco::config::rendering::GLVIZ_GLFW;

class DoublePendulum : public loco::kintree::TKinematicTree
{
public :

    DoublePendulum( const std::string& name = "double_pendulum",
                    const loco::TVec3& pos = { 0.0f, 0.0f, 2.0f },
                    const float& link_l1 = 0.5f,
                    const float& link_l2 = 0.5f )
        : loco::kintree::TKinematicTree( name, pos, loco::TMat3() )
    {
        this->base = new loco::kintree::TBox( name + "_base", { 0.1f, 0.1f, 0.1f } );
        this->link_1 = new loco::kintree::TCapsule( name + "_link_1", 0.05f, link_l1 );
        this->link_2 = new loco::kintree::TCapsule( name + "_link_2", 0.05f, link_l2 );

        const auto tf_jnt_1_to_link_1 = loco::TMat4( loco::TMat3(), { 0.0f, 0.0f, 0.5f * link_l1 } );
        const auto tf_jnt_2_to_link_2 = loco::TMat4( loco::TMat3(), { 0.0f, 0.0f, 0.5f * link_l2 } );
        const auto jnt_1_axis = loco::TVec3( 1.0f, 0.0f, 0.0f );
        const auto jnt_2_axis = loco::TVec3( 1.0f, 0.0f, 0.0f );
        this->jnt_base = new loco::kintree::TKinematicTreeFixedJoint( name + "_jnt_fixed", loco::TMat4() );
        this->jnt_1 = new loco::kintree::TKinematicTreeRevoluteJoint( name + "_jnt_rev_1", tf_jnt_1_to_link_1, jnt_1_axis );
        this->jnt_2 = new loco::kintree::TKinematicTreeRevoluteJoint( name + "_jnt_rev_2", tf_jnt_2_to_link_2, jnt_2_axis );

        const bool is_root = true;
        const auto tf_link_1_to_base = loco::TMat4( loco::TMat3(), { 0.0f, 0.0f, -0.5f * link_l1 } );
        const auto tf_link_2_to_link_1 = loco::TMat4( loco::TMat3(), { 0.0f, 0.0f, -0.5f * link_l1 - 0.5f * link_l2 } );
        this->AddBody( std::unique_ptr<loco::kintree::TKinematicTreeBody>( this->base ), is_root );
        this->base->AddJoint( std::unique_ptr<loco::kintree::TKinematicTreeJoint>( this->jnt_base ), loco::TMat4() );
        this->base->AddChild( std::unique_ptr<loco::kintree::TKinematicTreeBody>( this->link_1 ), tf_link_1_to_base );
        this->link_1->AddJoint( std::unique_ptr<loco::kintree::TKinematicTreeJoint>( this->jnt_1 ), tf_jnt_1_to_link_1 );
        this->link_1->AddChild( std::unique_ptr<loco::kintree::TKinematicTreeBody>( this->link_2 ), tf_link_2_to_link_1 );
        this->link_2->AddJoint( std::unique_ptr<loco::kintree::TKinematicTreeJoint>( this->jnt_2 ), tf_jnt_2_to_link_2 );
    }

    ~DoublePendulum()
    {
        base = nullptr;
        link_1 = nullptr;
        link_2 = nullptr;
    }

    loco::kintree::TKinematicTreeBody* base;
    loco::kintree::TKinematicTreeBody* link_1;
    loco::kintree::TKinematicTreeBody* link_2;

    loco::kintree::TKinematicTreeFixedJoint* jnt_base;
    loco::kintree::TKinematicTreeRevoluteJoint* jnt_1;
    loco::kintree::TKinematicTreeRevoluteJoint* jnt_2;
};

int main( int argc, char* argv[] )
{
    loco::InitUtils();

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
        else if ( choice_backend == "none" )
            PHYSICS_BACKEND = loco::config::physics::NONE;
    }

    LOCO_TRACE( "Physics-Backend: {0}", PHYSICS_BACKEND );
    LOCO_TRACE( "Rendering-Backend: {0}", RENDERING_BACKEND );

    auto scenario = std::make_unique<loco::TScenario>();
    scenario->AddKinematicTree( std::make_unique<DoublePendulum>() );

    auto runtime = std::make_unique<loco::TRuntime>( PHYSICS_BACKEND, RENDERING_BACKEND );
    auto simulation = runtime->CreateSimulation( scenario.get() );
    auto visualizer = runtime->CreateVisualizer( scenario.get() );

    while ( visualizer->IsActive() )
    {
        if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_ESCAPE ) )
            break;
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_R ) )
            simulation->Reset();
        else if ( visualizer->CheckSingleKeyPress( loco::Keys::KEY_P ) )
            simulation->running() ? simulation->Pause() : simulation->Resume();

        simulation->Step( 1. / 60. );
        visualizer->Render();
    }

    runtime->DestroySimulation();
    runtime->DestroyVisualizer();
}
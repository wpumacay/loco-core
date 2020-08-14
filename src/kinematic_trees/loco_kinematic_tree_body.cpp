
#include <kinematic_trees/loco_kinematic_tree_body.h>

namespace loco {
namespace kintree {

    //--------------------------------------------------------------------------------------------//
    //                                                                                            //
    //                               Kinematic-Tree-Body Interface                                //
    //                                                                                            //
    //--------------------------------------------------------------------------------------------//

    TKinematicTreeBody::TKinematicTreeBody( const std::string& name )
        : TObject( name ) {}

    TKinematicTreeBody::TKinematicTreeBody( const std::string& name,
                                            const TKinematicTreeBodyData& body_data )
        : TObject( name )
    {
        m_Data = body_data;

        const auto& collider_data = body_data.collider;
        m_Collider = std::make_unique<TKinematicTreeCollider>( name + SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this, collider_data.localTransform );

        const auto& drawable_data = body_data.drawable;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + SUFFIX_DRAWABLE, drawable_data );
        m_Drawable->SetParentObject( this );
        m_Drawable->SetLocalTransform( drawable_data.localTransform );

        const auto& joint_data = body_data.joint;
        /**/ if ( joint_data.type == eJointType::REVOLUTE )
            m_Joint = std::make_unique<TKinematicTreeRevoluteJoint>( name + SUFFIX_JOINT,
                                                                     joint_data.local_axis,
                                                                     joint_data.limits,
                                                                     joint_data.stiffness,
                                                                     joint_data.armature,
                                                                     joint_data.damping );
        else if ( joint_data.type == eJointType::PRISMATIC )
            m_Joint = std::make_unique<TKinematicTreePrismaticJoint>( name + SUFFIX_JOINT,
                                                                      joint_data.local_axis,
                                                                      joint_data.limits,
                                                                      joint_data.stiffness,
                                                                      joint_data.armature,
                                                                      joint_data.damping );
        else if ( joint_data.type == eJointType::SPHERICAL )
            m_Joint = std::make_unique<TKinematicTreeSphericalJoint>( name + SUFFIX_JOINT,
                                                                      joint_data.limits,
                                                                      joint_data.stiffness,
                                                                      joint_data.armature,
                                                                      joint_data.damping );
        else if ( joint_data.type == eJointType::PLANAR )
            m_Joint = std::make_unique<TKinematicTreePlanarJoint>( name + SUFFIX_JOINT,
                                                                   joint_data.plane_axis_1,
                                                                   joint_data.plane_axis_2 );
        else if ( joint_data.type == eJointType::FIXED )
            m_Joint = std::make_unique<TKinematicTreeFixedJoint>( name + SUFFIX_JOINT );
        else if ( joint_data.type == eJointType::FREE )
            m_Joint = std::make_unique<TKinematicTreeFreeJoint>( name + SUFFIX_JOINT );
        else
            LOCO_CORE_CRITICAL( "TKinematicTreeBody >>> found undefined joint-type while constructing kintree-body {0}", m_name );
        m_Joint->SetParentBody( this, joint_data.local_tf );
    }

    TKinematicTreeBody::~TKinematicTreeBody()
    {
        DetachSim();
        DetachViz();

        m_KinematicTreeRef = nullptr;
        m_BodyAdapterRef = nullptr;
        m_ParentBodyRef = nullptr;

        m_Collider = nullptr;
        m_Drawable = nullptr;
        m_Joint = nullptr;
    }

    void TKinematicTreeBody::SetBodyAdapter( TIKinematicTreeBodyAdapter* body_adapter_ref )
    {
        LOCO_CORE_ASSERT( body_adapter_ref, "TKinematicTreeBody::SetBodyAdapter >>> tried adding nullptr to kintree-body {0}", m_name );
        m_BodyAdapterRef = body_adapter_ref;
    }

    void TKinematicTreeBody::SetKintree( TKinematicTree* kintree_ref )
    {
        LOCO_CORE_ASSERT( kintree_ref, "TKinematicTreeBody::SetKintree >>> tried adding nullptr to kintree-body {0}", m_name );
        m_KinematicTreeRef = kintree_ref;
    }

    void TKinematicTreeBody::SetParentBody( TKinematicTreeBody* parent_body_ref, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( parent_body_ref, "TKinematicTreeBody::SetParentBody >>> tried adding nullptr to kintree-body {0}", m_name );
        m_ParentBodyRef = parent_body_ref;
        m_LocalTf = local_tf;
        m_Data.local_tf = local_tf;
        m_tf = m_ParentBodyRef->tf() * m_LocalTf;
    }

    void TKinematicTreeBody::SetDrawable( std::unique_ptr<visualizer::TDrawable> drawable, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( drawable, "TKinematicTreeBody::SetDrawable >>> tried setting nullptr to kintree-body {0}", m_name );
        m_Drawable = std::move( drawable );
        m_Drawable->SetParentObject( this );
        m_Drawable->SetLocalTransform( local_tf );
    }

    void TKinematicTreeBody::SetCollider( std::unique_ptr<TKinematicTreeCollider> collider, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( collider, "TKinematicTreeBody::SetCollider >>> tried setting nullptr to kintree-body {0}", m_name );
        m_Collider = std::move( collider );
        m_Collider->SetParentBody( this, local_tf );
    }

    void TKinematicTreeBody::SetJoint( std::unique_ptr<TKinematicTreeJoint> joint, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( joint, "TKinematicTreeBody::SetJoint >>> tried setting nullptr to kintree-body {0}", m_name );
        m_Joint = std::move( joint );
        m_Joint->SetParentBody( this, local_tf );
    }

    void TKinematicTreeBody::AddChild( std::unique_ptr<TKinematicTreeBody> body, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( body, "TKinematicTreeBody::AddChild >>> tried adding nullptr to kintree-body {0}", m_name );
        m_Children.push_back( std::move( body ) );
        m_Children.back()->SetParentBody( this, local_tf );
        if ( m_KinematicTreeRef )
            m_KinematicTreeRef->RegisterBody( m_Children.back().get() );
    }

    void TKinematicTreeBody::AddForceCOM( const TVec3& force )
    {
        m_TotalForce = m_TotalForce + force;
    }

    void TKinematicTreeBody::AddTorqueCOM( const TVec3& torque )
    {
        m_TotalTorque = m_TotalTorque + torque;
    }

    void TKinematicTreeBody::RemoveChild( const std::string& body_name )
    {
        for ( ssize_t i = 0; i < m_Children.size(); i++ )
        {
            if ( m_Children[i]->name() == body_name )
            {
                m_Children.erase( m_Children.begin() + i );
                break;
            }
        }
    }

    void TKinematicTreeBody::SetLocalTransform( const TMat4& local_tf )
    {
        m_LocalTf = local_tf;
        m_Data.local_tf = local_tf;
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
    }

    void TKinematicTreeBody::Forward()
    {
        if ( m_ParentBodyRef ) // Non-root bodies update from its parent's transform and the joint-transform
        {
            if ( m_Joint )
            {
                const auto joint_ref = m_Joint.get();
                const auto joint_type = joint_ref->type();
                /**/ if ( joint_type == eJointType::REVOLUTE ) // Do FK using DH-table for revolute joints
                {
                    const auto revolute_joint_ref = static_cast<const TKinematicTreeRevoluteJoint*>( joint_ref );
                    // Transform of the joint w.r.t. the parent of this body at its zero|rest|initial configuration
                    const auto tf_parent_to_joint_init = m_LocalTf * revolute_joint_ref->local_tf();
                    // Rotation along the revolution axis
                    const auto tf_rotation_joint = TMat4( tinymath::rotation( revolute_joint_ref->axis(), revolute_joint_ref->angle() ) );
                    // Compute the world-transform of this body
                    m_tf = m_ParentBodyRef->tf() * tf_parent_to_joint_init * tf_rotation_joint * revolute_joint_ref->local_tf().inverse();
                }
                else if ( joint_type == eJointType::PRISMATIC ) // Do FK using DH-table for prismatic joints
                {
                    // @todo: implement this functionality
                }
                else if ( joint_type == eJointType::PLANAR ) // Do FK using DH-table for planar joints
                {
                    // @todo: implement this functionality
                }
                else if ( joint_type == eJointType::FIXED ) // Do FK using DH-table for fixed joints
                {
                    // @todo: implement this functionality
                }
                else
                {
                    LOCO_CORE_WARN( "TKinematicTreeBody::Forward >>> FK using DH-representation is not "
                                    "supported for joint type {0} (while processing kintree-body {1})",
                                    loco::ToString( joint_type ), m_name );
                }
            }
            else
            {
                LOCO_CORE_WARN( "TKinematicTreeBody::Forward >>> kintree-body {0} has no joint", m_name );
            }
        }
        else if ( m_KinematicTreeRef ) // Root-bodies should update using the kintree world-tf
        {
        }

        for ( auto& child_body : m_Children )
            child_body->Forward();
    }

    std::vector<TKinematicTreeBody*> TKinematicTreeBody::children()
    {
        std::vector<TKinematicTreeBody*> children_list;
        for ( auto& child_body : m_Children )
            children_list.push_back( child_body.get() );
        return children_list;
    }

    std::vector<const TKinematicTreeBody*> TKinematicTreeBody::children() const
    {
        std::vector<const TKinematicTreeBody*> children_list;
        for ( auto& child_body : m_Children )
            children_list.push_back( child_body.get() );
        return children_list;
    }

    void TKinematicTreeBody::_InitializeInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
        else if ( m_KinematicTreeRef )
            m_tf = m_KinematicTreeRef->tf() * m_LocalTf;
        else
            LOCO_CORE_WARN( "TKinematicTreeBody::_InitializeInternal >>> non-root body {0} is dangling without parent", m_name );

        if ( m_BodyAdapterRef )
            m_BodyAdapterRef->Initialize();

        if ( m_Joint )
            m_Joint->Initialize();
        if ( m_Collider )
            m_Collider->Initialize();
        if ( m_Drawable )
            m_Drawable->Initialize();

        for ( auto& child_body : m_Children )
            child_body->Initialize();
    }

    void TKinematicTreeBody::_PreStepInternal()
    {
        if ( m_BodyAdapterRef )
        {
            m_BodyAdapterRef->SetForceCOM( m_TotalForce );
            m_BodyAdapterRef->SetTorqueCOM( m_TotalTorque );
        }

        if ( m_Joint )
            m_Joint->PreStep();
        if ( m_Collider )
            m_Collider->PreStep();
        if ( m_Drawable )
            m_Drawable->PreStep();

        for ( auto& child_body : m_Children )
            child_body->PreStep();
    }

    void TKinematicTreeBody::_PostStepInternal()
    {
        if ( m_BodyAdapterRef )
            m_BodyAdapterRef->GetTransform( m_tf );

        if ( m_Joint )
            m_Joint->PostStep();
        if ( m_Collider )
            m_Collider->PostStep();
        if ( m_Drawable )
            m_Drawable->PostStep();

        for ( auto& child_body : m_Children )
            child_body->PostStep();

        m_TotalForce = { 0.0f, 0.0f, 0.0f };
        m_TotalTorque = { 0.0f, 0.0f, 0.0f };
    }

    void TKinematicTreeBody::_ResetInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
        else if ( m_KinematicTreeRef )
            m_tf = m_KinematicTreeRef->tf() * m_LocalTf;
        else
            LOCO_CORE_WARN( "TKinematicTreeBody::_ResetInternal >>> non-root body {0} is dangling without parent", m_name );

        if ( m_BodyAdapterRef )
            m_BodyAdapterRef->Reset();

        if ( m_Joint )
            m_Joint->Reset();
        if ( m_Collider )
            m_Collider->Reset();
        if ( m_Drawable )
            m_Drawable->Reset();

        for ( auto& child_body : m_Children )
            child_body->Reset();

        m_TotalForce = { 0.0f, 0.0f, 0.0f };
        m_TotalTorque = { 0.0f, 0.0f, 0.0f };
    }

    void TKinematicTreeBody::_DetachSimInternal()
    {
        if ( m_BodyAdapterRef )
            m_BodyAdapterRef->OnDetach();
        m_BodyAdapterRef = nullptr;

        if ( m_Joint )
            m_Joint->DetachSim();
        if ( m_Collider )
            m_Collider->DetachSim();
        if ( m_Drawable )
            m_Drawable->DetachSim();

        for ( auto& child_body : m_Children )
            child_body->DetachSim();
    }

    void TKinematicTreeBody::_DetachVizInternal()
    {
        if ( m_Joint )
            m_Joint->DetachViz();
        if ( m_Collider )
            m_Collider->DetachViz();
        if ( m_Drawable )
            m_Drawable->DetachViz();

        for ( auto& child_body : m_Children )
            child_body->DetachViz();
    }

    void TKinematicTreeBody::_SetTransformInternal( const TMat4& transform )
    {
        LOCO_CORE_WARN( "TKinematicTreeBody::_SetTransformInternal >>> body objects don't have \
                        this functionality. Change the transform of the parent body instead. (body={0})", m_name );
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
    }

    //--------------------------------------------------------------------------------------------//
    //                                                                                            //
    //                              Kinematic-Tree-Body Primitives                                //
    //                                                                                            //
    //--------------------------------------------------------------------------------------------//

    //********************************************************************************************//
    //                                Box primitive Implementation                                //
    //********************************************************************************************//

    TBox::TBox( const std::string& name,
                const TVec3& extents,
                const int& collision_group,
                const int& collision_mask )
        : TKinematicTreeBody( name )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::BOX;
        collider_data.size = extents;
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::BOX;
        visual_data.size = extents;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );
        m_Drawable->SetLocalTransform( TMat4() );

        m_Data.collider = collider_data;
        m_Data.drawable = visual_data;
        m_Extents = extents;
    }

    void TBox::SetExtents( const TVec3& extents )
    {
        m_Extents = extents;
        m_Collider->ChangeSize( m_Extents );
        m_Drawable->ChangeSize( m_Extents );
    }

    void TBox::SetWidth( const TScalar& width )
    {
        m_Extents.x() = width;
        m_Collider->ChangeSize( m_Extents );
        m_Drawable->ChangeSize( m_Extents );
    }

    void TBox::SetDepth( const TScalar& depth )
    {
        m_Extents.y() = depth;
        m_Collider->ChangeSize( m_Extents );
        m_Drawable->ChangeSize( m_Extents );
    }

    void TBox::SetHeight( const TScalar& height )
    {
        m_Extents.z() = height;
        m_Collider->ChangeSize( m_Extents );
        m_Drawable->ChangeSize( m_Extents );
    }

    //********************************************************************************************//
    //                               Sphere primitive Implementation                              //
    //********************************************************************************************//

    TSphere::TSphere( const std::string& name,
                      const TScalar& radius,
                      const int& collision_group,
                      const int& collision_mask )
        : TKinematicTreeBody( name )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::SPHERE;
        collider_data.size = { radius, 0.0f, 0.0f };
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::SPHERE;
        visual_data.size = { radius, 0.0f, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );
        m_Drawable->SetLocalTransform( TMat4() );

        m_Data.collider = collider_data;
        m_Data.drawable = visual_data;
        m_Radius = radius;
    }

    void TSphere::SetRadius( const TScalar& radius )
    {
        m_Radius = radius;
        m_Collider->ChangeSize( { m_Radius, 0.0f, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, 0.0f, 0.0f } );
    }

    //********************************************************************************************//
    //                              Cylinder primitive Implementation                             //
    //********************************************************************************************//

    TCylinder::TCylinder( const std::string& name,
                          const TScalar& radius,
                          const TScalar& height,
                          const int& collision_group,
                          const int& collision_mask )
        : TKinematicTreeBody( name )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::CYLINDER;
        collider_data.size = { radius, height, 0.0f };
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::CYLINDER;
        visual_data.size = { radius, height, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );
        m_Drawable->SetLocalTransform( TMat4() );

        m_Data.collider = collider_data;
        m_Data.drawable = visual_data;
        m_Radius = radius;
        m_Height = height;
    }

    void TCylinder::SetRadius( const TScalar& radius )
    {
        m_Radius = radius;
        m_Collider->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    void TCylinder::SetHeight( const TScalar& height )
    {
        m_Height = height;
        m_Collider->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    //********************************************************************************************//
    //                              Capsule primitive Implementation                              //
    //********************************************************************************************//

    TCapsule::TCapsule( const std::string& name,
                        const TScalar& radius,
                        const TScalar& height,
                        const int& collision_group,
                        const int& collision_mask )
        : TKinematicTreeBody( name )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::CAPSULE;
        collider_data.size = { radius, height, 0.0f };
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::CAPSULE;
        visual_data.size = { radius, height, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );
        m_Drawable->SetLocalTransform( TMat4() );

        m_Data.collider = collider_data;
        m_Data.drawable = visual_data;
        m_Radius = radius;
        m_Height = height;
    }

    void TCapsule::SetRadius( const TScalar& radius )
    {
        m_Radius = radius;
        m_Collider->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    void TCapsule::SetHeight( const TScalar& height )
    {
        m_Height = height;
        m_Collider->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    //********************************************************************************************//
    //                            Ellipsoid primitive Implementation                              //
    //********************************************************************************************//

    TEllipsoid::TEllipsoid( const std::string& name,
                            const TVec3& radii,
                            const int& collision_group,
                            const int& collision_mask )
        : TKinematicTreeBody( name )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::ELLIPSOID;
        collider_data.size = radii;
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::ELLIPSOID;
        visual_data.size = radii;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );
        m_Drawable->SetLocalTransform( TMat4() );

        m_Data.collider = collider_data;
        m_Data.drawable = visual_data;
        m_Radii = radii;
    }

    void TEllipsoid::SetRadii( const TVec3& radii )
    {
        m_Radii = radii;
        m_Collider->ChangeSize( m_Radii );
        m_Drawable->ChangeSize( m_Radii );
    }

    void TEllipsoid::SetRadiusX( const TScalar& radius_x )
    {
        m_Radii.x() = radius_x;
        m_Collider->ChangeSize( m_Radii );
        m_Drawable->ChangeSize( m_Radii );
    }

    void TEllipsoid::SetRadiusY( const TScalar& radius_y )
    {
        m_Radii.y() = radius_y;
        m_Collider->ChangeSize( m_Radii );
        m_Drawable->ChangeSize( m_Radii );
    }

    void TEllipsoid::SetRadiusZ( const TScalar& radius_z )
    {
        m_Radii.z() = radius_z;
        m_Collider->ChangeSize( m_Radii );
        m_Drawable->ChangeSize( m_Radii );
    }

    //********************************************************************************************//
    //                               Mesh primitive Implementation                                //
    //********************************************************************************************//

    TConvexMesh::TConvexMesh( const std::string& name,
                              const std::string& mesh_collider_filepath,
                              const std::string& mesh_visual_filepath,
                              const TScalar& mesh_scale,
                              const int& collision_group,
                              const int& collision_mask )
        : TKinematicTreeBody( name )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::CONVEX_MESH;
        collider_data.size = { mesh_scale, mesh_scale, mesh_scale };
        collider_data.mesh_data.filename = mesh_collider_filepath;
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::CONVEX_MESH;
        visual_data.size = { mesh_scale, mesh_scale, mesh_scale };
        visual_data.mesh_data.filename = mesh_visual_filepath;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );
        m_Drawable->SetLocalTransform( TMat4() );

        m_Data.collider = collider_data;
        m_Data.drawable = visual_data;
        m_Scale = mesh_scale;
    }

    void TConvexMesh::SetScale( const TScalar& scale )
    {
        m_Scale = scale;
        m_Collider->ChangeSize( { m_Scale, m_Scale, m_Scale } );
        m_Drawable->ChangeSize( { m_Scale, m_Scale, m_Scale } );
    }
}}
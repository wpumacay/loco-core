
#include <kinematic_trees/loco_kinematic_tree_body.h>

namespace loco {
namespace kintree {

    //--------------------------------------------------------------------------------------------//
    //                                                                                            //
    //                               Kinematic-Tree-Body Interface                                //
    //                                                                                            //
    //--------------------------------------------------------------------------------------------//

    TKinematicTreeBody::TKinematicTreeBody( const std::string& name )
        : TObject( name )
    {
    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TKinematicTreeBody {0} @ {1} ", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TKinematicTreeBody " << m_name << " @ " << tinyutils::PointerToHexAddress( this );
    #endif
    }

    TKinematicTreeBody::TKinematicTreeBody( const std::string& name,
                                            const TKinematicTreeBodyData& body_data )
        : TObject( name )
    {
        m_Data = body_data;

        for ( auto collider_data : body_data.colliders )
        {
            m_Colliders.push_back( std::make_unique<TKinematicTreeCollider>( name + SUFFIX_COLLIDER, collider_data ) );
            m_Colliders.back()->SetParentBody( this, collider_data.localTransform );
        }

        for ( auto drawable_data : body_data.drawables )
        {
            m_Drawables.push_back( std::make_unique<visualizer::TDrawable>( name + SUFFIX_DRAWABLE, drawable_data ) );
            m_Drawables.back()->SetParentObject( this );
            m_Drawables.back()->SetLocalTransform( drawable_data.localTransform );
        }

        for ( auto joint_data : body_data.joints )
        {
            /**/ if ( joint_data.type == eJointType::REVOLUTE )
                m_Joints.push_back( std::make_unique<TKinematicTreeRevoluteJoint>( 
                                                        name + SUFFIX_JOINT,
                                                        joint_data.local_tf,
                                                        joint_data.local_axis,
                                                        joint_data.limits,
                                                        joint_data.stiffness,
                                                        joint_data.armature ) );
            else if ( joint_data.type == eJointType::PRISMATIC )
                m_Joints.push_back( std::make_unique<TKinematicTreePrismaticJoint>(
                                                        name + SUFFIX_JOINT,
                                                        joint_data.local_tf,
                                                        joint_data.local_axis,
                                                        joint_data.limits,
                                                        joint_data.stiffness,
                                                        joint_data.armature ) );
            else if ( joint_data.type == eJointType::SPHERICAL )
                m_Joints.push_back( std::make_unique<TKinematicTreeSphericalJoint>(
                                                        name + SUFFIX_JOINT,
                                                        joint_data.local_tf,
                                                        joint_data.limits,
                                                        joint_data.stiffness,
                                                        joint_data.armature ) );
            else if ( joint_data.type == eJointType::PLANAR )
                m_Joints.push_back( std::make_unique<TKinematicTreePlanarJoint>(
                                                        name + SUFFIX_JOINT,
                                                        joint_data.plane_axis_1,
                                                        joint_data.plane_axis_2 ) );
            else if ( joint_data.type == eJointType::FIXED )
                m_Joints.push_back( std::make_unique<TKinematicTreeFixedJoint>(
                                                        name + SUFFIX_JOINT,
                                                        joint_data.local_tf ) );
            else if ( joint_data.type == eJointType::FREE )
                m_Joints.push_back( std::make_unique<TKinematicTreeFreeJoint>( name + SUFFIX_JOINT ) );
            else
                LOCO_CORE_CRITICAL( "TKinematicTreeBody >>> found undefined joint-type while constructing kintree-body {0}", m_name );
            m_Joints.back()->SetParentBody( this, joint_data.local_tf );
        }

    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TKinematicTreeBody {0} @ {1} ", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TKinematicTreeBody " << m_name << " @ " << tinyutils::PointerToHexAddress( this );
    #endif
    }

    TKinematicTreeBody::~TKinematicTreeBody()
    {
        DetachSim();
        DetachViz();

        m_KinematicTreeRef = nullptr;
        m_BodyAdapterRef = nullptr;
        m_ParentBodyRef = nullptr;

        m_Colliders.clear();
        m_Drawables.clear();
        m_Joints.clear();

    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TKinematicTreeBody {0} @ {1} ", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TKinematicTreeBody " << m_name << " @ " << tinyutils::PointerToHexAddress( this );
    #endif
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
        m_tf = m_ParentBodyRef->tf() * m_LocalTf;
    }

    void TKinematicTreeBody::AddDrawable( std::unique_ptr<visualizer::TDrawable> drawable, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( drawable, "TKinematicTreeBody::AddDrawable >>> tried adding nullptr to kintree-body {0}", m_name );
        m_Drawables.push_back( std::move( drawable ) );
        m_Drawables.back()->SetParentObject( this );
        m_Drawables.back()->SetLocalTransform( local_tf );
    }

    void TKinematicTreeBody::AddCollider( std::unique_ptr<TKinematicTreeCollider> collider, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( collider, "TKinematicTreeBody::AddCollider >>> tried adding nullptr to kintree-body {0}", m_name );
        m_Colliders.push_back( std::move( collider ) );
        m_Colliders.back()->SetParentBody( this, local_tf );
    }

    void TKinematicTreeBody::AddJoint( std::unique_ptr<TKinematicTreeJoint> joint, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( joint, "TKinematicTreeBody::AddJoint >>> tried adding nullptr to kintree-body {0}", m_name );
        m_Joints.push_back( std::move( joint ) );
        m_Joints.back()->SetParentBody( this, local_tf );
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

    void TKinematicTreeBody::Forward()
    {
        if ( m_ParentBodyRef ) // Non-root bodies update from its parent's transform and the joint-transform
        {
            if ( m_Joints.size() > 1 ) // For multiple-joint bodies, we have to do some different computation
            {
                LOCO_CORE_WARN( "TKinematicTreeBody::Forward >>> multi-joint kintree bodies don't support forward-kin. yet \
                                (while processing kintree-body {0})", m_name );
            }
            else if ( m_Joints.size() == 1 )
            {
                const auto joint_ref = m_Joints[0].get();
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
                    LOCO_CORE_WARN( "TKinematicTreeBody::Forward >>> FK using DH-representation is not \
                                     supported for joint type {0} (while processing kintree-body {1})", loco::ToString( joint_type ), m_name );
                }
            }
            else
            {
                LOCO_CORE_WARN( "TKinematicTreeBody::Forward >>> kintree-body {0} has no joints (should at leas be connected through 1", m_name );
            }
        }
        else if ( m_KinematicTreeRef )
        {
        }

        for ( auto& child_body : m_Children )
            child_body->Forward();
    }

    std::vector<visualizer::TDrawable*> TKinematicTreeBody::drawables()
    {
        std::vector<visualizer::TDrawable*> drawables_list;
        for ( auto& drawable : m_Drawables )
            drawables_list.push_back( drawable.get() );
        return drawables_list;
    }

    std::vector<const visualizer::TDrawable*> TKinematicTreeBody::drawables() const
    {
        std::vector<const visualizer::TDrawable*> drawables_list;
        for ( auto& drawable : m_Drawables )
            drawables_list.push_back( drawable.get() );
        return drawables_list;
    }

    std::vector<TKinematicTreeCollider*> TKinematicTreeBody::colliders()
    {
        std::vector<TKinematicTreeCollider*> colliders_list;
        for ( auto& collider : m_Colliders )
            colliders_list.push_back( collider.get() );
        return colliders_list;
    }

    std::vector<const TKinematicTreeCollider*> TKinematicTreeBody::colliders() const
    {
        std::vector<const TKinematicTreeCollider*> colliders_list;
        for ( auto& collider : m_Colliders )
            colliders_list.push_back( collider.get() );
        return colliders_list;
    }

    std::vector<TKinematicTreeJoint*> TKinematicTreeBody::joints()
    {
        std::vector<TKinematicTreeJoint*> joints_list;
        for ( auto& joint : m_Joints )
            joints_list.push_back( joint.get() );
        return joints_list;
    }

    std::vector<const TKinematicTreeJoint*> TKinematicTreeBody::joints() const
    {
        std::vector<const TKinematicTreeJoint*> joints_list;
        for ( auto& joint : m_Joints )
            joints_list.push_back( joint.get() );
        return joints_list;
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

        for ( auto& joint : m_Joints )
            joint->Initialize();
        for ( auto& collider : m_Colliders )
            collider->Initialize();
        for ( auto& drawable : m_Drawables )
            drawable->Initialize();
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

        for ( auto& joint : m_Joints )
            joint->PreStep();
        for ( auto& collider : m_Colliders )
            collider->PreStep();
        for ( auto& drawable : m_Drawables )
            drawable->PreStep();
        for ( auto& child_body : m_Children )
            child_body->PreStep();
    }

    void TKinematicTreeBody::_PostStepInternal()
    {
        if ( m_BodyAdapterRef )
            m_BodyAdapterRef->GetTransform( m_tf );

        for ( auto& joint : m_Joints )
            joint->PostStep();
        for ( auto& collider : m_Colliders )
            collider->PostStep();
        for ( auto& drawable : m_Drawables )
            drawable->PostStep();
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

        for ( auto& joint : m_Joints )
            joint->Reset();
        for ( auto& collider : m_Colliders )
            collider->Reset();
        for ( auto& drawable : m_Drawables )
            drawable->Reset();
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

        for ( auto& joint : m_Joints )
            joint->DetachSim();
        for ( auto& collider : m_Colliders )
            collider->DetachSim();
        for ( auto& drawable : m_Drawables )
            drawable->DetachSim();
        for ( auto& child_body : m_Children )
            child_body->DetachSim();
    }

    void TKinematicTreeBody::_DetachVizInternal()
    {
        for ( auto& joint : m_Joints )
            joint->DetachViz();
        for ( auto& collider : m_Colliders )
            collider->DetachViz();
        for ( auto& drawable : m_Drawables )
            drawable->DetachViz();
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
        m_Colliders.push_back( std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data ) );
        m_Colliders.back()->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::BOX;
        visual_data.size = extents;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawables.push_back( std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data ) );
        m_Drawables.back()->SetParentObject( this );
        m_Drawables.back()->SetLocalTransform( TMat4() );

        m_Data.colliders.push_back( collider_data );
        m_Data.drawables.push_back( visual_data );
        m_Extents = extents;
    }

    void TBox::SetExtents( const TVec3& extents )
    {
        m_Extents = extents;
        m_Colliders.back()->ChangeSize( m_Extents );
        m_Drawables.back()->ChangeSize( m_Extents );
    }

    void TBox::SetWidth( const TScalar& width )
    {
        m_Extents.x() = width;
        m_Colliders.back()->ChangeSize( m_Extents );
        m_Drawables.back()->ChangeSize( m_Extents );
    }

    void TBox::SetDepth( const TScalar& depth )
    {
        m_Extents.y() = depth;
        m_Colliders.back()->ChangeSize( m_Extents );
        m_Drawables.back()->ChangeSize( m_Extents );
    }

    void TBox::SetHeight( const TScalar& height )
    {
        m_Extents.z() = height;
        m_Colliders.back()->ChangeSize( m_Extents );
        m_Drawables.back()->ChangeSize( m_Extents );
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
        m_Colliders.push_back( std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data ) );
        m_Colliders.back()->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::SPHERE;
        visual_data.size = { radius, 0.0f, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawables.push_back( std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data ) );
        m_Drawables.back()->SetParentObject( this );
        m_Drawables.back()->SetLocalTransform( TMat4() );

        m_Data.colliders.push_back( collider_data );
        m_Data.drawables.push_back( visual_data );
        m_Radius = radius;
    }

    void TSphere::SetRadius( const TScalar& radius )
    {
        m_Radius = radius;
        m_Colliders.back()->ChangeSize( { m_Radius, 0.0f, 0.0f } );
        m_Drawables.back()->ChangeSize( { m_Radius, 0.0f, 0.0f } );
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
        m_Colliders.push_back( std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data ) );
        m_Colliders.back()->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::CYLINDER;
        visual_data.size = { radius, height, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawables.push_back( std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data ) );
        m_Drawables.back()->SetParentObject( this );
        m_Drawables.back()->SetLocalTransform( TMat4() );

        m_Data.colliders.push_back( collider_data );
        m_Data.drawables.push_back( visual_data );
        m_Radius = radius;
        m_Height = height;
    }

    void TCylinder::SetRadius( const TScalar& radius )
    {
        m_Radius = radius;
        m_Colliders.back()->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawables.back()->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    void TCylinder::SetHeight( const TScalar& height )
    {
        m_Height = height;
        m_Colliders.back()->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawables.back()->ChangeSize( { m_Radius, m_Height, 0.0f } );
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
        m_Colliders.push_back( std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data ) );
        m_Colliders.back()->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::CAPSULE;
        visual_data.size = { radius, height, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawables.push_back( std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data ) );
        m_Drawables.back()->SetParentObject( this );
        m_Drawables.back()->SetLocalTransform( TMat4() );

        m_Data.colliders.push_back( collider_data );
        m_Data.drawables.push_back( visual_data );
        m_Radius = radius;
        m_Height = height;
    }

    void TCapsule::SetRadius( const TScalar& radius )
    {
        m_Radius = radius;
        m_Colliders.back()->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawables.back()->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    void TCapsule::SetHeight( const TScalar& height )
    {
        m_Height = height;
        m_Colliders.back()->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawables.back()->ChangeSize( { m_Radius, m_Height, 0.0f } );
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
        m_Colliders.push_back( std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data ) );
        m_Colliders.back()->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::ELLIPSOID;
        visual_data.size = radii;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawables.push_back( std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data ) );
        m_Drawables.back()->SetParentObject( this );
        m_Drawables.back()->SetLocalTransform( TMat4() );

        m_Data.colliders.push_back( collider_data );
        m_Data.drawables.push_back( visual_data );
        m_Radii = radii;
    }

    void TEllipsoid::SetRadii( const TVec3& radii )
    {
        m_Radii = radii;
        m_Colliders.back()->ChangeSize( m_Radii );
        m_Drawables.back()->ChangeSize( m_Radii );
    }

    void TEllipsoid::SetRadiusX( const TScalar& radius_x )
    {
        m_Radii.x() = radius_x;
        m_Colliders.back()->ChangeSize( m_Radii );
        m_Drawables.back()->ChangeSize( m_Radii );
    }

    void TEllipsoid::SetRadiusY( const TScalar& radius_y )
    {
        m_Radii.y() = radius_y;
        m_Colliders.back()->ChangeSize( m_Radii );
        m_Drawables.back()->ChangeSize( m_Radii );
    }

    void TEllipsoid::SetRadiusZ( const TScalar& radius_z )
    {
        m_Radii.z() = radius_z;
        m_Colliders.back()->ChangeSize( m_Radii );
        m_Drawables.back()->ChangeSize( m_Radii );
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
        collider_data.type = eShapeType::MESH;
        collider_data.size = { mesh_scale, mesh_scale, mesh_scale };
        collider_data.mesh_data.filename = mesh_collider_filepath;
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Colliders.push_back( std::make_unique<TKinematicTreeCollider>( name + loco::SUFFIX_COLLIDER, collider_data ) );
        m_Colliders.back()->SetParentBody( this, TMat4() );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::MESH;
        visual_data.size = { mesh_scale, mesh_scale, mesh_scale };
        visual_data.mesh_data.filename = mesh_visual_filepath;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawables.push_back( std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data ) );
        m_Drawables.back()->SetParentObject( this );
        m_Drawables.back()->SetLocalTransform( TMat4() );

        m_Data.colliders.push_back( collider_data );
        m_Data.drawables.push_back( visual_data );
        m_Scale = mesh_scale;
    }

    void TConvexMesh::SetScale( const TScalar& scale )
    {
        m_Scale = scale;
        m_Colliders.back()->ChangeSize( { m_Scale, m_Scale, m_Scale } );
        m_Drawables.back()->ChangeSize( { m_Scale, m_Scale, m_Scale } );
    }
}}
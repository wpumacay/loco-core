
#include <kinematic_trees/parsers/loco_kinematic_tree_urdf_parser.h>

namespace loco {
namespace kintree {

    ssize_t TKinematicTreeUrdfParser::NUM_UNNAMED_DRAWABLES = 0;
    ssize_t TKinematicTreeUrdfParser::NUM_UNNAMED_COLLIDERS = 0;
    ssize_t TKinematicTreeUrdfParser::NUM_UNNAMED_JOINTS = 0;
    ssize_t TKinematicTreeUrdfParser::NUM_UNNAMED_BODIES = 0;

    void TKinematicTreeUrdfParser::parse_kintree( const std::string& filepath_urdf )
    {
        m_UrdfModel = parsing::TElement::CreateFromXmlFile( parsing::eSchemaType::URDF, filepath_urdf );
        m_CurrentFilepath = filepath_urdf;
        m_CurrentFolderpath = tinyutils::GetFolderpath( filepath_urdf );
        parse_kintree();
    }

    void TKinematicTreeUrdfParser::parse_kintree( const parsing::TElement* model_urdf )
    {
        m_UrdfModel = parsing::TElement::CloneElement( model_urdf );
        m_CurrentFilepath = m_UrdfModel->GetString( "filepath", "" );
        m_CurrentFolderpath = m_UrdfModel->GetString( "folderpath", "" );
        parse_kintree();
    }

    void TKinematicTreeUrdfParser::parse_kintree()
    {
        if ( !m_UrdfModel )
        {
            LOCO_CORE_ERROR( "TKinematicTreeUrdfParser::parse_kintree >>> tried parsing using null urdf-model. \
                              Use the other parse_kintree functions instead" );
            return;
        }

        collect_materials( m_UrdfModel.get() );
        collect_bodies( m_UrdfModel.get() );
        collect_joints( m_UrdfModel.get() );
        connect_kintree();
    }

    void TKinematicTreeUrdfParser::collect_materials( const parsing::TElement* robot_elm )
    {
        // Collect materials stored at the beginning of the robot model
        auto materials_elms = robot_elm->GetChildrenOfType( "material" );
        for ( auto material_elm : materials_elms )
        {
            const auto material_name = material_elm->GetString( "name" );
            if ( m_Materials.find( material_name ) == m_Materials.end() )
            {
                m_Materials[material_name] = TGenericParams();
                if ( auto color_elm = material_elm->GetFirstChildOfType( "color" ) )
                    m_Materials[material_name].Set( "rgba", color_elm->GetVec4( "rgba", loco::DEFAULT_RGBA_COLOR ) );
            }
        }

        // Collect materials embedded into the links themselves
        auto link_elms = robot_elm->GetChildrenOfType( "link" );
        for ( auto link_elm : link_elms )
        {
            if ( auto visual_elm = link_elm->GetFirstChildOfType( "visual" ) )
            {
                if ( auto material_elm = visual_elm->GetFirstChildOfType( "material" ) )
                {
                    const auto material_name = material_elm->GetString( "name" );
                    if ( m_Materials.find( material_name ) == m_Materials.end() )
                    {
                        m_Materials[material_name] = TGenericParams();
                        if ( auto color_elm = material_elm->GetFirstChildOfType( "color" ) )
                            m_Materials[material_name].Set( "rgba", color_elm->GetVec4( "rgba", loco::DEFAULT_RGBA_COLOR ) );
                    }
                }
            }
        }
    }

    void TKinematicTreeUrdfParser::collect_bodies( const parsing::TElement* robot_elm )
    {
        auto bodies_elms = robot_elm->GetChildrenOfType( "link" );
        for ( auto body_elm : bodies_elms )
        {
            auto kintree_body = parse_body( body_elm );
            auto colliders_elms = body_elm->GetChildrenOfType( "collision" );
            for ( auto collider_elm : colliders_elms )
            {
                auto kintree_collider_local_tf_pair = parse_collider( collider_elm, kintree_body->name() );
                auto kintree_collider = std::move( kintree_collider_local_tf_pair.first );
                auto kintree_collider_local_tf = kintree_collider_local_tf_pair.second;
                kintree_body->AddCollider( std::move( kintree_collider ), kintree_collider_local_tf );
            }
            auto drawables_elms = body_elm->GetChildrenOfType( "visual" );
            for ( auto drawable_elm : drawables_elms )
            {
                auto kintree_drawable_local_tf_pair = parse_drawable( drawable_elm, kintree_body->name() );
                auto kintree_drawable = std::move( kintree_drawable_local_tf_pair.first );
                auto kintree_drawable_local_tf = kintree_drawable_local_tf_pair.second;
                kintree_body->AddDrawable( std::move( kintree_drawable ), kintree_drawable_local_tf );
            }
            const auto body_name = kintree_body->name();
            m_TempStorageBodies[body_name] = std::move( kintree_body );
        }
    }

    void TKinematicTreeUrdfParser::collect_joints( const parsing::TElement* robot_elm )
    {
        auto joints_elms = robot_elm->GetChildrenOfType( "joint" );
        for ( auto joint_elm : joints_elms )
        {
            auto kintree_joint = parse_joint( joint_elm );
            const auto joint_name = kintree_joint->name();
            m_TempStorageJoints[joint_name] = std::move( kintree_joint );
        }
    }

    void TKinematicTreeUrdfParser::connect_kintree()
    {
        reorder_treelike( m_JointLinkConnections );
        for ( auto& connection : m_JointLinkConnections )
        {
            const std::string joint_name = std::get<0>( connection );
            const std::string parent_link_name = std::get<1>( connection );
            const std::string child_link_name = std::get<2>( connection );
            const TMat4 tf_child_link_to_parent_link = std::get<3>( connection );

            if ( m_TempStorageJoints.find( joint_name ) == m_TempStorageJoints.end() )
            {
                LOCO_CORE_ERROR( "TKinematicTreeUrdfParser::connect_kintree >>> couldn't find joint {0}. \
                                  Error found while processing kintree {1} and file {2}", joint_name, m_KintreeRef->name(), m_CurrentFilepath );
                continue;
            }
            if ( m_TempStorageBodies.find( parent_link_name ) == m_TempStorageBodies.end() )
            {
                LOCO_CORE_ERROR( "TKinematicTreeUrdfParser::connect_kintree >>> couldn't find parent-link {0}. \
                                  Error found while processing kintree {1} and file {2}", parent_link_name, m_KintreeRef->name(), m_CurrentFilepath );
                continue;
            }
            if ( m_TempStorageBodies.find( child_link_name ) == m_TempStorageBodies.end() )
            {
                LOCO_CORE_ERROR( "TKinematicTreeUrdfParser::connect_kintree >>> couldn't find child-link {0}. \
                                  Error found while processing kintree {1} and file {2}", child_link_name, m_KintreeRef->name(), m_CurrentFilepath );
                continue;
            }

            auto kintree_joint = std::move( m_TempStorageJoints[joint_name] );
            auto kintree_parent_body = std::move( m_TempStorageBodies[parent_link_name] );
            auto kintree_child_body = std::move( m_TempStorageBodies[child_link_name] );
            kintree_child_body->AddJoint( std::move( kintree_joint ), TMat4() ); // Joint frame concides with owner body frame
            kintree_parent_body->AddChild( std::move( kintree_child_body ), tf_child_link_to_parent_link );
            m_TempStorageBodies[parent_link_name] = std::move( kintree_parent_body );
        }

        // Sanity check (make sure there is only one root body) ------------------------------------
        ssize_t num_roots = 0;
        for ( auto& kv_pair : m_TempStorageBodies )
            if ( kv_pair.second )
                num_roots++;

        if ( num_roots != 1 )
            LOCO_CORE_ERROR( "TKinematicTreeUrdfParser::connect_kintree >>> found multiple root bodies. \
                              Error found while processing kintree {0} and file {1}", m_KintreeRef->name(), m_CurrentFilepath );
        //------------------------------------------------------------------------------------------
        std::unique_ptr<TKinematicTreeBody> kintree_root_body = nullptr;
        for ( auto& kv_pair : m_TempStorageBodies )
        {
            if ( kv_pair.second )
                kintree_root_body = std::move( kv_pair.second );
        }
        if ( kintree_root_body )
            m_KintreeRef->SetRoot( std::move( kintree_root_body ) );
    }

    void TKinematicTreeUrdfParser::reorder_treelike( std::vector<std::tuple<std::string, std::string, std::string, TMat4>>& connections )
    {
        std::set<std::string> bodies;
        std::set<std::string> have_parent;
        for ( auto& tuple : connections )
        {
            bodies.insert( std::get<1>( tuple ) );
            bodies.insert( std::get<2>( tuple ) );
            have_parent.insert( std::get<2>( tuple ) );
        }
        std::string root_body_name;
        for ( auto body_name : bodies )
        {
            if ( have_parent.find( body_name ) == have_parent.end() )
            {
                root_body_name = body_name;
                break;
            }
        }
        LOCO_CORE_ASSERT( root_body_name != "", "TKinematicTreeUrdfParser::reorder_treelike >>> kintree has no root. \
                          Error found while processing kintree {0} and file {1}", m_KintreeRef->name(), m_CurrentFilepath );

        std::vector<ssize_t> top_to_bottom_order;
        std::queue<std::string> traversal_top_bot;
        traversal_top_bot.push( root_body_name );
        ssize_t current_index = connections.size() - 1;
        while ( !traversal_top_bot.empty() )
        {
            const auto current_body_name = traversal_top_bot.front();
            traversal_top_bot.pop();
            for ( ssize_t indx = 0; indx < connections.size(); indx++ )
            {
                const auto parent_name = std::get<1>( connections[indx] );
                const auto child_name = std::get<2>( connections[indx] );
                if ( parent_name == current_body_name )
                {
                    top_to_bottom_order.push_back( indx );
                    traversal_top_bot.push( child_name );
                }
            }
        }

        LOCO_CORE_ASSERT( top_to_bottom_order.size() == connections.size(), "TKinematicTreeUrdfParser::reorder_treelike >>> \
                          Urdf model seems to not contain a valid tree structure. Error found while processing kintree {0} and file {1}",
                          m_KintreeRef->name(), m_CurrentFilepath );

        std::vector<std::tuple<std::string, std::string, std::string, TMat4>> temp( connections.size() );
        for ( ssize_t i = 0; i < connections.size(); i++ )
            temp[connections.size() - 1 - i] = connections[top_to_bottom_order[i]];
        for ( ssize_t i = 0; i < connections.size(); i++ )
            connections[i] = temp[i];

        std::cout << "finished???" << std::endl;
    }

    std::unique_ptr<TKinematicTreeBody> TKinematicTreeUrdfParser::parse_body( const parsing::TElement* body_elm )
    {
        TKinematicTreeBodyData kintree_body_data;
        std::string kintree_body_name = m_KintreeRef->name() + "_" + body_elm->GetString( "name" ) + loco::SUFFIX_BODY;
        if ( body_elm->HasChildOfType( "inertial" ) )
            kintree_body_data.inertia = parse_inertia( body_elm->GetFirstChildOfType( "inertial" ) );
        return std::make_unique<TKinematicTreeBody>( kintree_body_name, kintree_body_data );
    }

    std::pair<std::unique_ptr<TKinematicTreeCollider>, TMat4> TKinematicTreeUrdfParser::parse_collider( 
                                                                    const parsing::TElement* collider_elm, 
                                                                    const std::string& body_parent_name )
    {
        TCollisionData kintree_collider_data;
        std::string kintree_collider_name;
        if ( collider_elm->HasAttributeString( "name" ) )
            kintree_collider_name = body_parent_name + "_" + collider_elm->GetString( "name" ) + loco::SUFFIX_COLLIDER;
        else
            kintree_collider_name = body_parent_name + "_" + std::to_string( NUM_UNNAMED_COLLIDERS++ ) + loco::SUFFIX_COLLIDER;
        if ( collider_elm->HasChildOfType( "origin" ) )
            kintree_collider_data.localTransform = get_transform( collider_elm->GetFirstChildOfType( "origin" ) );
        get_shape_information( collider_elm->GetFirstChildOfType( "geometry" ), kintree_collider_data );

        // Use default values, as the URDF specification doesn't provide control over these parameters for the colliders
        kintree_collider_data.collisionGroup = 1;
        kintree_collider_data.collisionMask = 1;
        kintree_collider_data.friction = { 1.0f, 0.005f, 0.0001f };
        kintree_collider_data.density = loco::DEFAULT_DENSITY;

        auto kintree_collider = std::make_unique<TKinematicTreeCollider>( kintree_collider_name, kintree_collider_data );
        auto kintree_collider_local_tf = kintree_collider_data.localTransform;
        return { std::move( kintree_collider ), kintree_collider_local_tf };
    }

    std::pair<std::unique_ptr<visualizer::TDrawable>, TMat4> TKinematicTreeUrdfParser::parse_drawable(
                                                                const parsing::TElement* drawable_elm,
                                                                const std::string& body_parent_name )
    {
        TVisualData kintree_drawable_data;
        std::string kintree_drawable_name;
        if ( drawable_elm->HasAttributeString( "name" ) )
            kintree_drawable_name = body_parent_name + "_" + drawable_elm->GetString( "name" ) + loco::SUFFIX_DRAWABLE;
        else
            kintree_drawable_name = body_parent_name + "_" + std::to_string( NUM_UNNAMED_DRAWABLES++ ) + loco::SUFFIX_DRAWABLE;
        if ( drawable_elm->HasChildOfType( "origin" ) )
            kintree_drawable_data.localTransform = get_transform( drawable_elm->GetFirstChildOfType( "origin" ) );
        get_shape_information( drawable_elm->GetFirstChildOfType( "geometry" ), kintree_drawable_data );
        if ( auto material_elm = drawable_elm->GetFirstChildOfType( "material" ) )
        {
            const auto material_name = material_elm->GetString( "name" );
            const auto rgba = m_Materials[material_name].GetVec4( "rgba", loco::DEFAULT_RGBA_COLOR );
            kintree_drawable_data.ambient = { rgba.x(), rgba.y(), rgba.z() };
            kintree_drawable_data.diffuse = { rgba.x(), rgba.y(), rgba.z() };
            kintree_drawable_data.specular = loco::DEFAULT_SPECULAR_COLOR;
            kintree_drawable_data.shininess = loco::DEFAULT_SHININESS;
        }

        auto kintree_drawable = std::make_unique<visualizer::TDrawable>( kintree_drawable_name, kintree_drawable_data );
        auto kintree_drawable_local_tf = kintree_drawable_data.localTransform;
        return { std::move( kintree_drawable ), kintree_drawable_local_tf };
    }

    std::unique_ptr<TKinematicTreeJoint> TKinematicTreeUrdfParser::parse_joint( const parsing::TElement* joint_elm )
    {
        TKinematicTreeJointData kintree_joint_data;
        std::string kintree_joint_name;
        if ( joint_elm->HasAttributeString( "name" ) )
            kintree_joint_name = m_KintreeRef->name() + "_" + joint_elm->GetString( "name" ) + loco::SUFFIX_JOINT;
        else
            kintree_joint_name = m_KintreeRef->name() + "_" + std::to_string( NUM_UNNAMED_JOINTS++ ) + loco::SUFFIX_JOINT;
        kintree_joint_data.type = ToEnumJoint( joint_elm->GetString( "type" ) );
        // Joint frame coincides with (child|owner) body reference frame according to the urdf-specification
        kintree_joint_data.local_tf = TMat4();
        if ( auto axis_elm = joint_elm->GetFirstChildOfType( "axis" ) )
            kintree_joint_data.local_axis = axis_elm->GetVec3( "xyz", { 1.0f, 0.0f, 0.0f } );
        else
            kintree_joint_data.local_axis = { 1.0f, 0.0f, 0.0f };
        if ( auto limit_elm = joint_elm->GetFirstChildOfType( "limit" ) )
        {
            const auto lower_limit = limit_elm->GetFloat( "lower", 1.0f );
            const auto upper_limit = limit_elm->GetFloat( "upper",  -1.0f );
            kintree_joint_data.limits = { lower_limit, upper_limit };
        }
        if ( auto dynamics_elm = joint_elm->GetFirstChildOfType( "dynamics" ) )
            kintree_joint_data.damping = dynamics_elm->GetFloat( "damping", 0.0f );

        // Grab the connection that this joint makes between link (will be used later to connect the links appropriately) -----------
        auto child_elm = joint_elm->GetFirstChildOfType( "child" );
        auto parent_elm = joint_elm->GetFirstChildOfType( "parent" );
        LOCO_CORE_ASSERT( child_elm, "TKinematicTreeUrdfParser::parse_joint >>> child-elm is required for the joint-urdf specification. \
                                      Error found while processing kintree {0} and file {1}", m_KintreeRef->name(), m_CurrentFilepath );
        LOCO_CORE_ASSERT( parent_elm, "TKinematicTreeUrdfParser::parse_joint >>> parent-elm is required for the joint-urdf specification. \
                                       Error found while processing kintree {0} and file {1}", m_KintreeRef->name(), m_CurrentFilepath );
        const TMat4 tf_child_link_to_parent_link = get_transform( joint_elm->GetFirstChildOfType( "origin" ) );
        const std::string child_link_name = m_KintreeRef->name() + "_" + child_elm->GetString( "link" ) + loco::SUFFIX_BODY;
        const std::string parent_link_name = m_KintreeRef->name() + "_" + parent_elm->GetString( "link" ) + loco::SUFFIX_BODY;
        m_JointLinkConnections.push_back( std::make_tuple( kintree_joint_name, parent_link_name, child_link_name, tf_child_link_to_parent_link ) );
        // --------------------------------------------------------------------------------------------------------------------------

        /**/ if ( kintree_joint_data.type == eJointType::REVOLUTE )
            return std::make_unique<TKinematicTreeRevoluteJoint>( kintree_joint_name,
                                                                  kintree_joint_data.local_axis,
                                                                  kintree_joint_data.limits,
                                                                  kintree_joint_data.stiffness,
                                                                  kintree_joint_data.armature,
                                                                  kintree_joint_data.damping );
        else if ( kintree_joint_data.type == eJointType::PRISMATIC )
            return std::make_unique<TKinematicTreePrismaticJoint>( kintree_joint_name,
                                                                   kintree_joint_data.local_axis,
                                                                   kintree_joint_data.limits,
                                                                   kintree_joint_data.stiffness,
                                                                   kintree_joint_data.armature,
                                                                   kintree_joint_data.damping );
        else if ( kintree_joint_data.type == eJointType::SPHERICAL )
            return std::make_unique<TKinematicTreeSphericalJoint>( kintree_joint_name,
                                                                   kintree_joint_data.limits,
                                                                   kintree_joint_data.stiffness,
                                                                   kintree_joint_data.armature,
                                                                   kintree_joint_data.damping );
        else if ( kintree_joint_data.type == eJointType::PLANAR )
            return std::make_unique<TKinematicTreePlanarJoint>( kintree_joint_name,
                                                                kintree_joint_data.plane_axis_1,
                                                                kintree_joint_data.plane_axis_2 );
        else if ( kintree_joint_data.type == eJointType::FIXED )
            return std::make_unique<TKinematicTreeFixedJoint>( kintree_joint_name );
        else if ( kintree_joint_data.type == eJointType::FREE )
            return std::make_unique<TKinematicTreeFreeJoint>( kintree_joint_name );
        LOCO_CORE_CRITICAL( "TKinematicTreeUrdfParser::parse_joint >>> couldn't parse a valid kintree-joint. \
                             Error found while processing kintree {0} and file {1}", m_KintreeRef->name(), m_CurrentFilepath );
        return nullptr;
    }

    TInertialData TKinematicTreeUrdfParser::parse_inertia( const parsing::TElement* inertial_elm )
    {
        TInertialData kintree_body_inertial_data;
        if ( inertial_elm->HasChildOfType( "origin" ) )
            kintree_body_inertial_data.localTransform = get_transform( inertial_elm->GetFirstChildOfType( "origin" ) );
        if ( auto mass_elm = inertial_elm->GetFirstChildOfType( "mass" ) )
            kintree_body_inertial_data.mass = mass_elm->GetFloat( "value", 1.0f );
        if ( auto inertia_elm = inertial_elm->GetFirstChildOfType( "inertia" ) )
        {
            kintree_body_inertial_data.ixx = inertia_elm->GetFloat( "ixx", 0.1f );
            kintree_body_inertial_data.iyy = inertia_elm->GetFloat( "iyy", 0.1f );
            kintree_body_inertial_data.izz = inertia_elm->GetFloat( "izz", 0.1f );
            kintree_body_inertial_data.ixy = inertia_elm->GetFloat( "ixy", 0.0f );
            kintree_body_inertial_data.ixz = inertia_elm->GetFloat( "ixz", 0.0f );
            kintree_body_inertial_data.iyz = inertia_elm->GetFloat( "iyz", 0.0f );
        }

        return kintree_body_inertial_data;
    }

    TMat4 TKinematicTreeUrdfParser::get_transform( const parsing::TElement* origin_elm )
    {
        // If missing origin-elm, the transform is just the identity matrix
        if ( !origin_elm )
            return TMat4();

        const auto pos_xyz = origin_elm->GetVec3( "xyz", { 0.0f, 0.0f, 0.0f } );
        const auto rot_rpy = origin_elm->GetVec3( "rpy", { 0.0f, 0.0f, 0.0f } );
        return TMat4( tinymath::rotation( rot_rpy ), pos_xyz );
    }

    void TKinematicTreeUrdfParser::get_shape_information( const parsing::TElement* geometry_elm, TShapeData& dst_shape_data )
    {
        if ( !geometry_elm )
        {
            LOCO_CORE_ERROR( "TKinematicTreeUrdfParser::get_shape_information >>> geometry-elm is required in the urdf-file specification. \
                              Error found while processing kintree {0} and file {1}", m_KintreeRef->name(), m_CurrentFilepath );
            return;
        }

        /**/ if ( geometry_elm->HasChildOfType( "box" ) )
        {
            auto box_elm = geometry_elm->GetFirstChildOfType( "box" );
            const auto extents = box_elm->GetVec3( "size", { 0.1f, 0.1f, 0.1f } );
            dst_shape_data.type = eShapeType::BOX;
            dst_shape_data.size = extents;
        }
        else if ( geometry_elm->HasChildOfType( "sphere" ) )
        {
            auto sphere_elm = geometry_elm->GetFirstChildOfType( "sphere" );
            const auto radius = sphere_elm->GetFloat( "radius", 0.1f );
            dst_shape_data.type = eShapeType::SPHERE;
            dst_shape_data.size = { radius, 0.0f, 0.0f };
        }
        else if ( geometry_elm->HasChildOfType( "cylinder" ) )
        {
            auto cylinder_elm = geometry_elm->GetFirstChildOfType( "cylinder" );
            const auto radius = cylinder_elm->GetFloat( "radius", 0.1f );
            const auto height = cylinder_elm->GetFloat( "length", 0.1f );
            dst_shape_data.type = eShapeType::CYLINDER;
            dst_shape_data.size = { radius, height, 0.0f };
        }
        else if ( geometry_elm->HasChildOfType( "capsule" ) )
        {
            auto capsule_elm = geometry_elm->GetFirstChildOfType( "capsule" );
            const auto radius = capsule_elm->GetFloat( "radius", 0.1f );
            const auto height = capsule_elm->GetFloat( "length", 0.1f );
            dst_shape_data.type = eShapeType::CAPSULE;
            dst_shape_data.size = { radius, height, 0.0f };
        }
        else if ( geometry_elm->HasChildOfType( "mesh" ) )
        {
            auto mesh_elm = geometry_elm->GetFirstChildOfType( "mesh" );
            const auto scale = mesh_elm->GetVec3( "scale", { 1.0f, 1.0f, 1.0f } );
            const auto filename = mesh_elm->GetString( "filename", "" );
            dst_shape_data.type = eShapeType::CONVEX_MESH;
            dst_shape_data.size = scale;
            dst_shape_data.mesh_data.filename = m_CurrentFolderpath + filename;
        }
        else
        {
            LOCO_CORE_ERROR( "TKinematicTreeUrdfParser::get_shape_information >>> unsupported shape found while \
                              parsing kintree {0} and file {1}", m_KintreeRef->name(), m_CurrentFilepath );
            dst_shape_data.type = eShapeType::SPHERE;
            dst_shape_data.size = { 0.1f, 0.0f, 0.0f };
        }
    }

}}

#include <kinematic_trees/parsers/loco_kinematic_tree_mjcf_parser.h>

namespace loco {
namespace kintree {

    ssize_t TKinematicTreeMjcfParser::NUM_UNNAMED_DRAWABLES = 0;
    ssize_t TKinematicTreeMjcfParser::NUM_UNNAMED_COLLIDERS = 0;
    ssize_t TKinematicTreeMjcfParser::NUM_UNNAMED_JOINTS = 0;
    ssize_t TKinematicTreeMjcfParser::NUM_UNNAMED_BODIES = 0;

    void TKinematicTreeMjcfParser::parse_kintree( const std::string& filepath_mjcf )
    {
        m_MjcfModel = parsing::TElement::CreateFromXmlFile( parsing::eSchemaType::MJCF, filepath_mjcf );
        m_CurrentFilepath = filepath_mjcf;
        m_CurrentFolderpath = tinyutils::GetFolderpath( filepath_mjcf );
        parse_kintree();
    }

    void TKinematicTreeMjcfParser::parse_kintree( const parsing::TElement* model_mjcf )
    {
        m_MjcfModel = parsing::TElement::CloneElement( model_mjcf );
        m_CurrentFilepath = m_MjcfModel->GetString( "filepath", "" );
        m_CurrentFolderpath = m_MjcfModel->GetString( "folderpath", "" );
        parse_kintree();
    }

    void TKinematicTreeMjcfParser::parse_kintree()
    {
        if ( !m_MjcfModel )
        {
            LOCO_CORE_ERROR( "TKinematicTreeMjcfParser::parse_kintree >>> tried parsing using null mjcf-model. \
                              Use the other parse_kintree functions instead" );
            return;
        }

        collect_settings( m_MjcfModel.get() );
        collect_assets( m_MjcfModel->GetFirstChildOfType( "asset" ) );
        collect_defaults_with_class( m_MjcfModel->GetFirstChildOfType( "default" ) );
        collect_defaults_with_no_class( m_MjcfModel->GetFirstChildOfType( "default" ) );

        auto worldbody_elm = m_MjcfModel->GetFirstChildOfType( MJCF_WORLDBODY_TAG );
        if ( !worldbody_elm )
        {
            LOCO_CORE_ERROR( "TKinematicTreeMjcfParser::parse_kintree >>> tried parsing mjcf-model {0} which \
                              didn't contain a worldbody element", m_CurrentFilepath );
            return;
        }

        auto rootbody_elm = worldbody_elm->GetFirstChildOfType( MJCF_BODY_TAG );
        if ( !rootbody_elm )
        {
            LOCO_CORE_ERROR( "TKinematicTreeMjcfParser::parse_kintree >>> tried parsing mjcf-model {0} which \
                              didn't contain any body element", m_CurrentFilepath );
            return;
        }

        std::stack<std::pair<const parsing::TElement*, TKinematicTreeBody*>> dfs_bodies;
        dfs_bodies.push( { rootbody_elm, nullptr } );

        while ( !dfs_bodies.empty() )
        {
            auto elm_bodyparent_pair = dfs_bodies.top();
            auto body_elm = elm_bodyparent_pair.first;
            auto body_parent = elm_bodyparent_pair.second;
            dfs_bodies.pop();
            if ( !body_elm )
                continue;

            auto kintree_body = parse_body( body_elm );
            kintree_body->SetKintree( m_KintreeRef );
            for ( ssize_t i = 0; i < body_elm->num_children(); i++ )
            {
                const auto child_elm = body_elm->get_child( i );
                const auto elm_type = child_elm->elementType();
                /**/ if ( elm_type == MJCF_GEOM_TAG )
                {
                    auto kintree_collider = parse_collider( child_elm );
                    auto kintree_collider_local_tf = kintree_collider->data().localTransform;
                    kintree_body->AddCollider( std::move( kintree_collider ), kintree_collider_local_tf );

                    auto kintree_drawable = parse_drawable( child_elm );
                    auto kintree_drawable_local_tf = kintree_drawable->data().localTransform;
                    kintree_body->AddDrawable( std::move( kintree_drawable ), kintree_drawable_local_tf );
                }
                else if ( elm_type == MJCF_JOINT_TAG )
                {
                    auto kintree_joint = parse_joint( child_elm );
                    auto kintree_joint_local_tf = kintree_joint->data().local_tf;
                    kintree_body->AddJoint( std::move( kintree_joint ), kintree_joint_local_tf );
                }
                else if ( elm_type == MJCF_BODY_TAG )
                {
                    dfs_bodies.push( { child_elm, kintree_body.get() } );
                }
            }

            if ( !body_parent )
            {
                m_KintreeRef->SetRoot( std::move( kintree_body ) );
            }
            else
            {
                auto kintree_body_local_tf = kintree_body->data().local_tf;
                body_parent->AddChild( std::move( kintree_body ), kintree_body_local_tf );
            }
        }
    }

    std::unique_ptr<TKinematicTreeBody> TKinematicTreeMjcfParser::parse_body( const parsing::TElement* body_elm )
    {
        return nullptr;
    }

    std::unique_ptr<TKinematicTreeCollider> TKinematicTreeMjcfParser::parse_collider( const parsing::TElement* collider_elm )
    {
        TCollisionData kintree_collider_data;
        std::string kintree_collider_name;
        if ( collider_elm->HasAttributeString( "name" ) )
            kintree_collider_name = m_KintreeRef->name() + "_" + collider_elm->GetString( "name" ) + loco::SUFFIX_COLLIDER;
        else
            kintree_collider_name = m_KintreeRef->name() + "_collider:" + std::to_string( NUM_UNNAMED_COLLIDERS++ ) + loco::SUFFIX_COLLIDER;
        kintree_collider_data.localTransform = get_transform( collider_elm );
        kintree_collider_data.type = loco::ToEnumShape( get_string( collider_elm, "type", "sphere" ) );
        kintree_collider_data.size = get_standard_size( collider_elm );
        if ( kintree_collider_data.type == eShapeType::MESH ) // @todo: check how to extract the mesh-id correctly
            kintree_collider_data.mesh_data.filename = get_string( collider_elm, "mesh", "" );

        kintree_collider_data.collisionGroup = get_int( collider_elm, "contype", 1 );
        kintree_collider_data.collisionMask = get_int( collider_elm, "conaffinity", 1 );
        kintree_collider_data.friction = get_vec3( collider_elm, "friction", { 1.0f, 0.005f, 0.0001f } );
        kintree_collider_data.density = get_float( collider_elm, "density", loco::DEFAULT_DENSITY );

        return std::make_unique<TKinematicTreeCollider>( kintree_collider_name, kintree_collider_data );
    }

    std::unique_ptr<visualizer::TDrawable> TKinematicTreeMjcfParser::parse_drawable( const parsing::TElement* drawable_elm )
    {
        TVisualData kintree_drawable_data;
        std::string kintree_drawable_name;
        if ( drawable_elm->HasAttributeString( "name" ) )
            kintree_drawable_name = m_KintreeRef->name() + "_" + drawable_elm->GetString( "name" ) + loco::SUFFIX_COLLIDER;
        else
            kintree_drawable_name = m_KintreeRef->name() + "_visual:" + std::to_string( NUM_UNNAMED_DRAWABLES++ ) + loco::SUFFIX_COLLIDER;
        kintree_drawable_data.localTransform = get_transform( drawable_elm );
        kintree_drawable_data.type = loco::ToEnumShape( get_string( drawable_elm, "type", "sphere" ) );
        kintree_drawable_data.size = get_standard_size( drawable_elm );
        if ( kintree_drawable_data.type == eShapeType::MESH ) // @todo: check how to extract the mesh-id correctly
            kintree_drawable_data.mesh_data.filename = get_string( drawable_elm, "mesh", "" );

        auto rgba_color = loco::DEFAULT_RGBA_COLOR; // ambient + diffuse components of the material
        auto specular_color = loco::DEFAULT_SPECULAR_COLOR; // specular component of the material
        auto shininess = loco::DEFAULT_SHININESS; // shininess of the material
        const std::string material_id = get_string( drawable_elm, "material", "" );

        if ( drawable_elm->HasAttributeVec4( "rgba" ) )
        {
            rgba_color = drawable_elm->GetVec4( "rgba", loco::DEFAULT_RGBA_COLOR );
            specular_color = { rgba_color.x(), rgba_color.y(), rgba_color.z() };
            shininess = loco::DEFAULT_SHININESS;
        }
        else if ( m_AssetsMaterials.find( material_id ) != m_AssetsMaterials.end() )
        {
            rgba_color = m_AssetsMaterials[material_id].GetVec4( "rgba", loco::DEFAULT_RGBA_COLOR );
            specular_color = { m_AssetsMaterials[material_id].GetFloat( "specular", loco::DEFAULT_SPECULAR_COLOR.x() ),
                               m_AssetsMaterials[material_id].GetFloat( "specular", loco::DEFAULT_SPECULAR_COLOR.y() ),
                               m_AssetsMaterials[material_id].GetFloat( "specular", loco::DEFAULT_SPECULAR_COLOR.z() ) };
            shininess = m_AssetsMaterials[material_id].GetFloat( "shininess", loco::DEFAULT_SHININESS );
        }
        kintree_drawable_data.ambient = { rgba_color.x(), rgba_color.y(), rgba_color.z() };
        kintree_drawable_data.diffuse = { rgba_color.x(), rgba_color.y(), rgba_color.z() };
        kintree_drawable_data.specular = specular_color;
        kintree_drawable_data.shininess = shininess;

        return std::make_unique<visualizer::TDrawable>( kintree_drawable_name, kintree_drawable_data );
    }

    std::unique_ptr<TKinematicTreeJoint> TKinematicTreeMjcfParser::parse_joint( const parsing::TElement* joint_elm )
    {
        TKinematicTreeJointData kintree_joint_data;
        std::string kintree_joint_name;
        if ( joint_elm->HasAttributeString( "name" ) )
            kintree_joint_name = m_KintreeRef->name() + "_" + joint_elm->GetString( "name" ) + loco::SUFFIX_JOINT;
        else
            kintree_joint_name = m_KintreeRef->name() + "_joint:" + std::to_string( NUM_UNNAMED_JOINTS ) + loco::SUFFIX_JOINT;
        kintree_joint_data.type = loco::ToEnumJoint( get_string( joint_elm, "type", "hinge" ) );
        kintree_joint_data.local_tf = get_transform( joint_elm );
        kintree_joint_data.local_axis = get_vec3( joint_elm, "axis", { 1.0f, 0.0f, 0.0f } );
        const bool is_limited = ( get_string( joint_elm, "limited", "false" ) == "true" );
        if ( is_limited )
        {
            if ( m_OptUseDegrees )
            {
                const auto limits = get_vec2( joint_elm, "range", { -180.0f, 180.0f } );
                kintree_joint_data.limits = { loco::Degrees2rad( limits.x() ), loco::Degrees2rad( limits.y() ) };
            }
            else
            {
                const auto limits = get_vec2( joint_elm, "range", { -loco::PI, loco::PI } );
                kintree_joint_data.limits = { limits.x(), limits.y() };
            }
        }
        else
        {
            kintree_joint_data.limits = { 1.0f, -1.0f }; // min. > max. <=> free-range
        }

        kintree_joint_data.stiffness = get_float( joint_elm, "stiffness", 0.0f );
        kintree_joint_data.armature = get_float( joint_elm, "armature", 0.0f );
        kintree_joint_data.damping = get_float( joint_elm, "damping", 0.0f );

        return std::make_unique<TKinematicTreeJoint>( kintree_joint_name, kintree_joint_data );
    }

    void TKinematicTreeMjcfParser::collect_settings( const parsing::TElement* model_elm )
    {
        if ( auto compiler_elm = model_elm->GetFirstChildOfType( "compiler" ) )
        {
            m_OptUseLocalCoordinates = ( compiler_elm->GetString( "coordinate", "local" ) == "local" );
            m_OptUseDegrees = ( compiler_elm->GetString( "angle", "degree" ) == "degree" );
        }
    }

    void TKinematicTreeMjcfParser::collect_assets( const parsing::TElement* assets_elm )
    {
        if ( !assets_elm )
            return; // No assets in this model, so just skip processing

        for ( ssize_t i = 0; i < assets_elm->num_children(); i++ )
        {
            const auto child_asset_elm = assets_elm->get_child( i );
            const auto child_asset_type = child_asset_elm->elementType();
            if ( child_asset_type == "mesh" )
            {
                TGenericParams mesh_asset;
                if ( child_asset_elm->HasAttributeString( "file" ) )
                    mesh_asset.Set( "file", child_asset_elm->GetString( "file" ) );
                else
                    LOCO_CORE_WARN( "TKinematicTreeMjcfParser::collect_assets >>> mesh-asset for model {0} doesn't have a valid linked file", m_CurrentFilepath );

                if ( child_asset_elm->HasAttributeString( "name" ) )
                    mesh_asset.Set( "name", child_asset_elm->GetString( "name" ) );
                else
                    mesh_asset.Set( "name", tinyutils::GetFilenameNoExtension( mesh_asset.GetString( "file" ) ) );

                mesh_asset.Set( "scale", child_asset_elm->GetVec3( "scale", { 1.0f, 1.0f, 1.0f } ) );
                m_AssetsMeshes[mesh_asset.GetString( "name" )] = mesh_asset;
            }
            else if ( child_asset_type == "material" )
            {
                TGenericParams material_asset;
                if ( child_asset_elm->HasAttributeString( "name" ) )
                    material_asset.Set( "name", child_asset_elm->GetString( "name" ) );
                else
                    LOCO_CORE_WARN( "TKinematicTreeMjcfParser::collect_assets >>> material-asset for model {0} doesn't have a valid name", m_CurrentFilepath );

                if ( child_asset_elm->HasAttributeVec4( "rgba" ) )
                    material_asset.Set( "rgba", child_asset_elm->GetVec4( "rgba", loco::DEFAULT_RGBA_COLOR ) );
                if ( child_asset_elm->HasAttributeFloat( "shininess" ) )
                    material_asset.Set( "shininess", child_asset_elm->GetFloat( "shininess", loco::DEFAULT_SHININESS / 128.0f ) * 128.0f );
                if ( child_asset_elm->HasAttributeFloat( "specular" ) )
                    material_asset.Set( "specular", child_asset_elm->GetFloat( "specular", 1.0f ) );
                m_AssetsMaterials[material_asset.GetString( "name" )] = material_asset;
            }
            else
            {
                LOCO_CORE_WARN( "TKinematicTreeMjcfParser::collect_assets >>> asset of type \"{0}\" isn't supported yet (while parsing model {1})", m_CurrentFilepath, child_asset_type );
            }
        }
    }

    void TKinematicTreeMjcfParser::collect_defaults_with_class( const parsing::TElement* defaults_elm )
    {
        std::stack<std::pair<const parsing::TElement*, std::string>> dfs_elements;
        auto children_defaults = defaults_elm->GetChildrenOfType( "default" );
        auto class_defaults = defaults_elm->GetString( "class", "" );
        for ( ssize_t i = 0; i < children_defaults.size(); i++ )
            dfs_elements.push( { children_defaults[i], class_defaults } );

        while ( dfs_elements.size() > 0 )
        {
            auto elm_class_pair = dfs_elements.top();
            const auto curr_element = elm_class_pair.first;
            const auto curr_class = elm_class_pair.second;
            dfs_elements.pop();
            if ( !curr_element )
                continue;

            if ( curr_element->elementType() == "default" )
            {
                if ( curr_element->HasAttributeString( "class" ) )
                {
                    const std::string child_class = curr_element->GetString( "class" );
                    if ( m_DefaultsPerClass.find( child_class ) == m_DefaultsPerClass.end() )
                        m_DefaultsPerClass[child_class] = std::unordered_map<std::string, TGenericParams>();

                    for ( ssize_t i = 0; i < curr_element->num_children(); i++ )
                        dfs_elements.push( { curr_element->get_child( i ), child_class } );
                }
                else
                {
                    LOCO_CORE_WARN( "TKinematicTreeMjcfParser::collect_defaults_with_class >>> nested default element \
                                     has no class attribute (while processing model {0})", m_CurrentFilepath );
                }
            }
            else if ( curr_class != "" )
            {
                const auto elm_type = curr_element->elementType();
                auto possible_attribs = curr_element->schema()->GetPossibleAttributes( elm_type );
                for ( auto key_val : possible_attribs )
                {
                    const auto attrib_id = key_val.first;
                    const auto attrib_type = key_val.second;
                    /**/ if ( attrib_type == parsing::eAttributeType::TYPE_ARRAY_INT &&
                              curr_element->HasAttributeArrayInt( attrib_id ) )
                        m_DefaultsPerClass[curr_class][elm_type].Set( attrib_id, curr_element->GetArrayInt( attrib_id ) );
                    else if ( attrib_type == parsing::eAttributeType::TYPE_ARRAY_FLOAT &&
                              curr_element->HasAttributeArrayFloat( attrib_id ) )
                        m_DefaultsPerClass[curr_class][elm_type].Set( attrib_id, curr_element->GetArrayFloat( attrib_id ) );
                    else if ( attrib_type == parsing::eAttributeType::TYPE_INT &&
                              curr_element->HasAttributeInt( attrib_id ) )
                        m_DefaultsPerClass[curr_class][elm_type].Set( attrib_id, curr_element->GetInt( attrib_id ) );
                    else if ( attrib_type == parsing::eAttributeType::TYPE_FLOAT &&
                              curr_element->HasAttributeFloat( attrib_id ) )
                        m_DefaultsPerClass[curr_class][elm_type].Set( attrib_id, curr_element->GetFloat( attrib_id ) );
                    else if ( attrib_type == parsing::eAttributeType::TYPE_STRING &&
                              curr_element->HasAttributeString( attrib_id ) )
                        m_DefaultsPerClass[curr_class][elm_type].Set( attrib_id, curr_element->GetString( attrib_id ) );
                }
            }
        }
    }

    void TKinematicTreeMjcfParser::collect_defaults_with_no_class( const parsing::TElement* defaults_elm )
    {
        for ( ssize_t i = 0; i < defaults_elm->num_children(); i++ )
        {
            const auto child_elm = defaults_elm->get_child( i );
            const std::string elm_type = child_elm->elementType();
            if ( elm_type == "default" )
                continue; // nested defaults are handled using classes

            auto possible_attribs = child_elm->schema()->GetPossibleAttributes( elm_type );
            for ( auto key_val : possible_attribs )
            {
                const auto attrib_id = key_val.first;
                const auto attrib_type = key_val.second;
                /**/ if ( attrib_type == parsing::eAttributeType::TYPE_ARRAY_INT &&
                          child_elm->HasAttributeArrayInt( attrib_id ) )
                    m_DefaultsNoClass[elm_type].Set( attrib_id, child_elm->GetArrayInt( attrib_id ) );
                else if ( attrib_type == parsing::eAttributeType::TYPE_ARRAY_FLOAT &&
                          child_elm->HasAttributeArrayFloat( attrib_id ) )
                    m_DefaultsNoClass[elm_type].Set( attrib_id, child_elm->GetArrayFloat( attrib_id ) );
                else if ( attrib_type == parsing::eAttributeType::TYPE_INT &&
                          child_elm->HasAttributeInt( attrib_id ) )
                    m_DefaultsNoClass[elm_type].Set( attrib_id, child_elm->GetInt( attrib_id ) );
                else if ( attrib_type == parsing::eAttributeType::TYPE_FLOAT &&
                          child_elm->HasAttributeFloat( attrib_id ) )
                    m_DefaultsNoClass[elm_type].Set( attrib_id, child_elm->GetFloat( attrib_id ) );
                else if ( attrib_type == parsing::eAttributeType::TYPE_STRING &&
                          child_elm->HasAttributeString( attrib_id ) )
                    m_DefaultsNoClass[elm_type].Set( attrib_id, child_elm->GetString( attrib_id ) );
            }
        }
    }

    std::string TKinematicTreeMjcfParser::get_string( const parsing::TElement* elm,
                                                      const std::string& attrib_id,
                                                      const std::string& default_value ) const
    {
        if ( elm->HasAttributeString( attrib_id ) )
            return elm->GetString( attrib_id );

        const std::string elm_type = elm->elementType();
        const std::string elm_class = get_class( elm, attrib_id );
        if ( elm_class != "" )
            return m_DefaultsPerClass.at( elm_class ).at( elm_type ).GetString( attrib_id, default_value );

        if ( has_default_no_class( elm_type, attrib_id ) )
            return m_DefaultsNoClass.at( elm_type ).GetString( attrib_id, default_value );
        return default_value;
    }

    ssize_t TKinematicTreeMjcfParser::get_int( const parsing::TElement* elm,
                                               const std::string& attrib_id,
                                               const ssize_t& default_value ) const
    {
        if ( elm->HasAttributeInt( attrib_id ) )
            return elm->GetInt( attrib_id );

        const std::string elm_type = elm->elementType();
        const std::string elm_class = get_class( elm, attrib_id );
        if ( elm_class != "" )
            return m_DefaultsPerClass.at( elm_class ).at( elm_type ).GetInt( attrib_id, default_value );

        if ( has_default_no_class( elm_type, attrib_id ) )
            return m_DefaultsNoClass.at( elm_type ).GetInt( attrib_id, default_value );
        return default_value;
    }

    float TKinematicTreeMjcfParser::get_float( const parsing::TElement* elm,
                                               const std::string& attrib_id,
                                               const float& default_value ) const
    {
        if ( elm->HasAttributeFloat( attrib_id ) )
            return elm->GetFloat( attrib_id );

        const std::string elm_type = elm->elementType();
        const std::string elm_class = get_class( elm, attrib_id );
        if ( elm_class != "" )
            return m_DefaultsPerClass.at( elm_class ).at( elm_type ).GetFloat( attrib_id, default_value );

        if ( has_default_no_class( elm_type, attrib_id ) )
            return m_DefaultsNoClass.at( elm_type ).GetFloat( attrib_id, default_value );
        return default_value;
    }

    TVec2 TKinematicTreeMjcfParser::get_vec2( const parsing::TElement* elm,
                                              const std::string& attrib_id,
                                              const TVec2& default_value ) const
    {
        if ( elm->HasAttributeVec2( attrib_id ) )
            return elm->GetVec2( attrib_id );

        const std::string elm_type = elm->elementType();
        const std::string elm_class = get_class( elm, attrib_id );
        if ( elm_class != "" )
            return m_DefaultsPerClass.at( elm_class ).at( elm_type ).GetVec2( attrib_id, default_value );

        if ( has_default_no_class( elm_type, attrib_id ) )
            return m_DefaultsNoClass.at( elm_type ).GetVec2( attrib_id, default_value );
        return default_value;
    }

    TVec3 TKinematicTreeMjcfParser::get_vec3( const parsing::TElement* elm,
                                              const std::string& attrib_id,
                                              const TVec3& default_value ) const
    {
        if ( elm->HasAttributeVec3( attrib_id ) )
            return elm->GetVec3( attrib_id );

        const std::string elm_type = elm->elementType();
        const std::string elm_class = get_class( elm, attrib_id );
        if ( elm_class != "" )
            return m_DefaultsPerClass.at( elm_class ).at( elm_type ).GetVec3( attrib_id, default_value );

        if ( has_default_no_class( elm_type, attrib_id ) )
            return m_DefaultsNoClass.at( elm_type ).GetVec3( attrib_id, default_value );
        return default_value;
    }

    TVec4 TKinematicTreeMjcfParser::get_vec4( const parsing::TElement* elm,
                                              const std::string& attrib_id,
                                              const TVec4& default_value ) const
    {
        if ( elm->HasAttributeVec4( attrib_id ) )
            return elm->GetVec4( attrib_id );

        const std::string elm_type = elm->elementType();
        const std::string elm_class = get_class( elm, attrib_id );
        if ( elm_class != "" )
            return m_DefaultsPerClass.at( elm_class ).at( elm_type ).GetVec4( attrib_id, default_value );

        if ( has_default_no_class( elm_type, attrib_id ) )
            return m_DefaultsNoClass.at( elm_type ).GetVec4( attrib_id, default_value );
        return default_value;
    }

    TSizei TKinematicTreeMjcfParser::get_array_int( const parsing::TElement* elm,
                                                    const std::string& attrib_id,
                                                    const TSizei& default_value ) const
    {
        if ( elm->HasAttributeArrayInt( attrib_id ) )
            return elm->GetArrayInt( attrib_id );

        const std::string elm_type = elm->elementType();
        const std::string elm_class = get_class( elm, attrib_id );
        if ( elm_class != "" )
            return m_DefaultsPerClass.at( elm_class ).at( elm_type ).GetSizei( attrib_id, default_value );

        if ( has_default_no_class( elm_type, attrib_id ) )
            return m_DefaultsNoClass.at( elm_type ).GetSizei( attrib_id, default_value );
        return default_value;
    }

    TSizef TKinematicTreeMjcfParser::get_array_float( const parsing::TElement* elm,
                                                      const std::string& attrib_id,
                                                      const TSizef& default_value ) const
    {
        if ( elm->HasAttributeArrayFloat( attrib_id ) )
            return elm->GetArrayFloat( attrib_id );

        const std::string elm_type = elm->elementType();
        const std::string elm_class = get_class( elm, attrib_id );
        if ( elm_class != "" )
            return m_DefaultsPerClass.at( elm_class ).at( elm_type ).GetSizef( attrib_id, default_value );

        if ( has_default_no_class( elm_type, attrib_id ) )
            return m_DefaultsNoClass.at( elm_type ).GetSizef( attrib_id, default_value );
        return default_value;
    }

    TMat4 TKinematicTreeMjcfParser::get_transform( const parsing::TElement* elm ) const
    {
        return TMat4();
    }

    TVec3 TKinematicTreeMjcfParser::get_standard_size( const parsing::TElement* elm ) const
    {
        return TVec3();
    }

    std::string TKinematicTreeMjcfParser::get_class( const parsing::TElement* elm, const std::string& attrib_id ) const
    {
        const parsing::TElement* current_elm = elm;
        while ( current_elm )
        {
            if ( elm->HasAttributeString( "class" ) )
            {
                const std::string curr_type = elm->elementType();
                const std::string curr_class = elm->GetString( "class ");
                if ( has_default_from_class( curr_type, attrib_id, curr_class ) )
                    return curr_class;
            }
            if ( elm->parent() && elm->parent()->HasAttributeString( "childclass" ) )
            {
                const std::string curr_type = elm->elementType();
                const std::string curr_class = elm->parent()->GetString( "childclass" );
                if ( has_default_from_class( curr_type, attrib_id, curr_class ) )
                    return curr_class;
            }
            current_elm = elm->parent();
        }
        return "";
    }

    bool TKinematicTreeMjcfParser::has_default_no_class( const std::string& elm_type,
                                                         const std::string& attrib_id ) const
    {
        return ( m_DefaultsNoClass.find( elm_type ) != m_DefaultsNoClass.end() ) &&
               ( m_DefaultsNoClass.at( elm_type ).HasParam( attrib_id ) );
    }

    bool TKinematicTreeMjcfParser::has_default_from_class( const std::string& elm_type,
                                                           const std::string& attrib_id,
                                                           const std::string& elm_class ) const
    {
        if ( m_DefaultsPerClass.find( elm_class ) == m_DefaultsPerClass.end() )
            return false;
        if ( m_DefaultsPerClass.at( elm_class ).find( elm_type ) == m_DefaultsPerClass.at( elm_class ).end() )
            return false;
        return m_DefaultsPerClass.at( elm_class ).at( elm_type ).HasParam( attrib_id );
    }
}}
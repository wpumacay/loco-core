
#include <utils/loco_parsing_element.h>

namespace loco {
namespace parsing {

    TElement::TElement( const std::string& elementType,
                        const eSchemaType& schemaType )
        : m_elementType( elementType ), m_schemaType( schemaType )
    {
        m_schemaRef = nullptr;
        m_parentRef = nullptr;

        if ( m_schemaType == eSchemaType::MJCF )
            m_schemaRef = TMjcfSchema::GetInstance();
        else if ( m_schemaType == eSchemaType::URDF )
            m_schemaRef = TUrdfSchema::GetInstance();
    }

    TElement::~TElement()
    {
        m_parentRef = nullptr;
        m_schemaRef = nullptr;
        m_children.clear();

        m_attribsInts.clear();
        m_attribsFloats.clear();
        m_attribsArrayInts.clear();
        m_attribsArrayFloats.clear();
        m_attribsStrings.clear();
    }

    std::unique_ptr<TElement> TElement::CreateFromXmlString( const eSchemaType& schemaType,
                                                             const std::string& xml_string )
    {
        auto element = std::make_unique<TElement>( "", schemaType );
        element->LoadFromXmlString( xml_string );
        return std::move( element );
    }

    std::unique_ptr<TElement> TElement::CreateFromXmlFile( const eSchemaType& schemaType,
                                                           const std::string& xml_filepath )
    {
        auto element = std::make_unique<TElement>( "", schemaType );
        element->LoadFromXmlFile( xml_filepath );
        return std::move( element );
    }

    std::unique_ptr<TElement> TElement::CloneElement( const TElement* other )
    {
        auto element_cp = std::make_unique<TElement>( other->elementType(), other->schemaType() );
        // Stack for DFS: pair <=> (dst-element, src-element)
        std::stack< std::pair<TElement*, const TElement*> > dfs_copy_pairs;
        dfs_copy_pairs.push( { element_cp.get(), other } );
        while ( dfs_copy_pairs.size() > 0 )
        {
            auto dst_src_pair = dfs_copy_pairs.top();
            auto dst_element = dst_src_pair.first;
            auto src_element = dst_src_pair.second;

            dst_element->m_attribsInts = src_element->m_attribsInts;
            dst_element->m_attribsFloats = src_element->m_attribsFloats;
            dst_element->m_attribsArrayInts = src_element->m_attribsArrayInts;
            dst_element->m_attribsArrayFloats = src_element->m_attribsArrayFloats;
            dst_element->m_attribsStrings = src_element->m_attribsStrings;

            dfs_copy_pairs.pop();

            if ( !dst_element || !src_element )
                continue;

            for ( size_t i = 0; i < src_element->num_children(); i++ )
            {
                auto src_child_element = src_element->get_child( i );
                auto dst_child_element = dst_element->Add( src_child_element->elementType() );
                dfs_copy_pairs.push( { dst_child_element, src_child_element } );
            }
        }

        return std::move( element_cp );
    }

    TElement* TElement::Add( const std::string& childElementType )
    {
        if ( !m_schemaRef->HasChild( m_elementType, childElementType ) )
            throw std::runtime_error( "TElement::Add >>> element (" + m_elementType + ") doesn't accept \
                                       children of type (" + childElementType + ") for schema " + loco::parsing::ToString( m_schemaType ) );

        auto child_element = std::make_unique<TElement>( childElementType, m_schemaType );
        child_element->m_parentRef = this;
        m_children.push_back( std::move( child_element ) );
        return m_children.back().get();
    }

    TElement* TElement::Add( std::unique_ptr<TElement> childElement )
    {
        if ( !m_schemaRef->HasChild( m_elementType, childElement->elementType() ) )
            throw std::runtime_error( "TElement::Add >>> element (" + m_elementType + ") doesn't accept \
                                       children of type (" + childElement->elementType() + ") for schema " + loco::parsing::ToString( m_schemaType ) );

        childElement->m_parentRef = this;
        m_children.push_back( std::move( childElement ) );
        return m_children.back().get();
    }

    void TElement::LoadFromXmlString( const std::string& xml_string )
    {
        tinyxml2::XMLDocument xml_doc;
        if ( xml_doc.Parse( xml_string.c_str() ) != tinyxml2::XML_SUCCESS )
        {
            LOCO_CORE_ERROR( "TElement::LoadFromXmlString >>> couldn't parse given xml-string: \n{0}", xml_string );
            return;
        }
        _LoadFromXmlElement( xml_doc.FirstChildElement() );
    }

    void TElement::LoadFromXmlFile( const std::string& xml_filepath )
    {
        tinyxml2::XMLDocument xml_doc;
        if ( xml_doc.LoadFile( xml_filepath.c_str() ) != tinyxml2::XML_SUCCESS )
        {
            LOCO_CORE_ERROR( "TElement::LoadFromXml >>> couldn't load xml-file @ {0}", xml_filepath );
            return;
        }
        _LoadFromXmlElement( xml_doc.FirstChildElement() );
    }

    void TElement::_LoadFromXmlElement( const tinyxml2::XMLElement* xml_root_element )
    {
        // "This" element is the "root" element, so collect its attributes before traversing
        m_elementType = std::string( xml_root_element->Value() );
        CollectAttributes( xml_root_element );
        // Add all children of the root to start the dfs-traversal (pair <=> (xml_element, element_parent))
        std::stack< std::pair<const tinyxml2::XMLElement*, TElement*> > dfs_xml_elements;
        auto root_child = xml_root_element->FirstChildElement();
        while ( root_child )
        {
            dfs_xml_elements.push( { root_child, this } );
            root_child = root_child->NextSiblingElement();
        }
        // Traverse over all elements in the xml-tree in a dfs-fashion
        while ( dfs_xml_elements.size() > 0 )
        {
            auto xmlelement_parent_pair = dfs_xml_elements.top();
            auto xml_element = xmlelement_parent_pair.first;
            auto element_parent = xmlelement_parent_pair.second;
            dfs_xml_elements.pop();
            if ( !xml_element || !element_parent )
                continue;

            auto element_type = std::string( xml_element->Value() );
            auto element = std::make_unique<TElement>( element_type, m_schemaType );
            element->CollectAttributes( xml_element );
            // Connect parent to child element
            element->m_parentRef = element_parent;
            element_parent->m_children.push_back( std::move( element ) );
            // Add children for next traversal
            auto xml_child_element = xml_element->FirstChildElement();
            while ( xml_child_element )
            {
                dfs_xml_elements.push( { xml_child_element, element_parent->m_children.back().get() } );
                xml_child_element = xml_child_element->NextSiblingElement();
            }
        }
    }

    void TElement::SaveToXml( const std::string& filepath )
    {
        tinyxml2::XMLDocument xml_doc;
        // Create the root xml-element associated with "this" element (root starts with "this" element)
        auto xml_root_element = xml_doc.NewElement( m_elementType.c_str() );
        InsertAttributes( xml_root_element );
        xml_doc.InsertEndChild( xml_root_element );
        // Add all children of the root(this) to start the traversal (pair <=> (parent_xml, child_element))
        std::stack< std::pair<tinyxml2::XMLElement*, TElement*> > dfs_traversal;
        for ( ssize_t i = ( m_children.size() - 1 ); i >= 0; i-- )
            dfs_traversal.push( { xml_root_element, m_children[i].get() } );
        // Traverse over all children in a dfs-like fashion
        while ( dfs_traversal.size() > 0 )
        {
            auto xmlparent_childelement_pair = dfs_traversal.top();
            auto xml_parent = xmlparent_childelement_pair.first;
            auto child_element = xmlparent_childelement_pair.second;
            dfs_traversal.pop();
            if ( !xml_parent || !child_element )
                continue;

            auto xml_child_element = xml_doc.NewElement( child_element->elementType().c_str() );
            child_element->InsertAttributes( xml_child_element );
            xml_parent->InsertEndChild( xml_child_element );

            for ( ssize_t i = ( child_element->m_children.size() - 1 ); i >= 0; i-- )
                dfs_traversal.push( { xml_child_element, child_element->m_children[i].get() } );
        }
        xml_doc.SaveFile( filepath.c_str() );
    }

    void TElement::CollectAttributes( const tinyxml2::XMLElement* xml_element )
    {
        auto possible_attributes = m_schemaRef->GetPossibleAttributes( m_elementType );
        for ( const auto& kv_pair : possible_attributes )
        {
            auto attrib_name = kv_pair.first;
            auto attrib_type = kv_pair.second;

            if ( !xml_element->Attribute( attrib_name.c_str() ) )
                continue;

            if ( attrib_type == eAttributeType::TYPE_INT )
                m_attribsInts[attrib_name] = loco::parsing::xml::GetAttributeInt( xml_element, attrib_name );
            else if ( attrib_type == eAttributeType::TYPE_FLOAT )
                m_attribsFloats[attrib_name] = loco::parsing::xml::GetAttributeFloat( xml_element, attrib_name );
            else if ( attrib_type == eAttributeType::TYPE_ARRAY_INT )
                m_attribsArrayInts[attrib_name] = loco::parsing::xml::GetAttributeArrayInt( xml_element, attrib_name );
            else if ( attrib_type == eAttributeType::TYPE_ARRAY_FLOAT )
                m_attribsArrayFloats[attrib_name] = loco::parsing::xml::GetAttributeArrayFloat( xml_element, attrib_name );
            else if ( attrib_type == eAttributeType::TYPE_STRING )
                m_attribsStrings[attrib_name] = loco::parsing::xml::GetAttributeString( xml_element, attrib_name );
        }
    }

    void TElement::InsertAttributes( tinyxml2::XMLElement* xml_element )
    {
        for ( const auto& kv_pair : m_attribsInts )
            xml_element->SetAttribute( kv_pair.first.c_str(), std::to_string( kv_pair.second ).c_str() );
        for ( const auto& kv_pair : m_attribsFloats )
            xml_element->SetAttribute( kv_pair.first.c_str(), std::to_string( kv_pair.second ).c_str() );
        for ( const auto& kv_pair : m_attribsArrayInts )
            xml_element->SetAttribute( kv_pair.first.c_str(), loco::parsing::xml::SerializeToString( kv_pair.second ).c_str() );
        for ( const auto& kv_pair : m_attribsArrayFloats )
            xml_element->SetAttribute( kv_pair.first.c_str(), loco::parsing::xml::SerializeToString( kv_pair.second ).c_str() );
        for ( const auto& kv_pair : m_attribsStrings )
            xml_element->SetAttribute( kv_pair.first.c_str(), kv_pair.second.c_str() );
    }

    void TElement::SetInt( const std::string& attribId, int32_t value )
    {
        if ( !m_schemaRef->HasAttribute( m_elementType, attribId ) )
            return;

        m_attribsInts[attribId] = value;
    }

    void TElement::SetFloat( const std::string& attribId, TScalar value )
    {
        if ( !m_schemaRef->HasAttribute( m_elementType, attribId ) )
            return;

        m_attribsFloats[attribId] = value;
    }

    void TElement::SetArrayInt( const std::string& attribId, const TSizei& value )
    {
        if ( !m_schemaRef->HasAttribute( m_elementType, attribId ) )
            return;

        m_attribsArrayInts[attribId] = value;
    }

    void TElement::SetArrayFloat( const std::string& attribId, const TSizef& value )
    {
        if ( !m_schemaRef->HasAttribute( m_elementType, attribId ) )
            return;

        m_attribsArrayFloats[attribId] = value;
    }

    void TElement::SetVec2( const std::string& attribId, const TVec2& value )
    {
        if ( !m_schemaRef->HasAttribute( m_elementType, attribId ) )
            return;

        TSizef array_float;
        array_float.ndim = 2;
        array_float[0] = value.x();
        array_float[1] = value.y();

        m_attribsArrayFloats[attribId] = array_float;
    }

    void TElement::SetVec3( const std::string& attribId, const TVec3& value )
    {
        if ( !m_schemaRef->HasAttribute( m_elementType, attribId ) )
            return;

        TSizef array_float;
        array_float.ndim = 3;
        array_float[0] = value.x();
        array_float[1] = value.y();
        array_float[2] = value.z();

        m_attribsArrayFloats[attribId] = array_float;
    }

    void TElement::SetVec4( const std::string& attribId, const TVec4& value )
    {
        if ( !m_schemaRef->HasAttribute( m_elementType, attribId ) )
            return;

        TSizef array_float;
        array_float.ndim = 4;
        array_float[0] = value.x();
        array_float[1] = value.y();
        array_float[2] = value.z();
        array_float[3] = value.w();

        m_attribsArrayFloats[attribId] = array_float;
    }

    void TElement::SetString( const std::string& attribId, const std::string& value )
    {
        if ( !m_schemaRef->HasAttribute( m_elementType, attribId ) )
            return;

        m_attribsStrings[attribId] = value;
    }

    int32_t TElement::GetInt( const std::string& attribId, int32_t def_int ) const
    {
        if ( !HasAttributeInt( attribId ) )
            return def_int;
        return m_attribsInts.at( attribId );
    }

    TScalar TElement::GetFloat( const std::string& attribId, TScalar def_float ) const
    {
        if ( !HasAttributeFloat( attribId ) )
            return def_float;
        return m_attribsFloats.at( attribId );
    }

    TSizei TElement::GetArrayInt( const std::string& attribId, const TSizei& def_arrint ) const
    {
        if ( !HasAttributeArrayInt( attribId ) )
            return def_arrint;
        return m_attribsArrayInts.at( attribId );
    }

    TSizef TElement::GetArrayFloat( const std::string& attribId, const TSizef& def_arrfloat ) const
    {
        if ( !HasAttributeArrayFloat( attribId ) )
            return def_arrfloat;
        return m_attribsArrayFloats.at( attribId );
    }

    TVec2 TElement::GetVec2( const std::string& attribId, const TVec2& def_vec2 ) const
    {
        if ( !HasAttributeVec2( attribId ) )
            return def_vec2;

        auto& array_float = m_attribsArrayFloats.at( attribId );
        return TVec2( array_float[0], array_float[1] );
    }

    TVec3 TElement::GetVec3( const std::string& attribId, const TVec3& def_vec3 ) const
    {
        if ( !HasAttributeVec3( attribId ) )
            return def_vec3;

        auto& array_float = m_attribsArrayFloats.at( attribId );
        return TVec3( array_float[0], array_float[1], array_float[2] );
    }

    TVec4 TElement::GetVec4( const std::string& attribId, const TVec4& def_vec4 ) const
    {
        if ( !HasAttributeVec4( attribId ) )
            return def_vec4;

        auto& array_float = m_attribsArrayFloats.at( attribId );
        return TVec4( array_float[0], array_float[1], array_float[2], array_float[3] );
    }

    std::string TElement::GetString( const std::string& attribId, const std::string& def_string ) const
    {
        if ( !HasAttributeString( attribId ) )
            return def_string;
        return m_attribsStrings.at( attribId );
    }

    bool TElement::HasAttributeInt( const std::string& attribId ) const
    {
        return m_attribsInts.find( attribId ) != m_attribsInts.end();
    }

    bool TElement::HasAttributeFloat( const std::string& attribId ) const
    {
        return m_attribsFloats.find( attribId ) != m_attribsFloats.end();
    }

    bool TElement::HasAttributeArrayInt( const std::string& attribId ) const
    {
        return m_attribsArrayInts.find( attribId ) != m_attribsArrayInts.end();
    }

    bool TElement::HasAttributeArrayFloat( const std::string& attribId ) const
    {
        return m_attribsArrayFloats.find( attribId ) != m_attribsArrayFloats.end();
    }

    bool TElement::HasAttributeVec2( const std::string& attribId ) const
    {
        if ( !HasAttributeArrayFloat( attribId ) )
            return false;
        return m_attribsArrayFloats.at( attribId ).ndim == 2;
    }

    bool TElement::HasAttributeVec3( const std::string& attribId ) const
    {
        if ( !HasAttributeArrayFloat( attribId ) )
            return false;
        return m_attribsArrayFloats.at( attribId ).ndim == 3;
    }

    bool TElement::HasAttributeVec4( const std::string& attribId ) const
    {
        if ( !HasAttributeArrayFloat( attribId ) )
            return false;
        return m_attribsArrayFloats.at( attribId ).ndim == 4;
    }

    bool TElement::HasAttributeString( const std::string& attribId ) const
    {
        return m_attribsStrings.find( attribId ) != m_attribsStrings.end();
    }

    TElement* TElement::GetFirstChildOfType( const std::string& childType )
    {
        if ( m_children.size() < 1 )
        {
            LOCO_CORE_WARN( "TElement::GetFirstChildOfType >>> element has no children" );
            return nullptr;
        }

        for ( size_t i = 0; i < m_children.size(); i++ )
            if ( m_children[i]->elementType() == childType )
                return m_children[i].get();

        LOCO_CORE_WARN( "TElement::GetFirstChildOfType >>> element {0} has no child of type {1}", m_elementType, childType );
        return nullptr;
    }

    const TElement* TElement::GetFirstChildOfType( const std::string& childType ) const
    {
        if ( m_children.size() < 1 )
        {
            LOCO_CORE_WARN( "TElement::GetFirstChildOfType >>> element has no children" );
            return nullptr;
        }

        for ( size_t i = 0; i < m_children.size(); i++ )
            if ( m_children[i]->elementType() == childType )
                return m_children[i].get();

        LOCO_CORE_WARN( "TElement::GetFirstChildOfType >>> element {0} has no child of type {1}", m_elementType, childType );
        return nullptr;
    }

    std::vector<TElement*> TElement::GetChildrenOfType( const std::string& childrenType )
    {
        std::vector<TElement*> vec_children;
        for ( size_t i = 0; i < m_children.size(); i++ )
            if ( m_children[i]->elementType() == childrenType )
                vec_children.push_back( m_children[i].get() );
        return vec_children;
    }

    std::vector<const TElement*> TElement::GetChildrenOfType( const std::string& childrenType ) const
    {
        std::vector<const TElement*> vec_children;
        for ( size_t i = 0; i < m_children.size(); i++ )
            if ( m_children[i]->elementType() == childrenType )
                vec_children.push_back( m_children[i].get() );
        return vec_children;
    }

    std::string TElement::ToString() const
    {
        const bool has_children = ( m_children.size() > 0 );
        std::string _strrep;
        _strrep += "<" + m_elementType + " ";
        for ( const auto& kv_pair : m_attribsInts )
            _strrep += kv_pair.first + "=" + std::to_string( kv_pair.second ) + " ";
        for ( const auto& kv_pair : m_attribsFloats )
            _strrep += kv_pair.first + "=" + std::to_string( kv_pair.second ) + " ";
        for ( const auto& kv_pair : m_attribsArrayInts )
            _strrep += kv_pair.first + "=" + loco::parsing::xml::SerializeToString( kv_pair.second ) + " ";
        for ( const auto& kv_pair : m_attribsArrayFloats )
            _strrep += kv_pair.first + "=" + loco::parsing::xml::SerializeToString( kv_pair.second ) + " ";
        for ( const auto& kv_pair : m_attribsStrings )
            _strrep += kv_pair.first + "=" + kv_pair.second + " ";
        _strrep += ( has_children ? ">" : "/>" );

        if ( !has_children )
            return _strrep;

        struct TraversalNode
        {
            const TElement* element;
            const TElement* element_parent;
            size_t element_depth;
            bool element_closes;
        };

        // Do a traversal over all children (pair <=> (element_ptr, element_depth))
        std::stack< TraversalNode > dfs_elements;
        dfs_elements.push( { this, nullptr, 0, true } );
        for ( size_t i = 0; i < m_children.size(); i++ )
            dfs_elements.push( { m_children[i].get(), this, 1, false } );
        while ( dfs_elements.size() > 0 )
        {
            auto dfs_node = dfs_elements.top();
            auto element = dfs_node.element;
            auto parent = dfs_node.element_parent;
            auto depth = dfs_node.element_depth;
            auto element_closes = dfs_node.element_closes;
            bool elm_has_children = ( element->num_children() > 0 );
            dfs_elements.pop();
            if ( !element )
                continue;

            _strrep += "\n";
            for ( size_t i = 0; i < depth; i++ )
                _strrep += "\t";

            if ( element_closes )
            {
                _strrep += "<" + element->elementType() + "/>";
                continue;
            }

            _strrep += "<" + element->elementType() + " ";
            for ( const auto& kv_pair : element->m_attribsInts )
                _strrep += kv_pair.first + "=" + std::to_string( kv_pair.second ) + " ";
            for ( const auto& kv_pair : element->m_attribsFloats )
                _strrep += kv_pair.first + "=" + std::to_string( kv_pair.second ) + " ";
            for ( const auto& kv_pair : element->m_attribsArrayInts )
                _strrep += kv_pair.first + "=" + loco::parsing::xml::SerializeToString( kv_pair.second ) + " ";
            for ( const auto& kv_pair : element->m_attribsArrayFloats )
                _strrep += kv_pair.first + "=" + loco::parsing::xml::SerializeToString( kv_pair.second ) + " ";
            for ( const auto& kv_pair : element->m_attribsStrings )
                _strrep += kv_pair.first + "=" + kv_pair.second + " ";
            _strrep += ( elm_has_children ? ">" : "/>" );

            if ( elm_has_children )
            {
                dfs_elements.push( { element, nullptr, depth, true } );
                for ( size_t i = 0; i < element->m_children.size(); i++ )
                    dfs_elements.push( { element->m_children[i].get(), element, depth + 1, false } );
            }
        }

        return _strrep;
    }

    TElement* TElement::get_child( size_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_children.size() ), "TElement::get_child >>> index \
                          {0} out of range [0,...{1}]", index, m_children.size() );
        return m_children[index].get();
    }

    const TElement* TElement::get_child( size_t index ) const
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_children.size() ), "TElement::get_child >>> index \
                          {0} out of range [0,...{1}]", index, m_children.size() );
        return m_children[index].get();
    }

}}
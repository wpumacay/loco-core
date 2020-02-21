
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

    TElement& TElement::Add( const std::string& childElementType )
    {
        if ( !m_schemaRef->HasChild( m_elementType, childElementType ) )
            throw std::runtime_error( "TElement::Add >>> element (" + m_elementType + ") doesn't accept \
                                       children of type (" + childElementType + ") for schema " + ToString( m_schemaType ) );

        auto child_element = std::make_unique<TElement>( childElementType, m_schemaType );
        child_element->m_parentRef = this;
        m_children.push_back( std::move( child_element ) );
        return *m_children.back().get();
    }

    void TElement::LoadFromXml( const std::string& filepath )
    {
        tinyxml2::XMLDocument xml_doc;
        if ( xml_doc.LoadFile( filepath.c_str() ) != tinyxml2::XML_SUCCESS )
        {
            LOCO_CORE_ERROR( "TElement::LoadFromXml >>> coulnd't load xml-file @ {0}", filepath );
            return;
        }

        // "This" element is the "root" element, so collect its attributes before traversing
        auto root_element = xml_doc.FirstChildElement();
        CollectAttributes( root_element );
        // Add all children of the root to start the dfs-traversal (pair <=> (xml_element, element_parent))
        std::stack< std::pair<const tinyxml2::XMLElement*, TElement*> > dfs_xml_elements;
        auto root_child = root_element->FirstChildElement();
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
            dfs_xml_elements.push( { xml_element->NextSiblingElement(),
                                     element_parent->m_children.back().get() } );
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
        for ( size_t i = 0; i < m_children.size(); i++ )
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

            for ( size_t i = 0; i < child_element->m_children.size(); i++ )
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
        array_float[0] = value.x();
        array_float[1] = value.y();

        m_attribsArrayFloats[attribId] = array_float;
    }

    void TElement::SetVec3( const std::string& attribId, const TVec3& value )
    {
        if ( !m_schemaRef->HasAttribute( m_elementType, attribId ) )
            return;

        TSizef array_float;
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
        if ( m_attribsInts.find( attribId ) == m_attribsInts.end() )
            return def_int;
        return m_attribsInts.at( attribId );
    }

    TScalar TElement::GetFloat( const std::string& attribId, TScalar def_float ) const
    {
        if ( m_attribsFloats.find( attribId ) == m_attribsFloats.end() )
            return def_float;
        return m_attribsFloats.at( attribId );
    }

    TSizei TElement::GetArrayInt( const std::string& attribId, const TSizei& def_arrint ) const
    {
        if ( m_attribsArrayInts.find( attribId ) == m_attribsArrayInts.end() )
            return def_arrint;
        return m_attribsArrayInts.at( attribId );
    }

    TSizef TElement::GetArrayFloat( const std::string& attribId, const TSizef& def_arrfloat ) const
    {
        if ( m_attribsArrayFloats.find( attribId ) == m_attribsArrayFloats.end() )
            return def_arrfloat;
        return m_attribsArrayFloats.at( attribId );
    }

    TVec2 TElement::GetVec2( const std::string& attribId, const TVec2& def_vec2 ) const
    {
        if ( m_attribsArrayFloats.find( attribId ) == m_attribsArrayFloats.end() )
            return def_vec2;
        if ( m_attribsArrayFloats.at( attribId ).ndim != 2 )
            return def_vec2;

        auto& array_float = m_attribsArrayFloats.at( attribId );
        return TVec2( array_float[0], array_float[1] );
    }

    TVec3 TElement::GetVec3( const std::string& attribId, const TVec3& def_vec3 ) const
    {
        if ( m_attribsArrayFloats.find( attribId ) == m_attribsArrayFloats.end() )
            return def_vec3;
        if ( m_attribsArrayFloats.at( attribId ).ndim != 3 )
            return def_vec3;

        auto& array_float = m_attribsArrayFloats.at( attribId );
        return TVec3( array_float[0], array_float[1], array_float[2] );
    }

    TVec4 TElement::GetVec4( const std::string& attribId, const TVec4& def_vec4 ) const
    {
        if ( m_attribsArrayFloats.find( attribId ) == m_attribsArrayFloats.end() )
            return def_vec4;
        if ( m_attribsArrayFloats.at( attribId ).ndim != 4 )
            return def_vec4;

        auto& array_float = m_attribsArrayFloats.at( attribId );
        return TVec4( array_float[0], array_float[1], array_float[2], array_float[3] );
    }

    std::string TElement::GetString( const std::string& attribId, const std::string& def_string ) const
    {
        if ( m_attribsStrings.find( attribId ) == m_attribsStrings.end() )
            return def_string;
        return m_attribsStrings.at( attribId );
    }

    TElement& TElement::get_child( size_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_children.size() ), "TElement::get_child >>> index \
                          {0} out of range [0,...{1}]", index, m_children.size() );
        return *m_children[index].get();
    }

    const TElement& TElement::get_child( size_t index ) const
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_children.size() ), "TElement::get_child >>> index \
                          {0} out of range [0,...{1}]", index, m_children.size() );
        return *m_children[index].get();
    }

}}
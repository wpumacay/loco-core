
#include <utils/loco_parsing_schema.h>

namespace loco {
namespace parsing {

    std::string ToString( const eAttributeType& type )
    {
        if ( type == eAttributeType::TYPE_INT ) return "int";
        else if ( type == eAttributeType::TYPE_FLOAT ) return "float";
        else if ( type == eAttributeType::TYPE_ARRAY_INT ) return "array_int";
        else if ( type == eAttributeType::TYPE_ARRAY_FLOAT ) return "array_float";
        else if ( type == eAttributeType::TYPE_STRING ) return "string";

        LOCO_CORE_WARN( "ToString >>> Undefined eAttributeType enum" );
        return "undefined";
    }

    std::string ToString( const eSchemaType& type )
    {
        if ( type == eSchemaType::MJCF ) return "MJCF";
        else if ( type == eSchemaType::URDF ) return "URDF";

        return "undefined";
    }

    /***********************************************************************************************
    *                                     TISchema Interface                                       *
    ************************************************************************************************/

    std::unordered_map<std::string, eAttributeType> TISchema::GetPossibleAttributes( const std::string& elementName ) const
    {
        if ( !HasElement( elementName ) )
            return std::unordered_map<std::string, eAttributeType >();

        return m_attribsMap.at( elementName );
    }

    eAttributeType TISchema::GetAttributeType( const std::string& elementName, const std::string& attributeName ) const
    {
        if ( !HasElement( elementName ) )
            return eAttributeType::TYPE_UNDEFINED;
        if ( !HasAttribute( elementName, attributeName ) )
            return eAttributeType::TYPE_UNDEFINED;
        
        return m_attribsMap.at( elementName ).at( attributeName );
    }

    std::unordered_set<std::string> TISchema::GetPossibleChildren( const std::string& elementName ) const
    {
        if ( !HasElement( elementName ) )
            return std::unordered_set<std::string>();

        return m_childrenMap.at( elementName );
    }

    bool TISchema::HasElement( const std::string& elementName ) const
    {
        return m_attribsMap.find( elementName ) != m_attribsMap.end();
    }

    bool TISchema::HasAttribute( const std::string& elementName, const std::string& attributeName ) const
    {
        if ( !HasElement( elementName ) )
            return false;

        return m_attribsMap.at( elementName ).find( attributeName ) != m_attribsMap.at( elementName ).end();
    }

    bool TISchema::HasChild( const std::string& elementName, const std::string& childElementName ) const
    {
        if ( !HasElement( elementName ) )
            return false;

        return m_childrenMap.at( elementName ).find( childElementName ) != m_childrenMap.at( elementName ).end();
    }

    std::string TISchema::ToString() const
    {
        std::string _strrep = "Schema(\n";
        _strrep += "type    : " + loco::parsing::ToString( m_type ) + "\n";
        _strrep += "elements:\n";
        for ( auto kv : m_attribsMap )
        {
            auto element_name = kv.first;
            auto element_attributes = kv.second;
            auto element_children = m_childrenMap.at( element_name );
            _strrep += "\tname : " + element_name + "\n";
            _strrep += "\tattributes:\n";
            for ( auto kv_att : element_attributes )
            {
                _strrep += "\t\tname : " + kv_att.first + "\n";
                _strrep += "\t\ttype : " + loco::parsing::ToString( kv_att.second ) + "\n";
            }
            _strrep += "\tchildren:\n";
            for ( auto child : element_children )
                _strrep += "\t\tname : " + child + "\n";
        }

        return _strrep;
    }

    /***********************************************************************************************
    *                                     TMjcfSchema impl.                                        *
    ************************************************************************************************/

    TMjcfSchema::TMjcfSchema( const std::string& filepath )
        : TISchema( filepath )
    {
        m_type = eSchemaType::MJCF;

        tinyxml2::XMLDocument xmldoc;
        if ( xmldoc.LoadFile( m_filepath.c_str() ) != tinyxml2::XML_SUCCESS )
        {
            LOCO_CORE_ERROR( "TMjcfSchema >>> couldn't load schema from filepath: {0}", m_filepath );
            return;
        }

        // Traverse whole tree "recursively", as there're elements within elements (see schema_mjcf.xml)
        std::stack<const tinyxml2::XMLElement*> xml_elements;
        xml_elements.push( xmldoc.FirstChildElement() );
        while ( xml_elements.size() > 0 )
        {
            auto xml_element = xml_elements.top();
            xml_elements.pop();
            if ( !xml_element )
                continue;

            auto xml_element_name = xml_element->Attribute( "name" );
            // Create entry in attributes map for this element
            if ( m_attribsMap.find( xml_element_name ) == m_attribsMap.end() )
                m_attribsMap[xml_element_name] = std::unordered_map<std::string, eAttributeType>();
            // Create entry in children map for this element
            if ( m_childrenMap.find( xml_element_name ) == m_childrenMap.end() )
                m_childrenMap[xml_element_name] = std::unordered_set<std::string>();

            auto xml_attribs_in_element = xml_element->FirstChildElement( "attributes" );
            if ( xml_attribs_in_element )
            {
                auto xml_attrib = xml_attribs_in_element->FirstChildElement( "attribute" );
                while ( xml_attrib )
                {
                    std::string attrib_name = xml_attrib->Attribute( "name" );
                    std::string attrib_type = xml_attrib->Attribute( "type" );

                    if ( attrib_type == "int" )
                        m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_INT;
                    else if ( attrib_type == "float" )
                        m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_FLOAT;
                    else if ( attrib_type == "array" )
                    {
                        std::string array_type = xml_attrib->Attribute( "array_type" );
                        if ( array_type == "int" )
                            m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_ARRAY_INT;
                        else
                            m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_ARRAY_FLOAT;
                    }
                    else if ( attrib_type == "keyword" )
                        m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_STRING;
                    else
                        m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_STRING;

                    xml_attrib = xml_attrib->NextSiblingElement();
                }
            }

            auto xml_children_in_element = xml_element->FirstChildElement( "children" );
            if ( xml_children_in_element )
            {
                auto xml_child_element = xml_children_in_element->FirstChildElement( "element" );
                while ( xml_child_element )
                {
                    auto xml_child_name = xml_child_element->Attribute( "name" );
                    m_childrenMap[xml_element_name].emplace( xml_child_name );

                    xml_elements.push( xml_child_element );
                    xml_child_element = xml_child_element->NextSiblingElement();
                }
            }
        }
    }

    std::unique_ptr<TMjcfSchema> TMjcfSchema::s_Instance = nullptr;

    TMjcfSchema* TMjcfSchema::GetInstance()
    {
        if ( !TMjcfSchema::s_Instance )
            TMjcfSchema::s_Instance = std::make_unique<TMjcfSchema>( loco::PATH_RESOURCES + "xml/schema_mjcf.xml" );
        return TMjcfSchema::s_Instance.get();
    }

    /***********************************************************************************************
    *                                     TUrdfSchema impl.                                        *
    ************************************************************************************************/

    TUrdfSchema::TUrdfSchema( const std::string& filepath )
        : TISchema( filepath )
    {
        m_type = eSchemaType::URDF;

        tinyxml2::XMLDocument xmldoc;
        if ( xmldoc.LoadFile( m_filepath.c_str() ) != tinyxml2::XML_SUCCESS )
        {
            LOCO_CORE_ERROR( "TUrdfSchema >>> couldn't load schema from filepath: {0}", m_filepath );
            return;
        }

        auto xml_root = xmldoc.FirstChildElement();
        if ( !xml_root )
        {
            LOCO_CORE_ERROR( "TUrdfSchema >>> there's no root element in schema from {0}", m_filepath );
            return;
        }

        // Collect complex-type elements (all in same level, so no need to traverse "recursively")
        auto xml_element = xml_root->FirstChildElement( "xs:complexType" );
        while ( xml_element )
        {
            auto xml_element_name = xml_element->Attribute( "name" );
            // Create entry in attributes map for this element
            if ( m_attribsMap.find( xml_element_name ) == m_attribsMap.end() )
                m_attribsMap[xml_element_name] = std::unordered_map<std::string, eAttributeType>();
            // Create entry in children map for this element
            if ( m_childrenMap.find( xml_element_name ) == m_childrenMap.end() )
                m_childrenMap[xml_element_name] = std::unordered_set<std::string>();

            // Collect attributes for primitive elements
            auto xml_attrib = xml_element->FirstChildElement( "xs:attribute" );
            while ( xml_attrib )
            {
                std::string attrib_name = xml_attrib->Attribute( "name" );
                std::string attrib_type = xml_attrib->Attribute( "type" );

                if ( attrib_type == "xs:double" )
                    m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_FLOAT;
                else if ( attrib_type == "xs:array" )
                    m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_ARRAY_FLOAT;
                else if ( attrib_type == "xs:anyURI" )
                    m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_STRING;
                else if ( attrib_type == "xs:string" )
                    m_attribsMap[xml_element_name][attrib_name] = eAttributeType::TYPE_STRING;

                xml_attrib = xml_attrib->NextSiblingElement();
            }

            // Collect children elements (choice: only one at a time)
            if ( auto xml_choice = xml_element->FirstChildElement( "xs:choice" ) )
            {
                auto xml_child_element = xml_choice->FirstChildElement( "xs:element" );
                while ( xml_child_element )
                {
                    auto xml_child_name = xml_child_element->Attribute( "name" );
                    auto xml_child_type = xml_child_element->Attribute( "type" );
                    // Add child as option in children set
                    m_childrenMap[xml_element_name].emplace( xml_child_name );
                    // Copy attributes and children from similar type, if type is an element (e.g. pose and origin)
                    if ( xml_child_type != NULL && HasElement( xml_child_type ) && !HasElement( xml_child_name ) )
                    {
                        m_attribsMap[xml_child_name] = m_attribsMap[xml_child_type];
                        m_childrenMap[xml_child_name] = m_childrenMap[xml_child_type];
                    }

                    xml_child_element = xml_child_element->NextSiblingElement();
                }
            }

            // Collect children elements (sequence: any of them is possible, even all)
            if ( auto xml_sequence = xml_element->FirstChildElement( "xs:sequence" ) )
            {
                auto xml_child_element = xml_sequence->FirstChildElement( "xs:element" );
                while ( xml_child_element )
                {
                    auto xml_child_name = xml_child_element->Attribute( "name" );
                    auto xml_child_type = xml_child_element->Attribute( "type" );
                    // Add child as option in children set
                    m_childrenMap[xml_element_name].emplace( xml_child_name );
                    // Copy attributes and children from similar type, if type is an element (e.g. pose and origin)
                    if ( xml_child_type != NULL && HasElement( xml_child_type ) && !HasElement( xml_child_name ) )
                    {
                        m_attribsMap[xml_child_name] = m_attribsMap[xml_child_type];
                        m_childrenMap[xml_child_name] = m_childrenMap[xml_child_type];
                    }

                    xml_child_element = xml_child_element->NextSiblingElement();
                }
            }

            // Collect children elements (all: similar to sequence?)
            if ( auto xml_all = xml_element->FirstChildElement( "xs:all" ) )
            {
                auto xml_child_element = xml_all->FirstChildElement( "xs:element" );
                while ( xml_child_element )
                {
                    auto xml_child_name = xml_child_element->Attribute( "name" );
                    auto xml_child_type = xml_child_element->Attribute( "type" );
                    // Add child as option in children set
                    m_childrenMap[xml_element_name].emplace( xml_child_name );
                    // Copy attributes and children from similar type, if type is an element (e.g. pose and origin)
                    if ( xml_child_type != NULL && HasElement( xml_child_type ) && !HasElement( xml_child_name ) )
                    {
                        m_attribsMap[xml_child_name] = m_attribsMap[xml_child_type];
                        m_childrenMap[xml_child_name] = m_childrenMap[xml_child_type];
                    }

                    xml_child_element = xml_child_element->NextSiblingElement();
                }
            }

            xml_element = xml_element->NextSiblingElement();
        }

        LOCO_CORE_TRACE( "Finished parsing urdf-schema" );
    }

    std::unique_ptr<TUrdfSchema> TUrdfSchema::s_Instance = nullptr;

    TUrdfSchema* TUrdfSchema::GetInstance()
    {
        if ( !TUrdfSchema::s_Instance )
            TUrdfSchema::s_Instance = std::make_unique<TUrdfSchema>( loco::PATH_RESOURCES + "xml/schema_urdf.xml" );
        return TUrdfSchema::s_Instance.get();
    }

}}
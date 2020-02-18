#pragma once

#include <loco_common.h>
#include <utils/loco_parsing_common.h>

namespace loco {
namespace parsing {

    enum class eAttributeType : uint8_t
    {
        TYPE_UNDEFINED = 0,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_ARRAY_INT,
        TYPE_ARRAY_FLOAT,
        TYPE_STRING
    };

    std::string ToString( const eAttributeType& type );

    enum class eSchemaType : uint8_t
    {
        MJCF = 0,
        URDF
    };

    std::string ToString( const eSchemaType& type );

    class TISchema
    {
    public :

        TISchema( const std::string& filepath ) : m_filepath( filepath ) {}

        virtual ~TISchema() = default;

        std::unordered_map<std::string, eAttributeType> GetPossibleAttributes( const std::string& elementName ) const;

        eAttributeType GetAttributeType( const std::string& elementName, const std::string& attributeName ) const;

        std::unordered_set<std::string> GetPossibleChildren( const std::string& elementName ) const;

        bool HasElement( const std::string& elementName ) const;

        bool HasAttribute( const std::string& elementName, const std::string& attributeName ) const;

        bool HasChild( const std::string& elementName, const std::string& childElementName ) const;

        eSchemaType type() const { return m_type; }

        std::string filepath() const { return m_filepath; }

        std::string ToString() const;

    protected :

        // Path to the file containing the schema
        std::string m_filepath;
        // Type of schema (see enum above)
        eSchemaType m_type;
        // Map of element-attribs { element_name : { attribute_name : attribute_type } }
        std::unordered_map<std::string,std::unordered_map<std::string, eAttributeType>> m_attribsMap;
        // Map of element-children { element_name : [ poss_child_name ] }
        std::unordered_map<std::string,std::unordered_set<std::string>> m_childrenMap;
    };

    class TMjcfSchema : public TISchema
    {
    public :

        TMjcfSchema( const std::string& filepath );

        TMjcfSchema( const TMjcfSchema& other ) = default;

        TMjcfSchema& operator= ( const TMjcfSchema& other ) = default;

        ~TMjcfSchema() = default;

    };

    class TUrdfSchema : public TISchema
    {
    public :

        TUrdfSchema( const std::string& filepath );

        TUrdfSchema( const TUrdfSchema& other ) = default;

        TUrdfSchema& operator= ( const TUrdfSchema& other ) = default;

        ~TUrdfSchema() = default;

    };

}}
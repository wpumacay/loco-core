#pragma once

#include <utils/loco_parsing_common.h>
#include <utils/loco_parsing_schema.h>

namespace loco {
namespace parsing {

    class TElement
    {
    public :

        TElement( const std::string& elementType,
                  const eSchemaType& schemaType );

        TElement( const TElement& other ) = delete;

        TElement& operator= ( const TElement& other ) = delete;

        virtual ~TElement() = default;

        TElement* Add( const std::string& childType );

        void LoadFromXml( const std::string& filepath );
        void SaveToXml( const std::string& filepath );
        void CollectAttribs( tinyxml2::XMLElement* xml_element );

        void SetInt( const std::string& attribId, int32_t value );
        void SetFloat( const std::string& attribId, TScalar value );
        void SetArrayInt( const std::string& attribId, const TSizei& value );
        void SetArrayFloat( const std::string& attribId, const TSizef& value );
        void SetVec2( const std::string& attribId, const TVec2& value );
        void SetVec3( const std::string& attribId, const TVec3& value );
        void SetVec4( const std::string& attribId, const TVec4& value );
        void SetString( const std::string& attribId, const std::string& value );

        int32_t GetInt( const std::string& attribId, int32_t def_int = 0 ) const;
        TScalar GetFloat( const std::string& attribId, TScalar def_float = 0.0f ) const;
        TSizei GetArrayInt( const std::string& attribId, const TSizei& def_arrint = TSizei() ) const;
        TSizef GetArrayFloat( const std::string& attribId, const TSizef& def_arrfloat = TSizef() ) const;
        TVec2 GetVec2( const std::string& attribId, const TVec2& def_vec2 = TVec2( 0.0f, 0.0f ) ) const;
        TVec3 GetVec3( const std::string& attribId, const TVec3& def_vec3 = TVec3( 0.0f, 0.0f, 0.0f ) ) const;
        TVec4 GetVec4( const std::string& attribId, const TVec4& def_vec4 = TVec4( 0.0f, 0.0f, 0.0f, 1.0f ) ) const;

        bool HasAttributeInt( const std::string& attribId ) const;
        bool HasAttributeFloat( const std::string& attribId ) const;
        bool HasAttributeArrayInt( const std::string& attribId ) const;
        bool HasAttributeArrayFloat( const std::string& attribId ) const;
        bool HasAttributeVec2( const std::string& attribId ) const;
        bool HasAttributeVec3( const std::string& attribId ) const;
        bool HasAttributeVec4( const std::string& attribId ) const;

        std::vector<TElement*> children() const;

        std::string elementType() const { return m_elementType; }

        eSchemaType schemaType() const { return m_schemaType; }

        TISchema* schema() const { return m_schema; }

    private :

        std::string m_elementType;

        eSchemaType m_schemaType;

        TElement* m_parent;

        TISchema* m_schema;

        std::vector<std::unique_ptr<TElement>> m_children;

        std::unordered_map<std::string, int32_t>        m_attribsInts;
        std::unordered_map<std::string, TScalar>        m_attribsFloats;
        std::unordered_map<std::string, TSizei>         m_attribsArrayInts;
        std::unordered_map<std::string, TSizef>         m_attribsArrayFloats;
        std::unordered_map<std::string, std::string>    m_attribsStrings;
    };

}}
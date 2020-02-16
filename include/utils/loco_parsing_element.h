#pragma once

#include <loco_parsing_xml.h>
#include <loco_parsing_json.h>

namespace loco {
namespace parsing {

    class TElement
    {
    public :

        TElement( const std::string& type );

        virtual ~TElement();

        TElement* Add( const std::string& childType );

        void CollectAttribs( Schema* schema );

        void SetInt( const std::string& attribId, int32_t value );
        void SetFloat( const std::string& attribId, TScalar value );
        void SetArrayInt( const std::string& attribId, const TSizei& value );
        void SetArrayFloat( const std::string& attribId, const TSizef& value );
        void SetVec2( const std::string& attribId, const TVec2& value );
        void SetVec3( const std::string& attribId, const TVec3& value );
        void SetVec4( const std::string& attribId, const TVec4& value );
        void SetString( const std::string& attribId, const std::string& value );

        int32_t GetInt( const std::string& attribId );
        TScalar GetFloat( const std::string& attribId );

        std::string type() const { return m_type; }

    private :

        std::string m_type;

        std::unordered_map<std::string, int32_t>        m_attribsInts;
        std::unordered_map<std::string, TScalar >       m_attribsFloats;
        std::unordered_map<std::string, TSizei >        m_attribsArrayInts;
        std::unordered_map<std::string, TSizef >        m_attribsArrayFloats;
        std::unordered_map<std::string, std::string >   m_attribsStrings;

        TElement* m_parent;

        std::vector<std::unique_ptr<TElement>> m_children;
    };

}}
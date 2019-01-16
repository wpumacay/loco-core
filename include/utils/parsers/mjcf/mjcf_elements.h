
#pragma once

#include "mjcf_common.h"

#include <utils/parsers/parsers_xml.h>

namespace tysoc {
namespace mjcf {

    struct GenericElement
    {
        std::string                     etype;
        GenericElement*                 parent;
        std::vector< GenericElement* >  children;

        std::map< std::string, int >            _ints;
        std::map< std::string, float >          _floats;
        std::map< std::string, TSizef >         _sizefs;
        std::map< std::string, TSizei >         _sizeis;
        std::map< std::string, std::string >    _strings;

        GenericElement();
        GenericElement( const std::string& elementType );

        void collectAttribs( Schema* schema, tinyxml2::XMLElement* xmlElement );
        void insertAttribs( tinyxml2::XMLElement* xmlElement );

        void setAttributeInt( const std::string& attribName, int val );
        void setAttributeFloat( const std::string& attribName, float val );
        void setAttributeArrayInt( const std::string& attribName, const TSizei& arrayint );
        void setAttributeArrayFloat( const std::string& attribName, const TSizef& arrayfloat );
        void setAttributeString( const std::string& attribName, const std::string& val );
        void setAttributeVec2( const std::string& attribName, const TVec2& vec );
        void setAttributeVec3( const std::string& attribName, const TVec3& vec );
        void setAttributeVec4( const std::string& attribName, const TVec4& vec );

        int getAttributeInt( const std::string& attribName, int def = 0 );
        float getAttributeFloat( const std::string& attribName, float def = 0.0 );
        TSizef getAttributeArrayFloat( const std::string& attribName );
        TSizei getAttributeArrayInt( const std::string& attribName );
        std::string getAttributeString( const std::string& attribName, const std::string& def = "" );
        TVec2 getAttributeVec2( const std::string& attribName, const TVec2& def = { 0, 0 } );
        TVec3 getAttributeVec3( const std::string& attribName, const TVec3& def = { 0, 0, 0 } );
        TVec4 getAttributeVec4( const std::string& attribName, const TVec4& def = { 0, 0, 0, 1 } );
    
        bool hasAttributeInt( const std::string& attribName );
        bool hasAttributeFloat( const std::string& attribName );
        bool hasAttributeArrayInt( const std::string& attribName );
        bool hasAttributeArrayFloat( const std::string& attribName );
        bool hasAttributeString( const std::string& attribName );
        bool hasAttributeVec2( const std::string& attribName );
        bool hasAttributeVec3( const std::string& attribName );
        bool hasAttributeVec4( const std::string& attribName );

        void print();
    };

    GenericElement* createWorldBody();

    GenericElement* createBody( const std::string& name,
                                const TVec3& pos = { 0, 0, 0 },
                                const TVec4& quat = { 1, 0, 0, 0 } );

    GenericElement* createGeometry( const std::string& name,
                                    const std::string& type,
                                    const TSizef& size,
                                    float mass = 0.0f,
                                    const TVec3& pos = { 0, 0, 0 },
                                    const TVec4& quat = { 1, 0, 0, 0 } );

    GenericElement* createGeometry( const std::string& name,
                                    const std::string& type,
                                    const TSizef& size,
                                    const TSizef& fromto,
                                    float mass = 0.0f );

}}
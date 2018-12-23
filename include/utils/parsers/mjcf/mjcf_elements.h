
#pragma once

#include "mjcf_common.h"
#include "mjcf_utils.h"

namespace tysoc {
namespace mjcf {

    struct GenericElement
    {
        std::string                     etype;
        GenericElement*                 parent;
        std::vector< GenericElement* >  children;

        std::map< std::string, int >            _ints;
        std::map< std::string, float >          _floats;
        std::map< std::string, Sizef >          _sizefs;
        std::map< std::string, Sizei >          _sizeis;
        std::map< std::string, std::string >    _strings;

        GenericElement();
        GenericElement( const std::string& elementType );

        void collectAttribs( Schema* schema, tinyxml2::XMLElement* xmlElement );
        void insertAttribs( tinyxml2::XMLElement* xmlElement );

        void setAttributeInt( const std::string& attribName, int val );
        void setAttributeFloat( const std::string& attribName, float val );
        void setAttributeArrayInt( const std::string& attribName, const Sizei& arrayint );
        void setAttributeArrayFloat( const std::string& attribName, const Sizef& arrayfloat );
        void setAttributeString( const std::string& attribName, const std::string& val );

        void setAttributeVec3( const std::string& attribName, const Vec3& vec );
        void setAttributeVec4( const std::string& attribName, const Vec4& vec );

        int getAttributeInt( const std::string& attribName );
        float getAttributeFloat( const std::string& attribName );
        Sizef getAttributeArrayFloat( const std::string& attribName );
        Sizei getAttributeArrayInt( const std::string& attribName );
        std::string getAttributeString( const std::string& attribName );

        Vec2 getAttributeVec2( const std::string& attribName, const Vec2& def = { 0, 0 } );
        Vec3 getAttributeVec3( const std::string& attribName, const Vec3& def = { 0, 0, 0 } );
        Vec4 getAttributeVec4( const std::string& attribName, const Vec4& def = { 0, 0, 0, 1 } );
 
        bool hasAttributeVec4( const std::string& attribName );
        bool hasAttributeVec3( const std::string& attribName );

        void print();
    };

    GenericElement* createWorldBody();

    GenericElement* createBody( const std::string& name,
                                const Vec3& pos = { 0, 0, 0 },
                                const Vec4& quat = { 0, 0, 0, 1 } );

    GenericElement* createGeometry( const std::string& name,
                                    const std::string& type,
                                    const Sizef& size,
                                    float mass = 0.0f,
                                    const Vec3& pos = { 0, 0, 0 },
                                    const Vec4& quat = { 0, 0, 0, 1 } );

    GenericElement* createGeometry( const std::string& name,
                                    const std::string& type,
                                    const Sizef& size,
                                    const Sizef& fromto,
                                    float mass = 0.0f );

}}
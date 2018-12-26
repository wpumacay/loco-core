
#pragma once

#include <tysoc_common.h>

#include <utils/parsers/parsers_xml.h>

namespace tysoc {
namespace mjcf {

    enum AttribType
    {
        TYPE_UNDEFINED,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_ARRAY_FLOAT,
        TYPE_ARRAY_INT,
        TYPE_STRING
    };

    class Schema
    {
        private :

        std::map< std::string, std::map< std::string, AttribType > > m_attribsPerElement;

        void _collectAttributesSchema( const std::string& elementName, 
                                       tinyxml2::XMLElement* xmlElement );
        void _traverse( tinyxml2::XMLElement* xmlElement );

        public :

        Schema();
        ~Schema();

        void load( const std::string& filename );
        void print();

        std::map< std::string, AttribType > getPossibleAttribs( const std::string& elementName );
        AttribType getAttribType( const std::string& elementName,
                                  const std::string& attribName );
    };

}}
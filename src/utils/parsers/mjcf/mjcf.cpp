
#include <utils/parsers/mjcf/mjcf.h>

namespace tysoc {
namespace mjcf {

    std::string MJCF_SCHEMA_PATH = std::string( TYSOC_PATH_RESOURCES ) + std::string( "xml/schema.xml" );
    
    GenericElement* _parseGenericElement( Schema* schema, tinyxml2::XMLElement* xmlElement )
    {
        auto _gelement = new GenericElement();
        _gelement->collectAttribs( schema, xmlElement );

        auto _currentChild = xmlElement->FirstChildElement();
        while ( _currentChild != NULL )
        {
            // parse child element
            auto _ieChildElement = _parseGenericElement( schema, _currentChild );
            _gelement->children.push_back( _ieChildElement );

            // check next sibling
            _currentChild = _currentChild->NextSiblingElement();
        }

        return _gelement;
    }

    GenericElement* loadGenericModel( const std::string& modelfile )
    {
        if ( !MJCF_SCHEMA )
        {
            std::cout << "INFO> creating mjcf schema: " << MJCF_SCHEMA_PATH << std::endl;
            // load the schema to be used with the mjcf helper
            MJCF_SCHEMA = new mjcf::Schema();
            MJCF_SCHEMA->load( MJCF_SCHEMA_PATH );
        }

        tinyxml2::XMLDocument _doc;
        _doc.LoadFile( modelfile.c_str() );

        auto _root = _parseGenericElement( MJCF_SCHEMA, _doc.FirstChildElement() );
        return _root;
    }

    void saveGenericModel( GenericElement* root, const std::string& modelfile )
    {
        tinyxml2::XMLDocument _doc;
        
        auto _rootXML = _doc.NewElement( root->etype.c_str() );
        _doc.InsertEndChild( _rootXML );

        for ( size_t i = 0; i < root->children.size(); i++ )
        {
            _createElement( _doc, _rootXML, root->children[i] );
        }

        _doc.SaveFile( modelfile.c_str() );
    }

    void _createElement( tinyxml2::XMLDocument& doc,
                         tinyxml2::XMLElement* parentXML,
                         GenericElement* element )
    {
        auto _xmlElement = doc.NewElement( element->etype.c_str() );
        element->insertAttribs( _xmlElement );
        parentXML->InsertEndChild( _xmlElement );

        for ( size_t i = 0; i < element->children.size(); i++ )
        {
            _createElement( doc, _xmlElement, element->children[i] );
        }
    }

    GenericElement* findFirstChildByType( GenericElement* target, const std::string& type )
    {
        if ( !target )
        {
            return NULL;
        }

        // Find using DFS, it does the job and our xmls are not that deep
        // @TODO: Change for other traversal if needed
        for ( size_t i = 0; i < target->children.size(); i++ )
        {
            if ( target->children[i]->etype == type )
            {
                return target->children[i];
            }
            else
            {
                auto _elm = findFirstChildByType( target->children[i], type );
                if ( _elm )
                {
                    return _elm;
                }
            }
        }

        return NULL;
    }

    std::vector< GenericElement* > getChildrenByType( GenericElement* target, const std::string& type )
    {
        std::vector< GenericElement* > _children;

        for ( size_t i = 0; i < target->children.size(); i++ )
        {
            if ( target->children[i]->etype == type )
            {
                _children.push_back( target->children[i] );
            }
        }

        return _children;
    }

    void replaceNameFields( GenericElement* elementPtr, 
                            const std::string& agentName,
                            const std::string& attribName,
                            const std::string& targetType )
    {
        if ( !elementPtr->hasAttributeString( attribName ) )
        {
            return;
        }

        auto _oldName = elementPtr->getAttributeString( attribName );
        elementPtr->setAttributeString( attribName, computeMjcfName( targetType,
                                                                     _oldName,
                                                                     agentName ) );
    }

    void deepCopy( GenericElement* target, 
                   GenericElement* source, 
                   GenericElement* parent,
                   const std::string& agentName )
    {
        target->etype     = source->etype;
        target->parent    = parent;
        target->_ints     = source->_ints;
        target->_floats   = source->_floats;
        target->_sizefs   = source->_sizefs;
        target->_sizeis   = source->_sizeis;
        target->_strings  = source->_strings;

        if ( agentName != "" )
        {
            replaceNameFields( target, agentName, "name", target->etype );
            replaceNameFields( target, agentName, "joint", "joint" );
            replaceNameFields( target, agentName, "target", "camera" );
            replaceNameFields( target, agentName, "body", "body" );
            replaceNameFields( target, agentName, "site", "site" );
            // @GENERALIZE : objname could be a body, joint, site, geom, ...
            replaceNameFields( target, agentName, "objname", "body" );
            replaceNameFields( target, agentName, "body1", "body" );
            replaceNameFields( target, agentName, "body2", "body" );
        }

        for ( size_t i = 0; i < source->children.size(); i++ )
        {
            auto _sourceChild   = source->children[i];
            auto _targetChild   = new GenericElement();

            deepCopy( _targetChild, _sourceChild, target, agentName );

            target->children.push_back( _targetChild );
        }
    }

    std::string computeMjcfName( const std::string& type, 
                                 const std::string& elementName, 
                                 const std::string& agentName )
    {
        std::string _res;

        if ( type == "body" )
        {
            _res += std::string( TYSOC_PREFIX_BODY ) + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "joint" )
        {
            _res += std::string( TYSOC_PREFIX_JOINT ) + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "geom" )
        {
            _res += std::string( TYSOC_PREFIX_GEOM ) + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "site" )
        {
            _res += std::string( TYSOC_PREFIX_SITE ) + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "actuator" || type == "motor" || type == "position" || type == "velocity" )
        {
            _res += std::string( TYSOC_PREFIX_ACTUATOR ) + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "sensor" )
        {
            _res += std::string( TYSOC_PREFIX_SENSOR ) + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "camera" )
        {
            _res += std::string( TYSOC_PREFIX_CAMERA ) + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "mesh" )
        {
            _res += elementName;
        }
        else
        {
            std::cout << "WARNING> not supported mjc type: " 
                      << type << " for mjcname generation" << std::endl;

            _res += elementName;
        }

        return _res;
    }
}}
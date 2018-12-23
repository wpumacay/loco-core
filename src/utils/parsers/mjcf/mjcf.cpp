
#include <utils/parsers/mjcf/mjcf.h>

namespace tysoc {
namespace mjcf {
    
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

    GenericElement* loadGenericModel( Schema* schema, const std::string& modelfile )
    {
        tinyxml2::XMLDocument _doc;
        _doc.LoadFile( modelfile.c_str() );

        auto _root = _parseGenericElement( schema, _doc.FirstChildElement() );
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

    void replaceNameRecursive( GenericElement* root, const std::string& name, const std::string& tag )
    {
        auto _oldName = root->getAttributeString( tag );
        // Look for the placeholder text
        size_t _startPlaceholder = _oldName.find( NAME_PLACEHOLDER_SECTION );

        if ( _startPlaceholder != std::string::npos )
        {
            auto _nameSubstrPre  = _oldName.substr( 0, _startPlaceholder );
            auto _nameSubstrPost = _oldName.substr( _startPlaceholder + 3 );
            auto _newname = _nameSubstrPre + name + _nameSubstrPost;

            root->setAttributeString( tag, _newname );
        }

        for ( size_t i = 0; i < root->children.size(); i++ )
        {
            replaceNameRecursive( root->children[i], name, tag );
        }
    }

    void deepCopy( GenericElement* target, GenericElement* source )
    {
        target->etype     = source->etype;
        target->parent    = source->parent;
        target->_ints     = source->_ints;
        target->_floats   = source->_floats;
        target->_sizefs   = source->_sizefs;
        target->_sizeis   = source->_sizeis;
        target->_strings  = source->_strings;

        for ( size_t i = 0; i < source->children.size(); i++ )
        {
            auto _sourceChild   = source->children[i];
            auto _targetChild   = new GenericElement();

            deepCopy( _targetChild, _sourceChild );

            target->children.push_back( _targetChild );
        }
    }
}}
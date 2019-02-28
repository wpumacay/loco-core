
#pragma once

#include "mjcf_common.h"
#include "mjcf_elements.h"

#define NAME_PLACEHOLDER_SECTION "###"

namespace tysoc {
namespace mjcf {

    static Schema* MJCF_SCHEMA = NULL;
    extern std::string MJCF_SCHEMA_PATH;

    /**
    * @brief            Creates a GenericElement, given the schema for error checking and a parsed xml element
    * @param schema     Object representing the schema parsed from the schema.xml file
    * @param xmlElement XML element from tinyxml2 which contains the xml parsed data
    */
    GenericElement* _parseGenericElement( Schema* schema, 
                                          tinyxml2::XMLElement* xmlElement );

    /**
    * @brief            Creates tinyxml2 XML element from a generic element, and inserts it into a tinyxml2 element node
    * @param doc        Base document element from tinyxml2
    * @param parentXML  XML element from tinyxml2 where we want to place the generic element data
    * @param element    Generic element we want to insert
    */
    void _createElement( tinyxml2::XMLDocument& doc,
                         tinyxml2::XMLElement* parentXML,
                         GenericElement* element );

    /**
    * @brief            Loads a model from an xml file
    * @param schema     Object representing the schema parsed from the schema.xml file
    * @param modelfile  Filename (fullpath) to the desired model file
    */
    GenericElement* loadGenericModel( const std::string& modelfile );

    /**
    * @brief            Saves generic model into xml file
    * @param root       The generic element that holds the data to save into an xml file
    * @param modelfile  Filename (fullpath) to save the model to
    */
    void saveGenericModel( GenericElement* root, 
                           const std::string& modelfile );

    /**
    * @brief            Finds the first generic element of a given type in a given generic element
    * @param target     Target generic element to search
    * @param type       Type of the element we are looking for
    */
    GenericElement* findFirstChildByType( GenericElement* target, 
                                          const std::string& type );

    /**
    * @brief            Gets a vector with all children of a given type in a given target element
    * @param target     Target generic element to search
    * @param type       Type of the elements we want to grab
    */
    std::vector< GenericElement* > getChildrenByType( GenericElement* target, 
                                                      const std::string& type );

    /**
    * @brief            Replaces the fields related to name ids by composing with the agentName
    * @param root       Generic element we want to modify
    * @param agentName  Name of the agent to use for name construction
    * @param attribName Attribute whose name we want to change
    * @param targetType Type to be used for name generation (body, joint, sensor, etc.)
    */
    void replaceNameFields( GenericElement* elementPtr, 
                            const std::string& agentName,
                            const std::string& attribName,
                            const std::string& targetType );

    /**
    * @brief            Makes a deep copy of the given source generic element
    * @param target     Target generic element where to copy the data
    * @param source     Source generic element from whom to copy the data
    * @param parent     Parent generic element in the fresh new tree of the target element
    * @param agentName  Name of the agent (optional) to be used to replace name attribs
    */
    void deepCopy( GenericElement* target, 
                   GenericElement* source,
                   GenericElement* parent,
                   const std::string& agentName = "" );

    /**
    * @brief                Creates the appropiate name for the mjcf element
    * @param type           Type element type from whom we want to compute the name
    * @param elementName    The name of the element in the template file
    * @param agentName      The name of the agent we are computing this for
    */
    std::string computeMjcfName( const std::string& type, 
                                 const std::string& elementName, 
                                 const std::string& agentName );
}}

#pragma once

#include "urdf_elements.h"

namespace tysoc {
namespace urdf {

    static int CURRENT_VISUALS_COUNTER = 0;
    static int CURRENT_COLLISIONS_COUNTER = 0;

    /**
    * @brief            Loads an urdf model file into the appropiate data structures
    * @param modelfile  A string representing the fullpath to the .urdf file
    * @return           Returns an UrdfModel pointer representing 
    *                   the loaded model, or NULL if something went wrong
    */
    UrdfModel* loadGenericModel( const std::string& modelfile );

    /**
    * @brief                Initializes the model by linking its inner components accordingly
    * @param modelDataPtr   The UrdfModel pointer representing the model to initialize
    * @return               Returns true or false depending of the initialization success
    */
    bool _initTreeAndRoot( UrdfModel* modelDataPtr );

    /**
    * @brief            Makes a deep copy of the given source UrdfModel
    * @param target     Target UrdfModel where to copy the data
    * @param source     Source UrdfModel from whom to copy the data
    * @param agentName  Name of the agent (optional) to be used to replace name fields
    */
    void deepCopy( UrdfModel* target, 
                   UrdfModel* source,
                   const std::string& agentName = "" );

    /**
    * @brief            Makes a deep copy of the given source UrdfLink link
    * @param target     Target UrdfLink where to copy the data
    * @param source     Source UrdfLink from whom to copy the data
    * @param agentName  Name of the agent to be used to replace name fields
    */
    void _deepCopyLink( UrdfLink* targetLink,
                        UrdfLink* sourceLink,
                        const std::string& agentName );

    /**
    * @brief            Makes a deep copy of the given source UrdfJoint joint
    * @param target     Target UrdfJoint where to copy the data
    * @param source     Source UrdfJoint from whom to copy the data
    * @param agentName  Name of the agent to be used to replace name fields
    */
    void _deepCopyJoint( UrdfJoint* targetJoint,
                         UrdfJoint* sourceJoint,
                         const std::string& agentName );

    /**
    * @brief                Creates the appropiate name for the urdf element
    * @param type           Type element type from whom we want to compute the name
    * @param elementName    The name of the element in the template file
    * @param agentName      The name of the agent we are computing this for
    */
    std::string computeUrdfName( const std::string& type, 
                                 const std::string& elementName, 
                                 const std::string& agentName );

    
}}

#pragma once

#include "rlsim_elements.h"

#define JSON_KEY_SKELETON       "Skeleton"
#define JSON_KEY_JOINTS         "Joints"
#define JSON_KEY_BODIES         "BodyDefs"
#define JSON_KEY_VISUALS        "DrawShapeDefs"
#define JSON_KEY_PD_CONTROLLERS "PDControllers"

namespace tysoc {
namespace rlsim {

    /**
    * @brief            Loads an rlsim model file into the appropiate RlsimModel structure
    * @param modelfile  The fullpath to the .json file for this rlsim model
    * @return           Returns an RlsimModel pointer representing 
    *                   the loaded model, or NULL if something went wrong
    */
    RlsimModel* loadGenericModel( const std::string& modelfile );

    /**
    * @brief                Initializes an RlsimModel by linking its inner components accordingly
    * @param modelDataPtr   The RlsimModel pointer representing the model to initialize
    * @return               Returns true or false depending of the initialization success
    */
    bool _initTreeAndRoot( RlsimModel* modelDataPtr );


    /**
    * @brief            Makes a deep copy of the given source RlsimModel
    * @param target     Target RlsimModel where to copy the data
    * @param source     Source RlsimModel from whom to copy the data
    * @param agentName  Name of the agent (optional) to be used to replace name fields
    */
    void deepCopy( RlsimModel* target, 
                   RlsimModel* source,
                   const std::string& agentName = "" );

    /**
    * @brief                Creates the appropiate name for the rlsim element
    * @param type           Type element type from whom we want to compute the name
    * @param elementName    The name of the element in the template file
    * @param agentName      The name of the agent we are computing this for
    */
    std::string computeName( const std::string& type, 
                             const std::string& elementName, 
                             const std::string& agentName );

}}
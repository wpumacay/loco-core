
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


}}
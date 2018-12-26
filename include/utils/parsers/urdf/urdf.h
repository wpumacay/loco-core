
#pragma once

#include "urdf_elements.h"

namespace tysoc {
namespace urdf {

    /**
    * @brief            Loads an urdf model file into the appropiate data structures
    * @param modelfile  A string representing the fullpath to the .urdf file
    * @return           Returns an UrdfModel pointer representing 
    *                   the loaded model, or NULL if something went wrong
    */
    UrdfModel* loadGenericModel( const std::string& modelfile );

    /**
    * @brief                Initializes the model by linkings its inner data accordingly
    * @param modelDataPtr   The UrdfModel pointer representing the model to initialize
    * @return               Returns true or false depending of the initialization success
    */
    bool _initTreeAndRoot( UrdfModel* modelDataPtr );

}}
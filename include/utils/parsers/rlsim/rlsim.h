
#pragma once

#include "rlsim_elements.h"

#define JSON_KEY_SKELETON       "Skeleton"
#define JSON_KEY_JOINTS         "Joints"
#define JSON_KEY_BODIES         "BodyDefs"
#define JSON_KEY_VISUALS        "DrawShapeDefs"
#define JSON_KEY_PD_CONTROLLERS "PDControllers"

namespace tysoc {
namespace rlsim {


    RlsimModel* loadGenericModel( const std::string& modelfile );

    bool _initTreeAndRoot( RlsimModel* modelDataPtr );


}}
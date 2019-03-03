
#pragma once

#include <string>

#ifndef TYSOCCORE_GLVIZ_LIB
    #define TYSOCCORE_GLVIZ_LIB "./glviz/libtysocRenderingGlviz.so"
#endif

namespace tysoc {
namespace config {

    namespace rendering
    {
        const std::string GLVIZ = std::string( TYSOCCORE_GLVIZ_LIB );
    }

    namespace physics
    {
        // Add available physics backend in this namespace
    }

}}
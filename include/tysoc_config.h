
#pragma once

#include <string>

#ifndef TYSOC_BACKEND_GRAPHICS_GLVIZ
    #ifdef __APPLE__
        #define TYSOC_BACKEND_GRAPHICS_GLVIZ "../libs/libtysocRenderingGlviz.dylib"
    #else
        #define TYSOC_BACKEND_GRAPHICS_GLVIZ "../libs/libtysocRenderingGlviz.so"
    #endif
#endif

namespace tysoc {
namespace config {

    namespace rendering
    {
        const std::string GLVIZ = std::string( TYSOC_BACKEND_GRAPHICS_GLVIZ );
    }

    namespace physics
    {
        // Add available physics backend in this namespace
        const std::string NONE = "";
    }

}}
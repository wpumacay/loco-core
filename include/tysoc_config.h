
#pragma once

#include <string>

#ifndef TYSOC_BACKEND_GRAPHICS_GLVIZ
    #define TYSOC_BACKEND_GRAPHICS_GLVIZ "../libs/libtysocRenderingGlviz.so"
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
    }

}}
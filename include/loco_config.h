
#pragma once

#include <string>

#ifndef LOCO_BACKEND_GRAPHICS_GLVIZ
    #if defined( _WIN32 )
        #define LOCO_BACKEND_GRAPHICS_GLVIZ "./libs/liblocoRenderingGlviz.dll"
    #elif defined( __APPLE__ )
        #define LOCO_BACKEND_GRAPHICS_GLVIZ "./libs/liblocoRenderingGlviz.dylib"
    #else
        #define LOCO_BACKEND_GRAPHICS_GLVIZ "./libs/liblocoRenderingGlviz.so"
    #endif
#endif

namespace loco {
namespace config {

    namespace rendering
    {
        const std::string GLVIZ = LOCO_BACKEND_GRAPHICS_GLVIZ;
    }

    namespace physics
    {
        // Add available physics backend in this namespace
        const std::string NONE = "";
    }

}}
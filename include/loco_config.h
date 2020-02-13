
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
        const std::string NONE = ""; // creates null-obj visualizer (dummy)
        const std::string GLVIZ = LOCO_BACKEND_GRAPHICS_GLVIZ;
    }

    // Add available physics backend in this namespace
    namespace physics
    {
        const std::string NONE = ""; // creates null-obj backend (dummy)
    }

}}